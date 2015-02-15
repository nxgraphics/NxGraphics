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
#ifndef __NXGRAPHICSFRAMEBUFFRECORDER_H__
#define __NXGRAPHICSFRAMEBUFFRECORDER_H__

#include "NxPrerequisites.h"

namespace Nx {

class NxGraphics_Export NxGraphicsRecorder : public NxFrameListener
{
public :
	NxGraphicsRecorder();
	~NxGraphicsRecorder();
	void StartRecording( const std::string & Filename, int Width, int Height, int Bpp );
	void StopRecording();
	void AddFrame( unsigned long elapsedMS );
private:
	bool frameRenderingQueued(const NxFrameEvent& evt);
	NxVideo_Avi_Recorder * mRecorder;
	unsigned char * RecordingVideoBuffer;
	int RecordingFps ;
	bool VideoRecord ;
	std::list< unsigned char * > RecordingList ;

	Ogre::HardwarePixelBuffer * RecordingTextureRT ;
	Ogre::Viewport * RecordingViewport ;
	Ogre::PixelBox  * RecordingPixelBox ;
};

}
#endif