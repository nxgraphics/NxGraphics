#include "NxGraphicsPreCompiled.h"

namespace Nx {

NxParticleSystemAffector::NxParticleSystemAffector( NxParticleSystem * Parent, const std::string & ParticleSystemName, NxParticleSystemAffectorTypes Type )  
{ 

	mAffector = NULL;

	std::string TypeStr = "";

	if( Type == NxParticleSystemAffectorScaler ) {
		TypeStr = "Scaler";
	}
	else if ( Type == NxParticleSystemAffectorRotator ) {
		TypeStr = "Rotator";
	}
	else if ( Type == NxParticleSystemAffectorLinearForce ) {
		TypeStr = "LinearForce";
	}
	else if ( Type == NxParticleSystemAffectorDirectionRandomiser ) {
		TypeStr = "DirectionRandomiser";
	}
	else if ( Type == NxParticleSystemAffectorDeflectorPlane ) {
		TypeStr = "DeflectorPlane";
	}
	else if ( Type == NxParticleSystemAffectorColourInterpolator ) {
		TypeStr = "ColourInterpolator";
	}
	else if ( Type == NxParticleSystemAffectorColourImage ) {
		TypeStr = "AffectorColourImage";
	}
	else if ( Type == NxParticleSystemAffectorColourFader2 ) {
		TypeStr = "ColourFader2";
	}
	else if ( Type == NxParticleSystemAffectorColourFader ) {
		TypeStr = "ColourFader";
	}
	else if ( Type == NxParticleSystemAffectorInterpolator ) {
		TypeStr = "Interpolation";
	}
	else if ( Type == NxParticleSystemAffectorJong ) {
		TypeStr = "Jong";
	}
	else
	{
		LogMsg("No affector type defined !");
	}
	

	if(!TypeStr.empty())
	{
		mAffector = Parent->GetParentSceneNode()->getCreator()->getParticleSystem(ParticleSystemName)->addAffector(TypeStr);
	}

 
	
 
}

NxParticleSystemAffector::~NxParticleSystemAffector()
{
 
}

void NxParticleSystemAffector::SetParameter( const std::string & Param,  const std::string & Value )
{
	if( mAffector ) mAffector->setParameter(Param,Value);
}




 

}//namespace