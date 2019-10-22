set(project_sources mainwindow.cpp main.cpp diagramscene.cpp screentriangle.cpp screenboard.cpp newgamedialog.cpp)
set(project_headers mainwindow.h diagramscene.h screentriangle.h screenboard.h newgamedialog.h)
set(project_ui mainwindow.ui newgamedialog.ui)
# wrap the ui file to a c++ header
qt5_wrap_ui(ui_wrap ${project_ui})
qt5_wrap_cpp(moc_sources ${project_headers})

#add_library (BoardLib triangle.cpp triangle.h trianglegrid.cpp trianglegrid.h util.h board.h board.cpp)
include_directories (../lib)
add_executable(TrigoTrain 
	${project_sources} 
	${project_headers}
	${ui_wrap} 
	${moc_sources})
target_link_libraries (TrigoTrain
    PUBLIC
    BoardLib
	Qt5::Core
	Qt5::Gui 
	Qt5::Widgets)
#   ${SHARK_LIBRARIES})
