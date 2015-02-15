#include "NxGraphicsPreCompiled.h"

namespace Nx {

NxTechnique::NxTechnique( MaterialNx * Material, const std::string & TechniqueName )
{
	mTechnique = Material->GetMaterial()->createTechnique();
	mTechnique->setName( TechniqueName );
}

NxTechnique::~NxTechnique()
{

}

const std::string & NxTechnique::GetName() const
{
	return mTechnique->getName().c_str();
}

NxPass * NxTechnique::CreatePass()
{
	NxPass * c = new NxPass( this, "DefaultPass" );
	mPasses.push_back(c);
	return c; 
}

NxPass * NxTechnique::CreatePass( const std::string & PassName )
{
	NxPass * c = new NxPass( this, PassName );
	mPasses.push_back(c);
	return c; 
}

NxPass * NxTechnique::GetPass( unsigned short index )
{
    assert (index < mPasses.size() && "Index out of bounds.");
    return mPasses[index];
}

NxPass * NxTechnique::GetPass( const std::string & name )
{
    Passes::iterator i    = mPasses.begin();
    Passes::iterator iend = mPasses.end();
    NxPass * foundTechnique = 0;

    // iterate through techniques to find a match
    while (i != iend)
    {
        if ( (*i)->GetName() == name ){
            foundTechnique = (*i);
            break;
        }
        ++i;
    }

    return foundTechnique;

}

unsigned short NxTechnique::GetNumPasses() const
{
	return static_cast<unsigned short>(mPasses.size());
}



Ogre::Technique * NxTechnique::GetTechnique()
{
	return mTechnique;
}

}