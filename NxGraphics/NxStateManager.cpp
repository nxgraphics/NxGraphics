#include "NxGraphicsPreCompiled.h"

namespace Nx {

	//http://www.dreamincode.net/forums/topic/118898-state-management/

NxStateManager::NxStateManager()
{


}

NxStateManager::~NxStateManager()
{

}

 
void NxStateManager::ChangeState(NxState* state)
{
	// Cleanup the current state.
	if (!mStates.empty()) {
		mStates.back()->Cleanup();
		mStates.pop_back();
	}

	// Store and init the new state.
	mStates.push_back(state);
	mStates.back()->Init();

}

void NxStateManager::PushState(NxState* state)
{
	if (!mStates.empty())
	mStates.back()->Pause();

	mStates.push_back(state);
	mStates.back()->Init();
}

void NxStateManager::RemoveState( NxState * state )
{

}

void NxStateManager::PopState()
{
	if (!mStates.empty()) {
		mStates.back()->Cleanup();
		mStates.pop_back();
	}

	if (!mStates.empty())
	mStates.back()->Resume();

}

void NxStateManager::Clear()
{
	while( !mStates.empty()) {
		mStates.back()->Cleanup();
		mStates.pop_back();
	}
}

void NxStateManager::Update( float Seconds )
{
	if (!mStates.empty())
	mStates.back()->Update( Seconds );
}


}