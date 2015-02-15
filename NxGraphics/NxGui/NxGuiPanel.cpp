
#include "NxGraphicsPreCompiled.h"

namespace Nx {

NxPanel::NxPanel( const std::string & HeaderText, float PosX, float PosY, float Width, float Height, NxGuiManager * Manager ) 
: NxWidget( PosX, PosY, Width, Height, Manager ), mHeaderVisible( true )
{
	mHeaderHeight = 20.0f;
	mHeader = new NxRectangle( 0, 0, Width, mHeaderHeight, Manager);
	mHeader->SetColour( FindColour( panelHeaderColourId ) );
	mHeader->SetParent( mBaseRectangle );

	mHeaderText = new NxText( HeaderText, 0, 0, Width, mHeaderHeight, Manager, Manager->GetFontName() );
	mHeaderText->SetParent( mBaseRectangle );
	mHeaderText->SetTextPixelHeight( mHeaderHeight );
	mHeaderText->SetTextPixelSpacing( 3 );
 
	mHeader->SetZOrder(mBaseRectangle->GetZOrder()+1);
	mHeaderText->SetZOrder(mBaseRectangle->GetZOrder()+2);

	//mHeader->SetTexture("NxLogo.jpg");

	//NxLine * Liner = new NxLine( 0, 0, Width, Height, Manager);
	//Liner->SetColour( Ogre::ColourValue::Green );
	//Liner->SetParent( mBaseRectangle );
	//Liner->addPoint(0,0,0);
	//Liner->addPoint(Width,0,0);
	//Liner->addPoint(Width,Height,0);
	//Liner->addPoint(0,Height,0);
	//Liner->Draw();
 
}

NxPanel::~NxPanel()
{

} 
 
void NxPanel::SetVisibilityFlags( int Flags )
{
	mHeader->SetVisibilityFlags( Flags );
	mHeaderText->SetVisibilityFlags( Flags );
	mBaseRectangle->SetVisibilityFlags( Flags );
}

void NxPanel::SetZOrder( unsigned short ZOrder )
{
	mBaseRectangle->SetZOrder(ZOrder);
	mHeader->SetZOrder(ZOrder+1);
	mHeaderText->SetZOrder(ZOrder+2);
}

NxButton * NxPanel::CreateButton( const std::string & ButtonText, float PosX, float PosY, float Width, float Height ) {

	NxButton * c = new NxButton( ButtonText, PosX, PosY, Width, Height, mManager );
	AddComponent( c );
	c->SetVisible( IsVisible() );
	return c;
}

NxButtonToggle * NxPanel::CreateButtonToggle( const std::string & ButtonText, float PosX, float PosY, float Width, float Height )
{
	NxButtonToggle * c = new NxButtonToggle( ButtonText, PosX, PosY, Width, Height, mManager );
	AddComponent( c );
	c->SetVisible( IsVisible() );
	return c;
}

NxButtonRadio * NxPanel::CreateButtonRadio( const std::string & ButtonText, int groupId, float PosX, float PosY, float Width, float Height )
{
	NxButtonRadio * c = new NxButtonRadio( ButtonText, groupId, PosX, PosY, Width, Height, mManager );
	AddComponent( c );
	c->SetVisible( IsVisible() );
	return c;
}

NxSlider * NxPanel::CreateSlider( float PosX, float PosY, float Width, float Height )
{
	NxSlider * c = new NxSlider( "", PosX, PosY, Width, Height, mManager );
	AddComponent( c );
	c->SetVisible( IsVisible() );
	return c;
}

NxLabel * NxPanel::CreateLabel( const std::string & Text, float PosX, float PosY, float Width, float Height )
{
	NxLabel * c = new NxLabel(  Text, PosX, PosY, Width, Height, mManager );
	AddComponent( c );
	c->SetVisible( IsVisible() );
	return c;
}

void NxPanel::OnViewportResized( float ScreenWidth, float ScreenHeight )
{
	//NxWidget::OnViewportResized( ScreenWidth, ScreenHeight );

	//mHeader->OnViewportResized( ScreenWidth, ScreenHeight );
	//mHeaderText->OnViewportResized( ScreenWidth, ScreenHeight );

	for( int i = 0 ; i < mChilds.size(); i++ ) {
		mChilds[i]->OnViewportResized( ScreenWidth, ScreenHeight );
	}
}

void NxPanel::SetHeaderHeight( float height )
{
	mHeaderHeight = height;
	mHeader->setHeight( mHeaderHeight );
	mHeaderText->setHeight( mHeaderHeight );
}

void NxPanel::SetHeaderTextHeight( float height )
{
	mHeaderText->SetTextPixelHeight(  height );
}

void NxPanel::SetHeaderText( const std::string & text )
{
	mHeaderText->SetCaption( text );
}

const float NxPanel::GetHeaderHeight()
{
	return mHeaderHeight;
}

void NxPanel::SetVisible( bool Visible, bool AffectChildren  )
{
	NxWidget::SetVisible( Visible );
	if( mHeaderVisible ){
		mHeader->SetVisible( Visible );
		mHeaderText->SetVisible( Visible );
	}

	if( AffectChildren ) {
	for( int i = 0 ; i < mChilds.size(); i++ ) {
		mChilds[i]->SetVisible( Visible );
	} }
}

void NxPanel::SetHeaderVisible( bool Visible )
{
	mHeaderVisible = Visible;
	mHeader->SetVisible( Visible );
	mHeaderText->SetVisible( Visible );
}

bool NxPanel::IsHeaderVisible()
{
	return mHeaderVisible;
}

void NxPanel::Update( float Time )
{
	NxWidget::Update( Time );
	mHeader->Draw();
	mHeaderText->Draw();

	for( int i = 0 ; i < mChilds.size(); i++ ) {
		mChilds[i]->Update( Time );
	}
}

void NxPanel::RePaint()
{

}

void NxPanel::ColourChanged()
{
	mBaseRectangle->SetColour( FindColour( panelColourId ) );
	mHeader->SetColour( FindColour( panelHeaderColourId ) );
	mHeaderText->SetColour( FindColour( panelHeaderTextColourId ) );
}
 


void NxPanel::SetPosition( float PosX, float PosY )
{
	NxWidget::SetPosition( PosX, PosY );
	for( int i = 0 ; i < mChilds.size(); i++ ) {
		mChilds[i]->RePaint();
	}
}

void NxPanel::SetSize( float Width, float Height )
{
	NxWidget::SetSize( Width, Height );
	mHeader->setWidth( Width );
	mHeader->setHeight( mHeaderHeight );
}

void NxPanel::mouseDragged( int x, int y, int button )
{
	for( int i = 0 ; i < mChilds.size(); i++ ) {
		mChilds[i]->mouseDragged( x, y, button );
	}
}

void NxPanel::mouseMoved( int x, int y )
{
	if( mHit && mLeftDown && mMovable ) { // move panel
		this->SetPosition( x - mHitPoint.x, y - mHitPoint.y );
		NxWidget::mouseMoved( x, y ); // send callback to listeners
	}

	for( int i = 0 ; i < mChilds.size(); i++ ) { // send information to attached childs
		mChilds[i]->mouseMoved( x, y );
	}
}

bool NxPanel::mousePressed( int x, int y, int button )
{
	
	if( isHit( x, y )  ) {
		
		mLeftDown = ( button == MB_Left ) ? true : false;
		mHit = true;
		mHitPoint.x = x - GetX();
		mHitPoint.y = y - GetY();
		if(mLeftDown) NxWidget::mousePressed( x, y, button );
	}

 
	for( int i = 0 ; i < mChilds.size(); i++ ) {
		mChilds[i]->mousePressed( x, y, button );
	}

	return mHit;
}

void NxPanel::mouseReleased( int x, int y, int button )
{
	mHit = false;
	mLeftDown = false;

	NxWidget::mouseReleased( x, y, button );
	for( int i = 0 ; i < mChilds.size(); i++ ) {
		mChilds[i]->mouseReleased( x,y,button );
	}
}

void NxPanel::keyPressed(int key) {

}

void NxPanel::keyReleased(int key) {

}

}