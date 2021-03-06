/******************************************************************************
 * THE OMEGA LIB PROJECT
 *-----------------------------------------------------------------------------
 * Copyright 2010-2014		Electronic Visualization Laboratory, 
 *							University of Illinois at Chicago
 * Authors:										
 *  Alessandro Febretti		febret@gmail.com
 *-----------------------------------------------------------------------------
 * Copyright (c) 2010-2013, Electronic Visualization Laboratory,  
 * University of Illinois at Chicago
 * All rights reserved.
 * Redistribution and use in source and binary forms, with or without modification, 
 * are permitted provided that the following conditions are met:
 * 
 * Redistributions of source code must retain the above copyright notice, this 
 * list of conditions and the following disclaimer. Redistributions in binary 
 * form must reproduce the above copyright notice, this list of conditions and 
 * the following disclaimer in the documentation and/or other materials provided 
 * with the distribution. 
 * 
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" 
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO THE 
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE 
 * ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE 
 * LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL 
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE  GOODS OR 
 * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER 
 * CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, 
 * OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE 
 * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *-----------------------------------------------------------------------------
 * What's in this file
 *	The implementation of an omegalib render pass that wraps the VTK drawing
 *  engine.
 ******************************************************************************/
#ifndef __VTK_RENDER_PASS_H__
#define __VTK_RENDER_PASS_H__

#include "omega/osystem.h"
#include "omega/RenderPass.h"

class vtkProp;
class vtkOpenGLRenderer;
class vtkOpaquePass;
class vtkLightsPass;
class CustomDepthPeelingPass;
class vtkTranslucentPass;
class vtkOverlayPass;
class vtkGenericOpenGLRenderWindow;
class vtkRenderState;
class vtkVolumetricPass;

namespace omegaVtk
{
	using namespace omega;

	///////////////////////////////////////////////////////////////////////////
	class VtkRenderPass: public RenderPass
	{
	friend class VtkRenderable;
	public:
		enum QueueType { QueueOpaque, QueueTransparent, QueueVolume, QueueOverlay, NumQueues };
		static const int MaxQueuedProps = 128;
		static RenderPass* createInstance(Renderer* client) { return new VtkRenderPass(client, "VtkRenderPass"); }

	public:
		VtkRenderPass(Renderer* client, const String& name);
		virtual ~VtkRenderPass();

		void initialize();
		virtual void render(Renderer* client, const DrawContext& context);
		void queueProp(vtkProp* actor, QueueType queue);
		void resetPropQueues();
		vtkOpenGLRenderer* getRenderer();

	private:
		vtkOpenGLRenderer* myRenderer;
		vtkRenderState* myRenderState;
		vtkGenericOpenGLRenderWindow* myRenderWindow;

		vtkProp* myPropQueue[NumQueues][MaxQueuedProps];
		int myPropQueueSize[NumQueues];

		// Rendering passes
		vtkLightsPass* myLightsPass;
		vtkOpaquePass* myOpaquePass;
		CustomDepthPeelingPass* myDepthPeelingPass;
		vtkTranslucentPass* myTranslucentPass;
		vtkOverlayPass* myOverlayPass;
		vtkVolumetricPass* myVolumetricPass;
	};
};
#endif