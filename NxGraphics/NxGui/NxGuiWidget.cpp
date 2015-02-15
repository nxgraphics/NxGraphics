#include "NxGraphicsPreCompiled.h"

namespace Nx {

	static uint32 nextComponentUID = 0;

NxWidget::NxWidget( float PosX, float PosY, float Width, float Height, NxGuiManager * Manager ) : 
mManager( Manager ), mHitPoint(0,0), mHit( false ), mLeftDown( false ), mVisible( true ), mParent( 0 ),
mMovable( true ) 
{
	mBaseRectangle = new NxRectangle( PosX, PosY, Width, Height, Manager );
	componentUID  = ++nextComponentUID;

	mIgnoresMouseClicksFlag = false;
	mAllowChildMouseClicksFlag = true;

}

NxWidget::~NxWidget()
{

}

void NxWidget::SetVisible(bool Visible, bool AffectChildren )
{
	mBaseRectangle->SetVisible( Visible );
	mVisible = Visible;
}

bool NxWidget::IsVisible() {
	return mVisible;
}

void NxWidget::SetZOrder( unsigned short ZOrder ) {
	mBaseRectangle->SetZOrder( ZOrder );
}

unsigned short NxWidget::GetZOrder() {
	return mBaseRectangle->GetZOrder();
}

void NxWidget::AddComponent( NxWidget * ChildComponent )
{
	ChildComponent->mParent = this;
	ChildComponent->GetRectangle()->SetParent( mBaseRectangle );
	mChilds.push_back( ChildComponent );
}

unsigned int NxWidget::GetNumChildComponents()
{
	return mChilds.size();
}

NxWidget* NxWidget::GetChildComponent (const int index) const throw()
{
	return mChilds[index];
}

void NxWidget::RemoveComponent( NxWidget * ChildComponent ) {

	std::vector<NxWidget *>::iterator it = std::find(mChilds.begin(), mChilds.end(), ChildComponent );
	if (it != mChilds.end()) {
		int index = std::distance(mChilds.begin(), it);
		RemoveComponent ( index );
	}

}
 
NxWidget* NxWidget::RemoveComponent (const int index) {

	NxWidget* const child = mChilds [index];

	if (child != 0) { 
		mChilds.erase(std::remove(mChilds.begin(), mChilds.end(), child), mChilds.end());
		child->mParent = 0;
		child->mBaseRectangle->SetParent(0);
		//delete child->mBaseRectangle;

	}
 
	return child;
}


void NxWidget::SetMovable( bool Movable )
{
	mMovable = Movable;
}

void NxWidget::Update( float Time )
{
	mBaseRectangle->Draw();
}

void NxWidget::SetPosition( float PosX, float PosY ) {
	mBaseRectangle->SetTopLeftPosition( PosX, PosY );
}

void NxWidget::OnViewportResized( float ScreenWidth, float ScreenHeight )
{
	mBaseRectangle->OnViewportResized( ScreenWidth, ScreenHeight );
}

void NxWidget::SetSize( float Width, float Height )
{
	mBaseRectangle->setWidth( Width );
	mBaseRectangle->setHeight( Height );
}

unsigned int NxWidget::GetWidth()
{
	return mBaseRectangle->getWidth();
}

unsigned int NxWidget::GetHeight()
{
	return mBaseRectangle->getHeight();
}

int NxWidget::GetX()
{
	return mBaseRectangle->getX();
}

int NxWidget::GetY() {
	return mBaseRectangle->getY();
}

int NxWidget::getDerivedPositionX() {
	return mBaseRectangle->getPositionDerivedX();
}
 
int NxWidget::getDerivedPositionY() {
	return mBaseRectangle->getPositionDerivedY();
}

void NxWidget::SetBackgroundTexture( const std::string & TextureName ) {
	mBaseRectangle->SetTexture( TextureName );
}

void NxWidget::setTextureCoordinates( const  Nx::Vector2 & topLeft, const  Nx::Vector2 & bottomLeft, const Nx::Vector2 & topRight, const Nx::Vector2 & bottomRight ) {
	mBaseRectangle->setTextureCoordinates( topLeft, bottomLeft, topRight, bottomRight );
}

bool NxWidget::hasTextureCoordinates() {
	return mBaseRectangle->hasTextureCoordinates();
}

const std::string & NxWidget::getBackgroundTextureName() const {
 
	//MessageBox( 0,  mBaseRectangle->getBackgroundTextureName().c_str(), "", 0 );   

	return mBaseRectangle->getBackgroundTextureName();
}

void NxWidget::getTextureCoordinates( Vector2 & topLeft, Vector2 & bottomLeft, Vector2 & topRight, Vector2 & bottomRight ) {
	mBaseRectangle->getTextureCoordinates( topLeft, bottomLeft, topRight, bottomRight );
}




NxWidgetType NxWidget::GetType()
{
	return mType;
}

void NxWidget::mouseMoved( int x, int y )
{
	WidgetListener::iterator Iter = mWidgetListeners.begin();
	while( Iter !=  mWidgetListeners.end() ) {
		(*Iter++)->OnMoved(this); 
	}

}

void NxWidget::mouseDragged(int x, int y, int button)
{


}

bool NxWidget::mousePressed(int x, int y, int button)
{
	WidgetListener::iterator Iter = mWidgetListeners.begin();
	while( Iter !=  mWidgetListeners.end() ) {
		(*Iter++)->OnPressed(this); 
	}
	return true;
}

void NxWidget::mouseReleased(int x, int y, int button)
{
	WidgetListener::iterator Iter = mWidgetListeners.begin();
	while( Iter !=  mWidgetListeners.end() ) {
		(*Iter++)->OnReleased(this); 
	}
}

void NxWidget::keyPressed(int key)
{


}

void NxWidget::keyReleased(int key)
{

}

void NxWidget::windowResized(int w, int h)
{


}


#ifdef __ANDROID__
#include <android/log.h>
#define  LOG_TAG    "NXWIDGET"
#define  LOGD(...)  __android_log_print(ANDROID_LOG_DEBUG, LOG_TAG, __VA_ARGS__)
#define  LOGE(...)  __android_log_print(ANDROID_LOG_ERROR, LOG_TAG, __VA_ARGS__)

#else 

#define  LOGD( ...)  printf( __VA_ARGS__  );


#endif

 
NxWidget* NxWidget::getComponentAt( const int x, const int y ) {
	
	if ( mVisible && ((unsigned int) x) < (unsigned int) GetWidth() && ((unsigned int) y) < (unsigned int) GetHeight() && isHit( x, y) ) {
		for (int i = mChilds.size(); --i >= 0;) {
			NxWidget* const child = mChilds[i];
			NxWidget* const c = child->getComponentAt (x - child->GetX(), y - child->GetY());

			if (c != 0)
				return c;
		}
		return this;
	}
	return 0;
}
 
void NxWidget::derivedPositionToLocal( int& x, int& y ) {
	mBaseRectangle->derivedPositionToLocal( x, y );
}

void NxWidget::localPositionToDerived( int& x, int& y ) {
	mBaseRectangle->localPositionToDerived( x, y );
}

bool NxWidget::isValidComponent() const throw()
{
	return (this != 0) /*&& isValidMessageListener()*/;
}


void NxWidget::setInterceptsMouseClicks( const bool allowClicks, const bool allowClicksOnChildComponents)  {
	mIgnoresMouseClicksFlag = ! allowClicks;
	mAllowChildMouseClicksFlag = allowClicksOnChildComponents;
}

bool NxWidget::isHit( float x, float y ) {
 

	int srcX = x;
	int srcY = y;
	mBaseRectangle->localPositionToDerived( srcX, srcY ); 

	if (!mIgnoresMouseClicksFlag) { // accepts clicks
		return mBaseRectangle->IsInside( srcX, srcY );
	}

	if (mAllowChildMouseClicksFlag) // accepts childs clicks.
	{
		 for( int i = mChilds.size(); --i >= 0; ) {
			NxWidget* const c = mChilds[i];
	
			if ( c->mVisible && c->mBaseRectangle->IsInside( srcX, srcY ) && c->isHit (x - c->GetX(), y - c->GetY())) {
				return true;
			} 
		}
	}

	return false;

	 
	
	 
	 /*
	if( mVisible ) { return mBaseRectangle->IsInside(x, y); }
	else { return false; }  */
}

void NxWidget::triggerEvent( NxWidget *child )
{
	if(mParent != NULL) {
		mParent->triggerEvent(child);
	}
}

NxWidgetState NxWidget::GetState() {
	return mState;
}

const NxColourValue NxWidget::FindColour( const int colourId, const bool inheritFromParent ) {
	ColourValue::iterator Iter = mColours.find( colourId );
	if (Iter != mColours.end()) { // item found
		return Iter->second;
	}
	return mManager->GetColours()->FindColour( colourId );
}

void NxWidget::SetColour( const int colourId, const NxColourValue & colour ) {
	ColourValue::iterator Iter = mColours.find( colourId );
	if (Iter != mColours.end()) { // id found
		 Iter->second = colour;
	} else { // create it
		mColours.insert( std::make_pair( colourId, colour  ) );
	}

	//mManager->GetColours()->SetColour(colourId, mManager->GetColours()->Pack( colour ) );
	ColourChanged();
}

const NxColourValue & NxWidget::getColour( const int colourId ) const {
	ColourValue::const_iterator Iter = mColours.find( colourId );
	if (Iter != mColours.end()) { // id found
		return Iter->second;
	}
	throw   "invalid colourId"  ; 
	 
}

void NxWidget::ColourChanged()
{


}

void NxWidget::AddWidgetListener( NxWidgetListener* const newListener ) throw()
{
	if (newListener != 0)
		mWidgetListeners.push_back(newListener);
}

void NxWidget::RemoveWidgetListener( NxWidgetListener* const listener ) throw()
{
	std::vector<NxWidgetListener*>::iterator Iter = mWidgetListeners.begin();
	while( Iter != mWidgetListeners.end() ){
		if( listener == *Iter ){ mWidgetListeners.erase(Iter); break; }
		Iter++;
	}
}

}

 