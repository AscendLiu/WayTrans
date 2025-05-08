cmake_minimum_required(VERSION 3.16)

project(WayTrans VERSION 0.1 LANGUAGES CXX C)
set(CMAKE_PREFIX_PATH "/home/liusheng/Applications/Qt/6.8.3/gcc_64/")
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Qt6 REQUIRED COMPONENTS Widgets Core WaylandClient)

set(PROJECT_SOURCES
        main.cpp
        widget.cpp
        widget.h
        waylandselectionlistener.h
        waylandselectionlistener.cpp
        wayland-wlr-data-control-unstable-v1-protocol.c
        wayland-wlr-data-control-unstable-v1-client-protocol.h
        qwayland-wlr-data-control-unstable-v1.cpp
        qwayland-wlr-data-control-unstable-v1.h
)

qt_add_executable(WayTrans
    MANUAL_FINALIZATION
    ${PROJECT_SOURCES}
)

# qt_generate_wayland_protocol_client_sources(WayTrans
#     FILES ${CMAKE_CURRENT_LIST_DIR}/wlr-data-control-unstable-v1.xml
# )


# target_include_directories(WayTrans
#     PUBLIC
#         ${CMAKE_CURRENT_BINARY_DIR}
# )

target_link_libraries(WayTrans PRIVATE
  Qt6::Widgets
  Qt6::Core
  Qt6::WaylandClient)

include(GNUInstallDirs)
install(TARGETS WayTrans
    BUNDLE DESTINATION .
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)

if(QT_VERSION_MAJOR EQUAL 6)
    qt_finalize_executable(WayTrans)
endif()
