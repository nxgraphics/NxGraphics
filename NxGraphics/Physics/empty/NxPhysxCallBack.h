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
#ifndef __NXGRAPHICS3DPHYSXCALLBACK_H__
#define __NXGRAPHICS3DPHYSXCALLBACK_H__

#include <NxPrerequisites.h>

#include "NxPhysicsInclude.h"

namespace Nx {

class Nx3D_SceneCallback : public NxUserContactReport, public NxUserTriggerReport
{
public:

	Nx3D_SceneCallback( Nx3D_Scene * CurrentScene ) : Scene( CurrentScene ) {
	
	}

	void onContactNotify( NxContactPair& pair, unsigned int events )
	{
		if ( pair.isDeletedActor[0] || pair.isDeletedActor[1] ) return;
		NxActor  * actor1 =  pair.actors[0];
		NxActor  * actor2 =  pair.actors[1];
		NxScene3DObjectEntity * ActorOne = NULL;
		NxScene3DObjectEntity * ActorTwo = NULL;
		float ResultNumPoints = 0.0f;

		if ( actor1 && actor2 )
		{
			NxContactStreamIterator i(pair.stream);
			while(i.goNextPair())
			{
				//user can also call getShape() and getNumPatches() here
				while(i.goNextPatch())
				{
					//user can also call getPatchNormal() and getNumPoints() here
					//contactReport.nxVec3crContactNormal = i.getPatchNormal();

					//do sum of all the pojnts
					int NumPoints = 0;
					float NumPointsSumForce = 0.0f;
					while(i.goNextPoint())
					{
						NumPoints++;
						NumPointsSumForce += i.getPointNormalForce();
						//user can also call getShape() and getNumPatches() here
						// contactReport.lsContactPoints.push_back(i.getPoint());
						// contactReport.nxVec3crContactNormalForce = pair.sumNormalForce;
						// contactReport.nxVec3crContactFrictionForce = pair.sumFrictionForce;
					}// end while(i.goNextPoint())

					ResultNumPoints = NumPointsSumForce / NumPoints;
							
				}// end while(i.goNextPatch())
			}// end while(i.goNextPair())

			bool SendNotification = false;
			NxEntityEvents TypeEvent;

			if( events & NX_IGNORE_PAIR)
			{
				//Log( " NX_IGNORE_PAIR returning ..." );
				return ;
			}
			else if(events & NX_NOTIFY_ON_START_TOUCH )
			{
				TypeEvent = NxEntityTouchStart;
				SendNotification = false;
			} 
			else if(events & NX_NOTIFY_ON_END_TOUCH )
			{
				TypeEvent = NxEntityTouchEnd;
				SendNotification = false;
			}
			else if(  NX_NOTIFY_ON_START_TOUCH_FORCE_THRESHOLD )
			{
				TypeEvent = NxEntityTouchStart;
				SendNotification = true;
			}
			else if(  NX_NOTIFY_ON_END_TOUCH_FORCE_THRESHOLD )
			{
				TypeEvent = NxEntityTouchEnd;
				SendNotification = true;
			}

			/*
			ActorOne = Scene->GetNxEntityByNxActor( actor1 );
			ActorTwo = Scene->GetNxEntityByNxActor( actor2 );

			if(ActorOne)
			{
				if(SendNotification)
				{ 
					//ActorOne->OnRaisedEvent( TypeEvent , ResultNumPoints ); 
				}
			}

		   if(ActorTwo)
		   {
			   if(SendNotification)
			   { 
				   //ActorTwo->OnRaisedEvent( TypeEvent, ResultNumPoints );
			   }
		   }	
		   */
		}
	}

	void onTrigger( NxShape& triggerShape, NxShape& otherShape, NxTriggerFlag status )
	{
		 
		/*
			NxScene3DObjectEntity * ActorOne = NULL;
			NxScene3DObjectEntity * ActorTwo = NULL;
			ActorOne = Scene->GetNxEntityByNxActor( &triggerShape.getActor() );
			ActorTwo = Scene->GetNxEntityByNxActor( &otherShape.getActor() );

			NxEntityEvents TypeEvent;
 			bool SendNotification = false;

			if( status == NX_TRIGGER_ON_ENTER )
			{
				TypeEvent = NxEntityTouchStart;
				SendNotification = true;
			} 
			else if( status == NX_TRIGGER_ON_LEAVE )
			{
				TypeEvent = NxEntityTouchEnd;
				SendNotification = true;
			}

			if( SendNotification )
			{
				for( std::list<Nx3DSceneListener  *>::iterator iter = Scene->ListenersCallbackList.begin(); iter != Scene->ListenersCallbackList.end(); iter++)
				{
					Nx3DSceneListener  * listen = *iter; 
					listen->OnCollide( ActorOne, ActorTwo, TypeEvent );
				}
			}
			*/
	}

