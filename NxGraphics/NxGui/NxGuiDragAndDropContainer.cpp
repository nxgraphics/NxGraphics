#include "NxGraphicsPreCompiled.h"


#ifdef __ANDROID__
#include <android/log.h>
#define  LOG_TAG    "DRAGDROPCONTAINER"
#define  LOGD(...)  __android_log_print(ANDROID_LOG_DEBUG, LOG_TAG, __VA_ARGS__)
#define  LOGE(...)  __android_log_print(ANDROID_LOG_ERROR, LOG_TAG, __VA_ARGS__)

#else 

#define  LOGD( ...)  printf( __VA_ARGS__  );


#endif

namespace Nx {

	/*
class    ComponentDeletionWatcher
{
public:
	ComponentDeletionWatcher (const NxWidget * const componentToWatch_) throw() : componentToWatch (componentToWatch_), componentUID (componentToWatch_->getComponentUID())
	{
		// not possible to check on an already-deleted object..
		//assert (componentToWatch_->isValidComponent());
	}

   ~ComponentDeletionWatcher() throw() {}

   bool hasBeenDeleted() const throw()
   {
	   return ! (componentToWatch->isValidComponent()
		   && componentToWatch->getComponentUID() == componentUID);
   }

   const NxWidget* getComponent() const throw()
   {
	   return hasBeenDeleted() ? 0 : componentToWatch;
   }

 

private:
    const NxWidget * const componentToWatch;
    const uint32 componentUID;

    ComponentDeletionWatcher (const ComponentDeletionWatcher&);
    const ComponentDeletionWatcher& operator= (const ComponentDeletionWatcher&);
}; 
*/

		 

	 



/*
template <class Type>
inline Type jlimit (const Type lowerLimit,
					const Type upperLimit,
					const Type valueToConstrain) throw()
{
	//assert (lowerLimit <= upperLimit); // if these are in the wrong order, results are unpredictable..

	return (valueToConstrain < lowerLimit) ? lowerLimit
		: ((valueToConstrain > upperLimit) ? upperLimit
		: valueToConstrain);
}
*/



	class DragImageComponent: public NxWidget {
	private:
		
		NxWidget* const source;
		NxDragAndDropContainer* const mMainNxPanel;
		NxWidget* mouseDragSource;
		NxDragAndDropTarget* currentlyOver;
		std::string dragDesc;
		int xOff, yOff;
		bool hasCheckedForExternalDrag, drawImage;

			bool isDown;
			bool isDragging;

 
		//Image* image;
			/*
		ComponentDeletionWatcher* sourceWatcher;
		ComponentDeletionWatcher* mouseDragSourceWatcher;
		ComponentDeletionWatcher* currentlyOverWatcher;
		*/


		DragImageComponent (const DragImageComponent&);
		const DragImageComponent& operator= (const DragImageComponent&);

	public:
		DragImageComponent ( const std::string& desc, NxWidget* const s, NxDragAndDropContainer * const o, NxGuiManager * m ) : 
			source( s ),
			mMainNxPanel( o ),
			currentlyOver( 0 ),
			/*currentlyOverWatcher( 0 ),*/
			dragDesc( desc ),
			hasCheckedForExternalDrag( false ),
			drawImage( true ),
			NxWidget( 0.0f, 0.0f, 1.0f, 1.0f, m )
		{

			isDown = false;
			isDragging = false;

			// copy material information from source widget
			if( s->hasTextureCoordinates() ) {
				mBaseRectangle->SetTexture( s->getBackgroundTextureName() );
				Nx::Vector2 topLeft, bottomLeft, topRight, bottomRight;
				s->getTextureCoordinates( topLeft, bottomLeft, topRight, bottomRight ); 
				mBaseRectangle->setTextureCoordinates( topLeft, bottomLeft, topRight, bottomRight ); 
			} else {
				mBaseRectangle->SetColour( s->getColour( panelColourId )  );
			}

			
			mBaseRectangle->SetZOrder( 150 );

			SetPosition( source->GetX(), source->GetY() );
			SetSize( source->GetWidth(), source->GetHeight() );

			//sourceWatcher = new ComponentDeletionWatcher (source);
			mouseDragSource = source;
			//mouseDragSourceWatcher = new ComponentDeletionWatcher (mouseDragSource); 
			int mx, my = 0;
			m->getMouseLastCoordinates (mx, my);

			xOff = mx - source->getDerivedPositionX(); 
			yOff = my - source->getDerivedPositionY(); 

			setInterceptsMouseClicks (false, false);
 
		}

