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
#ifndef __NXGRAPHICSPOSTEFFEMANAGER_H__
#define __NXGRAPHICSPOSTEFFEMANAGER_H__

#include "NxPrerequisites.h"

namespace Nx {

class NxGraphics_Export NxPostEffect
{
public:
	NxPostEffect( const std::string & Name, const std::string & MaterialName );
	~NxPostEffect();
	void SetActive( bool Active );
private :
	Ogre::Compositor * mEffect;
	MaterialNx * mMaterial;

};

class DOFManager;
class NxGraphics_Export NxPostEffectDof
{
public :
	NxPostEffectDof( Ogre::Viewport * View );
	~NxPostEffectDof();
	void SetActive( bool Active );
	void SetFocalDepths(float nearDepth, float focalDepth, float farDepth );
	void ZoomView(float delta);
	void Aperture(float delta);
	void MoveFocus(float delta);
	void SetZoom(float f);
	void SetAperture(float f);
	void SetFocus(float f);
	float GetNearDepth();
	float GetFocalDepth();
	float GetFarDepth();
private :
	DOFManager * mDofManager;
};  


//!  A class to manage post effects.
/*!
	Singleton Class.
*/
class NxGraphics_Export NxPostEffectManager : public NxSingleton<NxPostEffectManager>
{
public :
	//! PostEffectManager class Constructor.
	NxPostEffectManager();
	//! PostEffectManager class Destructor.
	~NxPostEffectManager();
	//! PostEffectManager Singleton.
	static NxPostEffectManager& getSingleton();
	//! PostEffectManager Singleton.	
	static NxPostEffectManager* getSingletonPtr();

	NxPostEffect * CreateEffect( const std::string& PostEffectName, const std::string & MaterialName );

	//! add a compositor.	
	void AddCompositor( NxViewport * view, const std::string& PostEffectName );
	//! Enable a post effect by name.	
	void SetCompositorEnabled( NxViewport * view, const std::string& PostEffectName, bool enabled );
	//! get Post Effect List.
	void GetAvailableEffects( std::vector<std::string> & PostEffectList );
	//! modify a compositor parameters / compositor / material / parameter / value.
	void setUniform( std::string compositor, std::string material, std::string uniform, float value, bool setVisible, int position );
};

}  // NxGraphics_Namespace 
#endif