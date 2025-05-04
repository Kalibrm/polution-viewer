cmake_minimum_required(VERSION 3.16)

project(polution_viewer VERSION 0.1 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD_REQUIRED ON)


find_package(Qt6 REQUIRED COMPONENTS Quick Network Test Graphs)
qt_standard_project_setup(REQUIRES 6.8)

set(SOURCES
    src/apimanager.cpp
    src/jsonrepository.cpp
    src/requestcontext.cpp
    src/appcontroller.cpp
)

set(HEADERS
    src/apimanager.h
    src/jsonrepository.h
    src/requestcontext.h
    src/appcontroller.h
)

qt_add_library(polution_viewer_lib
    SHARED
    ${SOURCES}
    ${HEADERS}
)

target_link_libraries(polution_viewer_lib
    PRIVATE
    Qt6::Core
    Qt6::Network
)

qt_add_executable(apppolution_viewer
    src/main.cpp
)

target_link_libraries(apppolution_viewer
    PRIVATE polution_viewer_lib
    Qt6::Core
    Qt6::Quick
    Qt6::Network
    Qt6::Graphs
)

qt_add_qml_module(apppolution_viewer
    URI polution_viewer
    VERSION 1.0
    QML_FILES src/MainView.qml
    QML_FILES src/StationListView.qml
    QML_FILES src/SensorListView.qml
    QML_FILES src/SensorDataView.qml
)

target_include_directories(apppolution_viewer PRIVATE src)

# Qt for iOS sets MACOSX_BUNDLE_GUI_IDENTIFIER automatically since Qt 6.1.
# If you are developing for iOS or macOS you should consider setting an
# explicit, fixed bundle identifier manually though.
set_target_properties(apppolution_viewer PROPERTIES
#    MACOSX_BUNDLE_GUI_IDENTIFIER com.example.apppolution_viewer
    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
    MACOSX_BUNDLE TRUE
    WIN32_EXECUTABLE TRUE
)

include(GNUInstallDirs)
install(TARGETS apppolution_viewer
    BUNDLE DESTINATION .
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)

enable_testing()

set(TEST_SOURCES
    tests/testjsonrepository.cpp
    tests/testrequestcontext.cpp
)

set(TEST_HEADERS
    tests/testjsonrepository.h
    tests/testrequestcontext.h
)

add_executable(polution_viewer_test
    ${TEST_SOURCES}
    ${TEST_HEADERS}
    tests/main.cpp
)

target_link_libraries(polution_viewer_test
    PRIVATE polution_viewer_lib
    Qt6::Core
    Qt6::Test
    Qt6::Network
)

add_test(NAME AllTests COMMAND polution_viewer_test)
