cmake_minimum_required(VERSION 3.0.0 FATAL_ERROR)
project(cmake0)

find_package(Qt5 COMPONENTS REQUIRED Core Gui Widgets)

find_package(Shark REQUIRED)
include(${SHARK_USE_FILE})

set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(project_sources mainwindow.cpp main.cpp)
set(project_headers mainwindow.h)
set(project_ui mainwindow.ui)
# wrap the ui file to a c++ header
qt5_wrap_ui(ui_wrap ${project_ui})
qt5_wrap_cpp(moc_sources ${project_headers})
add_executable(cmake0 
	${project_sources} 
	${project_headers}
	${ui_wrap} 
	${moc_sources})
target_link_libraries(cmake0 
	PUBLIC
	Qt5::Core
	Qt5::Gui 
	Qt5::Widgets
    ${SHARK_LIBRARIES})
#target_link_libraries(cmake0 ${SHARK_LIBRARIES})

