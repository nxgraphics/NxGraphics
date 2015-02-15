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

#include <OgreQuaternion.h>
#include <OgreVector2.h>
#include <OgreVector3.h>
#include <OgreVector4.h>

#include <OgreMatrix3.h>
#include <OgreMatrix4.h>

using namespace Ogre;


#include "../../nxdeps/include/Physx/v2.8.4_win/SDKs/Physics/include/NxPhysics.h"
#include "../../nxdeps/include/Physx/v2.8.4_win/SDKs/Cooking/include/Nxcooking.h"
#include "../../nxdeps/include/Physx/v2.8.4_win/SDKs/Physics/include/NxUtilLib.h"
#include "../../nxdeps/include/Physx/v2.8.4_win/SDKs/NxCharacter/include/NxControllerManager.h"
#include "../../nxdeps/include/Physx/v2.8.4_win/SDKs/NxCharacter/include/NxBoxController.h"
#include "../../nxdeps/include/Physx/v2.8.4_win/SDKs/NxCharacter/include/NxCapsuleController.h"
#include "../../nxdeps/include/Physx/v2.8.4_win/Samples/SampleCommonCode/src/UserAllocator.h"
#include "../../nxdeps/include/Physx/v2.8.4_win/Samples/SampleCommonCode/src/ErrorStream.h"
#include "../../nxdeps/include/Physx/v2.8.4_win/Samples/SampleCommonCode/src/MediaPath.h"
#include "../../nxdeps/include/Physx/v2.8.4_win/Samples/SampleCommonCode/src/SamplesVRDSettings.h"
#include "../../nxdeps/include/Physx/v2.8.4_win/Samples/SampleCommonCode/src/stream.h"
#include "../../nxdeps/include/Physx/v2.8.4_win/Tools/NxuStream2/NXU_helper.h"

inline Ogre::Vector3 VecToOgre( NxVec3 vec ) 
{
   return Ogre::Vector3(vec.x, vec.y, vec.z);
}

inline Nx::Vector3 PhysxToNxVec3( NxVec3 vec ) 
{
	return Nx::Vector3(vec.x, vec.y, vec.z);
}

inline NxVec3 VecToPhysx( Ogre::Vector3 vec ) 
{
   return NxVec3(vec.x, vec.y, vec.z);
}

inline NxVec3 NxVec3ToPhysx( Nx::Vector3 vec ) 
{
	return NxVec3(vec.x, vec.y, vec.z);
}
 
inline Ogre::Quaternion ogrequat( NxQuat quat ) 
{
   return Ogre::Quaternion( quat.w, quat.x, quat.y, quat.z );
}

inline Nx::Quaternion PhysxToQuatNx( NxQuat quat ) 
{
	return Nx::Quaternion( quat.w, quat.x, quat.y, quat.z );
}

inline NxQuat nxquat( Ogre::Quaternion quat ) 
{
   NxQuat new_quat;
   new_quat.setWXYZ( quat.w, quat.x, quat.y, quat.z );
   return new_quat;
}

inline NxQuat NxQuatToPhysx( Nx::Quaternion quat ) 
{
	NxQuat new_quat;
	new_quat.setWXYZ( quat.w, quat.x, quat.y, quat.z );
	return new_quat;
}