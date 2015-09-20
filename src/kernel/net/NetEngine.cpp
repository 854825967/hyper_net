#include "NetEngine.h"
#include "Marco.h"
#include "Acceptor.h"
#include "Connection.h"
#include "ConfigMgr.h"
#include "IKernel.h"
#include "Connector.h"
#include "tools.h"
#include "NetWorker.h"

NetEngine::NetEngine() {
    //ctor
}

NetEngine::~NetEngine() {
    //dtor
}

bool NetEngine::Ready() {
    return true;
}

bool NetEngine::Initialize() {
    s32 size = ConfigMgr::Instance()->GetNetSupportSize();
    _epollFd = epoll_create(size);
    OASSERT(_epollFd != net::INVALID_FD, "create epoll failed");

	_events = (epoll_event*)MALLOC(sizeof(epoll_event)* size);

	s32 workerCount = ConfigMgr::Instance()->GetNetThreadCount();
	OASSERT(workerCount > 0, "must have worker");
	for (s32 i = 0; i < workerCount; ++i) {
		_workers.push_back(NEW NetWorker(size / workerCount));
	}

	for (auto * worker : _workers) {
		if (!worker->Initialize()) {
			OASSERT(false, "initialize worker failed");
			return false;
		}
	}

    return true;
}

void NetEngine::Loop() {
    s64 tick = tools::GetTimeMillisecond();

    s32 count = CountHandlers();
    if (count == 0)
        return;

    s32 res = epoll_wait(_epollFd, _events, CountHandlers(), ConfigMgr::Instance()->GetNetFrameWaitTick());
	if (res < 0) {
		if (errno != EINTR) {
			OASSERT(false, "epoll wait error");
		}
		return;
	}
    
    if (res > 0) {
        for (s32 i = 0; i < res; ++i) {
            if (_events[i].events & EPOLLERR) {
                INetHandler * handler = GetHandler(_events[i].data.fd);
                if (handler != nullptr)
                    handler->OnError();
            }

            if (_events[i].events & EPOLLOUT) {
                INetHandler * handler = GetHandler(_events[i].data.fd);
                if (handler != nullptr)
                    handler->OnOut();
            }

            if (_events[i].events & EPOLLIN) {
                INetHandler * handler = GetHandler(_events[i].data.fd);
                if (handler != nullptr)
                    handler->OnIn();
            }
        }
    }

	for (auto * worker : _workers) {
		worker->Process(ConfigMgr::Instance()->GetNetFrameTick() / ConfigMgr::Instance()->GetNetThreadCount());
	}

    s64 use = tools::GetTimeMillisecond() - tick;
    if (use > ConfigMgr::Instance()->GetNetFrameTick()) {

    }
}

void NetEngine::Destroy() {
	for (auto * worker : _workers) {
		worker->Destroy();
		DEL worker;
	}

    if (_events)
        FREE(_events);
}

bool NetEngine::Listen(const char * ip, const s32 port, const s32 sendSize, const s32 recvSize, core::IPacketParser * parser, core::ISessionFactory * factory) {
    s32 fd = net::INVALID_FD;
    if ((fd = socket(AF_INET, SOCK_STREAM, 0)) == net::INVALID_FD) {
        OASSERT(false, "socket error %s", strerror(errno));
        return false;
    }

    struct timeval tv;
    if (0 != setsockopt(fd, SOL_SOCKET, SO_REUSEADDR, (const char*) &tv, sizeof (tv))) {
        OASSERT(false, "setsockopt error %s", strerror(errno));
        close(fd);
        return false;
    }

    if (!SetNonBlock(fd)) {
        OASSERT(false, "setnonbolocking error %s", strerror(errno));
        close(fd);
        return false;
    }

    sockaddr_in server;
    memset(&server, 0, sizeof (server));
    server.sin_family = AF_INET;
    server.sin_addr.s_addr = htonl(inet_addr(ip));
    server.sin_port = htons(port);

    if (bind(fd, (sockaddr *)&server, sizeof(server)) != 0) {
        close(fd);
        return false;
    }

    if (listen(fd, 4096) != 0) {
        close(fd);
        return false;
    }

    Acceptor * acceptor = Acceptor::Create(fd);
    OASSERT(acceptor != nullptr, "wtf");

    acceptor->SetParser(parser);
    acceptor->SetFactory(factory);
    acceptor->SetBufferSize(sendSize, recvSize);

    Add(acceptor);
    Add(acceptor, EPOLLIN);
    return true;
}

bool NetEngine::Connect(const char * ip, const s32 port, const s32 sendSize, const s32 recvSize, core::IPacketParser * parser, core::ISession * session) {
    s32 fd = net::INVALID_FD;
    if ((fd = socket(AF_INET, SOCK_STREAM, 0)) == net::INVALID_FD) {
        return false;
    }

    if (!SetNonBlock(fd)) {
        OASSERT(false, "setnonbolocking error %s", strerror(errno));
        close(fd);
        return false;
    }

    sockaddr_in client;
    memset(&client, 0, sizeof(client));
    client.sin_family = AF_INET;
    client.sin_port = htons(port);

    if (inet_pton(AF_INET, ip, &client.sin_addr) <= 0) {
        OASSERT(false, "inet_pton error %s", strerror(errno));
        close(fd);
        return false;
    }

    s32 ret = connect(fd, (struct sockaddr *) &client, sizeof(client));
    if (ret < 0 && errno != EINPROGRESS) {
		OASSERT(false, "connect error %s", strerror(errno));
        return false;
    }

    Connector * connector = Connector::Create(fd);
    if (connector == nullptr) {
        close(fd);
        return false;
    }
    connector->SetIp(ip, port);
    connector->SetParser(parser);
    connector->SetSession(session);
    connector->SetBufferSize(sendSize, recvSize);

    Add(connector);
    Add(connector, EPOLLOUT);
    return true;
}

void NetEngine::Add(INetHandler * handler, s32 events) {
	events |= EPOLLERR | EPOLLHUP | EPOLLET;

	epoll_event evt;
	memset(&evt, 0, sizeof(evt));
	evt.data.fd = handler->GetFD();
	evt.events = events;
	if (epoll_ctl(_epollFd, EPOLL_CTL_ADD, handler->GetFD(), &evt) == -1) {
		OASSERT(false, "epoll ctl failed");
	}
}

void NetEngine::Del(INetHandler * handler, s32 events) {
	events |= EPOLLERR | EPOLLHUP | EPOLLET;

	epoll_event evt;
	memset(&evt, 0, sizeof(evt));
	evt.data.fd = handler->GetFD();
	evt.events = events;
	if (epoll_ctl(_epollFd, EPOLL_CTL_DEL, handler->GetFD(), &evt) == -1) {
		OASSERT(false, "epoll ctl failed");
	}
}

void NetEngine::Add(INetHandler * handler) {
    OASSERT(_handlers.find(handler->GetFD()) == _handlers.end(), "wtf");
    _handlers[handler->GetFD()] = handler;
}

void NetEngine::Remove(INetHandler * handler) {
    OASSERT(_handlers.find(handler->GetFD()) != _handlers.end(), "wtf");
    _handlers.erase(handler->GetFD());
}

NetWorker * NetEngine::GetWorker() {
	NetWorker * ret = nullptr;
	for (auto * worker : _workers) {
		if (worker->GetFreeSlot() > 0) {
			if (ret == nullptr || ret->GetFreeSlot() < worker->GetFreeSlot()) {
				ret = worker;
			}
		}
	}
	OASSERT(ret, "there is not a worker has slot");
	return ret;
}
