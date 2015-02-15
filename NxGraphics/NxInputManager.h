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
#ifndef __NXINPUTMANAGER_H__
#define __NXINPUTMANAGER_H__
 
#include "NxPrerequisites.h"

namespace Nx {
 
//!  A class to Manage Inputs ( keyboard, Mouse, Joysticks )
class NxGraphics_Export NxInputManager : public NxSingleton<NxInputManager> // public OIS::KeyListener, public OIS::MouseListener, public OIS::JoyStickListener,
{
public :
	//! NxInputManager class Constructor
	NxInputManager();
	//! NxInputManager class Destructor.
	~NxInputManager();
	//! NxInputManager Singleton.
	static NxInputManager& getSingleton();
	//! NxInputManager Singleton Pointer.
	static NxInputManager* getSingletonPtr();
	//! Create.
	void AddWindow( NxRenderWindow * window );
	//! Add listener
	void AddListener( NxRenderWindow * Window, NxInputListener * Listener ); 
	//! Remove listener
	void RemoveListener( NxInputListener * Listener );
	//! Show Mouse Cursor.
	void ShowMouseCursor( bool Show );
	//! Update Screen Bounds.
	void UpdateBounds( float Width, float Height );
	//! Pump new mouse and keyboard values.
	void Update( float ms );
	//! Get Current Down Key.
	bool IsKeyDown( NxKeyCode Code );
	//! Get Current Mouse Button State
	bool IsMouseButtonDown( NxMouseButtonID ButtonId );
	//! Send key Status.
	bool SetKey( NxKeyCode Code, bool KeyUp );

	void SetMouseButtonsActive( bool Active );

	int GetMouseRelativeZ();

private :
	//! get mouse state.
	const OIS::MouseState& GetMouseState();
	bool HasActiveInput();
	NxInput * GetActiveInput();
	std::vector<NxInput*> mInputs;
};

}

#endif
