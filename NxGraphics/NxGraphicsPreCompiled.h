/*
-----------------------------------------------------------------------------
This file is part of NxGraphics, Multimedia Engine.
Author : Stephane Kyles. Developed in Prague, Czech Republic.
		_   __       ______                     __     _            
	   / | / /_  __ / ____/_____ ____ _ ____   / /_   (_)_____ _____
	  /  |/ /| |/_// / __ / ___// __ `// __ \ / __ \ / // ___// ___/
	 / /|  /_>  < / /_/ // /   / /_/ // /_/ // / / // // /__ (__  ) 
	/_/ |_//_/|_| \____//_/    \__,_// .___//_/ /_//_/ \___//____/  
									/_/                             
Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
THE SOFTWARE.
-----------------------------------------------------------------------------
*/
#pragma once
#pragma warning(disable:4006)

#if !defined _CRT_SECURE_NO_DEPRECATE && _MSC_VER > 1300
#define _CRT_SECURE_NO_DEPRECATE /* to avoid multiple Visual Studio 2005 warnings */
#endif
 
#include "NxGraphics.h"
#include "NxMath.h"
#include "NxVector2.h"
#include "NxVector3.h"
#include "NxVector4.h"
#include "NxMatrix3.h"
#include "NxMatrix4.h"
#include "NxStringConverter.h"
#include "NxListeners.h"
#include "NxAnimation.h"
#include "NxAnimationManager.h"
#include "NxColourValue.h"
#include "NxFrameListener.h"
#include "NxEngine.h"
#include "NxWindow.h"
#include "NxViewport.h"
#include "NxContentManager.h"
#include <NxPhysicsActor.h>
#include <NxPhysicsManager.h>
#include "NxCamera.h"
#include "NxEntitySound.h"
#include "NxLight.h"
#include "NxEntityCharacter.h"
#include "NxEntityCharacterController.h"
#include "NxTerrain.h"
#include "NxTerrainGrass.h"
#include "NxTerrainTree.h"
#include "NxEntityFluid.h"
#include "NxParticleSystem.h"
#include "NxParticleEmitter.h"
#include "NxParticleSystemAffector.h"
#include "NxEntityCloth.h"
#include "NxEntityVehicule.h"
#include "NxLine.h"
#include "NxObject.h"
#include "NxScene.h"
#include <NxPhysicsScene.h>
#include "NxText.h"
#include "NxTriangles.h"
#include "NxSceneManager.h"
#include "NxNode.h"
#include "NxSceneGizmo.h"
#include "NxUtils.h"
#include "NxLayer.h"
#include "NxLog.h"



#include "NxOverlay.h"
#include "NxOverlayContainer.h"
#include "NxOverlayElement.h"
#include "NxOverlayText.h"
#include "NxOverlayManager.h"
#include "NxScreen.h"
#include "NxScreenManager.h"
#include "NxFontManager.h"
#include "NxImage.h"
#include "NxInputManager.h"
#include "NxImportXfl.h"
#include "NxPostEffectManager.h"
#include "NxPlugin.h"

#include "NxStateManager.h"
#include "NxScriptManager.h"
#include "NxTexture.h"
#include "NxTextureManager.h"
#include "NxFrameBuffer.h"
#include "NxFrameBufferRecorder.h"
#include "NxParserXml.h"
#include "NxParserJson.h"
#include "NxRectangle2D.h"

#include "NxMaterialManager.h"
#include "NxMaterial.h"
#include "NxTechnique.h"
#include "NxPass.h"
#include "NxTextureUnit.h"
#include "NxShader.h"

//

#include "NxGui/NxGuiButton.h"
#include "NxGui/NxGuiButtonToggle.h"
#include "NxGui/NxGuiButtonRadio.h"
#include "NxGui/NxGuiColours.h"
#include "NxGui/NxGuiElement.h"
#include "NxGui/NxGuiLabel.h"
#include "NxGui/NxGuiLine.h"
#include "NxGui/NxGuiManager.h"
#include "NxGui/NxGuiPanel.h"
#include "NxGui/NxGuiRectangle.h"
#include "NxGui/NxGuiSlider.h"
#include "NxGui/NxGuiText.h"
#include "NxGui/NxGuiWidget.h"
#include "NxGui/NxGuiDragAndDropContainer.h"
#include "NxGui/NxGuiDragAndDropTarget.h"


#include <ogre.h>
using namespace Ogre;

#define GetInputManager() NxInputManager::getSingletonPtr()
#define GetScreenManager() NxScreenManager::getSingletonPtr()
#define LogMsg( msg ) NxLog::getSingleton().LogMessage( msg ) 