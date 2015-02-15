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
#ifndef __NXOVERLAYMANAGER_H__
#define __NXOVERLAYMANAGER_H__

#include "NxPrerequisites.h"

namespace Nx {

class NxGraphics_Export NxOverlayManager : public NxSingleton<NxOverlayManager>
{
public :
	//! NxOverlayManager class constructor
	NxOverlayManager();
	//! NxOverlayManager class destructor
	~NxOverlayManager();
	//! NxOverlayManager Singleton.
	static NxOverlayManager& getSingleton();
	//! NxOverlayManager Singleton.
	static NxOverlayManager* getSingletonPtr();
	//! Create an Overlay.
	NxOverlay * CreateOverlay( const std::string & Name );
	//! Create a container
	NxOverlayContainer * CreateContainer( const std::string & PanelName );
	//! Create a text element
	NxOverlayText * CreateTextElement( const std::string & TextElementName );
};

class NxLoadingBar;
class NxGraphics_Export ProgressBar 
{
public:
	ProgressBar();
	void setProgress( float progress);
	float getProgress();
	const std::string & getCaption();
	void setCaption( const std::string& caption);
	const std::string & getComment();
	void setComment(const std::string & comment);
	NxOverlayContainer * GetContainer();
	void Start( const Nx::Vector4 & size, const std::string & texturename, unsigned short NumGroups, unsigned short NumGroupsLoad, float Proportion );
	void Finish();
protected:
	NxOverlayText* mTextArea;
	NxOverlayText* mCommentTextArea;
	NxOverlayContainer* mMeter;
	NxOverlayElement* mFill;
	NxOverlayContainer * mElement;
	float mProgress;
private :
	NxLoadingBar * mLoadingBar;
};

}//namespace

#endif