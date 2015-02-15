#include "NxGraphicsPreCompiled.h"

namespace Nx {

NxSlider::NxSlider( const std::string & SliderText, float PosX, float PosY, float Width, float Height, NxGuiManager * Manager )
: NxWidget( PosX, PosY, Width, Height, Manager ), currentValue( 0.0 )
{

	mBaseRectangle->SetColour( FindColour( sliderBackgroundColourId ) );
 
	mSlider = new NxRectangle( 0, 0, Width, Height, Manager);
	mSlider->SetColour( FindColour( sliderColourId ) );
	mSlider->SetParent( mBaseRectangle );
	mSlider->SetZOrder(101);

	mText = new NxText( "0", 0, 0, Width, Height, Manager, Manager->GetFontName());
	mText->SetParent( mBaseRectangle );
	mText->SetHorizontalAlignment(NxTextHRight);
	mText->SetZOrder(102);

	SetRange( 0, 100, 1);
	SetValue( 0 );
}

NxSlider::~NxSlider()
{

}

void NxSlider::Update( float Time )
{
	NxWidget::Update( Time );
	mSlider->Draw();
	mText->Draw();
}

void NxSlider::RePaint()
{


}

void NxSlider::ColourChanged()
{
	mBaseRectangle->SetColour( FindColour( sliderBackgroundColourId ) );
	mSlider->SetColour( FindColour( sliderColourId ) );
	mText->SetColour( FindColour( sliderTextColourId ) );
}

void NxSlider::mouseMoved(int x, int y )
{
	if( mHit ) {
		double Slid = (double)( x- mBaseRectangle->getX() ) / (double)mBaseRectangle->getWidth(); // 0 to 1
		double Gap = maximum - minimum;
		SetValue( Slid * Gap + minimum, false, false );
	}
}

void NxSlider::mouseDragged(int x, int y, int button)
{

}

bool NxSlider::mousePressed(int x, int y, int button)
{
	if( mBaseRectangle->IsInside(x, y) ) {

		mHit = true;
		mState = NXWIDGET_STATE_DOWN;
		SetValue( ((double)( x- mBaseRectangle->getX() ) / (double)mBaseRectangle->getWidth() ) * maximum , false, false );
		triggerEvent(this);
	}
	else {
		mState = NXWIDGET_STATE_NORMAL;
	}

	return mHit;
 
}

void NxSlider::mouseReleased(int x, int y, int button)
{
	mHit = false;
}

void NxSlider::keyPressed(int key)
{

}

void NxSlider::keyReleased(int key)
{

}

double NxSlider::constrainedValue( double value ) const throw()
{
	if (interval > 0)
		value = minimum + interval * floor ((value - minimum) / interval + 0.5);

	if (value <= minimum || maximum <= minimum)
		value = minimum;
	else if (value >= maximum)
		value = maximum;

	return value;
}

void NxSlider::SetValue( double newValue, const bool sendUpdateMessage /*= true*/, const bool sendMessageSynchronously /*= false */ )
{
	newValue = constrainedValue( newValue );

	if( currentValue != newValue ) {
		currentValue = newValue;
		mSlider->setWidth( ( (currentValue - minimum) /  (maximum - minimum) ) * (double)mBaseRectangle->getWidth() );
		mText->SetCaption( Ogre::StringConverter::toString( (float) currentValue ) );
		//LogManager::getSingleton().logMessage( "Done Value : " + Ogre::StringConverter::toString( (float) currentValue )    );
		stateChange();
	}
}

double NxSlider::GetValue() const throw()
{
	return currentValue;
}

void NxSlider::SetRange( const double newMinimum, const double newMaximum, const double newInterval /*= 0*/ )
{
	if( minimum != newMinimum || maximum != newMaximum || interval != newInterval) {

		minimum = newMinimum; maximum = newMaximum; interval = newInterval;
		SetValue (currentValue, false, false);
	}

}

double NxSlider::GetMaximum() const throw()
{ 
	return maximum; 
}

double NxSlider::GetMinimum() const throw()
{
	return minimum; 
}

double NxSlider::GetInterval() const throw() 
{ 
	return interval;
}
 

void NxSlider::AddSliderListener( NxSliderListener* const newListener ) throw()
{
	if (newListener != 0)
		mSliderListeners.push_back(newListener);
}

void NxSlider::RemoveSliderListener( NxSliderListener* const listener ) throw()
{
	std::vector<NxSliderListener*>::iterator Iter = mSliderListeners.begin();
	while( Iter != mSliderListeners.end() ){
		if( listener == *Iter ){ mSliderListeners.erase(Iter); break; }
		Iter++;
	}
}

void NxSlider::stateChange()
{
	SliderListener::iterator Iter = mSliderListeners.begin();
	while( Iter !=  mSliderListeners.end() ) {
		(*Iter++)->SliderValueChanged( this );	 
	}
}




}
