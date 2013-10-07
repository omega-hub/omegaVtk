add_executable(vtkhello vtkScene.cpp)
set_target_properties(vtkhello PROPERTIES FOLDER examples)
target_link_libraries(vtkhello
	omega 
	omegaToolkit
    omegaVtk
	vtkRendering)
