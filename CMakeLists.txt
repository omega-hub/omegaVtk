# vtk configuration, libraries and applications
find_package(VTK)
if(NOT VTK_DIR)
  message(FATAL_ERROR "Please set VTK_DIR.")
endif(NOT VTK_DIR)
include(${VTK_USE_FILE})

add_subdirectory(omegaVtk)
add_subdirectory(examples/vtkhello)

# Pass the vtk include dirs as a cache variable, so other modules can use it.
set(VTK_INCLUDES ${VTK_INCLUDE_DIRS} CACHE INTERNAL "")
