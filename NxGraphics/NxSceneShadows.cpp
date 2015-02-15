#include "NxGraphicsPreCompiled.h"
#include "NxSceneShadows.h"

using namespace Ogre;

namespace Nx {

class LightMaterialGeneratorCG : public MaterialGenerator::Impl
{
public:
	typedef MaterialGenerator::Perm Perm;
	LightMaterialGeneratorCG(const String &baseName): mBaseName(baseName) 
	{

	}
	virtual ~LightMaterialGeneratorCG()
	{

	}

	virtual GpuProgramPtr generateVertexShader(Perm permutation)
	{
        String programName = "DeferredShading/post/";

		if (permutation & LightMaterialGenerator::MI_DIRECTIONAL)
		{
			programName += "vs";
		}
		else
		{
			programName += "LightMaterial_vs";
		}

		GpuProgramPtr ptr = HighLevelGpuProgramManager::getSingleton().getByName(programName);
		assert(!ptr.isNull());
		return ptr;
	}

	virtual GpuProgramPtr generateFragmentShader(Perm permutation)
	{
		/// Create shader
		if (mMasterSource.empty())
		{
			DataStreamPtr ptrMasterSource = ResourceGroupManager::getSingleton().openResource( "DeferredShading/post/LightMaterial_ps.cg", ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME);
			assert(ptrMasterSource.isNull()==false);
			mMasterSource = ptrMasterSource->getAsString();
		}

		assert(mMasterSource.empty()==false);

		// Create name
		String name = mBaseName+StringConverter::toString(permutation)+"_ps";		

		// Create shader object
		HighLevelGpuProgramPtr ptrProgram = HighLevelGpuProgramManager::getSingleton().createProgram(
			name, ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME, "cg", GPT_FRAGMENT_PROGRAM);
		ptrProgram->setSource(mMasterSource);
		ptrProgram->setParameter("entry_point","main");
	    ptrProgram->setParameter("profiles","ps_2_x arbfp1");
		// set up the preprocessor defines
		// Important to do this before any call to get parameters, i.e. before the program gets loaded
		ptrProgram->setParameter("compile_arguments", getPPDefines(permutation));

		setUpBaseParameters(ptrProgram->getDefaultParameters());

		return GpuProgramPtr(ptrProgram);
	}

	virtual MaterialPtr generateTemplateMaterial(Perm permutation)
	{
		String materialName = mBaseName;
	
        if(permutation & LightMaterialGenerator::MI_DIRECTIONAL)
		{   
			materialName += "Quad";
		}
		else
		{
			materialName += "Geometry";
		}

		if(permutation & LightMaterialGenerator::MI_SHADOW_CASTER)
		{
			materialName += "Shadow";
		}
		return MaterialManager::getSingleton().getByName(materialName);
	}

	protected:
		String mBaseName;
        String mMasterSource;
		// Utility method
		String getPPDefines(Perm permutation)
		{
			String strPPD;

			//Get the type of light
			String lightType;
			if (permutation & LightMaterialGenerator::MI_POINT)
			{
				lightType = "POINT";
			}
			else if (permutation & LightMaterialGenerator::MI_SPOTLIGHT)
			{
				lightType = "SPOT";
			}
			else if (permutation & LightMaterialGenerator::MI_DIRECTIONAL)
			{
				lightType = "DIRECTIONAL";
			}
			else
			{
				assert(false && "Permutation must have a light type");
			}
			strPPD += "-DLIGHT_TYPE=LIGHT_" + lightType + " ";

			//Optional parameters
            if (permutation & LightMaterialGenerator::MI_SPECULAR)
			{
				strPPD += "-DIS_SPECULAR ";
			}
			if (permutation & LightMaterialGenerator::MI_ATTENUATED)
			{
				strPPD += "-DIS_ATTENUATED ";
			}
			if (permutation & LightMaterialGenerator::MI_SHADOW_CASTER)
			{
				strPPD += "-DIS_SHADOW_CASTER ";
			}
			return strPPD;
		}

