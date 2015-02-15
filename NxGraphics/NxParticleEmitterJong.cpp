#include "NxGraphicsPreCompiled.h"
#include "NxParticleSystemEmitterFactory.h"

namespace Nx {

class JongEmitter : public ParticleEmitter
{
public:
	class CmdA : public ParamCommand
	{
	public:
		String doGet(const void* target) const {
			return StringConverter::toString( static_cast<const JongEmitter*>(target)->getA());
		}

		void doSet(void* target, const String& val) {
			static_cast<JongEmitter*>(target)->setA( StringConverter::parseReal(val));
		}
	};

	class CmdB : public ParamCommand
	{
	public:
		String doGet(const void* target) const {
			return StringConverter::toString( static_cast<const JongEmitter*>(target)->getB());
		}
		void doSet(void* target, const String& val) {
			static_cast<JongEmitter*>(target)->setB( StringConverter::parseReal(val));
		}
	};

	class CmdC : public ParamCommand
	{
	public:
		String doGet(const void* target) const {
			return StringConverter::toString( static_cast<const JongEmitter*>(target)->getC());
		}
		void doSet(void* target, const String& val) {
			static_cast<JongEmitter*>(target)->setC( StringConverter::parseReal(val));
		}
	};

	class CmdD : public ParamCommand
	{
	public:
		String doGet(const void* target) const {
			return StringConverter::toString( static_cast<const JongEmitter*>(target)->getD());
		}
		void doSet(void* target, const String& val) {
			static_cast<JongEmitter*>(target)->setD( StringConverter::parseReal(val));
		}
	};

	class CmdScale : public ParamCommand
	{
	public:
		String doGet(const void* target) const {
			return StringConverter::toString( static_cast<const JongEmitter*>(target)->getScale());
		}
		void doSet(void* target, const String& val) {
			static_cast<JongEmitter*>(target)->setScale( StringConverter::parseReal(val));
		}
	};

	JongEmitter(ParticleSystem* psys)  : ParticleEmitter(psys)
	{
		mType = "Jong";

		// defaults
		mA = -2.24;
		mB = 0.43;
		mC = -0.65;
		mD = -2.43;
		mScale = 100.0f;

		// Set up parameters
		if (createParamDictionary("Jong"))
		{
			addBaseParameters();
			// Add extra paramaters
			ParamDictionary* dict = getParamDictionary();
			dict->addParameter(ParameterDef("A", "Detemines A", PT_REAL), &msA);
			dict->addParameter(ParameterDef("B", "Detemines D", PT_REAL), &msB);
			dict->addParameter(ParameterDef("C", "Detemines C", PT_REAL), &msC);
			dict->addParameter(ParameterDef("D", "Detemines D", PT_REAL), &msD);
			dict->addParameter(ParameterDef("scale", "Detemines scale", PT_REAL), &msScale);
		}
	}

	void getnext( double & x, double & y ) {
 
		double nx = x;
		double ny = y;
		x = Math::Sin(mA * ny) - Math::Cos(mB * nx);
		y = Math::Sin(mC * nx) - Math::Cos(mD * ny);

	}

	unsigned short _getEmissionCount(Real timeElapsed) 
	{
		return genConstantEmissionCount(timeElapsed);
	}
 

	/** See ParticleEmitter. */
	void _initParticle(Particle* pParticle)
	{
		// Call superclass
		ParticleEmitter::_initParticle(pParticle);

		static double x = 0.0;
		static double y = 0.0;
		getnext( x, y ) ;

		pParticle->position = mPosition + Ogre::Vector3(x*mScale,y*mScale,0);

		// Generate complex data by reference
		genEmissionColour(pParticle->colour);
		genEmissionDirection(pParticle->direction, pParticle->position );//added for new ogre
		genEmissionVelocity(pParticle->direction);

		// Generate simpler data
		pParticle->timeToLive = pParticle->totalTimeToLive = genEmissionTTL();
 
	}

	void setA( float A)
	{
		mA = A;
	}
	void setB( float B )
	{
		mB = B;
	}
	//-----------------------------------------------------------------------
	void setC( float C )
	{
		mC = C;
	}
	//-----------------------------------------------------------------------
	void setD( float D)
	{
		mD = D;
	}
	//-----------------------------------------------------------------------
	void setScale( float scale )
	{
		mScale = scale;
	}

	float  getA(void) const
	{
		return mA;
	}
	//-----------------------------------------------------------------------
	float  getB(void) const
	{
		return mB;
	}
	//-----------------------------------------------------------------------
	float  getC(void) const
	{
		return mC;
	}
	//-----------------------------------------------------------------------
	float  getD(void) const
	{
		return mD;
	}
	//-----------------------------------------------------------------------
	float  getScale(void) const
	{
		return mScale;
	}



	/// Command objects
	static CmdA msA;
	static CmdB msB;
	static CmdC msC;
	static CmdD msD;
	static CmdScale msScale;

	float mA;
	float mB;
	float mC;
	float mD;
	float mScale;

 
 
};

JongEmitter::CmdA JongEmitter::msA;
JongEmitter::CmdB JongEmitter::msB;
JongEmitter::CmdC JongEmitter::msC;
JongEmitter::CmdD JongEmitter::msD;
JongEmitter::CmdScale JongEmitter::msScale;


ParticleEmitter* JongEmitterFactory::createEmitter(ParticleSystem* psys)
{
	ParticleEmitter* emit = OGRE_NEW JongEmitter(psys);
	mEmitters.push_back(emit);
	return emit;
}





}