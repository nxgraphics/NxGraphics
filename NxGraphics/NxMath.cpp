/*
-----------------------------------------------------------------------------
This source file is part of OGRE
    (Object-oriented Graphics Rendering Engine)
For the latest info, see http://www.ogre3d.org/

Copyright (c) 2000-2013 Torus Knot Software Ltd

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

#include "NxGraphicsPreCompiled.h"
#include "NxMath.h"
#include "NxMathAsm.h"
#include "OgreVector2.h"
#include "NxVector3.h"
#include "OgreVector4.h"
using namespace Ogre;

namespace Nx
{

    const Real Math::POS_INFINITY = std::numeric_limits<Real>::infinity();
    const Real Math::NEG_INFINITY = -std::numeric_limits<Real>::infinity();
    const Real Math::PI = Real( 4.0 * atan( 1.0 ) );
    const Real Math::TWO_PI = Real( 2.0 * PI );
    const Real Math::HALF_PI = Real( 0.5 * PI );
	const Real Math::fDeg2Rad = PI / Real(180.0);
	const Real Math::fRad2Deg = Real(180.0) / PI;
	const Real Math::LOG2 = log(Real(2.0));

    int Math::mTrigTableSize;
   Math::AngleUnit Math::msAngleUnit;

    Real  Math::mTrigTableFactor;
    Real *Math::mSinTable = NULL;
    Real *Math::mTanTable = NULL;

    Math::RandomValueProvider* Math::mRandProvider = NULL;

    //-----------------------------------------------------------------------
    Math::Math( unsigned int trigTableSize )
    {
        msAngleUnit = AU_DEGREE;
        mTrigTableSize = trigTableSize;
        mTrigTableFactor = mTrigTableSize / Math::TWO_PI;

        mSinTable = OGRE_ALLOC_T(Real, mTrigTableSize, MEMCATEGORY_GENERAL);
        mTanTable = OGRE_ALLOC_T(Real, mTrigTableSize, MEMCATEGORY_GENERAL);

        buildTrigTables();
    }

    //-----------------------------------------------------------------------
    Math::~Math()
    {
        OGRE_FREE(mSinTable, MEMCATEGORY_GENERAL);
        OGRE_FREE(mTanTable, MEMCATEGORY_GENERAL);
    }

    //-----------------------------------------------------------------------
    void Math::buildTrigTables(void)
    {
        // Build trig lookup tables
        // Could get away with building only PI sized Sin table but simpler this 
        // way. Who cares, it'll ony use an extra 8k of memory anyway and I like 
        // simplicity.
        Real angle;
        for (int i = 0; i < mTrigTableSize; ++i)
        {
            angle = Math::TWO_PI * i / mTrigTableSize;
            mSinTable[i] = sin(angle);
            mTanTable[i] = tan(angle);
        }
    }
	//-----------------------------------------------------------------------	
	Real Math::SinTable (Real fValue)
    {
        // Convert range to index values, wrap if required
        int idx;
        if (fValue >= 0)
        {
            idx = int(fValue * mTrigTableFactor) % mTrigTableSize;
        }
        else
        {
            idx = mTrigTableSize - (int(-fValue * mTrigTableFactor) % mTrigTableSize) - 1;
        }

        return mSinTable[idx];
    }
	//-----------------------------------------------------------------------
	Real Math::TanTable (Real fValue)
    {
        // Convert range to index values, wrap if required
		int idx = int(fValue *= mTrigTableFactor) % mTrigTableSize;
		return mTanTable[idx];
    }
    //-----------------------------------------------------------------------
    int Math::ISign (int iValue)
    {
        return ( iValue > 0 ? +1 : ( iValue < 0 ? -1 : 0 ) );
    }
    //-----------------------------------------------------------------------
    Radian Math::ACos (Real fValue)
    {
        if ( -1.0 < fValue )
        {
            if ( fValue < 1.0 )
                return Radian(acos(fValue));
            else
                return Radian(0.0);
        }
        else
        {
            return Radian(PI);
        }
    }
    //-----------------------------------------------------------------------
    Radian Math::ASin (Real fValue)
    {
        if ( -1.0 < fValue )
        {
            if ( fValue < 1.0 )
                return Radian(asin(fValue));
            else
                return Radian(HALF_PI);
        }
        else
        {
            return Radian(-HALF_PI);
        }
    }
    //-----------------------------------------------------------------------
    Real Math::Sign (Real fValue)
    {
        if ( fValue > 0.0 )
            return 1.0;

        if ( fValue < 0.0 )
            return -1.0;

        return 0.0;
    }
    //-----------------------------------------------------------------------
    Real Math::InvSqrt(Real fValue)
    {
        return Real(asm_rsq(fValue));
    }
    //-----------------------------------------------------------------------
    Real Math::UnitRandom ()
    {
        if (mRandProvider)
            return mRandProvider->getRandomUnit();
        else return asm_rand() / asm_rand_max();
    }
    
    //-----------------------------------------------------------------------
    Real Math::RangeRandom (Real fLow, Real fHigh)
    {
        return (fHigh-fLow)*UnitRandom() + fLow;
    }

    //-----------------------------------------------------------------------
    Real Math::SymmetricRandom ()
    {
        return 2.0f * UnitRandom() - 1.0f;
    }

    //-----------------------------------------------------------------------
    void Math::SetRandomValueProvider(RandomValueProvider* provider)
    {
        mRandProvider = provider;
    }


   //-----------------------------------------------------------------------
    void Math::setAngleUnit(Math::AngleUnit unit)
   {
       msAngleUnit = unit;
   }
   //-----------------------------------------------------------------------
   Math::AngleUnit Math::getAngleUnit(void)
   {
       return msAngleUnit;
   }
    //-----------------------------------------------------------------------
    Real Math::AngleUnitsToRadians(Real angleunits)
    {
       if (msAngleUnit == AU_DEGREE)
           return angleunits * fDeg2Rad;
       else
           return angleunits;
    }

    //-----------------------------------------------------------------------
    Real Math::RadiansToAngleUnits(Real radians)
    {
       if (msAngleUnit == AU_DEGREE)
           return radians * fRad2Deg;
       else
           return radians;
    }

    //-----------------------------------------------------------------------
    Real Math::AngleUnitsToDegrees(Real angleunits)
    {
       if (msAngleUnit == AU_RADIAN)
           return angleunits * fRad2Deg;
       else
           return angleunits;
    }

    //-----------------------------------------------------------------------
    Real Math::DegreesToAngleUnits(Real degrees)
    {
       if (msAngleUnit == AU_RADIAN)
           return degrees * fDeg2Rad;
       else
           return degrees;
    }

    
    //-----------------------------------------------------------------------
    bool Math::RealEqual( Real a, Real b, Real tolerance )
    {
        if (fabs(b-a) <= tolerance)
            return true;
        else
            return false;
    }

     
    //-----------------------------------------------------------------------
    Vector3 Math::calculateTangentSpaceVector(
        const Vector3& position1, const Vector3& position2, const Vector3& position3,
        Real u1, Real v1, Real u2, Real v2, Real u3, Real v3)
    {
	    //side0 is the vector along one side of the triangle of vertices passed in, 
	    //and side1 is the vector along another side. Taking the cross product of these returns the normal.
	    Vector3 side0 = position1 - position2;
	    Vector3 side1 = position3 - position1;
	    //Calculate face normal
	    Vector3 normal = side1.crossProduct(side0);
	    normal.normalise();
	    //Now we use a formula to calculate the tangent. 
	    Real deltaV0 = v1 - v2;
	    Real deltaV1 = v3 - v1;
	    Vector3 tangent = deltaV1 * side0 - deltaV0 * side1;
	    tangent.normalise();
	    //Calculate binormal
	    Real deltaU0 = u1 - u2;
	    Real deltaU1 = u3 - u1;
	    Vector3 binormal = deltaU1 * side0 - deltaU0 * side1;
	    binormal.normalise();
	    //Now, we take the cross product of the tangents to get a vector which 
	    //should point in the same direction as our normal calculated above. 
	    //If it points in the opposite direction (the dot product between the normals is less than zero), 
	    //then we need to reverse the s and t tangents. 
	    //This is because the triangle has been mirrored when going from tangent space to object space.
	    //reverse tangents if necessary
	    Vector3 tangentCross = tangent.crossProduct(binormal);
	    if (tangentCross.dotProduct(normal) < 0.0f)
	    {
		    tangent = -tangent;
		    binormal = -binormal;
	    }

        return tangent;

    }
  
   
	//-----------------------------------------------------------------------
	Real Math::gaussianDistribution(Real x, Real offset, Real scale)
	{
		Real nom = Math::Exp(
			-Math::Sqr(x - offset) / (2 * Math::Sqr(scale)));
		Real denom = scale * Math::Sqrt(2 * Math::PI);

		return nom / denom;

	}
	 
	 

}
