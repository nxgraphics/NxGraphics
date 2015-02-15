#include "NxGraphicsPreCompiled.h"

namespace Nx {

NxButton::NxButton( const std::string & ButtonText, float PosX, float PosY, float Width, float Height, NxGuiManager * Manager )
: NxWidget( PosX, PosY, Width, Height, Manager ), mRadioGroupId(0), mIsOn(false), mClickTogglesState(false)
{
	mText = new NxText( ButtonText, 0, 0, Width, Height, Manager, Manager->GetFontName() );
	mText->SetParent( mBaseRectangle );
	//mText->SetTextPixelHeight( 18 );  Manager->getdefaultfont
	//mText->SetTextPixelSpacing( 2 );



	//mText->SetTexColour( Ogre::ColourValue::Blue );
	//mText->SetTextWidthWrap( true );
}

NxButton::~NxButton()
{

}

void NxButton::SetText (const std::string & newText) throw()
{
	if ( mText->GetCaption() != newText){
		mText->SetCaption( newText );
	}
}

void NxButton::SetTextHeight( float Height )
{
	mText->SetTextPixelHeight( Height );
}

void NxButton::ColourChanged()
{
	mBaseRectangle->SetColour( FindColour( buttonColourId ) );
	mText->SetColour( FindColour( buttontextColourId ) );
}

void NxButton::SetVisible( bool Visible, bool AffectChildren  )
{
	NxWidget::SetVisible( Visible );
	mText->SetVisible( Visible );
}

void NxButton::OnViewportResized( float ScreenWidth, float ScreenHeight )
{
	NxWidget::OnViewportResized( ScreenWidth, ScreenHeight );
	mText->OnViewportResized( ScreenWidth, ScreenHeight );
}

void NxButton::AddButtonListener( NxButtonListener* const newListener ) throw()
{
	if (newListener != 0)
		mButtonListeners.push_back(newListener);
}

void NxButton::RemoveButtonListener( NxButtonListener* const listener ) throw()
{
	std::vector<NxButtonListener*>::iterator Iter = mButtonListeners.begin();
	while( Iter != mButtonListeners.end() ){
		if( listener == *Iter ){ mButtonListeners.erase(Iter); break; }
		Iter++;
	}
}

void NxButton::Update( float Time )
{
	NxWidget::Update( Time );
	mText->Draw();
}

void NxButton::RePaint()
{
	
}

NxText * NxButton::GetTextLabel()
{
	return mText;
}

void NxButton::setToggleState( const bool shouldBeOn, const bool sendChangeNotification )
{
	if (shouldBeOn != mIsOn) {
		mIsOn = shouldBeOn;
		mBaseRectangle->SetColour( FindColour( mIsOn ? buttonClickedColourId : buttonOnColourId   )    );
		if (mIsOn) turnOffOtherButtonsInGroup (sendChangeNotification);
	}
}

void NxButton::setClickingTogglesState (const bool shouldToggle) throw()
{
	mClickTogglesState = shouldToggle;
}

bool NxButton::getClickingTogglesState() const throw()
{
	return mClickTogglesState;
}

/////

void NxButton::setRadioGroupId (const int newGroupId)
{
	if (mRadioGroupId != newGroupId) {
		mRadioGroupId = newGroupId;
		if (mIsOn) turnOffOtherButtonsInGroup (true);
	}
}


void NxButton::turnOffOtherButtonsInGroup (const bool sendChangeNotification)
{
	NxWidget* const p = mParent;
	if (p != 0 && mRadioGroupId != 0) {
		for (int i = p->GetNumChildComponents(); --i >= 0;) {
			NxWidget* const c = p->GetChildComponent (i);
			if (c != this) {
				NxButton* const b = dynamic_cast <NxButton*> (c);
				if (b != 0 && b->getRadioGroupId() == mRadioGroupId) {
					b->setToggleState (false, sendChangeNotification);
				}
			}
		}
	}
}

////////

void NxButton::mouseMoved( int x, int y )
{
	if(mBaseRectangle->IsInside(x, y)) {
		mState = NXWIDGET_STATE_OVER;

		if (!mClickTogglesState)
			mBaseRectangle->SetColour( FindColour( mHit ? buttonClickedColourId : buttonOnColourId ) ); 
		else
			if( !mIsOn ) mBaseRectangle->SetColour( FindColour( buttonOnColourId ) ); 
	}
	else {
		if( mState == NXWIDGET_STATE_OVER && !mClickTogglesState ) mBaseRectangle->SetColour( FindColour( buttonColourId ) ); // it was over, but isnt anymore
		else
			if( !mIsOn ) mBaseRectangle->SetColour( FindColour( buttonColourId ) ); 

		mState = NXWIDGET_STATE_NORMAL;
	}
	stateChange(); 

}

void NxButton::mouseDragged(int x, int y, int button)
{

	// see here :
// http://www.juce.com/forum/topic/problems-dragging-component





	if( mHit ) {
		if(mBaseRectangle->IsInside(x, y)) {
			mState = NXWIDGET_STATE_DOWN;
		}
		else {
			mHit = false;
			mState = NXWIDGET_STATE_NORMAL;
			triggerEvent(this);
		}
		stateChange();
	}
}

bool NxButton::mousePressed(int x, int y, int button)
{

	 
 
	if(mBaseRectangle->IsInside(x, y)) {

		
		
		mHit = true;
		mState = NXWIDGET_STATE_DOWN;

		if (mClickTogglesState)
			setToggleState ((mRadioGroupId != 0) || !mIsOn, false);
		else
		mBaseRectangle->SetColour( FindColour( buttonClickedColourId ) );

		ButtonListener::iterator Iter = mButtonListeners.begin();
		while( Iter !=  mButtonListeners.end() ) {
			(*Iter++)->ButtonClicked( this );	 
		}

		triggerEvent(this);
	}
	else {
		mState = NXWIDGET_STATE_NORMAL;
	}
	stateChange(); 

	return mHit;

}

void NxButton::mouseReleased(int x, int y, int button) 
{
	if(mHit) {
		if(mBaseRectangle->IsInside(x, y)) {
			mState = NXWIDGET_STATE_OVER;
			if (!mClickTogglesState) mBaseRectangle->SetColour( FindColour( buttonOnColourId )  );	 
		}
		else {
			mState = NXWIDGET_STATE_NORMAL;
		}
		triggerEvent(this);
	}
	else {
		mState = NXWIDGET_STATE_NORMAL;
	}
	stateChange();
	mHit = false; 

}

void NxButton::stateChange()
{

	ButtonListener::iterator Iter = mButtonListeners.begin();
	while( Iter !=  mButtonListeners.end() ) {
		(*Iter++)->ButtonStateChanged( this );	 
	}

	 

	/*
	switch (state) {
			case NXWIDGET_STATE_NORMAL:
				{
					//draw_fill_highlight = false;
					//draw_outline_highlight = false;
					//label->unfocus();
				}
				break;
			case NXWIDGET_STATE_OVER:
				{
					//draw_fill_highlight = false;
					//draw_outline_highlight = true;
					//label->focus();
				}
				break;
			case NXWIDGET_STATE_DOWN:
				{
					//draw_fill_highlight = true;
					//draw_outline_highlight = false;
					//label->focus();
				}
				break;
			//case NXWIDGET_STATE_SUSTAINED:
			//	{
			//		//draw_fill_highlight = false;
			//		//draw_outline_highlight = false;
			//		//label->unfocus();
			//	}
			//	break;

			default:
				break;
	}
	*/
}


void NxButton::keyPressed(int key) 
{


}

void NxButton::keyReleased(int key)
{


}

}


