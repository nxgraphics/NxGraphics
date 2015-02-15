#include "NxGraphicsPreCompiled.h"

namespace Nx {

template<> NxAnimationManager* NxSingleton<NxAnimationManager>::ms_Singleton = 0;

NxAnimationManager::NxAnimationManager()
{
	
}

NxAnimationManager::~NxAnimationManager()
{


}

NxAnimationManager * NxAnimationManager::getSingletonPtr()
{
	return ms_Singleton;
}

NxAnimationManager& NxAnimationManager::getSingleton()
{
	assert( ms_Singleton );  return ( *ms_Singleton );
}


void NxAnimationManager::AddAnimation( NxAnimation * Anim )
{
	AnimationList.push_back( Anim );
}

void NxAnimationManager::RemoveAnimation( NxAnimation * Anim )
{
	std::vector<NxAnimation*>::iterator i = std::find(AnimationList.begin(), AnimationList.end(), Anim);
	if ( i != AnimationList.end() ) // found it
	{
       AnimationList.erase( i );         
	} 
}

void NxAnimationManager::RemoveAnimation( const std::string & Name )
{
	std::vector<NxAnimation *>::iterator it;
	for(it=AnimationList.end(); it!=AnimationList.begin(); ){
		--it;
		if( (*it)->GetAnimationName() == Name ){
			//delete *it;
			it=AnimationList.erase(it);
		}	
	}
}

void NxAnimationManager::RemoveAllAnimations()
{
	std::vector<NxAnimation *>::iterator it;
	for(it=AnimationList.end(); it!=AnimationList.begin(); ){
		--it;
		it=AnimationList.erase(it);
	}
}

bool NxAnimationManager::HasAnimation( const std::string & AnimationName )
{
	bool Res = false;
	std::vector<NxAnimation*>::iterator i;
	for (i = AnimationList.begin(); i != AnimationList.end(); ++i) {
		if( (*i)->GetAnimationName().compare( AnimationName ) == 0 ) {
			Res = true;
		}
	}
	return Res;
}

NxAnimation & NxAnimationManager::GetAnimation( const std::string & AnimationName )
{
	std::vector<NxAnimation*>::iterator i;
	for (i = AnimationList.begin(); i != AnimationList.end(); ++i) {
		if( (*i)->GetAnimationName().compare( AnimationName ) == 0 ){
			return *(*i);
		}
	}

	throw "Didnt find Animation Name!";
}

void NxAnimationManager::Update( unsigned long elapsedMS )
{
	std::vector<NxAnimation*>::iterator i;
	for (i = AnimationList.begin(); i != AnimationList.end(); ++i) {
		(*i)->Update( elapsedMS );
	}
}

}// namespace