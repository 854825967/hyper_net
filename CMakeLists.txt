#cmake file for project serverd
#author:ooeyusea
#created:2015-07-08 09:38:00

cmake_minimum_required(VERSION 2.8)
PROJECT(serverd)
option(USE_32BITS "Compile for 32bits processors(linux only)" OFF)
IF(UNIX)
    SET(CMAKE_CXX_COMPILER "/usr/bin/g++")
    SET(CMAKE_CXX_FLAGS "-g -Wall -std=c++0x -lpthread -rdynamic -ldl -Dlinux -D_DEBUG")
    SET(CMAKE_CXX_FLAGS_DEBUG "-g -std=c++0x -lpthread -rdynamic -ldl -Dlinux -D_DEBUG")
    SET(CMAKE_CXX_FLAGS_RELEASE "-g -std=c++0x -lpthread -rdynamic -ldl -Dlinux")
    MESSAGE(STATUS "Linux")
ENDIF(UNIX)

SET(PUBLIC
    "${PROJECT_SOURCE_DIR}/src/public/util.h"
    "${PROJECT_SOURCE_DIR}/src/public/util.cpp"
    "${PROJECT_SOURCE_DIR}/src/public/tools.h"
    "${PROJECT_SOURCE_DIR}/src/public/singleton.h"
	"${PROJECT_SOURCE_DIR}/src/public/CycleQueue.h"
	"${PROJECT_SOURCE_DIR}/src/public/Fsm.h"
	"${PROJECT_SOURCE_DIR}/src/public/ThreadModule.h"
    )

SET(PUBLIC_XML
    "${PROJECT_SOURCE_DIR}/src/public/tinyxml/tinystr.cpp"
    "${PROJECT_SOURCE_DIR}/src/public/tinyxml/tinystr.h"
    "${PROJECT_SOURCE_DIR}/src/public/tinyxml/tinyxml.cpp"
    "${PROJECT_SOURCE_DIR}/src/public/tinyxml/tinyxmlerror.cpp"
    "${PROJECT_SOURCE_DIR}/src/public/tinyxml/tinyxml.h"
    "${PROJECT_SOURCE_DIR}/src/public/tinyxml/tinyxmlparser.cpp"
    )

SET(API
    "${PROJECT_SOURCE_DIR}/src/API/IKernel.h"
    "${PROJECT_SOURCE_DIR}/src/API/IModule.h"
    )

ADD_SUBDIRECTORY(${PROJECT_SOURCE_DIR}/src/kernel)
ADD_SUBDIRECTORY(${PROJECT_SOURCE_DIR}/src/logic)
