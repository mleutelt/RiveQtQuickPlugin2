cmake_minimum_required(VERSION 3.25)

project(QtQuickRivePlugin2
    VERSION 2.0.0
    LANGUAGES C CXX
)

include(GNUInstallDirs)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_POSITION_INDEPENDENT_CODE ON)

set(RIVEQT_RELEASE_NAME "QtQuickRivePlugin2")

if(APPLE)
    enable_language(OBJCXX)
endif()

option(RIVEQT_BUILD_EXAMPLES "Build example applications." ON)
option(RIVEQT_BUILD_TESTS "Build tests." ON)

find_package(Qt6 6.6.0 REQUIRED COMPONENTS Concurrent Core Gui Network Qml Quick QuickControls2 Test QuickTest)
find_package(Python3 REQUIRED COMPONENTS Interpreter)

qt_standard_project_setup(REQUIRES 6.6.0)

list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake")
include(RiveThirdParty)
include(WinDeployQt)

if(RIVEQT_BUILD_TESTS)
    include(CTest)
    enable_testing()
endif()

add_subdirectory(src/RiveQtQuick)

if(RIVEQT_BUILD_EXAMPLES)
    add_subdirectory(examples/icon_grid)
    add_subdirectory(examples/minimal)
    add_subdirectory(examples/interactive)
    add_subdirectory(examples/marketplace_demo)
endif()

if(RIVEQT_BUILD_TESTS)
    add_subdirectory(tests)
endif()

install(FILES
    "${CMAKE_CURRENT_SOURCE_DIR}/LICENSE"
    "${CMAKE_CURRENT_SOURCE_DIR}/README.md"
    DESTINATION "${CMAKE_INSTALL_DATADIR}/${PROJECT_NAME}"
)
