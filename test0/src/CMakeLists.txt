set(project_sources mainwindow.cpp main.cpp)
set(project_headers mainwindow.h)
set(project_ui mainwindow.ui)
# wrap the ui file to a c++ header
qt5_wrap_ui(ui_wrap ${project_ui})
qt5_wrap_cpp(moc_sources ${project_headers})

add_library (BoardLib triangle.cpp triangle.h trianglegrid.cpp trianglegrid.h)
add_executable(board 
	${project_sources} 
	${project_headers}
	${ui_wrap} 
	${moc_sources})
target_link_libraries (board 
    PUBLIC
    BoardLib
	Qt5::Core
	Qt5::Gui 
	Qt5::Widgets
   ${SHARK_LIBRARIES})
