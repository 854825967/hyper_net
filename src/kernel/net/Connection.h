#ifndef __CONNECTION_H__
#define __CONNECTION_H__

#include "INetHandler.h"
#include "IKernel.h"
#include <unordered_map>
#include "Fsm.h"
#include "NetEvent.h"

#define BUFFER_ARRAY_LEN 2
class NetWorker;
class Connection : public INetHandler, public core::IPipe {
	struct Buffer {
		char * data;
		s32 size;
		s32 offset;
	};

	enum {
		INDEX_WORKING = 0,
		INDEX_USING = 1,
	};

public:
	enum {
		WRET_DONE,
		WRET_PENDING,
		WRET_ERROR,
		WRET_CLOSE,
	};

    static Connection * Create(const s32 fd) {
        return NEW Connection(fd);
    }

    inline void SetParser(core::IPacketParser * parser) { _parser = parser; }
    inline void SetSession(core::ISession * session) {
        _session = session;
        session->SetPipe(this);
    }
    void SetBufferSize(const s32 sendSize, const s32 recvSize);

    inline void SetLocalIp(const char * ip) { SafeSprintf(_localIp, sizeof(_localIp) - 1, ip); }
    virtual const char * GetLocalIp() const { return _localIp; }

    inline void SetLocalPort(const s32 port) { _localPort = port; }
    virtual s32 GetLocalPort() const { return _localPort; }

    inline void SetRemoteIp(const char * ip) { SafeSprintf(_remoteIp, sizeof(_remoteIp) - 1, ip); }
    virtual const char * GetRemoteIp() const { return _remoteIp; }

    inline void SetRemotePort(const s32 port) { _remotePort = port; }
    virtual s32 GetRemotePort() const { return _remotePort; }

	inline void SetWorker(NetWorker * worker) { _worker = worker; }
	inline NetWorker * GetWorker() const { return _worker; }

    void OnConnected();
    virtual void OnIn();
    virtual void OnOut();
    virtual void OnError();
    virtual void Send(const void * context, const s32 size);
    virtual void Close();

	void OnEvent(const NetEvent& evt);
	void ChangeSendState(olib::State<Connection, NetEvent> * state);
	void ChangeRecvState(olib::State<Connection, NetEvent> * state);

	bool ProcessPacket();

	bool BeginSend();
	void BeginRecv();

	s32 OnRecv();
	s32 OnSend();
	void OnClose();

private:
    Connection(const s32 fd);
    virtual ~Connection();
    void OnError(s32 error);

private:
    core::IPacketParser * _parser;
    core::ISession * _session;
	NetWorker * _worker;
    char _localIp[MAX_IP_SIZE];
    s32 _localPort;
    char _remoteIp[MAX_IP_SIZE];
    s32 _remotePort;

	Buffer _sendbuffer[BUFFER_ARRAY_LEN];

	Buffer _recvbuffer[BUFFER_ARRAY_LEN];

	olib::StateMachine<Connection, NetEvent> _sendMachine;
	olib::StateMachine<Connection, NetEvent> _recvMachine;
};

#endif //__CONNECTION_H__
