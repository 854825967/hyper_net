
INCLUDE_DIRECTORIES(
    "${PROJECT_SOURCE_DIR}/src/API"
    "${PROJECT_SOURCE_DIR}/src/public"
    "${PROJECT_SOURCE_DIR}/src/public/tinyxml"
    "${PROJECT_SOURCE_DIR}/src/logic/interface"
    )

SET(HARBOR
    "${PROJECT_SOURCE_DIR}/src/logic/harbor/Harbor.h"
    "${PROJECT_SOURCE_DIR}/src/logic/harbor/Harbor.cpp"
    "${PROJECT_SOURCE_DIR}/src/logic/harbor/NodeSession.h"
    "${PROJECT_SOURCE_DIR}/src/logic/harbor/NodeSession.cpp"
    "${PROJECT_SOURCE_DIR}/src/logic/harbor/main.cpp"
    )

SOURCE_GROUP(interface FILES ${LOGIC_INTERFACE})
SOURCE_GROUP(API FILES ${API})
SOURCE_GROUP(public\\tinyxml FILES ${PUBLIC_XML})
SOURCE_GROUP(public FILES ${PUBLIC})
SOURCE_GROUP(\\ FILES ${HARBOR})

ADD_LIBRARY(Harbor SHARED
        ${HARBOR}
        ${API}
        ${LOGIC_INTERFACE}
        ${PUBLIC}
        ${PUBLIC_XML}
        )

