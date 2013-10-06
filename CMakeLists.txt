# vtk configuration, libraries and applications
find_package(VTK)
if(NOT VTK_DIR)
  message(FATAL_ERROR "Please set VTK_DIR.")
endif(NOT VTK_DIR)
include(${VTK_USE_FILE})

add_subdirectory(omegaVtk)
add_subdirectory(examples/vtkhello)
