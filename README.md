== omegaVtk ==

The omegaVtk module makes it possible to render VTK visualization pipelines (http://www.vtk.org/) inside omegalib. 


== How To Install ==
This module ia tested with vtk 5.10. Download and build vtk 5.10 from here: http://www.vtk.org/VTK/resources/software.html#previous
The module will probably work with vtk 6.0 but has not been tested with that version yet.

After enabling the module in the omegalib CMake options, the module will be downloaded and will look up for vtk automatically.
If vtk is not found, you can set the `VTK_DIR` Cmake option manually, pointing it to the directory where you built vtk.

Do do all of the above from the command line, assuming `[omegaBuild]` is the omegalib build directory and `[vtkBuild]` is the vtk build directory:
```
> cd [omegaBuild]
> cmake -DMODULES="omegaVtk" -DVTK_DIR="[vtkBuild]"
> make
(to run an example)
> bin/orun -s omegaVtk/examples/python/Sample.py
```