		void setUpBaseParameters(const GpuProgramParametersSharedPtr& params)
		{
			assert(params.isNull()==false);

			struct AutoParamPair { String name; GpuProgramParameters::AutoConstantType type; };	

			//A list of auto params that might be present in the shaders generated
			static const AutoParamPair AUTO_PARAMS[] = {
				{ "vpWidth",			GpuProgramParameters::ACT_VIEWPORT_WIDTH },
				{ "vpHeight",			GpuProgramParameters::ACT_VIEWPORT_HEIGHT },
				{ "worldView",			GpuProgramParameters::ACT_WORLDVIEW_MATRIX },
				{ "invProj",			GpuProgramParameters::ACT_INVERSE_PROJECTION_MATRIX },
				{ "invView",			GpuProgramParameters::ACT_INVERSE_VIEW_MATRIX },
				{ "flip",				GpuProgramParameters::ACT_RENDER_TARGET_FLIPPING },
				{ "lightDiffuseColor",	GpuProgramParameters::ACT_LIGHT_DIFFUSE_COLOUR },
				{ "lightSpecularColor", GpuProgramParameters::ACT_LIGHT_SPECULAR_COLOUR },
				{ "lightFalloff",		GpuProgramParameters::ACT_LIGHT_ATTENUATION },
				{ "lightPos",			GpuProgramParameters::ACT_LIGHT_POSITION_VIEW_SPACE },
				{ "lightDir",			GpuProgramParameters::ACT_LIGHT_DIRECTION_VIEW_SPACE },
				{ "spotParams",			GpuProgramParameters::ACT_SPOTLIGHT_PARAMS },
				{ "farClipDistance",	GpuProgramParameters::ACT_FAR_CLIP_DISTANCE },
				{ "shadowViewProjMat",	GpuProgramParameters::ACT_TEXTURE_VIEWPROJ_MATRIX }
			};
			int numParams = sizeof(AUTO_PARAMS) / sizeof(AutoParamPair);

			for (int i=0; i<numParams; i++)
			{
				if (params->_findNamedConstantDefinition(AUTO_PARAMS[i].name))
				{
					params->setNamedAutoConstant(AUTO_PARAMS[i].name, AUTO_PARAMS[i].type);
				}
			}
		}
};

LightMaterialGenerator::LightMaterialGenerator()
{
	vsMask = 0x00000004;
	fsMask = 0x0000003F;
	matMask =	LightMaterialGenerator::MI_DIRECTIONAL | 
				LightMaterialGenerator::MI_SHADOW_CASTER;
	
	materialBaseName = "DeferredShading/LightMaterial/";
    mImpl = new LightMaterialGeneratorCG("DeferredShading/LightMaterial/");
}

LightMaterialGenerator::~LightMaterialGenerator()
{

}

void GeomUtils::createSphere(  const String& strName
							 , float radius
							 , int nRings, int nSegments
							 , bool bNormals
							 , bool bTexCoords)
{
	MeshPtr pSphere = MeshManager::getSingleton().createManual(strName, ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME);
	SubMesh *pSphereVertex = pSphere->createSubMesh();
	pSphere->sharedVertexData = new VertexData();

	createSphere(pSphere->sharedVertexData, pSphereVertex->indexData
		, radius
		, nRings, nSegments
		, bNormals // need normals
		, bTexCoords // need texture co-ordinates
		);

	// Generate face list
	pSphereVertex->useSharedVertices = true;

	// the original code was missing this line:
	pSphere->_setBounds(  AxisAlignedBox( Ogre::Vector3(-radius, -radius, -radius), Ogre::Vector3(radius, radius, radius) ), false );
	pSphere->_setBoundingSphereRadius(radius);
	// this line makes clear the mesh is loaded (avoids memory leaks)
	pSphere->load();
}

void GeomUtils::createSphere(VertexData*& vertexData, IndexData*& indexData
						 , float radius
						 , int nRings, int nSegments
						 , bool bNormals
						 , bool bTexCoords)
{
	assert(vertexData && indexData);

	// define the vertex format
	VertexDeclaration* vertexDecl = vertexData->vertexDeclaration;
	size_t currOffset = 0;
	// positions
	vertexDecl->addElement(0, currOffset, VET_FLOAT3, VES_POSITION);
	currOffset += VertexElement::getTypeSize(VET_FLOAT3);

	if (bNormals)
	{
		// normals
		vertexDecl->addElement(0, currOffset, VET_FLOAT3, VES_NORMAL);
		currOffset += VertexElement::getTypeSize(VET_FLOAT3);

	}
	// two dimensional texture coordinates
	if (bTexCoords)
	{
		vertexDecl->addElement(0, currOffset, VET_FLOAT2, VES_TEXTURE_COORDINATES, 0);
		currOffset += VertexElement::getTypeSize(VET_FLOAT2);
	}

	// allocate the vertex buffer
	vertexData->vertexCount = (nRings + 1) * (nSegments+1);
	HardwareVertexBufferSharedPtr vBuf = HardwareBufferManager::getSingleton().createVertexBuffer(vertexDecl->getVertexSize(0), vertexData->vertexCount, HardwareBuffer::HBU_STATIC_WRITE_ONLY, false);
	VertexBufferBinding* binding = vertexData->vertexBufferBinding;
	binding->setBinding(0, vBuf);
	float* pVertex = static_cast<float*>(vBuf->lock(HardwareBuffer::HBL_DISCARD));

	// allocate index buffer
	indexData->indexCount = 6 * nRings * (nSegments + 1);
	indexData->indexBuffer = HardwareBufferManager::getSingleton().createIndexBuffer(HardwareIndexBuffer::IT_16BIT, indexData->indexCount, HardwareBuffer::HBU_STATIC_WRITE_ONLY, false);
	HardwareIndexBufferSharedPtr iBuf = indexData->indexBuffer;
	unsigned short* pIndices = static_cast<unsigned short*>(iBuf->lock(HardwareBuffer::HBL_DISCARD));

	float fDeltaRingAngle = (Math::PI / nRings);
	float fDeltaSegAngle = (2 * Math::PI / nSegments);
	unsigned short wVerticeIndex = 0 ;

	// Generate the group of rings for the sphere
	for( int ring = 0; ring <= nRings; ring++ ) {
		float r0 = radius * sinf (ring * fDeltaRingAngle);
		float y0 = radius * cosf (ring * fDeltaRingAngle);

		// Generate the group of segments for the current ring
		for(int seg = 0; seg <= nSegments; seg++) {
			float x0 = r0 * sinf(seg * fDeltaSegAngle);
			float z0 = r0 * cosf(seg * fDeltaSegAngle);

			// Add one vertex to the strip which makes up the sphere
			*pVertex++ = x0;
			*pVertex++ = y0;
			*pVertex++ = z0;

			if (bNormals)
			{
				Vector3 vNormal = Vector3(x0, y0, z0).normalisedCopy();
				*pVertex++ = vNormal.x;
				*pVertex++ = vNormal.y;
				*pVertex++ = vNormal.z;
			}
			if (bTexCoords)
			{
				*pVertex++ = (float) seg / (float) nSegments;
				*pVertex++ = (float) ring / (float) nRings;			
			}

			if (ring != nRings) 
			{
				// each vertex (except the last) has six indices pointing to it
				*pIndices++ = wVerticeIndex + nSegments + 1;
				*pIndices++ = wVerticeIndex;               
				*pIndices++ = wVerticeIndex + nSegments;
				*pIndices++ = wVerticeIndex + nSegments + 1;
				*pIndices++ = wVerticeIndex + 1;
				*pIndices++ = wVerticeIndex;
				wVerticeIndex ++;
			}
		}; // end for seg
	} // end for ring

	// Unlock
	vBuf->unlock();
	iBuf->unlock();
}

void GeomUtils::createQuad(VertexData*& vertexData)
{
	assert(vertexData);

	vertexData->vertexCount = 4;
	vertexData->vertexStart = 0;

	VertexDeclaration* vertexDecl = vertexData->vertexDeclaration;
	VertexBufferBinding* bind = vertexData->vertexBufferBinding;

	vertexDecl->addElement(0, 0, VET_FLOAT3, VES_POSITION);

	HardwareVertexBufferSharedPtr vbuf = 
		HardwareBufferManager::getSingleton().createVertexBuffer(
		vertexDecl->getVertexSize(0),
		vertexData->vertexCount,
		HardwareBuffer::HBU_STATIC_WRITE_ONLY);

	// Bind buffer
	bind->setBinding(0, vbuf);
	// Upload data
	float data[]={
		-1,1,-1,  // corner 1
		-1,-1,-1, // corner 2
		1,1,-1,   // corner 3
		1,-1,-1}; // corner 4
		vbuf->writeData(0, sizeof(data), data, true);
}

void GeomUtils::createCone(const Ogre::String& strName , float radius , float height, int nVerticesInBase)
{
	MeshPtr pCone = MeshManager::getSingleton().createManual(strName, ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME);
	SubMesh *pConeVertex = pCone->createSubMesh();
	pCone->sharedVertexData = new VertexData();

	createCone(pCone->sharedVertexData, pConeVertex->indexData
		, radius
		, height
		, nVerticesInBase);

	// Generate face list
	pConeVertex->useSharedVertices = true;

	// the original code was missing this line:
	pCone->_setBounds( AxisAlignedBox( 
		Ogre::Vector3(-radius, 0, -radius), 
		Ogre::Vector3(radius, height, radius) ), false );

	pCone->_setBoundingSphereRadius(Math::Sqrt(height*height + radius*radius));
	// this line makes clear the mesh is loaded (avoids memory leaks)
	pCone->load();
}


void GeomUtils::createCone(Ogre::VertexData*& vertexData, Ogre::IndexData*& indexData, 
					   float radius , float height, int nVerticesInBase)
{
	assert(vertexData && indexData);

	// define the vertex format
	VertexDeclaration* vertexDecl = vertexData->vertexDeclaration;
	// positions
	vertexDecl->addElement(0, 0, VET_FLOAT3, VES_POSITION);
	
	// allocate the vertex buffer
	vertexData->vertexCount = nVerticesInBase + 1;
	HardwareVertexBufferSharedPtr vBuf = HardwareBufferManager::getSingleton().createVertexBuffer(vertexDecl->getVertexSize(0), vertexData->vertexCount, HardwareBuffer::HBU_STATIC_WRITE_ONLY, false);
	VertexBufferBinding* binding = vertexData->vertexBufferBinding;
	binding->setBinding(0, vBuf);
	float* pVertex = static_cast<float*>(vBuf->lock(HardwareBuffer::HBL_DISCARD));

	// allocate index buffer - cone and base
	indexData->indexCount = (3 * nVerticesInBase) + (3 * (nVerticesInBase - 2));
	indexData->indexBuffer = HardwareBufferManager::getSingleton().createIndexBuffer(HardwareIndexBuffer::IT_16BIT, indexData->indexCount, HardwareBuffer::HBU_STATIC_WRITE_ONLY, false);
	HardwareIndexBufferSharedPtr iBuf = indexData->indexBuffer;
	unsigned short* pIndices = static_cast<unsigned short*>(iBuf->lock(HardwareBuffer::HBL_DISCARD));

	//Positions : cone head and base
	for (int i=0; i<3; i++)
		*pVertex++ = 0.0f;

	//Base :
	float fDeltaBaseAngle = (2 * Math::PI) / nVerticesInBase;
	for (int i=0; i<nVerticesInBase; i++)
	{
		float angle = i * fDeltaBaseAngle;
		*pVertex++ = radius * cosf(angle);
		*pVertex++ = height;
		*pVertex++ = radius * sinf(angle);
	}

	//Indices :
	//Cone head to vertices
	for (int i=0; i<nVerticesInBase; i++)
	{
		*pIndices++ = 0;
		*pIndices++ = (i%nVerticesInBase) + 1;
		*pIndices++ = ((i+1)%nVerticesInBase) + 1;
	}
	//Cone base
	for (int i=0; i<nVerticesInBase-2; i++)
	{
		*pIndices++ = 1;
		*pIndices++ = i + 3;
		*pIndices++ = i + 2;
	}

	// Unlock
	vBuf->unlock();
	iBuf->unlock();
}


AmbientLight::AmbientLight()
{
	setRenderQueueGroup(RENDER_QUEUE_2);

	mRenderOp.vertexData = new VertexData();
	mRenderOp.indexData = 0;

	GeomUtils::createQuad(mRenderOp.vertexData);

	mRenderOp.operationType = RenderOperation::OT_TRIANGLE_STRIP; 
	mRenderOp.useIndexes = false; 

	// Set bounding
	setBoundingBox(AxisAlignedBox(-10000,-10000,-10000,10000,10000,10000));
	mRadius = 15000;

	mMatPtr = MaterialManager::getSingleton().getByName("DeferredShading/AmbientLight");
	assert(mMatPtr.isNull()==false);
	mMatPtr->load();

    //This shader needs to be aware if its running under OpenGL or DirectX.
    //Real depthFactor = (Root::getSingleton().getRenderSystem()->getName() ==
    //    "OpenGL Rendering Subsystem") ? 2.0 : 1.0;
    //mMatPtr->getTechnique(0)->getPass(0)->getFragmentProgramParameters()->setNamedConstant(
    //        "depthFactor", depthFactor);
}

AmbientLight::~AmbientLight()
{
	// need to release IndexData and vertexData created for renderable
	delete mRenderOp.indexData;
	delete mRenderOp.vertexData;
}

/** @copydoc MovableObject::getBoundingRadius */
Real AmbientLight::getBoundingRadius(void) const
{
	return mRadius;

}
/** @copydoc Renderable::getSquaredViewDepth */
Real AmbientLight::getSquaredViewDepth(const Camera*) const
{
	return 0.0;
}

const MaterialPtr& AmbientLight::getMaterial(void) const
{
	return mMatPtr;
}

void AmbientLight::getWorldTransforms(Ogre::Matrix4* xform) const
{
	*xform = Ogre::Matrix4::IDENTITY;
}
void AmbientLight::updateFromCamera(Ogre::Camera* camera)
{
	Ogre::Technique* tech = getMaterial()->getBestTechnique();
	Ogre::Vector3 farCorner = camera->getViewMatrix(true) * camera->getWorldSpaceCorners()[4];

	for (unsigned short i=0; i<tech->getNumPasses(); i++) 
	{
		Ogre::Pass* pass = tech->getPass(i);
		// get the vertex shader parameters
		Ogre::GpuProgramParametersSharedPtr params = pass->getVertexProgramParameters();
		// set the camera's far-top-right corner
		if (params->_findNamedConstantDefinition("farCorner"))
			params->setNamedConstant("farCorner", farCorner);
	    
		params = pass->getFragmentProgramParameters();
		if (params->_findNamedConstantDefinition("farCorner"))
			params->setNamedConstant("farCorner", farCorner);
	}
}


#define ENABLE_BIT(mask, flag) (mask) |= (flag)
#define DISABLE_BIT(mask, flag) (mask) &= ~(flag)

 
//-----------------------------------------------------------------------
DLight::DLight(MaterialGenerator *sys, Ogre::Light* parentLight):
    mParentLight(parentLight), bIgnoreWorld(false), mGenerator(sys), mPermutation(0)
{
	// Set up geometry
	// Allocate render operation
	mRenderOp.operationType = RenderOperation::OT_TRIANGLE_LIST;
	mRenderOp.indexData = 0;
	mRenderOp.vertexData = 0;
	mRenderOp.useIndexes = true;

	updateFromParent();
}
//-----------------------------------------------------------------------
DLight::~DLight()
{
	// need to release IndexData and vertexData created for renderable
    delete mRenderOp.indexData;
    delete mRenderOp.vertexData;
}
//-----------------------------------------------------------------------
void DLight::setAttenuation(float c, float b, float a)
{
	// Set Attenuation parameter to shader
	//setCustomParameter(3, Vector4(c, b, a, 0));
	float outerRadius = mParentLight->getAttenuationRange();
	/// There is attenuation? Set material accordingly
	if(c != 1.0f || b != 0.0f || a != 0.0f)
	{
		ENABLE_BIT(mPermutation, LightMaterialGenerator::MI_ATTENUATED);
		if (mParentLight->getType() == Light::LT_POINT)
		{
			//// Calculate radius from Attenuation
			int threshold_level = 10;// difference of 10-15 levels deemed unnoticeable
			float threshold = 1.0f/((float)threshold_level/256.0f); 

			//// Use quadratic formula to determine outer radius
			c = c-threshold;
			float d=sqrt(b*b-4*a*c);
			outerRadius = (-2*c)/(b+d);
			outerRadius *= 1.2;
		}
	}
	else
	{
		DISABLE_BIT(mPermutation,LightMaterialGenerator::MI_ATTENUATED);
	}
    
	rebuildGeometry(outerRadius);
}
//-----------------------------------------------------------------------
void DLight::setSpecularColour(const ColourValue &col)
{
	//setCustomParameter(2, Vector4(col.r, col.g, col.b, col.a));
	/// There is a specular component? Set material accordingly
	
	if(col.r != 0.0f || col.g != 0.0f || col.b != 0.0f)
		ENABLE_BIT(mPermutation,LightMaterialGenerator::MI_SPECULAR);
	else
		DISABLE_BIT(mPermutation,LightMaterialGenerator::MI_SPECULAR);
		
}
//-----------------------------------------------------------------------
void DLight::rebuildGeometry(float radius)
{
	//Disable all 3 bits
	DISABLE_BIT(mPermutation, LightMaterialGenerator::MI_POINT);
	DISABLE_BIT(mPermutation, LightMaterialGenerator::MI_SPOTLIGHT);
	DISABLE_BIT(mPermutation, LightMaterialGenerator::MI_DIRECTIONAL);

	switch (mParentLight->getType())
	{
	case Light::LT_DIRECTIONAL:
		createRectangle2D();
        ENABLE_BIT(mPermutation,LightMaterialGenerator::MI_DIRECTIONAL);
		break;
	case Light::LT_POINT:
		/// XXX some more intelligent expression for rings and segments
		createSphere(radius, 10, 10);
		ENABLE_BIT(mPermutation,LightMaterialGenerator::MI_POINT);
		break;
	case Light::LT_SPOTLIGHT:
		Real height = mParentLight->getAttenuationRange();
		Ogre::Radian coneRadiusAngle = mParentLight->getSpotlightOuterAngle() / 2;
        Real radius = Ogre::Math::Tan(coneRadiusAngle) * height;
		createCone(radius, height, 20);
		ENABLE_BIT(mPermutation,LightMaterialGenerator::MI_SPOTLIGHT);
		break;
	}	
}
//-----------------------------------------------------------------------
void DLight::createRectangle2D()
{
	/// XXX this RenderOp should really be re-used between DLight objects,
	/// not generated every time
	delete mRenderOp.vertexData; 
	delete mRenderOp.indexData; 

	mRenderOp.vertexData = new VertexData();
    mRenderOp.indexData = 0;

	GeomUtils::createQuad(mRenderOp.vertexData);

    mRenderOp.operationType = RenderOperation::OT_TRIANGLE_STRIP; 
    mRenderOp.useIndexes = false; 

	// Set bounding
    setBoundingBox(AxisAlignedBox(-10000,-10000,-10000,10000,10000,10000));
	mRadius = 15000;
	bIgnoreWorld = true;
}
//-----------------------------------------------------------------------
void DLight::createSphere(float radius, int nRings, int nSegments)
{
	delete mRenderOp.vertexData; 
	delete mRenderOp.indexData;
	mRenderOp.operationType = RenderOperation::OT_TRIANGLE_LIST;
	mRenderOp.indexData = new IndexData();
	mRenderOp.vertexData = new VertexData();
	mRenderOp.useIndexes = true;

	GeomUtils::createSphere(mRenderOp.vertexData, mRenderOp.indexData
		, radius
		, nRings, nSegments
		, false // no normals
		, false // no texture coordinates
		);

	// Set bounding box and sphere
	setBoundingBox( AxisAlignedBox( Ogre::Vector3(-radius, -radius, -radius), Ogre::Vector3(radius, radius, radius) ) );
	mRadius = radius;
	bIgnoreWorld = false;
}
//-----------------------------------------------------------------------
void DLight::createCone(float radius, float height, int nVerticesInBase)
{
	delete mRenderOp.vertexData;
	delete mRenderOp.indexData;
	mRenderOp.operationType = RenderOperation::OT_TRIANGLE_LIST;
	mRenderOp.indexData = new IndexData();
	mRenderOp.vertexData = new VertexData();
	mRenderOp.useIndexes = true;

	GeomUtils::createCone(mRenderOp.vertexData, mRenderOp.indexData
		, radius
		, height, nVerticesInBase);

	// Set bounding box and sphere
	setBoundingBox( AxisAlignedBox( 
			Ogre::Vector3(-radius, 0, -radius), 
			Ogre::Vector3(radius, height, radius) ) );

	mRadius = radius;
	bIgnoreWorld = false;
}
//-----------------------------------------------------------------------
Real DLight::getBoundingRadius(void) const
{
	return mRadius;
}
//-----------------------------------------------------------------------
Real DLight::getSquaredViewDepth(const Camera* cam) const
{
	if(bIgnoreWorld)
	{
		return 0.0f;
	}
	else
	{
		Ogre::Vector3 dist = cam->getDerivedPosition() - getParentSceneNode()->_getDerivedPosition();
		return dist.squaredLength();
	}
}
//-----------------------------------------------------------------------
const MaterialPtr& DLight::getMaterial(void) const
{
	return mGenerator->getMaterial(mPermutation);
}
//-----------------------------------------------------------------------
void DLight::getWorldTransforms(Ogre::Matrix4* xform) const
{
	if (mParentLight->getType() == Light::LT_SPOTLIGHT)
	{
		Ogre::Quaternion quat = Ogre::Vector3::UNIT_Y.getRotationTo(mParentLight->getDerivedDirection());
		xform->makeTransform(mParentLight->getDerivedPosition(),
			Ogre::Vector3::UNIT_SCALE, quat);
	}
	else
	{
		xform->makeTransform(mParentLight->getDerivedPosition(),
			Ogre::Vector3::UNIT_SCALE, Ogre::Quaternion::IDENTITY);
	}
	
}
//-----------------------------------------------------------------------
void DLight::updateFromParent()
{
	//TODO : Don't do this unless something changed
	setAttenuation(mParentLight->getAttenuationConstant(), 
		mParentLight->getAttenuationLinear(), mParentLight->getAttenuationQuadric());	
	setSpecularColour(mParentLight->getSpecularColour());

	if (getCastChadows())
	{
		ENABLE_BIT(mPermutation,LightMaterialGenerator::MI_SHADOW_CASTER);
	}
	else
	{
		DISABLE_BIT(mPermutation, LightMaterialGenerator::MI_SHADOW_CASTER);
	}
}
//-----------------------------------------------------------------------
bool DLight::isCameraInsideLight(Ogre::Camera* camera)
{
	switch (mParentLight->getType())
	{
	case Ogre::Light::LT_DIRECTIONAL:
		return false;
	case Ogre::Light::LT_POINT:
		{
		Ogre::Real distanceFromLight = camera->getDerivedPosition()
			.distance(mParentLight->getDerivedPosition());
		//Small epsilon fix to account for the fact that we aren't a true sphere.
		return distanceFromLight <= mRadius + camera->getNearClipDistance() + 0.1; 
		}
	case Ogre::Light::LT_SPOTLIGHT:
		{
		Ogre::Vector3 lightPos = mParentLight->getDerivedPosition();
		Ogre::Vector3 lightDir = mParentLight->getDerivedDirection();
		Ogre::Radian attAngle = mParentLight->getSpotlightOuterAngle();
		
		//Extend the analytic cone's radius by the near clip range by moving its tip accordingly.
		//Some trigonometry needed here.
		Ogre::Vector3 clipRangeFix = -lightDir * (camera->getNearClipDistance() / Ogre::Math::Tan(attAngle/2));
		lightPos = lightPos + clipRangeFix;
    
		Ogre::Vector3 lightToCamDir = camera->getDerivedPosition() - lightPos;
		Ogre::Real distanceFromLight = lightToCamDir.normalise();

		Ogre::Real cosAngle = lightToCamDir.dotProduct(lightDir);
		Ogre::Radian angle = Ogre::Math::ACos(cosAngle);
		//Check whether we will see the cone from our current POV.
		return (distanceFromLight <= (mParentLight->getAttenuationRange() + clipRangeFix.length()))
			&& (angle <= attAngle);
		}
	default:
		//Please the compiler
		return false;
	}
}
//-----------------------------------------------------------------------
bool DLight::getCastChadows() const
{
	return 
		mParentLight->_getManager()->isShadowTechniqueInUse() &&
		mParentLight->getCastShadows() && 
		(mParentLight->getType() == Light::LT_DIRECTIONAL || mParentLight->getType() == Light::LT_SPOTLIGHT);
}
//-----------------------------------------------------------------------
void DLight::updateFromCamera(Ogre::Camera* camera)
{
	//Set shader params
	const Ogre::MaterialPtr& mat = getMaterial();
	if (!mat->isLoaded()) 
	{
		mat->load();
	}
	Ogre::Technique* tech = mat->getBestTechnique();
	Ogre::Vector3 farCorner = camera->getViewMatrix(true) * camera->getWorldSpaceCorners()[4];

	for (unsigned short i=0; i<tech->getNumPasses(); i++) 
	{
		Ogre::Pass* pass = tech->getPass(i);
		// get the vertex shader parameters
		Ogre::GpuProgramParametersSharedPtr params = pass->getVertexProgramParameters();
		// set the camera's far-top-right corner
		if (params->_findNamedConstantDefinition("farCorner"))
			params->setNamedConstant("farCorner", farCorner);
	    
		params = pass->getFragmentProgramParameters();
		if (params->_findNamedConstantDefinition("farCorner"))
			params->setNamedConstant("farCorner", farCorner);

		//If inside light geometry, render back faces with CMPF_GREATER, otherwise normally
		if (mParentLight->getType() == Ogre::Light::LT_DIRECTIONAL)
		{
			pass->setCullingMode(Ogre::CULL_CLOCKWISE);
			pass->setDepthCheckEnabled(false);
		}
		else
		{
			pass->setDepthCheckEnabled(true);
			if (isCameraInsideLight(camera))
			{
				pass->setCullingMode(Ogre::CULL_ANTICLOCKWISE);
				pass->setDepthFunction(Ogre::CMPF_GREATER_EQUAL);
			}
			else
			{
				pass->setCullingMode(Ogre::CULL_CLOCKWISE);
				pass->setDepthFunction(Ogre::CMPF_LESS_EQUAL);
			}
		}

		Camera shadowCam("ShadowCameraSetupCam", 0);
		shadowCam._notifyViewport(camera->getViewport());
		SceneManager* sm = mParentLight->_getManager();
		sm->getShadowCameraSetup()->getShadowCamera(sm, 
			camera, camera->getViewport(), mParentLight, &shadowCam, 0);
			
		//Get the shadow camera position
		if (params->_findNamedConstantDefinition("shadowCamPos")) 
		{
			params->setNamedConstant("shadowCamPos", shadowCam.getPosition());
		}
		if (params->_findNamedConstantDefinition("shadowFarClip"))
		{
			params->setNamedConstant("shadowFarClip", shadowCam.getFarClipDistance());
		}

	}
}


//-----------------------------------------------------------------------
DeferredLightRenderOperation::DeferredLightRenderOperation(
	CompositorInstance* instance, const CompositionPass* pass)
{
	mViewport = instance->getChain()->getViewport();
	
	//Get the names of the GBuffer textures
	const CompositionPass::InputTex& input0 = pass->getInput(0);
	mTexName0 = instance->getTextureInstanceName(input0.name, input0.mrtIndex);
	const CompositionPass::InputTex& input1 = pass->getInput(1);
	mTexName1 = instance->getTextureInstanceName(input1.name, input1.mrtIndex);

	// Create lights material generator
	mLightMaterialGenerator = new LightMaterialGenerator();
	
	// Create the ambient light
	mAmbientLight = new AmbientLight();
	const MaterialPtr& mat = mAmbientLight->getMaterial();
	mat->load();
}
//-----------------------------------------------------------------------
DLight* DeferredLightRenderOperation::createDLight(Ogre::Light* light)
{
	DLight *rv = new DLight(mLightMaterialGenerator,light);
	mLights[light] = rv;
	return rv;
}
//-----------------------------------------------------------------------
void injectTechnique(SceneManager* sm, Technique* tech, Renderable* rend, const Ogre::LightList* lightList)
{
    for(unsigned short i=0; i<tech->getNumPasses(); ++i)
	{
		Ogre::Pass* pass = tech->getPass(i);
        if (lightList != 0) 
		{
			sm->_injectRenderWithPass(pass, rend, false, false, lightList);
		} 
		else
		{
			sm->_injectRenderWithPass(pass, rend, false);
		}
		
	}
}
//-----------------------------------------------------------------------
void DeferredLightRenderOperation::execute(SceneManager *sm, RenderSystem *rs)
{
    Ogre::Camera* cam = mViewport->getCamera();

	mAmbientLight->updateFromCamera(cam);
    Technique* tech = mAmbientLight->getMaterial()->getBestTechnique();
	injectTechnique(sm, tech, mAmbientLight, 0);

	const LightList& lightList = sm->_getLightsAffectingFrustum();
    for (LightList::const_iterator it = lightList.begin(); it != lightList.end(); it++) 
	{
        Light* light = *it;
		Ogre::LightList ll;
		ll.push_back(light);

		//if (++i != 2) continue;
        //if (light->getType() != Light::LT_DIRECTIONAL) continue;
		//if (light->getDiffuseColour() != ColourValue::Red) continue;

		LightsMap::iterator dLightIt = mLights.find(light);
		DLight* dLight = 0;
		if (dLightIt == mLights.end()) 
		{
			dLight = createDLight(light);
		}
		else 
		{
			dLight = dLightIt->second;
			dLight->updateFromParent();
		}
		dLight->updateFromCamera(cam);
		tech = dLight->getMaterial()->getBestTechnique();

		//Update shadow texture
		if (dLight->getCastChadows())
		{
			SceneManager::RenderContext* context = sm->_pauseRendering();

			sm->prepareShadowTextures(cam, mViewport, &ll);
			sm->_resumeRendering(context);
			
			Pass* pass = tech->getPass(0);
			TextureUnitState* tus = pass->getTextureUnitState("ShadowMap");
			assert(tus);
			const TexturePtr& shadowTex = sm->getShadowTexture(0);
			if (tus->_getTexturePtr() != shadowTex)
			{
				tus->_setTexturePtr(shadowTex);
			}
			
		}
		
        injectTechnique(sm, tech, dLight, &ll);
	}
}
//-----------------------------------------------------------------------
DeferredLightRenderOperation::~DeferredLightRenderOperation()
{
	for (LightsMap::iterator it = mLights.begin(); it != mLights.end(); ++it)
	{
		delete it->second;
	}
	mLights.clear();
	
	delete mAmbientLight;
	delete mLightMaterialGenerator;
}


class ssaoListener: public Ogre::CompositorInstance::Listener
{
public:
	
