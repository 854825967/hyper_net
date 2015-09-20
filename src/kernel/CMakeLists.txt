INCLUDE_DIRECTORIES(
    "${PROJECT_SOURCE_DIR}/src/API/"
    "${PROJECT_SOURCE_DIR}/src/public/"
    "${PROJECT_SOURCE_DIR}/src/public/tinyxml"
    "${PROJECT_SOURCE_DIR}/src/kernel/"
    "${PROJECT_SOURCE_DIR}/src/kernel/config/"
    "${PROJECT_SOURCE_DIR}/src/kernel/net/"
	"${PROJECT_SOURCE_DIR}/src/kernel/net/state/"
	"${PROJECT_SOURCE_DIR}/src/kernel/net/state/send/"
	"${PROJECT_SOURCE_DIR}/src/kernel/net/state/recv/"
    "${PROJECT_SOURCE_DIR}/src/kernel/lua/"
    "${PROJECT_SOURCE_DIR}/src/kernel/define/"
    "${PROJECT_SOURCE_DIR}/src/kernel/logic/"
    )

SET(KERNEL_CONFIG
    "${PROJECT_SOURCE_DIR}/src/kernel/config/ConfigMgr.h"
    "${PROJECT_SOURCE_DIR}/src/kernel/config/ConfigMgr.cpp"
    )

SET(KERNEL_NET
    "${PROJECT_SOURCE_DIR}/src/kernel/net/NetEngine.h"
    "${PROJECT_SOURCE_DIR}/src/kernel/net/NetEngine.cpp"
    "${PROJECT_SOURCE_DIR}/src/kernel/net/INetHandler.h"
    "${PROJECT_SOURCE_DIR}/src/kernel/net/NetHeader.h"
    "${PROJECT_SOURCE_DIR}/src/kernel/net/Acceptor.h"
    "${PROJECT_SOURCE_DIR}/src/kernel/net/Acceptor.cpp"
    "${PROJECT_SOURCE_DIR}/src/kernel/net/Connector.h"
    "${PROJECT_SOURCE_DIR}/src/kernel/net/Connector.cpp"
    "${PROJECT_SOURCE_DIR}/src/kernel/net/Connection.h"
    "${PROJECT_SOURCE_DIR}/src/kernel/net/Connection.cpp"
	"${PROJECT_SOURCE_DIR}/src/kernel/net/NetWorker.h"
    "${PROJECT_SOURCE_DIR}/src/kernel/net/NetWorker.cpp"
    )

SET(KERNEL_NET_STATE
	"${PROJECT_SOURCE_DIR}/src/kernel/net/state/NetEvent.h"
	)

SET(KERNEL_NET_STATE_SEND
	"${PROJECT_SOURCE_DIR}/src/kernel/net/state/send/SendClose.cpp"
	"${PROJECT_SOURCE_DIR}/src/kernel/net/state/send/SendClose.h"
	"${PROJECT_SOURCE_DIR}/src/kernel/net/state/send/Sending.cpp"
	"${PROJECT_SOURCE_DIR}/src/kernel/net/state/send/Sending.h"
	"${PROJECT_SOURCE_DIR}/src/kernel/net/state/send/SendingWait.cpp"
	"${PROJECT_SOURCE_DIR}/src/kernel/net/state/send/SendingWait.h"
	"${PROJECT_SOURCE_DIR}/src/kernel/net/state/send/SendingWaitClose.cpp"
	"${PROJECT_SOURCE_DIR}/src/kernel/net/state/send/SendingWaitClose.h"
	"${PROJECT_SOURCE_DIR}/src/kernel/net/state/send/SendNone.cpp"
	"${PROJECT_SOURCE_DIR}/src/kernel/net/state/send/SendNone.h"
	"${PROJECT_SOURCE_DIR}/src/kernel/net/state/send/SendWaitClose.cpp"
	"${PROJECT_SOURCE_DIR}/src/kernel/net/state/send/SendWaitClose.h"
	"${PROJECT_SOURCE_DIR}/src/kernel/net/state/send/SendingReady.cpp"
	"${PROJECT_SOURCE_DIR}/src/kernel/net/state/send/SendingReady.h"
	"${PROJECT_SOURCE_DIR}/src/kernel/net/state/send/SendPending.cpp"
	"${PROJECT_SOURCE_DIR}/src/kernel/net/state/send/SendPending.h"
	"${PROJECT_SOURCE_DIR}/src/kernel/net/state/send/SendingReadyWait.cpp"
	"${PROJECT_SOURCE_DIR}/src/kernel/net/state/send/SendingReadyWait.h"
	)

