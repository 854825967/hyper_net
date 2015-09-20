#include "Connection.h"
#include "NetHeader.h"
#include "NetEngine.h"
#include "ConfigMgr.h"
#include "kernel.h"
#include "SendNone.h"
#include "RecvNone.h"
#include "SendClose.h"
#include "RecvClose.h"
#include "NetWorker.h"

#define ERROR_OVER_FLOW -1

Connection::Connection(const s32 fd)
    : INetHandler(fd)
    , _parser(nullptr)
    , _session(nullptr)
	, _worker(nullptr)
    , _localPort(0)
    , _remotePort(0)
    , _sendMachine(this)
    , _recvMachine(this) {

	memset(_sendbuffer, 0, sizeof(_sendbuffer));
	memset(_recvbuffer, 0, sizeof(_recvbuffer));

	_sendMachine.Initialize(SendNone::Instance());
	_recvMachine.Initialize(RecvNone::Instance());
}

Connection::~Connection() {
	for (s32 i = 0; i < BUFFER_ARRAY_LEN; ++i) {
		OASSERT(_sendbuffer[i].data, "wtf");
		FREE(_sendbuffer[i].data);
	}

	for (s32 i = 0; i < BUFFER_ARRAY_LEN; ++i) {
		OASSERT(_recvbuffer[i].data, "wtf");
		FREE(_recvbuffer[i].data);
	}
}

void Connection::SetBufferSize(const s32 sendSize, const s32 recvSize) {
	for (s32 i = 0; i < BUFFER_ARRAY_LEN; ++i) {
		OASSERT(_sendbuffer[i].data == nullptr, "wtf");

		_sendbuffer[i].data = (char*)MALLOC(sendSize);
		_sendbuffer[i].size = sendSize;
		_sendbuffer[i].offset = 0;
	}

	for (s32 i = 0; i < BUFFER_ARRAY_LEN; ++i) {
		OASSERT(_recvbuffer[i].data == nullptr, "wtf");

		_recvbuffer[i].data = (char*)MALLOC(recvSize);
		_recvbuffer[i].size = recvSize;
		_recvbuffer[i].offset = 0;
	}
}

void Connection::OnConnected() {
    OASSERT(_session, "session is null");
    _session->OnConnected(Kernel::Instance());
}

void Connection::OnIn() {
	NetEvent evt;
	evt.type = NEVT_RECV;
	OnEvent(evt);
}

void Connection::OnOut() {
	NetEvent evt;
	evt.type = NEVT_SEND_READY;
	OnEvent(evt);
}

void Connection::OnError() {
    OnError(errno);

	NetEvent evt;
	evt.type = NEVT_ERROR;
	OnEvent(evt);
}

void Connection::Send(const void * context, const s32 size) {
	Buffer& buffer = _sendbuffer[INDEX_USING];
	OASSERT(buffer.data, "where is send buffer");

	if (buffer.offset + size > buffer.size) {
		NetEvent evt;
		evt.type = NEVT_ERROR;
		OnEvent(evt);
	}
	else {
		memcpy(buffer.data + buffer.offset, context, size);
		buffer.offset += size;

		NetEvent evt;
		evt.type = NEVT_SEND;
		OnEvent(evt);
	}
}

void Connection::Close() {
    OASSERT(_session, "session is null");
    
	NetEvent evt;
	evt.type = NEVT_CLOSE;
	OnEvent(evt);
}

void Connection::OnError(s32 error) {
    OASSERT(_session, "session is null");
    _session->OnError(Kernel::Instance(), error);
}

void Connection::OnEvent(const NetEvent& evt) {
	_sendMachine.OnEvent(evt);
	_recvMachine.OnEvent(evt);

	if (_sendMachine.GetCurrentState() == SendClose::Instance() && _recvMachine.GetCurrentState() == RecvClose::Instance())
		OnClose();
}

void Connection::ChangeSendState(olib::State<Connection, NetEvent> * state) {
	_sendMachine.ChangeState(state);
}

void Connection::ChangeRecvState(olib::State<Connection, NetEvent> * state) {
	_recvMachine.ChangeState(state);
}

bool Connection::ProcessPacket() {
	Buffer& worked = _recvbuffer[INDEX_WORKING];
	Buffer& used = _recvbuffer[INDEX_USING];

	if (worked.offset > 0) {
		if (worked.offset + used.offset > used.size) {
			return false;
		}

		memcpy(used.data + used.offset, worked.data, worked.offset);
		used.offset += worked.offset;
		worked.offset = 0;

		s32 pos = 0;
		while (pos < used.offset) {
			s32 u = _parser->ParsePacket(used.data + pos, used.offset - pos);
			if (u == 0)
				break;
			else if (u > 0) {
				_session->OnRecv(Kernel::Instance(), used.data + pos, u);
				pos += u;
			}
			else {
				return false;
			}
		}
		if (pos > 0 && pos < used.offset)
			::memmove(used.data, used.data + pos, used.offset - pos);
		used.offset -= pos;
	}
	else {
		//OASSERT(false, "recv empty data");
	}

	return true;
}

bool Connection::BeginSend() {
	Buffer& worked = _sendbuffer[INDEX_WORKING];
	Buffer& used = _sendbuffer[INDEX_USING];

	if (used.offset > 0) {
		if (worked.offset + used.offset > worked.size) {
			return false;
		}

		memcpy(worked.data + worked.offset, used.data, used.offset);
		worked.offset += used.offset;
		used.offset = 0;
	}

	_worker->Send(this);
	return true;
}

void Connection::BeginRecv() {
	_worker->Recv(this);
}

s32 Connection::OnRecv() {
	Buffer& buffer = _recvbuffer[INDEX_WORKING];
	OASSERT(buffer.offset == 0, "recv buffer is not empty");

	s32 offset = buffer.offset;
	while (offset < buffer.size) {
		s32 res = ::recv(_fd, buffer.data + offset, buffer.size - offset, 0);
		if (res < 0) {
			if (errno != EAGAIN) {
				return WRET_ERROR;
			}
			else {
				break;
			}
		}
		else if (res == 0)
			return WRET_CLOSE;
		else
			offset += res;
	}

	buffer.offset = offset;
	return WRET_DONE;
}

s32 Connection::OnSend() {
	Buffer& buffer = _sendbuffer[INDEX_WORKING];
	OASSERT(buffer.offset != 0, "send buffer is empty");

	s32 offset = 0;
	s32 ret = WRET_DONE;
	while (offset < buffer.offset) {
		s32 res = ::send(_fd, buffer.data + offset, buffer.offset - offset, 0);
		if (res < 0) {
			if (errno != EAGAIN) {
				return WRET_ERROR;
			}
			else {
				ret = WRET_PENDING;
				break;
			}
		}
		else if (res > 0)
			offset += res;
	}

	if (offset < buffer.offset)
		::memmove(buffer.data, buffer.data + offset, buffer.offset - offset);
	buffer.offset -= offset;

	return ret;
}

void Connection::OnClose() {
	OASSERT(_session && _worker, "session or worker is null");

	_session->OnDisconnected(Kernel::Instance());
	_session->SetPipe(nullptr);
	_session->OnRelease();

	_worker->Remove(this);
	NetEngine::Instance()->Del(this, EPOLLIN | EPOLLOUT);
	close(_fd);
	NetEngine::Instance()->Remove(this);
	DEL this;
}