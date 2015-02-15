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
#ifndef __NXGRAPHICSLISTENERS_H__
#define __NXGRAPHICSLISTENERS_H__
 
#include "NxPrerequisites.h"

namespace Nx {

class NxGraphics_Export NxInputListener
{
public :
	NxInputListener(){}
	virtual ~NxInputListener(){}
	virtual void OnKeyPressed( const NxKeyCode &e ) = 0;
	virtual void OnKeyReleased( const NxKeyCode &e ) = 0;
	virtual void OnMouseMoved( const OIS::MouseEvent &e ) = 0;
	virtual void OnMouseButtonPressed( const OIS::MouseEvent &e, NxMouseButtonID id ) = 0;
	virtual void OnMouseButtonReleased( const OIS::MouseEvent &e, NxMouseButtonID id ) = 0;
	//! Joystick callbacks
	virtual void OnJoystickButtonPressed( const OIS::JoyStickEvent &arg, int button ) = 0;
	virtual void OnJoystickButtonReleased( const OIS::JoyStickEvent &arg, int button ) = 0;
	virtual void OnJoystickAxisMoved( const OIS::JoyStickEvent &arg, int axis ) = 0;

};

class NxGraphics_Export NxMixerManagerListener
{
public :
	NxMixerManagerListener(){}
	virtual ~NxMixerManagerListener(){}
	virtual void ReceivedStatus( const std::string & task, const std::string & textstatus, float pourcent ){};
	virtual void OnSolo( const NxLayer & Layer, bool Activate ){};
	virtual void OnMute( const NxLayer & Layer, bool Activate ){};
//	virtual void OnPosition( const NxLayer & Layer, const NxVector3 NewPosition ){};
//	virtual void OnScale(  const NxLayer & Layer, const NxVector3 NewPosition ){};
//	virtual void OnRotation( const NxLayer & Layer, const NxVector3 NewPosition ){};
	virtual void OnOpacity( const NxLayer & Layer, const float Opacity ){};

};

}


#endif