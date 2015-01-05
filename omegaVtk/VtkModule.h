#ifndef __VTK_MODULE_H__
#define __VTK_MODULE_H__

#include "ovtkbase.h"
#include "oVtkRenderPass.h"
#include "VtkAttachPoint.h"

#include "omega/osystem.h"
//#include "omega/Renderer.h"
#include "omega/Engine.h"
#include "omega/Application.h"

class vtkLight;

namespace omegaVtk
{
	using namespace omega;

	///////////////////////////////////////////////////////////////////////////
	class OVTK_API VtkModule: public EngineModule
	{
	public:
	friend class VtkAttachPoint;
		//! Creates and initializes the vtk module singleton.
		//! If called multiple times, subsequent calls will do nothing.
		static VtkModule* createAndInitialize();
	public:
		VtkModule();
		~VtkModule();

		static VtkModule* instance();

		virtual void initialize();
		virtual void dispose();
		virtual void initializeRenderer(Renderer* r);
		virtual void update(const UpdateContext& context);
		virtual void handleEvent(const Event& evt) {}

		void attachProp(vtkProp3D* actor, SceneNode* node);
		void detachProp(vtkProp3D* actor, SceneNode* node);
		void addLight(vtkLight* light);
		void removeLight(vtkLight* light);
		void removeAllLights();

	private:
		Dictionary<SceneNode*, VtkAttachPoint*> myAttachPoints;

		Lock myClientLock;
		List<VtkRenderPass*> myRenderPasses;
		Renderer* myActiveClient;
	};
};
#endif