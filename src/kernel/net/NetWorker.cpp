#include "NetWorker.h"
#include "Connection.h"
#include "tools.h"

NetWorker::NetWorker(const s32 maxConnection) 
	: _workQueue(maxConnection * 2)
	, _resultQueue(maxConnection * 2) {
	_maxConnection = maxConnection;
	_nowConnection = 0;
	_terminated = false;
}

NetWorker::~NetWorker() {

}

bool NetWorker::Initialize() {
	_thread = std::thread(&NetWorker::ThreadProc, this);
	return true;
}

void NetWorker::Destroy() {
	_terminated = true;
	_thread.join();
}

void NetWorker::Add(Connection* connection) {
	OASSERT(connection->GetWorker() == nullptr, "connection is alreay has worker");
	connection->SetWorker(this);
	++_nowConnection;
}

void NetWorker::Remove(Connection* connection) {
	OASSERT(connection->GetWorker() == this, "connection's worker is not this worker");
	connection->SetWorker(nullptr);
	--_nowConnection;
	OASSERT(_nowConnection >= 0, "connection count error");
}

void NetWorker::Recv(Connection* connection) {
	OASSERT(connection->GetWorker() == this, "connection's worker is not this worker");

	WorkRequest request;
	request.connection = connection;
	request.evt = WorkRequest::Event::RECV;

	_workQueue.Push(request);
}

void NetWorker::Send(Connection* connection) {
	OASSERT(connection->GetWorker() == this, "connection's worker is not this worker");

	WorkRequest request;
	request.connection = connection;
	request.evt = WorkRequest::Event::SEND;

	_workQueue.Push(request);
}

void NetWorker::ThreadProc() {
	while (!_terminated) {
		WorkRequest request;
		if (_workQueue.Read(request)) {
			DoRequest(request);
		}
		else {
			CSLEEP(1);
		}
	}
}

void NetWorker::DoRequest(const WorkRequest& request) {
	OASSERT(request.connection, "where is connection");

	WorkResult result;
	result.connection = request.connection;

	switch (request.evt) {
	case WorkRequest::Event::RECV: {
			s32 ret = request.connection->OnRecv();
			switch (ret) {
			case Connection::WRET_DONE: result.evt.type = NEVT_RECV_DONE; break;
			case Connection::WRET_ERROR: result.evt.type = NEVT_RECV_ERROR; break;
			case Connection::WRET_CLOSE: result.evt.type = NEVT_RECV_CLOSE; break;
			}
		}
		break;
	case WorkRequest::Event::SEND: {
			s32 ret = request.connection->OnSend();
			switch (ret) {
			case Connection::WRET_DONE: result.evt.type = NEVT_SEND_DONE; break;
			case Connection::WRET_ERROR: result.evt.type = NEVT_SEND_ERROR; break;
			case Connection::WRET_PENDING: result.evt.type = NEVT_SEND_PENDING; break;
			}
		}break;
	}

	_resultQueue.Push(result);
}

void NetWorker::Process(const s64 frame) {
	s64 tick = tools::GetTimeMillisecond();
	WorkResult result;
	while (_resultQueue.Read(result)) {
		DoResult(result);
		
		s64 now = tools::GetTimeMillisecond();
		if (now > tick + frame)
			break;
	}
}

void NetWorker::DoResult(const WorkResult& result) {
	OASSERT(result.connection, "where is connection");
	result.connection->OnEvent(result.evt);
}