		~DragImageComponent()
		{

				LOGD("DragImageComponent: delete " );
			
			if (mMainNxPanel->dragImageComponent == this)
				mMainNxPanel->dragImageComponent = 0;


			/* neeeded ???
			if (! mouseDragSourceWatcher->hasBeenDeleted())
			{
				//mouseDragSource->RemoveWidgetListener (this);

				if (currentlyOverWatcher != 0 && ! currentlyOverWatcher->hasBeenDeleted())
					if (currentlyOver->isInterestedInDragSource (dragDesc, source))
						currentlyOver->itemDragExit (dragDesc, source);
			}*/

			LOGD("DragImageComponent: pass1 \r\n " );

			delete mBaseRectangle;

			/*
			delete mouseDragSourceWatcher;
			delete sourceWatcher;
			//delete image;
			delete currentlyOverWatcher;*/

			LOGD("DragImageComponent: pass2 \r\n " );

		}


		void RePaint(){
		
		
		}

		void mouseMoved( int x, int y ) {
			if( isDown ) isDragging = true;  
			updateLocation (true, x, y);
		}

		void mouseReleased(int x, int y, int button)
		{
			isDown = false;
			isDragging = false;
 
			 //if ( widget != this)
			 //{
				// if (! mouseDragSourceWatcher->hasBeenDeleted())
					// mouseDragSource->RemoveWidgetListener (this);

				 bool dropAccepted = false;
				 NxDragAndDropTarget* ddt = 0;
				 int relX = 0, relY = 0;

				 if (IsVisible())
				 {
					 SetVisible (false);
					 ddt = findTarget (x, y, relX, relY);
 
					 // fade this component and remove it - it'll be deleted later by the timer callback

					 dropAccepted = ddt != 0;

					 SetVisible (true);

					 if (dropAccepted /*|| sourceWatcher->hasBeenDeleted()*/ ) {
						// fadeOutComponent (120);

						 LOGD("dropAccepted ... " );
					 }
					 else {
						 int targetX = source->GetWidth() / 2;
						 int targetY = source->GetHeight() / 2;
						 source->localPositionToDerived(targetX, targetY);

						 int ourCentreX = GetWidth() / 2;
						 int ourCentreY = GetHeight() / 2;
						 localPositionToDerived (ourCentreX, ourCentreY);

						/* fadeOutComponent (120,
							 targetX - ourCentreX,
							 targetY - ourCentreY);*/
					 }
				 }

				// if (getParentComponent() != 0)
				//	 getParentComponent()->removeChildComponent (this);

				 if( dropAccepted && ddt != 0 ) {
					 // (note: use a local copy of the dragDesc member in case the callback runs a modal loop and deletes this object before the method completes)
					 const std::string dragDescLocal (dragDesc);

					 ddt->itemDropped (dragDescLocal, source, relX, relY);
				 }

				 // careful - this object could now be deleted..
			 //}
		}
 
		NxDragAndDropTarget* findTarget (const int screenX, const int screenY, int& relX, int& relY)
		{

			NxWidget* hit = getParentComponent(); // get main container panel of this overcomp

			int rx = screenX, ry = screenY;
			hit->derivedPositionToLocal( rx, ry);
			hit = hit->getComponentAt (rx, ry);

			const std::string dragDescLocal (dragDesc);
			while (hit != 0) {
				NxDragAndDropTarget* const ddt = dynamic_cast <NxDragAndDropTarget*> (hit);
				if (ddt != 0 && ddt->isInterestedInDragSource (dragDescLocal, source)) {
					relX = screenX;
					relY = screenY;
					hit->derivedPositionToLocal( relX, relY);
					return ddt;
				}
				hit = hit->getParentComponent();
			}
			return 0;	
		}