SET(KERNEL_NET_STATE_RECV
	"${PROJECT_SOURCE_DIR}/src/kernel/net/state/recv/RecvClose.cpp"
	"${PROJECT_SOURCE_DIR}/src/kernel/net/state/recv/RecvClose.h"
	"${PROJECT_SOURCE_DIR}/src/kernel/net/state/recv/Recving.cpp"
	"${PROJECT_SOURCE_DIR}/src/kernel/net/state/recv/Recving.h"
	"${PROJECT_SOURCE_DIR}/src/kernel/net/state/recv/RecvingWait.cpp"
	"${PROJECT_SOURCE_DIR}/src/kernel/net/state/recv/RecvingWait.h"
	"${PROJECT_SOURCE_DIR}/src/kernel/net/state/recv/RecvNone.cpp"
	"${PROJECT_SOURCE_DIR}/src/kernel/net/state/recv/RecvNone.h"
	"${PROJECT_SOURCE_DIR}/src/kernel/net/state/recv/RecvWaitClose.cpp"
	"${PROJECT_SOURCE_DIR}/src/kernel/net/state/recv/RecvWaitClose.h"
	)

SET(KERNEL_LUA
    "${PROJECT_SOURCE_DIR}/src/kernel/lua/lua_engine.h"
    "${PROJECT_SOURCE_DIR}/src/kernel/lua/lua_engine.cpp"
    )

SET(KERNEL_LOGIC
    "${PROJECT_SOURCE_DIR}/src/kernel/logic/LogicMgr.h"
    "${PROJECT_SOURCE_DIR}/src/kernel/logic/LogicMgr.cpp"
    )

SET(KERNEL
    "${PROJECT_SOURCE_DIR}/src/kernel/kernel.h"
    "${PROJECT_SOURCE_DIR}/src/kernel/kernel.cpp"
    "${PROJECT_SOURCE_DIR}/src/kernel/main.cpp"
)

SOURCE_GROUP(src\\config FILES ${KERNEL_CONFIG})
SOURCE_GROUP(src\\net FILES ${KERNEL_NET})
SOURCE_GROUP(src\\net\\state FILES ${KERNEL_NET_STATE})
SOURCE_GROUP(src\\net\\state\\send FILES ${KERNEL_NET_STATE_SEND})
SOURCE_GROUP(src\\net\\state\\recv FILES ${KERNEL_NET_STATE_RECV})
SOURCE_GROUP(src\\lua FILES ${KERNEL_LUA})
SOURCE_GROUP(src\\logic FILES ${KERNEL_LOGIC})
SOURCE_GROUP(API FILES ${API})
SOURCE_GROUP(public\\tinyxml FILES ${PUBLIC_XML})
SOURCE_GROUP(public FILES ${PUBLIC})
SOURCE_GROUP(src FILES ${KERNEL})

ADD_EXECUTABLE(serverd
        ${API}
        ${KERNEL_CONFIG}
        ${KERNEL_NET}
		${KERNEL_NET_STATE}
		${KERNEL_NET_STATE_SEND}
		${KERNEL_NET_STATE_RECV}
        ${KERNEL_LOGIC}
        ${KERNEL_LUA}
        ${PUBLIC}
        ${PUBLIC_XML}
        ${KERNEL}
        )

SET(EXECUTABLE_OUTPUT_PATH "${PROJECT_SOURCE_DIR}/bin")
