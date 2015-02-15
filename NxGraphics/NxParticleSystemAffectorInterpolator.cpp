#include "NxGraphicsPreCompiled.h"
#include "NxParticleSystemAffectorFactory.h"

namespace Nx {

typedef std::vector<std::pair<Real, Real> > RealPairVector;

class InterpolationAffector : public ParticleAffector
{
public:
	class CmdRelativeVelocity : public ParamCommand
	{
	public:
		String doGet(const void* target) const {
			return StringConverter::toString( static_cast<const InterpolationAffector*>(target)->getRelativeVelocity());
		}

		void doSet(void* target, const String& val) {
			static_cast<InterpolationAffector*>(target)->setRelativeVelocity( StringConverter::parseBool(val));
		}
	};

	class CmdSpinVelocity : public ParamCommand
	{
	public:
		String doGet(const void* target) const {
			return StringConverter::toString( static_cast<const InterpolationAffector*>(target)->getSpinVelocity());
		}
		void doSet(void* target, const String& val) {
			static_cast<InterpolationAffector*>(target)->setSpinVelocity( StringConverter::parseBool(val));
		}
	};

	class CmdRelativeSize : public ParamCommand
	{
	public:
		String doGet(const void* target) const {
			return StringConverter::toString( static_cast<const InterpolationAffector*>(target)->getRelativeSize());
		}
		void doSet(void* target, const String& val) {
			static_cast<InterpolationAffector*>(target)->setRelativeSize( StringConverter::parseBool(val));
		}
	};

	class CmdInterpValues : public ParamCommand
	{
	public:
		String doGet(const void* target) const
		{
			const RealPairVector &InterpValues =
				static_cast<const InterpolationAffector*>(target)->getInterpValues();
			String params;
			size_t numParams = InterpValues.size();
			unsigned int i;
			for (i=0; i<numParams; ++i)
			{
				params += StringConverter::toString(InterpValues[i].first);
				params += " ";
				params += StringConverter::toString(InterpValues[i].second);
				params += " ";
			}
			return params;
		}
		void doSet(void* target, const String& val)
		{
			StringVector vecParams = StringUtil::split(val, " \t");
			size_t numParams = vecParams.size();
			unsigned int i;
			Real time;
			Real value;
			RealPairVector InterpValues;
			for (i=0; i<numParams-1; )
			{
				time = StringConverter::parseReal(vecParams[i++]);
				value = StringConverter::parseReal(vecParams[i++]);
				InterpValues.push_back(std::make_pair(time, value));
			}
			static_cast<InterpolationAffector*>(target)->setInterpValues(InterpValues);
		}
	};

	/// Default constructor
	InterpolationAffector(ParticleSystem* psys) : ParticleAffector(psys)
	{
		mType = "Interpolation";

		// defaults
		mRelativeVelocity = false;
		mSpinVelocity = false;
		mRelativeSize = false;

		// Set up parameters
		if (createParamDictionary("Interpolation"))
		{
			addBaseParameters();
			// Add extra paramaters
			ParamDictionary* dict = getParamDictionary();
			dict->addParameter(ParameterDef("relative_velocity",
				"Detemines whether the velocity of the particles is scaled.",
				PT_BOOL), &msRelativeVelocityCmd);
			dict->addParameter(ParameterDef("spin_velocity",
				"Detemines whether the spin velocity of the particles is scaled.",
				PT_BOOL), &msSpinVelocityCmd);
			dict->addParameter(ParameterDef("relative_size",
				"Detemines whether the size of the particles is scaled.",
				PT_BOOL), &msRelativeSizeCmd);
			dict->addParameter(ParameterDef("values",
				"Values to interpolate between.",
				PT_REAL), &msInterpValuesCmd);
		}
	}