	ssaoListener(Ogre::CompositorInstance* instance) : mInstance(instance) {}
   
	// this callback we will use to modify SSAO parameters
    void notifyMaterialRender(Ogre::uint32 pass_id, Ogre::MaterialPtr &mat)
    {
        if (pass_id != 42) // not SSAO, return
            return;

        // this is the camera you're using
		Ogre::Camera *cam = mInstance->getChain()->getViewport()->getCamera();

        // calculate the far-top-right corner in view-space
        Ogre::Vector3 farCorner = cam->getViewMatrix(true) * cam->getWorldSpaceCorners()[4];

        // get the pass
        Ogre::Pass *pass = mat->getBestTechnique()->getPass(0);

        // get the vertex shader parameters
        Ogre::GpuProgramParametersSharedPtr params = pass->getVertexProgramParameters();
        // set the camera's far-top-right corner
        if (params->_findNamedConstantDefinition("farCorner"))
            params->setNamedConstant("farCorner", farCorner);

        // get the fragment shader parameters
        params = pass->getFragmentProgramParameters();
        // set the projection matrix we need
        static const Ogre::Matrix4 CLIP_SPACE_TO_IMAGE_SPACE(
            0.5,    0,    0,  0.5,
            0,   -0.5,    0,  0.5,
            0,      0,    1,    0,
            0,      0,    0,    1);
        if (params->_findNamedConstantDefinition("ptMat"))
            params->setNamedConstant("ptMat", CLIP_SPACE_TO_IMAGE_SPACE * cam->getProjectionMatrixWithRSDepth());
        if (params->_findNamedConstantDefinition("far"))
            params->setNamedConstant("far", cam->getFarClipDistance());
    }
private:
	Ogre::CompositorInstance* mInstance;
};

Ogre::CompositorInstance::Listener* SSAOLogic::createListener(Ogre::CompositorInstance* instance)
{
	return new ssaoListener(instance);
}


MaterialGenerator::MaterialGenerator():
	vsMask(0), fsMask(0), matMask(0), mImpl(0)
{
}
MaterialGenerator::~MaterialGenerator()
{
	delete mImpl;
}

const MaterialPtr &MaterialGenerator::getMaterial(Perm permutation)
{
	/// Check if material/shader permutation already was generated
	MaterialMap::iterator i = mMaterials.find(permutation);
	if(i != mMaterials.end())
	{
		return i->second;
	}
	else
	{
		/// Create it
		MaterialPtr templ = getTemplateMaterial(permutation & matMask);
		GpuProgramPtr vs = getVertexShader(permutation & vsMask);
		GpuProgramPtr fs = getFragmentShader(permutation & fsMask);
		
		/// Create material name
		String name = materialBaseName + StringConverter::toString(permutation);

		std::cerr << name << " " << vs->getName() << " " << fs->getName() << std::endl;
		/// Create material from template, and set shaders
		MaterialPtr mat = templ->clone(name);
		Technique *tech = mat->getTechnique(0);
		Pass *pass = tech->getPass(0);
		pass->setFragmentProgram(fs->getName());
		pass->setVertexProgram(vs->getName());
	
		/// And store it
		mMaterials[permutation] = mat;
		return mMaterials[permutation];
	}
}

const GpuProgramPtr &MaterialGenerator::getVertexShader(Perm permutation)
{
	ProgramMap::iterator i = mVs.find(permutation);
	if(i != mVs.end())
	{
		return i->second;
	}
	else
	{
		/// Create it
		mVs[permutation] = mImpl->generateVertexShader(permutation);
		return mVs[permutation];
	}
}

const GpuProgramPtr &MaterialGenerator::getFragmentShader(Perm permutation)
{
	ProgramMap::iterator i = mFs.find(permutation);
	if(i != mFs.end())
	{
		return i->second;
	}
	else
	{
		/// Create it
		mFs[permutation] = mImpl->generateFragmentShader(permutation);
		return mFs[permutation];
	}
}

const MaterialPtr &MaterialGenerator::getTemplateMaterial(Perm permutation)
{
	MaterialMap::iterator i = mTemplateMat.find(permutation);
	if(i != mTemplateMat.end())
	{
		return i->second;
	}
	else
	{
		/// Create it
		mTemplateMat[permutation] = mImpl->generateTemplateMaterial(permutation);
		return mTemplateMat[permutation];
	}
}

MaterialGenerator::Impl::~Impl()
{
}

//Use this directive to control whether you are writing projective (regular) or linear depth.
#define WRITE_LINEAR_DEPTH

//This is the concrete implementation of the material generator.
class GBufferMaterialGeneratorImpl : public MaterialGenerator::Impl
{
public:
	GBufferMaterialGeneratorImpl(const Ogre::String& baseName) : mBaseName(baseName) {}
	
protected:
	Ogre::String mBaseName;
	virtual Ogre::GpuProgramPtr generateVertexShader(MaterialGenerator::Perm permutation);
	virtual Ogre::GpuProgramPtr generateFragmentShader(MaterialGenerator::Perm permutation);
	virtual Ogre::MaterialPtr generateTemplateMaterial(MaterialGenerator::Perm permutation);

};

GBufferMaterialGenerator::GBufferMaterialGenerator() {
	vsMask = VS_MASK;
	fsMask = FS_MASK;
	matMask = MAT_MASK;
	materialBaseName = "DeferredShading/GBuffer/";
	mImpl = new GBufferMaterialGeneratorImpl(materialBaseName);
}

Ogre::GpuProgramPtr GBufferMaterialGeneratorImpl::generateVertexShader(MaterialGenerator::Perm permutation)
{
	Ogre::StringStream ss;
	
	ss << "void ToGBufferVP(" << std::endl;
	ss << "	float4 iPosition : POSITION," << std::endl;
	ss << "	float3 iNormal   : NORMAL," << std::endl;

	Ogre::uint32 numTexCoords = (permutation & GBufferMaterialGenerator::GBP_TEXCOORD_MASK) >> 8;
	for (Ogre::uint32 i=0; i<numTexCoords; i++) 
	{
		ss << "	float2 iUV" << i << " : TEXCOORD" << i << ',' << std::endl;
	}

	if (permutation & GBufferMaterialGenerator::GBP_NORMAL_MAP)
	{
		ss << "	float3 iTangent : TANGENT0," << std::endl;
	}

	//TODO : Skinning inputs
	ss << std::endl;
	


	ss << "	out float4 oPosition : POSITION," << std::endl;
#ifdef WRITE_LINEAR_DEPTH
    ss << "	out float3 oViewPos : TEXCOORD0," << std::endl;
#else
	ss << "	out float oDepth : TEXCOORD0," << std::endl;
#endif
	ss << "	out float3 oNormal : TEXCOORD1," << std::endl;
	int texCoordNum = 2;
	if (permutation & GBufferMaterialGenerator::GBP_NORMAL_MAP) 
	{
		ss << "	out float3 oTangent : TEXCOORD" << texCoordNum++ << ',' << std::endl;
		ss << "	out float3 oBiNormal : TEXCOORD" << texCoordNum++ << ',' << std::endl;
	}
	for (Ogre::uint32 i=0; i<numTexCoords; i++) 
	{
		ss << "	out float2 oUV" << i << " : TEXCOORD" << texCoordNum++ << ',' << std::endl;
	}

	ss << std::endl;

	ss << "	uniform float4x4 cWorldViewProj," << std::endl;
	ss << "	uniform float4x4 cWorldView" << std::endl;

	ss << "	)" << std::endl;
	
	
	ss << "{" << std::endl;
	ss << "	oPosition = mul(cWorldViewProj, iPosition);" << std::endl;
	ss << "	oNormal = mul(cWorldView, float4(iNormal,0)).xyz;" << std::endl;
	if (permutation & GBufferMaterialGenerator::GBP_NORMAL_MAP)
	{
		ss << "	oTangent = mul(cWorldView, float4(iTangent,0)).xyz;" << std::endl;
		ss << "	oBiNormal = cross(oNormal, oTangent);" << std::endl;
	}

#ifdef WRITE_LINEAR_DEPTH
    ss << "	oViewPos = mul(cWorldView, iPosition).xyz;" << std::endl;
#else
	ss << "	oDepth = oPosition.w;" << std::endl;
#endif

	for (Ogre::uint32 i=0; i<numTexCoords; i++) {
		ss << "	oUV" << i << " = iUV" << i << ';' << std::endl;
	}

	ss << "}" << std::endl;
	
	Ogre::String programSource = ss.str();
	Ogre::String programName = mBaseName + "VP_" + Ogre::StringConverter::toString(permutation);

#if OGRE_DEBUG_MODE
	Ogre::LogManager::getSingleton().getDefaultLog()->logMessage(programSource);
#endif

	// Create shader object
	Ogre::HighLevelGpuProgramPtr ptrProgram = Ogre::HighLevelGpuProgramManager::getSingleton().createProgram(
		programName, Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME,
		"cg", Ogre::GPT_VERTEX_PROGRAM);
	ptrProgram->setSource(programSource);
	ptrProgram->setParameter("entry_point","ToGBufferVP");
	ptrProgram->setParameter("profiles","vs_1_1 arbvp1");

	const Ogre::GpuProgramParametersSharedPtr& params = ptrProgram->getDefaultParameters();
	params->setNamedAutoConstant("cWorldViewProj", Ogre::GpuProgramParameters::ACT_WORLDVIEWPROJ_MATRIX);
	params->setNamedAutoConstant("cWorldView", Ogre::GpuProgramParameters::ACT_WORLDVIEW_MATRIX);
	ptrProgram->load();

	return Ogre::GpuProgramPtr(ptrProgram);
}

Ogre::GpuProgramPtr GBufferMaterialGeneratorImpl::generateFragmentShader(MaterialGenerator::Perm permutation)
{
	Ogre::StringStream ss;
	
	ss << "void ToGBufferFP(" << std::endl;
#ifdef WRITE_LINEAR_DEPTH
    ss << "	float3 iViewPos : TEXCOORD0," << std::endl;
#else
    ss << "	float1 iDepth : TEXCOORD0," << std::endl;
#endif
	ss << "	float3 iNormal   : TEXCOORD1," << std::endl;

	int texCoordNum = 2;
	if (permutation & GBufferMaterialGenerator::GBP_NORMAL_MAP) 
	{
		ss << "	float3 iTangent : TEXCOORD" << texCoordNum++ << ',' << std::endl;
		ss << "	float3 iBiNormal : TEXCOORD" << texCoordNum++ << ',' << std::endl;
	}

	Ogre::uint32 numTexCoords = (permutation & GBufferMaterialGenerator::GBP_TEXCOORD_MASK) >> 8;
	for (Ogre::uint32 i=0; i<numTexCoords; i++) 
	{
		ss << "	float2 iUV" << i << " : TEXCOORD" << texCoordNum++ << ',' << std::endl;
	}

	ss << std::endl;

	ss << "	out float4 oColor0 : COLOR0," << std::endl;
	ss << "	out float4 oColor1 : COLOR1," << std::endl;

	ss << std::endl;

	int samplerNum = 0;
	if (permutation & GBufferMaterialGenerator::GBP_NORMAL_MAP)
	{
		ss << "	uniform sampler sNormalMap : register(s" << samplerNum++ << ")," << std::endl;
	}
	Ogre::uint32 numTextures = permutation & GBufferMaterialGenerator::GBP_TEXTURE_MASK;
	for (Ogre::uint32 i=0; i<numTextures; i++) {
		ss << "	uniform sampler sTex" << i << " : register(s" << samplerNum++ << ")," << std::endl;
	}
    if (numTextures == 0 || permutation & GBufferMaterialGenerator::GBP_HAS_DIFFUSE_COLOUR)
	{
		ss << "	uniform float4 cDiffuseColour," << std::endl;
	}

#ifdef WRITE_LINEAR_DEPTH
    ss << "	uniform float cFarDistance," << std::endl;
#endif
	
	ss << "	uniform float cSpecularity" << std::endl;

	ss << "	)" << std::endl;
	
	
	ss << "{" << std::endl;

	if (numTexCoords > 0 && numTextures > 0) 
	{
		ss << "	oColor0.rgb = tex2D(sTex0, iUV0);" << std::endl;
        if (permutation & GBufferMaterialGenerator::GBP_HAS_DIFFUSE_COLOUR)
        {
            ss << "	oColor0.rgb *= cDiffuseColour.rgb;" << std::endl;
        }
	}
    else
	{
		ss << "	oColor0.rgb = cDiffuseColour.rgb;" << std::endl;
	}
    
	
	ss << "	oColor0.a = cSpecularity;" << std::endl;
	if (permutation & GBufferMaterialGenerator::GBP_NORMAL_MAP) 
	{
		ss << "	float3 texNormal = (tex2D(sNormalMap, iUV0)-0.5)*2;" << std::endl;
		ss << "	float3x3 normalRotation = float3x3(iTangent, iBiNormal, iNormal);" << std::endl;
		ss << "	oColor1.rgb = normalize(mul(texNormal, normalRotation));" << std::endl;
	} else 
	{
		ss << "	oColor1.rgb = normalize(iNormal);" << std::endl;
	}
#ifdef WRITE_LINEAR_DEPTH
    ss << "	oColor1.a = length(iViewPos) / cFarDistance;" << std::endl;
#else
    ss << "	oColor1.a = iDepth;" << std::endl;
#endif

	ss << "}" << std::endl;
	
	Ogre::String programSource = ss.str();
	Ogre::String programName = mBaseName + "FP_" + Ogre::StringConverter::toString(permutation);

#if OGRE_DEBUG_MODE
	Ogre::LogManager::getSingleton().getDefaultLog()->logMessage(programSource);
#endif

	// Create shader object
	Ogre::HighLevelGpuProgramPtr ptrProgram = Ogre::HighLevelGpuProgramManager::getSingleton().createProgram(
		programName, Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME,
		"cg", Ogre::GPT_FRAGMENT_PROGRAM);
	ptrProgram->setSource(programSource);
	ptrProgram->setParameter("entry_point","ToGBufferFP");
	ptrProgram->setParameter("profiles","ps_2_0 arbfp1");

	const Ogre::GpuProgramParametersSharedPtr& params = ptrProgram->getDefaultParameters();
	params->setNamedAutoConstant("cSpecularity", Ogre::GpuProgramParameters::ACT_SURFACE_SHININESS);
	if (numTextures == 0 || permutation & GBufferMaterialGenerator::GBP_HAS_DIFFUSE_COLOUR)
	{
		params->setNamedAutoConstant("cDiffuseColour", Ogre::GpuProgramParameters::ACT_SURFACE_DIFFUSE_COLOUR);
	}

#ifdef WRITE_LINEAR_DEPTH
    //TODO : Should this be the distance to the far corner, not the far clip distance?
    params->setNamedAutoConstant("cFarDistance", Ogre::GpuProgramParameters::ACT_FAR_CLIP_DISTANCE);
#endif

	ptrProgram->load();
	return Ogre::GpuProgramPtr(ptrProgram);
}

Ogre::MaterialPtr GBufferMaterialGeneratorImpl::generateTemplateMaterial(MaterialGenerator::Perm permutation)
{
	Ogre::String matName = mBaseName + "Mat_" + Ogre::StringConverter::toString(permutation);

	Ogre::MaterialPtr matPtr = Ogre::MaterialManager::getSingleton().create
		(matName, Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME);
	Ogre::Pass* pass = matPtr->getTechnique(0)->getPass(0);
	pass->setName(mBaseName + "Pass_" + Ogre::StringConverter::toString(permutation));
	pass->setLightingEnabled(false);
	if (permutation & GBufferMaterialGenerator::GBP_NORMAL_MAP)
	{
		pass->createTextureUnitState();
	}
	Ogre::uint32 numTextures = permutation & GBufferMaterialGenerator::GBP_TEXTURE_MASK;
	for (Ogre::uint32 i=0; i<numTextures; i++)
	{
		pass->createTextureUnitState();
	}

	return matPtr;
}

const String GBufferSchemeHandler::NORMAL_MAP_PATTERN = "normal";

Technique* GBufferSchemeHandler::handleSchemeNotFound(unsigned short schemeIndex, 
		const String& schemeName, Material* originalMaterial, unsigned short lodIndex, 
		const Renderable* rend)
{
	Ogre::MaterialManager& matMgr = Ogre::MaterialManager::getSingleton();
	String curSchemeName = matMgr.getActiveScheme();
	matMgr.setActiveScheme(MaterialManager::DEFAULT_SCHEME_NAME);
	Technique* originalTechnique = originalMaterial->getBestTechnique(lodIndex, rend);
	matMgr.setActiveScheme(curSchemeName);

	Technique* gBufferTech = originalMaterial->createTechnique();
	gBufferTech->removeAllPasses();
	gBufferTech->setSchemeName(schemeName);

	Technique* noGBufferTech = originalMaterial->createTechnique();
	noGBufferTech->removeAllPasses();
	noGBufferTech->setSchemeName("NoGBuffer");

	for (unsigned short i=0; i<originalTechnique->getNumPasses(); i++)
	{
		Pass* originalPass = originalTechnique->getPass(i);
		PassProperties props = inspectPass(originalPass, lodIndex, rend);
		
		if (!props.isDeferred)
		{
			//Just copy the technique so it gets rendered regularly
			Pass* clonePass = noGBufferTech->createPass();
			*clonePass = *originalPass;
			continue;
		}

		Pass* newPass = gBufferTech->createPass();
		MaterialGenerator::Perm perm = getPermutation(props);

		const Ogre::MaterialPtr& templateMat = mMaterialGenerator.getMaterial(perm);
    	
		//We assume that the GBuffer technique contains only one pass. But its true.
		*newPass = *(templateMat->getTechnique(0)->getPass(0));
		fillPass(newPass, originalPass, props);    
	}
    
	return gBufferTech;
}

bool GBufferSchemeHandler::checkNormalMap(
	TextureUnitState* tus, GBufferSchemeHandler::PassProperties& props)
{
	bool isNormal = false;
	Ogre::String lowerCaseAlias = tus->getTextureNameAlias();
	Ogre::StringUtil::toLowerCase(lowerCaseAlias);
	if (lowerCaseAlias.find(NORMAL_MAP_PATTERN) != Ogre::String::npos)
	{
		isNormal = true;
	}
	else 
	{
		Ogre::String lowerCaseName = tus->getTextureName();
		Ogre::StringUtil::toLowerCase(lowerCaseName);
		if (lowerCaseName.find(NORMAL_MAP_PATTERN) != Ogre::String::npos)
		{
			isNormal = true;
		}
	}

	if (isNormal)
	{
		if (props.normalMap == 0)
		{
			props.normalMap = tus;
		}
		else
		{
			OGRE_EXCEPT(Exception::ERR_DUPLICATE_ITEM,
				"Multiple normal map patterns matches",
				"GBufferSchemeHandler::inspectPass");
		}
	}
	return isNormal;
}

GBufferSchemeHandler::PassProperties GBufferSchemeHandler::inspectPass(
	Pass* pass, unsigned short lodIndex, const Renderable* rend)
{
	PassProperties props;
	
	//TODO : Use renderable to indicate wether this has skinning.
	//Probably use same const cast that renderSingleObject uses.
	if (pass->hasVertexProgram())
	{
		props.isSkinned = pass->getVertexProgram()->isSkeletalAnimationIncluded();
	}
	else 
	{
		props.isSkinned = false;
	}

	for (unsigned short i=0; i<pass->getNumTextureUnitStates(); i++) 
	{
		TextureUnitState* tus = pass->getTextureUnitState(i);
		if (!checkNormalMap(tus, props))
		{
			props.regularTextures.push_back(tus);
		}
		if (tus->getEffects().size() > 0)
		{
			props.isDeferred = false;
		}
		
	}

    if (pass->getDiffuse() != ColourValue::White)
    {
        props.hasDiffuseColour = true;
    }

    //Check transparency
    if (pass->getDestBlendFactor() != Ogre::SBF_ZERO)
    {
        //TODO : Better ways to do this
        props.isDeferred = false;
    }
	return props;
}

MaterialGenerator::Perm GBufferSchemeHandler::getPermutation(const PassProperties& props)
{
	MaterialGenerator::Perm perm = 0;
	switch (props.regularTextures.size())
	{
	case 0:
		perm |= GBufferMaterialGenerator::GBP_NO_TEXTURES;
		
		if (props.normalMap != 0)
		{
			perm |= GBufferMaterialGenerator::GBP_ONE_TEXCOORD;
		}
		else
		{
			perm |= GBufferMaterialGenerator::GBP_NO_TEXCOORDS;
		}
		break;
	case 1:
		perm |= GBufferMaterialGenerator::GBP_ONE_TEXTURE;
		perm |= GBufferMaterialGenerator::GBP_ONE_TEXCOORD;
		break;
	case 2:
		perm |= GBufferMaterialGenerator::GBP_TWO_TEXTURES;
		//TODO : When do we use two texcoords?
		perm |= GBufferMaterialGenerator::GBP_ONE_TEXCOORD;
		break;
	case 3:
		perm |= GBufferMaterialGenerator::GBP_THREE_TEXTURES;
		perm |= GBufferMaterialGenerator::GBP_ONE_TEXCOORD;
		break;
	default:
		OGRE_EXCEPT(Exception::ERR_NOT_IMPLEMENTED,
			"Can not generate G-Buffer materials for '>3 regular-texture' objects",
			"GBufferSchemeHandler::inspectPass");
	}

	if (props.isSkinned)
	{
		perm |= GBufferMaterialGenerator::GBP_SKINNED;
	}

	if (props.normalMap != 0)
	{
		perm |= GBufferMaterialGenerator::GBP_NORMAL_MAP;
	}

    if (props.hasDiffuseColour)
    {
        perm |= GBufferMaterialGenerator::GBP_HAS_DIFFUSE_COLOUR;
    }
	return perm;
}

void GBufferSchemeHandler::fillPass(
	Pass* gBufferPass, Pass* originalPass, const PassProperties& props)
{
	//Reference the correct textures. Normal map first!
	int texUnitIndex = 0;
	if (props.normalMap != 0)
	{
		*(gBufferPass->getTextureUnitState(texUnitIndex)) = *(props.normalMap);
		texUnitIndex++;
	}
	for (size_t i=0; i<props.regularTextures.size(); i++)
	{
		*(gBufferPass->getTextureUnitState(texUnitIndex)) = *(props.regularTextures[i]);
		texUnitIndex++;
	}
	gBufferPass->setAmbient(originalPass->getAmbient());
	gBufferPass->setDiffuse(originalPass->getDiffuse());
	gBufferPass->setSpecular(originalPass->getSpecular());
	gBufferPass->setShininess(originalPass->getShininess());
    gBufferPass->setCullingMode(originalPass->getCullingMode());
    gBufferPass->setLightingEnabled(false);
}



const Ogre::uint8 DeferredShadingSystem::PRE_GBUFFER_RENDER_QUEUE = Ogre::RENDER_QUEUE_1;
const Ogre::uint8 DeferredShadingSystem::POST_GBUFFER_RENDER_QUEUE = Ogre::RENDER_QUEUE_8;

DeferredShadingSystem::DeferredShadingSystem( Viewport *vp, SceneManager *sm, Camera *cam ): mViewport(vp), mSceneMgr(sm), mCamera(cam)
{
	sm->setShadowTechnique(SHADOWTYPE_TEXTURE_ADDITIVE_INTEGRATED);
	sm->setShadowTextureCasterMaterial("DeferredShading/Shadows/Caster");
	mSceneMgr->setShadowFarDistance(1024);
	mSceneMgr->setShadowTextureSize(512);//512);
	mSceneMgr->setShadowTextureCount(1);
	mSceneMgr->setShadowTexturePixelFormat( Ogre::PF_FLOAT32_R);
	mSceneMgr->setShadowDirectionalLightExtrusionDistance(75);//75);

	//sm->setShadowTechnique(SHADOWTYPE_TEXTURE_ADDITIVE);
	//sm->setShadowTextureCasterMaterial("DeferredShading/Shadows/Caster");
	//mSceneMgr->setShadowFarDistance(150);
	//mSceneMgr->setShadowTextureSize(512);
	//mSceneMgr->setShadowTextureCount(1);
	//mSceneMgr->setShadowTexturePixelFormat(PF_FLOAT16_R);
	//mSceneMgr->setShadowDirectionalLightExtrusionDistance(75);



}

void DeferredShadingSystem::initialize()
{
	for(int i=0; i<DSM_COUNT; ++i)
		mInstance[i]=0;

	createResources();
	
	mActive = false;
	
	mSSAO = false;
	mCurrentMode = DSM_SHOWLIT;
	setActive(true);
}

DeferredShadingSystem::~DeferredShadingSystem()
{
	CompositorChain *chain = CompositorManager::getSingleton().getCompositorChain(mViewport);
	for(int i=0; i<DSM_COUNT; ++i)
		chain->_removeInstance(mInstance[i]);
	CompositorManager::getSingleton().removeCompositorChain(mViewport);
}

void DeferredShadingSystem::setMode(DSMode mode)
{
	assert( 0 <= mode && mode < DSM_COUNT);

	// prevent duplicate setups
	if (mCurrentMode == mode && mInstance[mode]->getEnabled()==mActive)
		return;

	for(int i=0; i<DSM_COUNT; ++i)
	{
		if(i == mode)
		{
			mInstance[i]->setEnabled(mActive);
		}
		else
		{
			mInstance[i]->setEnabled(false);
		}
	}

	mCurrentMode = mode;

	mSSAOInstance->setEnabled(mActive && mSSAO && mCurrentMode == DSM_SHOWLIT);
}

void DeferredShadingSystem::setSSAO(bool ssao)
{
	if (ssao != mSSAO) 
	{
		mSSAO = ssao;
		if (mActive && mCurrentMode == DSM_SHOWLIT)
		{
			mSSAOInstance->setEnabled(ssao);
		}
	}
}
	
bool DeferredShadingSystem::getSSAO() const
{
	return mSSAO;
}
void DeferredShadingSystem::setActive(bool active)
{
	if (mActive != active)
	{
		mActive = active;
		mGBufferInstance->setEnabled(active);

		// mCurrentMode could have changed with a prior call to setMode, so iterate all
		setMode(mCurrentMode);
	}
}

DeferredShadingSystem::DSMode DeferredShadingSystem::getMode(void) const
{
	return mCurrentMode;
}

void DeferredShadingSystem::createResources(void)
{
	CompositorManager &compMan = CompositorManager::getSingleton();

	//Hook up the compositor logic and scheme handlers.
	//This can theoretically happen in a loaded plugin, but in this case the demo contains the code.
	static bool firstTime = true;
	if (firstTime)
	{
		MaterialManager::getSingleton().addListener(new GBufferSchemeHandler, "GBuffer");
		MaterialManager::getSingleton().addListener(new NullSchemeHandler, "NoGBuffer");
		compMan.registerCompositorLogic("SSAOLogic", new SSAOLogic);
		compMan.registerCustomCompositionPass("DeferredLight", new DeferredLightCompositionPass);
		firstTime = false;
	}
	
	// Create the main GBuffer compositor
	mGBufferInstance = compMan.addCompositor(mViewport, "DeferredShading/GBuffer");
	// Create filters
	mInstance[DSM_SHOWLIT] = compMan.addCompositor(mViewport, "DeferredShading/ShowLit");
	mInstance[DSM_SHOWNORMALS] = compMan.addCompositor(mViewport, "DeferredShading/ShowNormals");
	mInstance[DSM_SHOWDSP] = compMan.addCompositor(mViewport, "DeferredShading/ShowDepthSpecular");
	mInstance[DSM_SHOWCOLOUR] = compMan.addCompositor(mViewport, "DeferredShading/ShowColour");
	mSSAOInstance =  compMan.addCompositor(mViewport, "DeferredShading/SSAO");

}

void DeferredShadingSystem::logCurrentMode(void)
{
	if (mActive==false)
	{
		LogManager::getSingleton().logMessage("No Compositor Enabled!");
		return;
	}

	CompositorInstance* ci = mInstance[mCurrentMode];
	assert(ci->getEnabled()==true);

	LogManager::getSingleton().logMessage("Current Mode: ");
	LogManager::getSingleton().logMessage(ci->getCompositor()->getName());
}

}//namespace
