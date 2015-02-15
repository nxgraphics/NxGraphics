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
#ifndef __NXGRAPHICSPOSTEFFECTHDR_H__
#define __NXGRAPHICSPOSTEFFECTHDR_H__

#include "NxPrerequisites.h"
#include "../nxdeps/include/OGRE/OgreCompositorInstance.h"
#include "../nxdeps/include/OGRE/OgreCompositorLogic.h"

namespace Nx {

	//The simple types of compositor logics will all do the same thing -
	//Attach a listener to the created compositor
	class ListenerFactoryLogic : public Ogre::CompositorLogic
	{
	public:
		/** @copydoc CompositorLogic::compositorInstanceCreated */
		virtual void compositorInstanceCreated(Ogre::CompositorInstance* newInstance) 
		{
			Ogre::CompositorInstance::Listener* listener = createListener(newInstance);
			newInstance->addListener(listener);
			mListeners[newInstance] = listener;
		}

		/** @copydoc CompositorLogic::compositorInstanceDestroyed */
		virtual void compositorInstanceDestroyed(Ogre::CompositorInstance* destroyedInstance)
		{
			delete mListeners[destroyedInstance];
			mListeners.erase(destroyedInstance);
		}

	protected:
		//This is the method that implementations will need to override
		virtual Ogre::CompositorInstance::Listener* createListener(Ogre::CompositorInstance* instance) = 0;
	private:
		typedef std::map<Ogre::CompositorInstance*, Ogre::CompositorInstance::Listener*> ListenerMap;
		ListenerMap mListeners;

	};

	//The compositor logic for the hdr compositor
	class HDRLogic : public ListenerFactoryLogic
	{
	protected:
		/** @copydoc ListenerFactoryLogic::createListener */
		virtual Ogre::CompositorInstance::Listener* createListener(Ogre::CompositorInstance* instance);
	};

	//The compositor logic for the gaussian blur compositor
	class GaussianBlurLogic : public ListenerFactoryLogic
	{
	protected:
		/** @copydoc ListenerFactoryLogic::createListener */
		virtual Ogre::CompositorInstance::Listener* createListener(Ogre::CompositorInstance* instance);
	};
 
}

#endif