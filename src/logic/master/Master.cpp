#include "Master.h"
#include "NodeProtocol.h"
#include "tinyxml.h"

Master * Master::s_self = nullptr;
IHarbor * Master::s_harbor = nullptr;

s32 Master::s_port = 0;
std::unordered_map<s64, Master::NodeInfo> Master::s_nodes;

bool Master::Initialize(IKernel * kernel) {
    s_self = this;
    _kernel = kernel;

    TiXmlDocument doc;
    std::string coreConfigPath = std::string(tools::GetAppPath()) + "/config/server_conf.xml";
    if (!doc.LoadFile(coreConfigPath.c_str())) {
        OASSERT(false, "can't find core file : %s", coreConfigPath.c_str());
        return false;
    }

    const TiXmlElement * pRoot = doc.RootElement();
    OASSERT(pRoot != nullptr, "core xml format error");

    const TiXmlElement * p = pRoot->FirstChildElement("master");
    s_port = tools::StringAsInt(p->Attribute("port"));

    return true;
}

bool Master::Launched(IKernel * kernel) {
    s_harbor = (IHarbor*)kernel->FindModule("Harbor");
    OASSERT(s_harbor, "where is harbor");
    s_harbor->AddNodeListener(this, "Master");

    return true;
}

bool Master::Destroy(IKernel * kernel) {
    DEL this;
    return true;
}

void Master::OnOpen(IKernel * kernel, s32 nodeType, s32 nodeId, bool hide, const char * ip, s32 port) {
    if (hide)
        return;

    s64 check = ((s64)nodeType << 32) | ((s64)nodeId);
#ifdef _DEBUG
    {
        auto itr = s_nodes.find(check);
        if (itr != s_nodes.end()) {
            OASSERT(strcmp(ip, itr->second.ip) == 0 && port == itr->second.port, "wtf");
        }
    }
#endif
    NodeInfo& info = s_nodes[check];
    info.nodeType = nodeType;
    info.nodeId = nodeId;
    SafeSprintf(info.ip, sizeof(info.ip), ip);
    info.port = port;

    for (auto itr = s_nodes.begin(); itr != s_nodes.end(); ++itr) {
		if (itr->first != check) {
			SendNewNode(kernel, itr->second.nodeType, itr->second.nodeId, nodeType, nodeId, ip, port);
			SendNewNode(kernel, nodeType, nodeId, itr->second.nodeType, itr->second.nodeId, itr->second.ip, itr->second.port);
		}
    }
}

void Master::OnClose(IKernel * kernel, s32 nodeType, s32 nodeId) {

}

void Master::SendNewNode(IKernel * kernel, s32 nodeType, s32 nodeId, s32 newNodeType, s32 newNodeId, const char * ip, s32 port) {
    node_proto::NewNode info;
    info.nodeType = newNodeType;
	info.nodeId = newNodeId;
    SafeSprintf(info.ip, sizeof(info.ip), ip);
    info.port = port;

    s_harbor->PrepareSend(nodeType, nodeId, node_proto::NEW_NODE, sizeof(info));
    s_harbor->Send(nodeType, nodeId, &info, sizeof(info));
}