	void _affectParticles(ParticleSystem* pSystem, Real  timeElapsed)
	{

 
		ParticleIterator pi = pSystem->_getIterator();
		Particle *p;
		Real prevLifeFrac, lifeFrac;
		Real prevValue, value;
		Real scale;
		Real Width = pSystem->getDefaultWidth(), Height = pSystem->getDefaultHeight();

		while (!pi.end())
		{
			p = pi.getNext();
			lifeFrac = 1-(p->timeToLive / p->totalTimeToLive);
			value = getValue(lifeFrac);
			if (mSpinVelocity)
			{
				p->rotationSpeed = Ogre::Degree(value);
				p->setRotation(p->rotation+p->rotationSpeed*timeElapsed);
			}
			if (mRelativeSize)
			{
				p->setDimensions(Width*value, Height*value);
			}

			if (mRelativeVelocity)
			{
				prevLifeFrac = 1-((p->timeToLive+timeElapsed) / p->totalTimeToLive);
				prevValue = getValue(prevLifeFrac);
				// if prevValue is zero then this particle cannot be affected (division by 0)
				if (prevValue != 0)
				{
					scale = value/prevValue;
					p->direction *= scale;
				}
			}
		}


	}

	/** See ParticleAffector. */
	void _initParticle(Particle* pParticle)
	{
		if (mSpinVelocity)
			pParticle->rotationSpeed = getValue(0);
		if (mRelativeVelocity)
			pParticle->direction *= getValue(0);
	}

	void setRelativeVelocity(bool relativeVelocity)
	{
		mRelativeVelocity = relativeVelocity;
	}
	void setSpinVelocity(bool spinVelocity)
	{
		mSpinVelocity = spinVelocity;
	}
	//-----------------------------------------------------------------------
	void setRelativeSize(bool relativeSize)
	{
		mRelativeSize = relativeSize;
	}
	//-----------------------------------------------------------------------
	void setInterpValues(RealPairVector &interpValues)
	{
		mInterpValues = interpValues;
	}

	bool  getRelativeVelocity(void) const
	{
		return mRelativeVelocity;
	}
	//-----------------------------------------------------------------------
	bool  getSpinVelocity(void) const
	{
		return mSpinVelocity;
	}
	//-----------------------------------------------------------------------
	bool  getRelativeSize(void) const
	{
		return mRelativeSize;
	}
	//-----------------------------------------------------------------------
	const RealPairVector & getInterpValues() const
	{
		return mInterpValues;
	}


	/// Command objects
	static CmdRelativeVelocity msRelativeVelocityCmd;
	static CmdSpinVelocity msSpinVelocityCmd;
	static CmdRelativeSize msRelativeSizeCmd;
	static CmdInterpValues msInterpValuesCmd;

protected:
	Real getValue(Real time)
	{
		if (mInterpValues.empty())
			return 1;
		if (time <= mInterpValues.front().first)
			return mInterpValues.front().second;
		if (time >= mInterpValues.back().first)
			return mInterpValues.back().second;
		RealPairVector::iterator itCurrentValue;
		RealPairVector::iterator itPrevValue = mInterpValues.end();
		for (itCurrentValue = mInterpValues.begin(); itCurrentValue != mInterpValues.end(); ++itCurrentValue)
		{
			if (itPrevValue != mInterpValues.end())
			{
				if (time <= itCurrentValue->first)
				{
					Real u = (time-itPrevValue->first)/(itCurrentValue->first-itPrevValue->first);
					Real value = itPrevValue->second + u*(itCurrentValue->second-itPrevValue->second);
					return value;
				}
			}
			itPrevValue = itCurrentValue;
		}
		return 1;   // Should never get here
	}

	RealPairVector mInterpValues;
	bool mRelativeVelocity;
	bool mSpinVelocity;
	bool mRelativeSize;
};

/////////////////////////////////
// Instantiate statics
InterpolationAffector::CmdRelativeVelocity InterpolationAffector::msRelativeVelocityCmd;
InterpolationAffector::CmdSpinVelocity InterpolationAffector::msSpinVelocityCmd;
InterpolationAffector::CmdRelativeSize InterpolationAffector::msRelativeSizeCmd;
InterpolationAffector::CmdInterpValues InterpolationAffector::msInterpValuesCmd;

ParticleAffector* InterpolationAffectorFactory::createAffector(ParticleSystem* psys)
{
	ParticleAffector* p = new InterpolationAffector(psys);
	mAffectors.push_back(p);
	return p;
}


}