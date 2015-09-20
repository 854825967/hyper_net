SET(LIBRARY_OUTPUT_PATH "${PROJECT_SOURCE_DIR}/bin")
SET(LOGIC_INTERFACE
    "${PROJECT_SOURCE_DIR}/src/logic/interface/IHarbor.h"
    "${PROJECT_SOURCE_DIR}/src/logic/interface/Define.h"
    "${PROJECT_SOURCE_DIR}/src/logic/interface/NodeProtocol.h"
    "${PROJECT_SOURCE_DIR}/src/logic/interface/NodeType.h"
    "${PROJECT_SOURCE_DIR}/src/logic/interface/IMaster.h"
	"${PROJECT_SOURCE_DIR}/src/logic/interface/IScriptEngine.h"
)

ADD_SUBDIRECTORY(${PROJECT_SOURCE_DIR}/src/logic/harbor)
ADD_SUBDIRECTORY(${PROJECT_SOURCE_DIR}/src/logic/cluster)
ADD_SUBDIRECTORY(${PROJECT_SOURCE_DIR}/src/logic/master)
ADD_SUBDIRECTORY(${PROJECT_SOURCE_DIR}/src/logic/starter)
ADD_SUBDIRECTORY(${PROJECT_SOURCE_DIR}/src/logic/scriptengine)
ADD_SUBDIRECTORY(${PROJECT_SOURCE_DIR}/src/logic/redis)
ADD_SUBDIRECTORY(${PROJECT_SOURCE_DIR}/src/logic/rpc)
ADD_SUBDIRECTORY(${PROJECT_SOURCE_DIR}/src/logic/http)

