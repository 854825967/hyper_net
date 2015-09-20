#include "Acceptor.h"
#include "NetEngine.h"
#include "NetHeader.h"
#include "Marco.h"
#include "Connection.h"
#include "IKernel.h"
#include "NetWorker.h"

Acceptor::Acceptor(const s32 fd)
    : INetHandler(fd)
    , _parser(nullptr)
    , _sendSize(0)
    , _recvSize(0)
{
    //ctor
}

Acceptor::~Acceptor()
{
    //dtor
}

void Acceptor::OnIn() {
    while (Connection * connection = accept()) {
        NetEngine::Instance()->Add(connection);
        NetEngine::Instance()->Add(connection, EPOLLIN | EPOLLOUT);
		
		NetWorker * worker = NetEngine::Instance()->GetWorker();
		worker->Add(connection);

        connection->OnConnected();
    }
}

void Acceptor::OnError() {
    OASSERT(false, "acceptor error");
}

Connection * Acceptor::accept() {
    OASSERT(_recvSize > 0 && _sendSize > 0, "buff size is zero");

    sockaddr_in remote;
    u32 len = sizeof(remote);
    s32 fd = ::accept(_fd, (sockaddr*)&remote, &len);
    if (fd == net::INVALID_FD) {
        OASSERT(errno == EAGAIN, "wtf");
        return nullptr;
    }

    SetHardClose(fd);
    SetNonBlock(fd);
    SetNonNegal(fd);

    Connection * connection = Connection::Create(fd);
    OASSERT(connection != nullptr, "wtf");

    connection->SetParser(_parser);
    connection->SetBufferSize(_sendSize, _recvSize);

    sockaddr_in local;
    len = sizeof(local);
    getsockname(_fd, (sockaddr*)&local, &len);

    connection->SetLocalIp(inet_ntoa(local.sin_addr));
    connection->SetLocalPort(ntohs(local.sin_port));

    connection->SetRemoteIp(inet_ntoa(remote.sin_addr));
    connection->SetRemotePort(ntohs(remote.sin_port));

    core::ISession * session = _factory->Create();
    OASSERT(session != nullptr, "wtf");
    session->SetFactory(_factory);
    connection->SetSession(session);

    return connection;
}
