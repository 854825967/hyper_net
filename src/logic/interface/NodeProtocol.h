#ifndef __NODE_PROTOCOL_H__
#define __NODE_PROTOCOL_H__
#include "util.h"
#include "Define.h"

namespace node_proto {
    enum {
        NEW_NODE = 1,

        RPCNR = 2,
        RPC = 3,
        RESPONE = 4,
    };

    struct NewNode {
        s32 nodeType;
        s32 nodeId;
        char ip[MAX_IP_SIZE];
        s32 port;
    };
}

#endif //__NODE_PROTOCOL_H__