	private:
	Nx3D_Scene * Scene;

};


class NxGraphics_Export NxScene3DPhysics3DNotify : public NXU_userNotify, public NXU_errorReport
{
public:
	void NXU_errorMessage( bool isError, const char *str )
	{
		LogMsg( "Physics Error : " + std::string( str ) );
	}
	void NXU_notifyScene( NxU32 sno, NxScene * scene, const char * userProperties )
	{

	}
	void NXU_notifyJoint( NxJoint * joint, const char * userProperties )
	{

	}
	void NXU_notifyActor( NxActor * actor, const char * userProperties )
	{

	}
	void NXU_notifyCloth( NxCloth * cloth, const char * userProperties )
	{

	}
	void NXU_notifyFluid( NxFluid * fluid, const char * userProperties )
	{

	}
	void NXU_notifyTriangleMesh( NxTriangleMesh *t, const char * userProperties )
	{



	}
	void NXU_notifyConvexMesh( NxConvexMesh *c, const char * userProperties )
	{

	}
	void NXU_notifyClothMesh( NxClothMesh *t, const char * userProperties )
	{

	}
	void NXU_notifyCCDSkeleton( NxCCDSkeleton *t, const char *userProperties )
	{

	}
	void NXU_notifyHeightField( NxHeightField *t, const char *userProperties )
	{

	}
	NxScene * NXU_preNotifyScene( unsigned int sno, NxSceneDesc &scene, const char * userProperties )
	{
		return 0;
	}
	bool NXU_preNotifyJoint( NxJointDesc &joint, const char	*userProperties)
	{ 
		return true;
	}
	bool NXU_preNotifyActor( NxActorDesc &actor, const char * userProperties )
	{ 
		return true;
	}
	bool NXU_preNotifyCloth( NxClothDesc &cloth, const char * userProperties )
	{ 
		return true;
	}
#if NX_USE_SOFTBODY_API
	bool NXU_preNotifySoftBody( NxSoftBodyDesc &softBody, const char * userProperties )
	{ 
		return true;
	}
	void NXU_notifySoftBody( NxSoftBody * softBody, const char * userProperties )
	{

	}
	void NXU_notifySoftBodyMesh( NxSoftBodyMesh *t, const char * userProperties )
	{

	}
	bool NXU_preNotifySoftBodyMesh( NxSoftBodyMeshDesc &t, const char * userProperties )
	{ 
		return true;
	}
	void NXU_notifySoftBodyFailed( NxSoftBodyDesc &softBody, const char * userProperties )
	{

	}
	void NXU_notifySoftBodyMeshFailed( NxSoftBodyMeshDesc &t, const char * userProperties )
	{

	}
#endif
#if NX_USE_FLUID_API
	bool NXU_preNotifyFluid( NxFluidDesc &fluid, const char * userProperties )
	{ 
		return true;
	}
#endif
	bool NXU_preNotifyTriangleMesh( NxTriangleMeshDesc &t, const char * userProperties )
	{ 

		return true;
	}
	bool NXU_preNotifyConvexMesh( NxConvexMeshDesc &t, const char * userProperties )
	{ 
		return true;
	}
	bool NXU_preNotifyClothMesh( NxClothMeshDesc &t, const char * userProperties )
	{ 
		return true;
	}
	bool NXU_preNotifyCCDSkeleton( NxSimpleTriangleMesh &t, const char *userProperties )
	{ 
		return true;
	}
	bool NXU_preNotifyHeightField( NxHeightFieldDesc &t, const char * userProperties )
	{ 
		return true;
	}
	bool NXU_preNotifySceneInstance( const char *name, const char *sceneName, const char * userProperties, NxMat34 &rootNode, NxU32 depth )
	{ 
		return true;
	}
	void NXU_notifySceneFailed( unsigned int sno, NxSceneDesc &scene, const char * userProperties )
	{

	}
	void NXU_notifyJointFailed( NxJointDesc &joint, const char	*userProperties)
	{

	}
	void NXU_notifyActorFailed( NxActorDesc &actor, const char	*userProperties)
	{
		LogMsg( "Physics Error : Actor Failed" );

	}
	void NXU_notifyClothFailed( NxClothDesc &cloth, const char * userProperties )
	{

	}
#if NX_USE_FLUID_API
	void NXU_notifyFluidFailed( NxFluidDesc &fluid, const char * userProperties )
	{

	}
#endif
	void NXU_notifyTriangleMeshFailed( NxTriangleMeshDesc &t, const char *userProperties )
	{

	}
	void NXU_notifyConvexMeshFailed( NxConvexMeshDesc &t, const char *userProperties )
	{

	}
	void NXU_notifyClothMeshFailed( NxClothMeshDesc &t, const char *userProperties )
	{

	}
	void NXU_notifyCCDSkeletonFailed( NxSimpleTriangleMesh &t, const char *userProperties )
	{

	}
	void NXU_notifyHeightFieldFailed( NxHeightFieldDesc &t, const char *userProperties )
	{

	}
};


}

#endif