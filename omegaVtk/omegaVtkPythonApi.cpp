#include <vtkPython.h>

#include "PyVTKObject.h"
#include "omega/PythonInterpreter.h"
#include "VtkModule.h"
#include "omega/PythonInterpreterWrapper.h"

using namespace omegaVtk;
using namespace omega;

////////////////////////////////////////////////////////////////////////////////
void attachProp(boost::python::object pyactor, SceneNode* node)
{
	if(pyactor != NULL && node != NULL)
	{
		PyVTKObject* vtkactor = (PyVTKObject*)pyactor.ptr();
		VtkModule::instance()->attachProp((vtkProp3D*)vtkactor->vtk_ptr, node);
	}
}

////////////////////////////////////////////////////////////////////////////////
void detachProp(boost::python::object pyactor, SceneNode* node)
{
	if(pyactor != NULL && node != NULL)
	{
		PyVTKObject* vtkactor = (PyVTKObject*)pyactor.ptr();
		VtkModule::instance()->detachProp((vtkProp3D*)vtkactor->vtk_ptr, node);
	}
}

////////////////////////////////////////////////////////////////////////////////
void addLight(boost::python::object pylight)
{
	if(pylight != NULL)
	{
		PyVTKObject* vtklight = (PyVTKObject*)pylight.ptr();
		VtkModule::instance()->addLight((vtkLight*)vtklight->vtk_ptr);
	}
}

////////////////////////////////////////////////////////////////////////////////
void removeLight(boost::python::object pylight)
{
	if(pylight != NULL)
	{
		PyVTKObject* vtklight = (PyVTKObject*)pylight.ptr();
		VtkModule::instance()->removeLight((vtkLight*)vtklight->vtk_ptr);
	}
}

////////////////////////////////////////////////////////////////////////////////
void removeAllLights()
{
	VtkModule::instance()->removeAllLights();
}

////////////////////////////////////////////////////////////////////////////////
BOOST_PYTHON_MODULE(omegaVtk)
{
	// SceneLoader
	PYAPI_REF_BASE_CLASS(VtkModule)
		PYAPI_STATIC_REF_GETTER(VtkModule, createAndInitialize)
		//PYAPI_METHOD(VtkModule, attachProp)
		//PYAPI_METHOD(VtkModule, detachProp)
		;

	def("vtkAttachProp", attachProp);
	def("vtkDetachProp", detachProp);
	def("vtkAddLight", addLight);
	def("vtkRemoveLight", removeLight);
	def("vtkRemoveAllLights", removeAllLights);
}

////////////////////////////////////////////////////////////////////////////////
void OVTK_API omegaVtkPythonApiInit()
{
	static bool sApiInitialized = false;

	if(!sApiInitialized)
	{
		omsg("omegaVtkPythonApiInit()");
		//omega::PythonInterpreter* interp = SystemManager::instance()->getScriptInterpreter();
		//interp->addModule("omegaVtk", ovtkMethods);
		initomegaVtk();
	}
}

