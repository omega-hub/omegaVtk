#include <vtkActor.h>
#include <vtkProperty.h>
#include <vtkOpenGLRenderer.h>
#include <vtkLight.h>

//#include "omegaVtk/PyVtk.h"
#include "omega/PythonInterpreter.h"
#include "VtkModule.h"
//#include "VtkRenderPass.h"
#include "VtkAttachPoint.h"

using namespace omega;
using namespace omegaVtk;

#if defined(OMEGA_OS_WIN)
#define VTK_LIBRARY_DIR_POSTFIX "/bin/Release"
#else
#define VTK_LIBRARY_DIR_POSTFIX 
#endif

static VtkModule* sInstance = NULL;

///////////////////////////////////////////////////////////////////////////////
VtkModule* VtkModule::instance()
{
    return sInstance;
}

///////////////////////////////////////////////////////////////////////////////
VtkModule* VtkModule::createAndInitialize()
{
    if(sInstance == NULL)
    {
        sInstance = new VtkModule();
        ModuleServices::addModule(sInstance);
        sInstance->doInitialize(Engine::instance());
    }
    return sInstance;
}

///////////////////////////////////////////////////////////////////////////////
VtkModule::VtkModule(): EngineModule("VtkModule")
{
    sInstance = this;
    myActiveClient = NULL;
}

///////////////////////////////////////////////////////////////////////////////
VtkModule::~VtkModule()
{
    sInstance = NULL;
}

///////////////////////////////////////////////////////////////////////////////
void VtkModule::initialize()
{
    omsg("VtkModule initializing...");

    PythonInterpreter* interp = SystemManager::instance()->getScriptInterpreter();
    interp->addPythonPath(VTK_LIBRARY_DIR VTK_LIBRARY_DIR_POSTFIX);
    interp->addPythonPath(VTK_PYTHON_DIR);
    
    omsg("VtkModule initialization OK");
}

///////////////////////////////////////////////////////////////////////////////
void VtkModule::dispose()
{
}

///////////////////////////////////////////////////////////////////////////////
void VtkModule::initializeRenderer(Renderer* r)
{
    VtkRenderPass* vtkrp = new VtkRenderPass(r, "VtkRenderPass");
    vtkrp->setUserData(this);
    r->addRenderPass(vtkrp);
    myRenderPasses.push_back(vtkrp);

    typedef Dictionary<SceneNode*, VtkAttachPoint*>::Item DItem;
    foreach(DItem item, myAttachPoints)
    {
        item->queueProps(vtkrp);
    }
}

///////////////////////////////////////////////////////////////////////////////
void VtkModule::attachProp(vtkProp3D* actor, SceneNode* node)
{
    VtkAttachPoint* vtkap = myAttachPoints[node];
    if(vtkap == NULL)
    {
        ofmsg("VtkModule::attachProp: creating attach point for node %1%", %node->getName());
        vtkap = new VtkAttachPoint();
        myAttachPoints[node] = vtkap;
        node->addComponent(vtkap);
    }
    vtkap->attachProp(actor);
    vtkap->requestBoundingBoxUpdate();
}

///////////////////////////////////////////////////////////////////////////////
void VtkModule::detachProp(vtkProp3D* actor, SceneNode* node)
{
    VtkAttachPoint* vtkap = myAttachPoints[node];
    if(vtkap != NULL)
    {
        vtkap->detachProp(actor);
    }
}

///////////////////////////////////////////////////////////////////////////////
void VtkModule::update(const UpdateContext& context)
{
    bool itemDirty = false;
    bool allRenderPassesInitialized = true;

    // Reset render queues only if all render passes are initialized.
    foreach(VtkRenderPass* rp, myRenderPasses) allRenderPassesInitialized &= rp->isInitialized();
    
    if(allRenderPassesInitialized)
    {
        typedef Dictionary<SceneNode*, VtkAttachPoint*>::Item DItem;

        // See if any attach point is dirty.
        foreach(DItem item, myAttachPoints)	itemDirty |= item->isDirty();

        // If any item is dirty, reset render queues for all renderers
        if(itemDirty)
        {
            foreach(VtkRenderPass* rp, myRenderPasses)
            {
                rp->resetPropQueues();
                foreach(DItem item, myAttachPoints)
                {
                    item->queueProps(rp);
                }
            }
        }
    }
}

///////////////////////////////////////////////////////////////////////////////
void VtkModule::addLight(vtkLight* light)
{
    foreach(VtkRenderPass* rp, myRenderPasses) rp->getRenderer()->AddLight(light);
}

///////////////////////////////////////////////////////////////////////////////
void VtkModule::removeLight(vtkLight* light)
{
    foreach(VtkRenderPass* rp, myRenderPasses) rp->getRenderer()->RemoveLight(light);
}

///////////////////////////////////////////////////////////////////////////////
void VtkModule::removeAllLights()
{
    foreach(VtkRenderPass* rp, myRenderPasses) rp->getRenderer()->RemoveAllLights();
}

