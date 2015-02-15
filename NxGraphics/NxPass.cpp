#include "NxGraphicsPreCompiled.h"
#include "NxConversions.h"

namespace Nx {

NxPass::NxPass( NxTechnique * parent, const std::string & PassName )
{
	mPass = parent->GetTechnique()->createPass();
	mPass->setName( PassName );
}

NxPass::~NxPass()
{ 

}

const std::string & NxPass::GetName() const
{
	return mPass->getName();
}
 
NxTextureUnit * NxPass::CreateTextureUnit( const std::string & UnitName )
{
	NxTextureUnit * c = new NxTextureUnit( this, UnitName );
	mUnits.push_back( c );
	return c;
}

NxTextureUnit * NxPass::GetTextureUnit( const std::string & name )
{
    Units::iterator i    = mUnits.begin();
    Units::iterator iend = mUnits.end();
    NxTextureUnit * foundUnit= 0;

    // iterate through techniques to find a match
    while (i != iend)
    {
        if ( (*i)->GetName() == name ){
            foundUnit = (*i);
            break;
        }
        ++i;
    }

    return foundUnit;

}

unsigned short NxPass::GetNumTextureUnit() const
{
	return mPass->getNumTextureUnitStates();
}

void NxPass::SetVertexProgram( const std::string & VertexProgramName )
{
	mPass->setVertexProgram( VertexProgramName, true );
}

void NxPass::SetVertexParameterValue( const std::string & Name, int Value )
{
	GpuProgramParametersSharedPtr params = mPass->getVertexProgramParameters();
	params->setNamedConstant( Name, Value);
}

void NxPass::SetVertexParameterValue( const std::string & Name, float Value )
{
	GpuProgramParametersSharedPtr params = mPass->getVertexProgramParameters();
	params->setNamedConstant ( Name, Value);
}

void NxPass::SetVertexParameterValue( const std::string & Name, const Nx::Vector3 & Value )
{
	GpuProgramParametersSharedPtr params = mPass->getVertexProgramParameters();
	params->setNamedConstant( Name, NxVec3ToOgre( Value ) );
}

void NxPass::SetVertexParameterValue( const std::string & Name, const Nx::Vector4 & Value )
{
	GpuProgramParametersSharedPtr params = mPass->getVertexProgramParameters();
	params->setNamedConstant( Name, NxVec4ToOgre( Value ) );
}

void NxPass::SetVertexParameterValue( const std::string & Name, const Nx::Matrix4 & Value )
{
	GpuProgramParametersSharedPtr params = mPass->getVertexProgramParameters();
	params->setNamedConstant( Name, NxMat4toOgre ( Value ));
}

void NxPass::SetVertexAutoParameterValue( const std::string & Name, NxAutoConstantType Type )
{
	GpuProgramParametersSharedPtr params = mPass->getVertexProgramParameters();
	params->setNamedAutoConstant( Name, (Ogre::GpuProgramParameters::AutoConstantType)Type );
}

void NxPass::SetVertexAutoParameterFloatValue( const std::string & Name,  NxAutoConstantType Type, float Value )
{
	GpuProgramParametersSharedPtr params = mPass->getVertexProgramParameters();
	params->setNamedAutoConstantReal( Name, (Ogre::GpuProgramParameters::AutoConstantType)Type, Value );
}

void NxPass::SetFragmentParameterValue( const std::string & Name, int Value )
{
	GpuProgramParametersSharedPtr params = mPass->getFragmentProgramParameters();
	params->setNamedConstant( Name, Value);
}
 
void NxPass::SetFragmentParameterValue( const std::string & Name, float Value )
{
	GpuProgramParametersSharedPtr params = mPass->getFragmentProgramParameters();
	params->setNamedConstant( Name, Value);
}

void NxPass::SetFragmentParameterValue( const std::string & Name, const Nx::Vector2 & Value )
{
	GpuProgramParametersSharedPtr params = mPass->getFragmentProgramParameters();
	params->setNamedConstant( Name , NxVec2ToOgre( Value ).ptr(), 2);
}

void NxPass::SetFragmentParameterValue( const std::string & Name, const Nx::Vector3 & Value )
{
	GpuProgramParametersSharedPtr params = mPass->getFragmentProgramParameters();
	params->setNamedConstant( Name, NxVec3ToOgre( Value ));
}

void NxPass::SetFragmentParameterValue( const std::string & Name, const Nx::Vector4 & Value )
{
	GpuProgramParametersSharedPtr params = mPass->getFragmentProgramParameters();
	params->setNamedConstant( Name, NxVec4ToOgre( Value ) );
}

void NxPass::SetFragmentParameterValue( const std::string & Name, const Nx::Matrix4 & Value )
{
	GpuProgramParametersSharedPtr params = mPass->getFragmentProgramParameters();
	params->setNamedConstant( Name, NxMat4toOgre ( Value ));
}

void NxPass::SetFragmentAutoParameterValue( const std::string & Name, NxAutoConstantType Type )
{
	GpuProgramParametersSharedPtr params = mPass->getFragmentProgramParameters();
	params->setNamedAutoConstant( Name, (Ogre::GpuProgramParameters::AutoConstantType )Type );
}

void NxPass::SetFragmentProgram( const std::string & FragmentProgramName )
{
	mPass->setFragmentProgram( FragmentProgramName, true );
}

void NxPass::SetGeometryProgram( const std::string & GeometryProgramName )
{
	mPass->setGeometryProgram( GeometryProgramName, true );
}

void NxPass::SetGeometryParameterValue( const std::string & Name, float Value )
{
	GpuProgramParametersSharedPtr params = mPass->getGeometryProgramParameters();
	params->setNamedConstant( Name, Value );
}

void NxPass::SetGeometryParameterValue( const std::string & Name, const Nx::Vector2 & Value )
{
	GpuProgramParametersSharedPtr params = mPass->getGeometryProgramParameters();
	params->setNamedConstant( Name , NxVec2ToOgre( Value ) .ptr(), 2);
}

void NxPass::SetGeometryParameterValue( const std::string & Name, const Nx::Vector3 & Value )
{
	GpuProgramParametersSharedPtr params = mPass->getGeometryProgramParameters();
	params->setNamedConstant( Name, NxVec3ToOgre( Value ) );
}

void NxPass::SetGeometryParameterValue( const std::string & Name, const Nx::Vector4 & Value )
{
	GpuProgramParametersSharedPtr params = mPass->getGeometryProgramParameters();
	params->setNamedConstant( Name, NxVec4ToOgre( Value ) );
}

void NxPass::SetGeometryParameterValue( const std::string & Name, const float * dataArray, size_t Arraysize, size_t multiple )
{
	GpuProgramParametersSharedPtr params = mPass->getGeometryProgramParameters();
	params->setNamedConstant( Name, dataArray, Arraysize,  multiple );  
}
 
void NxPass::SetGeometryAutoParameterValue( const std::string & Name, NxAutoConstantType Type )
{
	GpuProgramParametersSharedPtr params = mPass->getGeometryProgramParameters();
	params->setNamedAutoConstant( Name, (Ogre::GpuProgramParameters::AutoConstantType)Type );
}

/*
enum TrackVertexColourEnum {
TVC_NONE        = 0x0,
TVC_AMBIENT     = 0x1,        
TVC_DIFFUSE     = 0x2,
TVC_SPECULAR    = 0x4,
TVC_EMISSIVE    = 0x8
};
*/
void NxPass::SetVertexColourTracking( NxTrackVertexColourEnum track )
{
	mPass->setVertexColourTracking( (TrackVertexColourEnum) track );
}

NxTrackVertexColourType NxPass::GetVertexColourTracking() const
{
	return (NxTrackVertexColourType)mPass->getVertexColourTracking(); 
}
/*
void NxPass::SetDiffuse( bool TrackVertexColour )
{
	mPass->setVertexColourTracking(  mPass->getVertexColourTracking() | TVC_DIFFUSE );
}
 
void NxPass::SetAmbient( bool TrackVertexColour )
{
	mPass->setVertexColourTracking( mPass->getVertexColourTracking() | TVC_AMBIENT );
}

void NxPass::SetSpecular( bool TrackVertexColour )
{
mPass->setVertexColourTracking( mPass->getVertexColourTracking() | TVC_SPECULAR );
}
 
void NxPass::SetEmissive( bool TrackVertexColour )
{
	mPass->setVertexColourTracking( mPass->getVertexColourTracking() | TVC_EMISSIVE );
}
*/

void NxPass::SetDiffuse( const NxColourValue& diffuse )
{
	mPass->setDiffuse( NxColourtoOgre( diffuse ) );
}

void NxPass::SetAmbient( const NxColourValue& ambient )
{
	mPass->setAmbient( NxColourtoOgre( ambient ) );
}

void NxPass::SetSpecular( const NxColourValue& specular )
{
	mPass->setSpecular( NxColourtoOgre( specular ) );
}

void NxPass::SetEmissive( const NxColourValue& emissive )
{
	mPass->setSelfIllumination( NxColourtoOgre( emissive ) );
}

void NxPass::SetShininess( float val )
{
	mPass->setShininess( val );
}

void NxPass::SetSceneBlending( const NxSceneBlendType sbt )
{
	mPass->setSceneBlending( ( SceneBlendType ) sbt );
}

void NxPass::SetDepthCheckEnabled( bool Enabled )
{
	mPass->setDepthCheckEnabled( Enabled );
}

void NxPass::SetDepthWriteEnabled( bool Enabled )
{
	mPass->setDepthWriteEnabled( Enabled );
}

void NxPass::SetLightingEnabled( bool Enabled )
{
	mPass->setLightingEnabled( Enabled );
}

void NxPass::SetCullingMode( NxCullingMode Mode )
{
	mPass->setCullingMode( ( CullingMode) Mode );
}

void NxPass::SetPointSpritesEnabled( bool Enabled )
{
	mPass->setPointSpritesEnabled( Enabled );
}

void NxPass::SetPointSize( float size )
{
	mPass->setPointSize( size );
}

void NxPass::SetPointAttenuation( bool enabled, float constant , float linear , float quadratic )
{
	mPass->setPointAttenuation( enabled, constant, linear, quadratic );
}

void NxPass::SetPointMinSize( float Min )
{
	mPass->setPointMinSize( Min );
}

void NxPass::SetPointMaxSize( float Max )
{
	mPass->setPointMaxSize( Max );
}

void NxPass::SetPolygonMode( NxPolygonMode mode  )
{
	mPass->setPolygonMode( ( Ogre::PolygonMode )mode  );
}

void NxPass::SetAlphaToCoverageEnabled(bool enabled) {
	mPass->setAlphaToCoverageEnabled(enabled);
}

void NxPass::SetTransparentSortingEnabled(bool enabled) {
	mPass->setTransparentSortingEnabled(enabled);
}

bool NxPass::GetTransparentSortingEnabled() const {
	return mPass->getTransparentSortingEnabled();
}

void NxPass::SetTransparentSortingForced(bool enabled) {
	mPass->setTransparentSortingForced(enabled);
}

bool NxPass::GetTransparentSortingForced() const {
	return mPass->getTransparentSortingForced();
}

void NxPass::SetAlphaRejectSettings( NxCompareFunction func, unsigned char value, bool alphaToCoverage ) {
	mPass->setAlphaRejectSettings( (Ogre::CompareFunction) func,  value, alphaToCoverage );
}

void NxPass::SetAlphaRejectFunction( NxCompareFunction func ){
	mPass->setAlphaRejectFunction( (Ogre::CompareFunction) func );
}

void NxPass::SetManualCullingMode( NxManualCullingMode mode ) {
	mPass->setManualCullingMode( (Ogre::ManualCullingMode) mode );
}

void NxPass::SetSeparateSceneBlending( const NxSceneBlendType sbt, const NxSceneBlendType sbta ) {
	mPass->setSeparateSceneBlending(  (Ogre::SceneBlendType) sbt, (Ogre::SceneBlendType) sbta );
}

void NxPass::SetSceneBlending( const NxSceneBlendFactor sourceFactor, const NxSceneBlendFactor destFactor) {
	mPass->setSceneBlending(  (Ogre::SceneBlendFactor) sourceFactor, (Ogre::SceneBlendFactor) destFactor ); 
}

void NxPass::SetSeparateSceneBlending( const NxSceneBlendFactor sourceFactor, const NxSceneBlendFactor destFactor, const NxSceneBlendFactor sourceFactorAlpha, const NxSceneBlendFactor destFactorAlpha ){
	mPass->setSeparateSceneBlending( (Ogre::SceneBlendFactor) sourceFactor, (Ogre::SceneBlendFactor) destFactor, (Ogre::SceneBlendFactor) sourceFactorAlpha, (Ogre::SceneBlendFactor) destFactorAlpha );
}

void NxPass::SetSceneBlendingOperation( NxSceneBlendOperation op) {
	mPass->setSceneBlendingOperation( (Ogre::SceneBlendOperation) op );
}

void NxPass::SetSeparateSceneBlendingOperation( NxSceneBlendOperation op, NxSceneBlendOperation alphaOp ) {
	mPass->setSeparateSceneBlendingOperation( (Ogre::SceneBlendOperation) op, (Ogre::SceneBlendOperation) alphaOp );
}

void NxPass::SetDepthBias( float constantBias, float slopeScaleBias ) {
	mPass->setDepthBias( constantBias, slopeScaleBias );
}

Ogre::Pass * NxPass::GetPass()
{
	return mPass;
}

}