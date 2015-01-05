add_definitions( -D_CRT_SECURE_NO_WARNINGS -DOVTK_EXPORTING -DVTK_LIBRARY_DIR="${VTK_DIR}" -DVTK_PYTHON_DIR="${VTK_DIR}/Wrapping/Python")

set( srcs 
        vtkGenericOpenGLRenderWindow.cpp
        CustomDepthPeelingPass.cpp
        VtkModule.cpp
        VtkRenderPass.cpp
        VtkAttachPoint.cpp
        )
        
if(OMEGA_USE_PYTHON)
set( srcs ${srcs}
        #PyVtk.cpp
        omegaVtkPythonApi.cpp
        )
endif()
        
set( headers 
        # omega
        CustomDepthPeelingPass.h
        ovtkbase.h
        #PyVtk.h
        VtkModule.h
        vtkGenericOpenGLRenderWindow.h
        # NOTE: the weird name for this file is needed because VtkRenderPass.h
        # clashes with the VTK file vtkRenderPass.h
        oVtkRenderPass.h
        VtkAttachPoint.h
        omegaVtkPythonApi.h
        ) 
        
source_group(omegaVtk REGULAR_EXPRESSION omegaVtk/)		

###############################################################################
# Setup compile info
add_library(omegaVtk SHARED ${srcs} ${headers})
add_library(omegaVtk_loader SHARED omegaVtk_loader.cpp)
target_link_libraries(omegaVtk_loader omegaVtk)


###############################################################################
# Setup link info.
target_link_libraries( omegaVtk 
    omega 
    omegaToolkit
    ${VTK_LIBRARIES}
    )

set_target_properties(omegaVtk_loader PROPERTIES OUTPUT_NAME "omegaVtk" IMPORT_PREFIX "module_" PREFIX "")
if(WIN32)
    set_target_properties(omegaVtk_loader PROPERTIES FOLDER modules SUFFIX ".pyd" PDB_NAME "omegaVtk_loader")
    set_target_properties(omegaVtk PROPERTIES FOLDER modules)
else()
    set_target_properties(omegaVtk_loader PROPERTIES SUFFIX ".so")
endif()
    