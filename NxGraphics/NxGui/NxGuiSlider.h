#ifndef __NXGUISLIDER_H__
#define __NXGUISLIDER_H__

#include "NxGuiWidget.h"

namespace Nx {

class NxGraphics_Export NxSliderListener
{
public:
	// Destructor.
	virtual ~NxSliderListener(){}

	// Called when the slider value has changed.
	virtual void SliderValueChanged (NxSlider* slider) = 0;

};

class NxGraphics_Export NxSlider : public NxWidget
{
public :
	NxSlider( const std::string & SliderText, float PosX, float PosY, float Width, float Height, NxGuiManager * Manager );
	~NxSlider();
	void Update( float Time );
	void RePaint();
	void mouseMoved( int x, int y );
	void mouseDragged( int x, int y, int button );
	bool mousePressed( int x, int y, int button );
	void mouseReleased( int x, int y, int button );
	void keyPressed( int key ) ;
	void keyReleased( int key ) ;
	void SetValue( double newValue, const bool sendUpdateMessage = true, const bool sendMessageSynchronously = false );
	double GetValue() const throw();
	void SetRange( const double newMinimum, const double newMaximum, const double newInterval = 0);
    double GetMaximum() const throw();
    double GetMinimum() const throw();
    double GetInterval() const throw();


	void AddSliderListener( NxSliderListener* const newListener ) throw();
	void RemoveSliderListener( NxSliderListener* const listener ) throw();

	double constrainedValue (double value) const throw();
	void ColourChanged();
 
	enum NxColourIds 
	{
		sliderBackgroundColourId        = 0x1000300,   
		sliderColourId					= 0x1000301,
		sliderTextColourId				= 0x1000302
	};

private :
	virtual void stateChange();
	double currentValue; 
	double minimum, maximum, interval;
	NxRectangle * mSlider;
	NxText * mText;

	typedef std::vector<NxSliderListener*> SliderListener;
	SliderListener mSliderListeners;

};

}


#endif