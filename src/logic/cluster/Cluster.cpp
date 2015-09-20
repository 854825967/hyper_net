#include "Cluster.h"
#include "IHarbor.h"
#include "tinyxml.h"
#include "NodeProtocol.h"
#include "NodeType.h"

Cluster * Cluster::s_self = nullptr;
IHarbor * Cluster::s_harbor = nullptr;

std::unordered_set<s64> Cluster::s_openNode;
std::string Cluster::_ip;
s32 Cluster::_port = 0;

bool Cluster::Initialize(IKernel * kernel) {
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
    _ip = p->Attribute("ip");
    _port = tools::StringAsInt(p->Attribute("port"));

    return true;
}

bool Cluster::Launched(IKernel * kernel) {
    s_harbor = (IHarbor*)kernel->FindModule("Harbor");
    OASSERT(s_harbor, "where is harbor");

    s_harbor->Connect(_ip.c_str(), _port);
    REGPROTOCOL(node_type::MASTER, node_proto::NEW_NODE, Cluster::NewNodeComming);

    return true;
}

bool Cluster::Destroy(IKernel * kernel) {
    DEL this;
    return true;
}

void Cluster::NewNodeComming(IKernel * kernel, s32 nodeType, s32 nodeId, const void * context, const s32 size) {
    OASSERT(size == sizeof(node_proto::NewNode), "invalid node size");
    node_proto::NewNode& info = *((node_proto::NewNode*)context);
    OASSERT(info.port > 0, "where is harbor port");

	s64 check = ((s64)info.nodeType << 32) | ((s64)info.nodeId);
    if (s_openNode.find(check) != s_openNode.end())
        return;

    if (info.nodeType == s_harbor->GetNodeType() && info.nodeId <= s_harbor->GetNodeId())
        return;

    s_openNode.insert(check);
    s_harbor->Connect(info.ip, info.port);
}
