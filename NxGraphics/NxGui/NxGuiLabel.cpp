#include "NxGraphicsPreCompiled.h"

namespace Nx {
 
NxLabel::NxLabel( const std::string & Text, float PosX, float PosY, float Width, float Height, NxGuiManager * Manager ) : NxWidget( PosX, PosY, Width, Height, Manager )
{
	//mBaseRectangle->SetColour( Ogre::ColourValue(0,0,0,0) );
	mText = new NxText( Text, 0, 0, Width, Height, Manager, Manager->GetFontName() );
	mText->SetParent( mBaseRectangle );
	//mText->SetTextPixelHeight( Manager->getDefaultFontSize() );//  18 );
	//mText->SetTexColour( Ogre::ColourValue::Blue );
}

NxLabel::~NxLabel()
{

}

void NxLabel::SetTextPixelsHeight( float Height )
{
	mText->SetTextPixelHeight( Height );
}

void NxLabel::SetTextPixelsSpacing( float Spacing )
{
	mText->SetTextPixelSpacing( Spacing );
}

void NxLabel::ColourChanged()
{
	mBaseRectangle->SetColour( FindColour( backgroundColourId ) );
	mText->SetColour( FindColour( labeltextColourId ) );
}

void NxLabel::Update( float Time )
{
	NxWidget::Update( Time );
	mText->Draw();
}
void NxLabel::RePaint(){

}
void NxLabel::SetVisible( bool Visible, bool AffectChildren  ){

}
void NxLabel::OnViewportResized( float ScreenWidth, float ScreenHeight ){
	NxWidget::OnViewportResized( ScreenWidth, ScreenHeight );
	mText->OnViewportResized( ScreenWidth, ScreenHeight );

}

void NxLabel::SetVerticalAlignment( NxTextVerticalAlignment VAlign )
{
	mText->SetVerticalAlignment( VAlign );
}

void NxLabel::SetHorizontalAlignment( NxTextHorizontalAlignment HAlign )
{
	mText->SetHorizontalAlignment( HAlign );
}

void NxLabel::mouseMoved(int x, int y ){

}

void NxLabel::mouseDragged(int x, int y, int button) {

}

bool NxLabel::mousePressed(int x, int y, int button) {

	if( mBaseRectangle->IsInside(x, y) ) {

		mHit = true;
		mState = NXWIDGET_STATE_DOWN;
		triggerEvent(this);
	}
	else {
		mState = NXWIDGET_STATE_NORMAL;
	}

	return mHit;
}

void NxLabel::mouseReleased(int x, int y, int button) {

	mHit = false;

}

void NxLabel::keyPressed(int key) {

}

void NxLabel::keyReleased(int key) {

}
 
void NxLabel::stateChange(){

}

}