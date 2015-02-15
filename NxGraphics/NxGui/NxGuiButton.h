#ifndef __NXGUIBUTTON_H__
#define __NXGUIBUTTON_H__

#include "NxGuiWidget.h"

namespace Nx {

class NxGraphics_Export NxButtonListener
{
public:
	// Destructor.
	virtual ~NxButtonListener(){}

	// Called when the button is clicked.
	virtual void ButtonClicked (NxButton* button) = 0;

	// Called when the button's state changes. 
	virtual void ButtonStateChanged (NxButton*) {}
};

class NxGraphics_Export NxButton : public NxWidget
{
public :
	//! NxButton Constructor.
	NxButton( const std::string & ButtonText, float PosX, float PosY, float Width, float Height, NxGuiManager * Manager );
	//! NxButton destructor.
	~NxButton();
	//! Update.
	void Update( float Time ) ;
	//! async repaint.
	void RePaint();
	//! Set button Text label
	void SetText( const std::string & newText) throw();
	void SetTextHeight( float Height );
	void SetVisible( bool Visible, bool AffectChildren = true  );

	void OnViewportResized( float ScreenWidth, float ScreenHeight );
    void AddButtonListener( NxButtonListener* const newListener ) throw();
    void RemoveButtonListener( NxButtonListener* const listener ) throw();
 
	// get Text Label
	NxText * GetTextLabel();

	void mouseMoved(int x, int y );
	void mouseDragged(int x, int y, int button) ;
	bool mousePressed(int x, int y, int button) ;
	void mouseReleased(int x, int y, int button) ;
	void keyPressed(int key) ;
	void keyReleased(int key) ;
	void setRadioGroupId (const int newGroupId);
	int getRadioGroupId() const throw() { return mRadioGroupId; }
    void setToggleState (const bool shouldBeOn,  const bool sendChangeNotification);
    bool getToggleState() const throw() { return mIsOn; }
    void setClickingTogglesState (const bool shouldToggle) throw();
    bool getClickingTogglesState() const throw();
	

    enum NxColourIds 
    {
        buttonColourId                  = 0x1000100,   
        buttonOnColourId                = 0x1000101,  
        buttonClickedColourId           = 0x1000102,  
        buttontextColourId              = 0x1000103
    };

	int mRadioGroupId;
	bool mIsOn : 1;
	bool mClickTogglesState : 1;

	 void ColourChanged();

private :
	virtual void stateChange();
	void turnOffOtherButtonsInGroup (const bool sendChangeNotification);

	NxText * mText;
	typedef std::vector<NxButtonListener*> ButtonListener;
	ButtonListener mButtonListeners;
};

}

#endif