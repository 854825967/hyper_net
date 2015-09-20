#include "NodeSession.h"
#include "Harbor.h"

NodeSession::NodeSession()
    : _ready(false)
    , _nodeType(0)
    , _nodeId(0)
    , _connect(false) {
}

NodeSession::~NodeSession() {

}

void NodeSession::OnConnected(IKernel * kernel) {
    OASSERT(!_ready, "why is ready?");
    char buf[sizeof(HarborHeader) + sizeof(NodeReport)];
    HarborHeader * header = (HarborHeader*)buf;
    header->message = harbor::REPORT;
    header->len = sizeof(buf);

    NodeReport * report = (NodeReport*)(buf + sizeof(HarborHeader));
    report->nodeType = Harbor::Self()->GetNodeType();
    report->nodeId = Harbor::Self()->GetNodeId();
    report->port = Harbor::Self()->GetPort();
    report->hide = Harbor::Self()->IsHide();

    Send(buf, sizeof(buf));
}

void NodeSession::OnRecv(IKernel * kernel, const void * context, const s32 size) {
    HarborHeader * header = (HarborHeader*)context;
    if (!_ready) {
        if (header->message != harbor::REPORT) {
            OASSERT(false, "wtf");
            return;
        }

         NodeReport * report = (NodeReport*)((const char*)context + sizeof(HarborHeader));
        _nodeType = report->nodeType;
        _nodeId = report->nodeId;

        Harbor::Self()->OnNodeOpen(kernel, _nodeType, _nodeId, GetRemoteIp(), report->port, report->hide, this);

        _ready = true;
    }
    else {
        if (header->message != harbor::MESSAGE) {
            OASSERT(false, "wtf");
            return;
        }

        Harbor::Self()->OnNodeMessage(kernel, _nodeType, _nodeId, (const char *)context + sizeof(HarborHeader), size - sizeof(HarborHeader));
    }
}

void NodeSession::OnError(IKernel * kernel, const s32 error) {

}

void NodeSession::OnDisconnected(IKernel * kernel) {
    if (_ready)
        Harbor::Self()->OnNodeClose(kernel, _nodeType, _nodeId);

	if (_connect) {
		Harbor::Self()->AddReconnect(this, _ip, _port);
		_ready = false;
	}
}

void NodeSession::OnConnectFailed(IKernel * kernel) {
    if (_connect)
        Harbor::Self()->AddReconnect(this, _ip, _port);
}

bool NodeSession::PrepareSendNodeMessage(const s32 size) {
    HarborHeader header;
    header.message = harbor::MESSAGE;
    header.len = size + sizeof(HarborHeader);

    Send(&header, sizeof(header));
    return true;
}

bool NodeSession::SendNodeMessage(const void * context, const s32 size) {
    Send(context, size);
    return true;
}
