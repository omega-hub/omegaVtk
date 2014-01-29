## omegaVtk 

The omegaVtk module makes it possible to render VTK visualization pipelines (http://www.vtk.org/) inside omegalib. 


### How To Install
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

### Running vtk python scripts in omegalib
The general structure of a vtk/omegalib python script is the following
```python
# Import and initialize the omegaVtk module
from omegaVtk import *
VtkModule.createAndInitialize()

# Import the root vtk module. This MUST be done after initializing omegaVtk.
import vtk

# VTK CODE GOES HERE
# Note: Do not create vtk renderers or render windows. Omegalib will take care of it.
# All you need to do is attaching your vtk actors and lights to an omegalib scene node:
actor = vtk.vtkActor()
actor.SetMapper([mapper from your pipeline goes here])

vtkNode = SceneNode.create("vtkNode")

# Use this special omegaVtk function to attach actor to vtkNode
# you can use vtkAttachLight to do the same with vtk light objects.
vtkAttachProp(actor, vtkNode)

# All transformations applied to the omegalib scene node will apply to attached vtk actors:
vtkNode.setPosition(0, 2, -2) 
```

You can look at a complete example here: https://github.com/omega-hub/omegaVtk/blob/master/examples/python/Sample.py

### C++ vtk programming in omegalib
See this guide: https://github.com/uic-evl/omegalib/wiki/BasicVtk
And this example: https://github.com/omega-hub/omegaVtk/blob/master/examples/vtkhello/vtkScene.cpp
