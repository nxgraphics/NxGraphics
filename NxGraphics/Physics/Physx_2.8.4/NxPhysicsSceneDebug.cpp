#include <NxGraphicsPreCompiled.h>
#include <NxPhysicsInclude.h>
#include <NxPhysicsDraw.h>

namespace Nx {

NxPhysics3DDebugDraw::NxPhysics3DDebugDraw( Nx3D_Scene * Scene ) : CurrentScene( Scene ), mDebuggerView(0)
{
	assert( CurrentScene->GetNxSceneManager() );

	mDebuggerView = true ;

	mVisualDebugger = CurrentScene->GetNxSceneManager()->createManualObject("mVisualDebugger");
	mVisualDebugger->setCastShadows(false);

	mVisualDebuggerNode = CurrentScene->GetNxSceneManager()->getRootSceneNode()->createChildSceneNode();
	mVisualDebuggerNode->attachObject( mVisualDebugger );

	MaterialPtr mVisualDebuggerMaterial = MaterialManager::getSingleton().create("mVisualDebuggerMaterial","General"); 
	mVisualDebuggerMaterial->setReceiveShadows(false); 
	mVisualDebuggerMaterial->getTechnique(0)->setLightingEnabled(false);
 
	mVisualDebugger->begin("BaseWhiteNoLighting", Ogre::RenderOperation::OT_POINT_LIST);
	mVisualDebugger->end();
	mVisualDebugger->begin("BaseWhiteNoLighting", Ogre::RenderOperation::OT_LINE_LIST);
	mVisualDebugger->end();
	mVisualDebugger->begin("BaseWhiteNoLighting", Ogre::RenderOperation::OT_TRIANGLE_LIST);
	mVisualDebugger->end();

	AxisAlignedBox aabInf;
	aabInf.setInfinite();
	mVisualDebugger->setBoundingBox(aabInf);
}

NxPhysics3DDebugDraw::~NxPhysics3DDebugDraw()
{
	mVisualDebuggerNode->detachAllObjects();
	mVisualDebuggerNode->getParentSceneNode()->removeAndDestroyChild(mVisualDebuggerNode->getName());
	mVisualDebuggerNode = NULL;
	mVisualDebugger->clear();
	CurrentScene->GetNxSceneManager()->destroyManualObject(mVisualDebugger);
	mVisualDebugger = NULL;
}

void convertColor( NxU32 color, float & r, float & g, float & b)
{
	b = NxF32((color)&0xff)/255.0f;
	g = NxF32((color>>8)&0xff)/255.0f;
	r = NxF32((color>>16)&0xff)/255.0f;
}

void NxPhysics3DDebugDraw::Clear()
{
	if( mVisualDebugger )
	{
		mVisualDebugger->clear();
	}
}

void NxPhysics3DDebugDraw::Draw()
{
	if (mDebuggerView)
	{ 
		mVisualDebugger->clear();
		const NxDebugRenderable * data = CurrentScene->GetPhysicScene()->GetPhysicsScene3D().getDebugRenderable();
		unsigned int numVertices = data->getNbPoints();
		numVertices += data->getNbLines() * 2;
		numVertices += data->getNbTriangles() * 3;
		mVisualDebugger->estimateVertexCount(numVertices);
 
		mVisualDebugger->begin("BaseWhiteNoLighting", Ogre::RenderOperation::OT_POINT_LIST);
		for(unsigned int i = 0; i < data->getNbPoints(); ++i)
		{
			const NxVec3 &pos = data->getPoints()[i].p;
			mVisualDebugger->position( pos[0], pos[1], pos[2] );

			float r,g,b;
			convertColor( data->getPoints()[i].color, r,g,b );
			mVisualDebugger->colour( r ,g ,b );
		}
		mVisualDebugger->end();
 
		mVisualDebugger->begin("BaseWhiteNoLighting", Ogre::RenderOperation::OT_LINE_LIST);
		for(unsigned int i = 0; i < data->getNbLines(); ++i)
		{
			float r,g,b;
			convertColor(data->getLines()[i].color, r,g,b);

			const NxVec3 &pos1 = data->getLines()[i].p0;
			mVisualDebugger->position(pos1[0], pos1[1], pos1[2]);
			mVisualDebugger->colour(r,g,b);

			const NxVec3 &pos2 = data->getLines()[i].p1;
			mVisualDebugger->position(pos2[0], pos2[1], pos2[2]);
			mVisualDebugger->colour(r,g,b);
		}
		mVisualDebugger->end();

 		mVisualDebugger->begin("BaseWhiteNoLighting", Ogre::RenderOperation::OT_TRIANGLE_LIST);
		for(unsigned int i = 0; i < data->getNbTriangles(); ++i)
		{
			float r,g,b;
			convertColor(data->getTriangles()[i].color, r,g,b);

			const NxVec3 &pos1 = data->getTriangles()[i].p0;
			mVisualDebugger->position(pos1[0], pos1[1], pos1[2]);
			mVisualDebugger->colour(r,g,b);

			const NxVec3 &pos2 = data->getTriangles()[i].p1;
			mVisualDebugger->position(pos2[0], pos2[1], pos2[2]);
			mVisualDebugger->colour(r,g,b);

			const NxVec3 &pos3 = data->getTriangles()[i].p2;
			mVisualDebugger->position(pos3[0], pos3[1], pos3[2]);
			mVisualDebugger->colour(r,g,b);
		}
		mVisualDebugger->end();

	}
}

void NxPhysics3DDebugDraw::SetViewDebug( bool View )
{
	if( mVisualDebugger )
	{
		mVisualDebugger->setVisible(View); 
	}
}

void NxPhysics3DDebugDraw::FlipDebug()
{
	mDebuggerView = !mDebuggerView;
	if (!mDebuggerView)
	{
		mVisualDebugger->clear();
		mVisualDebugger->begin("mVisualDebuggerMaterial", Ogre::RenderOperation::OT_LINE_LIST); 
		mVisualDebugger->position(Ogre::Vector3(0,0,0));
		mVisualDebugger->position(Ogre::Vector3(0,1.8,0));
		mVisualDebugger->position(Ogre::Vector3(0,1.8,0));
		mVisualDebugger->position(Ogre::Vector3(4,1.8,0));
		mVisualDebugger->position(Ogre::Vector3(0,1.8,0));
		mVisualDebugger->position(Ogre::Vector3(0,1.8,2));
		mVisualDebugger->end();
	}
}

}