		/* x and y are global pos */
		void updateLocation (const bool canDoExternalDrag, int x, int y)
		{
			const std::string dragDescLocal (dragDesc);

			int newX = x - xOff;
			int newY = y - yOff;

			if (getParentComponent() != 0)
				getParentComponent()->derivedPositionToLocal(newX, newY);

			if (newX != GetX() || newY != GetY()) {
 
				SetPosition ( newX, newY ); 

				int relX = 0, relY = 0;
				NxDragAndDropTarget* const ddt = findTarget ( x, y, relX, relY );
 
				drawImage = (ddt == 0) || ddt->shouldDrawDragImageWhenOver();

				if (ddt != currentlyOver) {
					 //if (currentlyOverWatcher != 0 && ! currentlyOverWatcher->hasBeenDeleted()) {
						NxWidget* const over = dynamic_cast <NxWidget*> (currentlyOver);
						if (over != 0 && over->isValidComponent()  && /*!(sourceWatcher->hasBeenDeleted())  && */currentlyOver->isInterestedInDragSource (dragDescLocal, source)) {
							currentlyOver->itemDragExit (dragDescLocal, source);
						}
					//}

					currentlyOver = ddt;
					/*if (currentlyOverWatcher) delete currentlyOverWatcher;
					currentlyOverWatcher = 0;*/

					if (ddt != 0) {
						//currentlyOverWatcher = new ComponentDeletionWatcher (dynamic_cast <NxWidget*> (ddt));
						if (currentlyOver->isInterestedInDragSource (dragDescLocal, source)) {
							currentlyOver->itemDragEnter (dragDescLocal, source, relX, relY);
						}
					}
				}
				/*else if (currentlyOverWatcher != 0 && currentlyOverWatcher->hasBeenDeleted()) {
					currentlyOver = 0;
					if (currentlyOverWatcher) delete currentlyOverWatcher;
					currentlyOverWatcher = 0;
				}*/

				if (currentlyOver != 0 && currentlyOver->isInterestedInDragSource (dragDescLocal, source))
					currentlyOver->itemDragMove (dragDescLocal, source, relX, relY);

		 

			 
			}
		}

		/*
		void timerCallback()
		{
			if (sourceWatcher->hasBeenDeleted())
			{
				delete this;
			}
			else if (! isMouseButtonDownAnywhere())
			{
				if (! mouseDragSourceWatcher->hasBeenDeleted())
					mouseDragSource->removeMouseListener (this);

				delete this;
			}
		}*/


	};
 

	//================

	NxDragAndDropContainer::NxDragAndDropContainer( NxGuiManager * manager ) : mManager( manager ), dragImageComponent (0) {

	}

	NxDragAndDropContainer::~NxDragAndDropContainer() {

		if( dragImageComponent != 0 ) {
			delete dragImageComponent;
		}
			
	}

	void NxDragAndDropContainer::startDragging( const std::string& sourceDescription, NxWidget* sourceComponent, const bool allowDraggingToExternalWindows )
	{
		if (dragImageComponent != 0) {
			//if (im != 0)
				//delete im;
		} 
		else {
			NxWidget * const thisComp = dynamic_cast <NxWidget*>( this ); // source NxPanel main container
			if (thisComp != 0) {
				int mx, my = 0;
				mManager->getMouseLastCoordinates( mx, my ) ;

				DragImageComponent* const dic = new DragImageComponent ( sourceDescription, sourceComponent, this, mManager);

				dragImageComponent = dic;
				currentDragDesc = sourceDescription;
				thisComp->AddComponent( dic );
				dic->updateLocation( false, mx, my );
				dic->SetVisible( true );
			}
			else {
				// this class must only be implemented by an object that is also a NxWidget.
			}
		}
	}


	void NxDragAndDropContainer::stopDragging() {

		if( dragImageComponent ) {
			int mx, my = 0;
			mManager->getMouseLastCoordinates( mx, my ) ;
			dragImageComponent->mouseReleased(mx, my ,0);
			dragImageComponent->getParentComponent()->RemoveComponent( dragImageComponent );
			delete dragImageComponent;
			dragImageComponent = 0;
		}
	}

	bool NxDragAndDropContainer::isDragAndDropActive() const
	{
		return dragImageComponent != 0;
	}

	const std::string NxDragAndDropContainer::getCurrentDragDescription() const
	{
		return (dragImageComponent != 0) ? currentDragDesc : "";
	}

	NxDragAndDropContainer * NxDragAndDropContainer::findParentDragContainerFor( NxWidget* c )
	{
		if (c == 0)
			return 0;

		// (unable to use the syntax findParentComponentOfClass <DragAndDropContainer> () because of a VC6 compiler bug)
		return c->findParentComponentOfClass ((NxDragAndDropContainer*) 0);
	}

	bool NxDragAndDropContainer::shouldDropFilesWhenDraggedExternally( const std::string&, NxWidget*, vector<std::string>&, bool& )
	{
		return false;
	}


	//==============================================================================
	void NxDragAndDropTarget::itemDragEnter (const std::string&, NxWidget*, int, int)
	{
	}

	void NxDragAndDropTarget::itemDragMove (const std::string&, NxWidget*, int, int)
	{
	}

	void NxDragAndDropTarget::itemDragExit (const std::string&, NxWidget*)
	{
	}

	bool NxDragAndDropTarget::shouldDrawDragImageWhenOver()
	{
		return true;
	}



}