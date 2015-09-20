#include "Connector.h"
#include "NetEngine.h"
#include "NetHeader.h"
#include "Marco.h"
#include "Connection.h"
#include "IKernel.h"
#include "kernel.h"
#include "NetWorker.h"

Connector::Connector(const s32 fd)
    : INetHandler(fd)
    , _parser(nullptr)
    , _sendSize(0)
    , _recvSize(0)
{
    //ctor
}

Connector::~Connector() {
    //dtor
}

void Connector::OnOut() {
    OASSERT(_recvSize > 0 && _sendSize > 0, "buff size is zero");

    SetHardClose(_fd);
    SetNonNegal(_fd);

    Connection * connection = Connection::Create(_fd);
    OASSERT(connection != nullptr, "wtf");

    connection->SetParser(_parser);
    connection->SetBufferSize(_sendSize, _recvSize);

    sockaddr_in local;
    socklen_t len = sizeof(local);
    getsockname(_fd, (sockaddr*)&local, &len);

    connection->SetLocalIp(inet_ntoa(local.sin_addr));
    connection->SetLocalPort(ntohs(local.sin_port));

    connection->SetRemoteIp(_ip);
    connection->SetRemotePort(_port);

    _session->SetFactory(nullptr);
    connection->SetSession(_session);

    NetEngine::Instance()->Del(this, EPOLLOUT);
    NetEngine::Instance()->Remove(this);
    NetEngine::Instance()->Add(connection);
    NetEngine::Instance()->Add(connection, EPOLLIN | EPOLLOUT);

	NetWorker * worker = NetEngine::Instance()->GetWorker();
	worker->Add(connection);

    connection->OnConnected();
    DEL this;
}

void Connector::OnError() {
    ::close(_fd);
    NetEngine::Instance()->Remove(this);
    _session->OnConnectFailed(Kernel::Instance());
    DEL this;
}
