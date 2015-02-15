#ifndef __NXGUIWIDGET_H__
#define __NXGUIWIDGET_H__

#include <NxPrerequisites.h>

namespace Nx {

class NxGraphics_Export NxWidgetListener
{
public:
	// Destructor.
	virtual ~NxWidgetListener(){}
	// on pressed
	virtual void OnPressed( NxWidget* widget ) = 0;
	// on released
	virtual void OnReleased( NxWidget* widget ) = 0;
	// Called when the widget is moved.
	virtual void OnMoved( NxWidget* widget ) = 0;
	// Called when the widget changes. 
	virtual void OnResized ( NxWidget* widget )= 0;
};

class NxGraphics_Export NxWidget
{
protected :
	NxWidget( float PosX, float PosY, float Width, float Height, NxGuiManager * Manager );
public :

	enum NxButtonID
	{
		MB_Left = 0, MB_Right, MB_Middle,
		MB_Button3, MB_Button4,	MB_Button5, MB_Button6,	MB_Button7
	};

	virtual ~NxWidget();
	virtual void SetVisible(bool Visible, bool AffectChildren = true );
	virtual bool IsVisible();
	NxWidgetType GetType();
	virtual void Update( float Time );
	virtual void RePaint() = 0;
	virtual void mouseMoved(int x, int y );
	virtual void mouseDragged(int x, int y, int button);
	virtual bool mousePressed(int x, int y, int button);
	virtual void mouseReleased(int x, int y, int button);
	virtual void keyPressed(int key) ;
	virtual void keyReleased(int key) ;
	virtual void windowResized(int w, int h);
	virtual bool isHit(float x, float y);
	virtual void triggerEvent(NxWidget *child);
	virtual void SetPosition( float PosX, float PosY );
	virtual int GetX();
	virtual int GetY();
	//! set z order.
	virtual void SetZOrder( unsigned short ZOrder );
	unsigned short GetZOrder();
	//! set background texture
	void SetBackgroundTexture( const std::string & TextureName );
	//! set texture coordiantes
	void setTextureCoordinates( const  Nx::Vector2 &topLeft, const  Nx::Vector2 &bottomLeft, const  Nx::Vector2 &topRight, const  Nx::Vector2 &bottomRight);
	//! get derived x position from all parents: screen position
	int getDerivedPositionX();
	//! get derived y position from all parents: screen position
	int getDerivedPositionY();

	virtual void SetSize( float Width, float Height );
	unsigned int GetWidth();
	unsigned int GetHeight();
 
	virtual void OnViewportResized( float ScreenWidth, float ScreenHeight );
	NxRectangle * GetRectangle(){ return mBaseRectangle; }

	const NxColourValue FindColour( const int colourId, const bool inheritFromParent = false ) ;
	void SetColour( const int colourId, const NxColourValue & colour);
	const NxColourValue & getColour( const int colourId ) const;
	virtual void ColourChanged();

	unsigned int GetNumChildComponents();
	NxWidget* GetChildComponent (const int index) const throw();
	void SetMovable( bool Movable ); 


	bool hasTextureCoordinates();
	const std::string & getBackgroundTextureName() const;
	void getTextureCoordinates( Vector2 & topLeft, Vector2 & bottomLeft, Vector2 & topRight, Vector2 & bottomRight ); 


	enum NxColourIds 
	{
		panelColourId			= 0x2000100

	};
	

	NxWidgetState GetState();

	NxWidget * getParentComponent() { return mParent; }

  NxWidget* getComponentAt (const int x, const int y);

	void AddComponent( NxWidget * ChildComponent ); 
	//! remove component.
	void RemoveComponent( NxWidget * ChildComponent );
	NxWidget * RemoveComponent( const int childIndexToRemove );

	void AddWidgetListener( NxWidgetListener* const newListener ) throw();
	void RemoveWidgetListener( NxWidgetListener* const listener ) throw();


	uint32 getComponentUID() const throw()                { return componentUID; }

	//! convert local position to derived.
	void localPositionToDerived( int& x, int& y );
	//! convert derived position to local.
	void derivedPositionToLocal( int& x, int& y );


	void setInterceptsMouseClicks( const bool allowClicks, const bool allowClicksOnChildComponents) ;

	/* Searches the parent components for a component of a specified class.
        For example findParentComponentOfClass \<MyComp\>() would return the first parent
        component that can be dynamically cast to a MyComp, or will return 0 if none
        of the parents are suitable.
        N.B. The dummy parameter is needed to work around a VC6 compiler bug.
    */
    template <class TargetClass>
    TargetClass* findParentComponentOfClass( TargetClass * const dummyParameter = 0 ) const {
        (void) dummyParameter;
        NxWidget* p = mParent;
        while (p != 0) {
            TargetClass* target = dynamic_cast <TargetClass*> (p);
            if (target != 0)
                return target;

            p = p->mParent;
        }
        return 0;
    }

   //==============================================================================
    /** Checks whether this Component object has been deleted.

        This will check whether this object is still a valid component, or whether
        it's been deleted.

        It's safe to call this on null or dangling pointers, but note that there is a
        small risk if another new (but different) component has been created at the
        same memory address which this one occupied, this methods can return a
        false positive.
    */
    bool isValidComponent() const throw();


protected :
	

	typedef std::vector<NxWidgetListener*> WidgetListener;
	WidgetListener mWidgetListeners;


	typedef std::map<int,NxColourValue> ColourValue;
	ColourValue mColours;

	NxWidgetType mType;
	bool mVisible;
	bool mHit;
	bool mLeftDown;
	NxWidgetState mState;
	Vector2 mHitPoint;
	NxGuiManager * mManager;
	NxWidget * mParent;
	NxRectangle * mBaseRectangle;  
	std::vector <NxWidget *> mChilds;
	bool mMovable;
	uint32 componentUID;
	bool mIgnoresMouseClicksFlag;
	bool mAllowChildMouseClicksFlag;
};

}

#endif

