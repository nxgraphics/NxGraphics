#ifndef __NXGUIDRAGDROPTARGET_H__
#define __NXGUIDRAGDROPTARGET_H__

#include "NxGuiWidget.h"

namespace Nx {

	class NxGraphics_Export NxDragAndDropTarget
	{
public:
    /** Destructor. */
    virtual ~NxDragAndDropTarget()  {}

    /** Callback to check whether this target is interested in the type of object being
        dragged.

        @param sourceDescription    the description string passed into DragAndDropContainer::startDragging()
        @param sourceComponent      the component that was passed into DragAndDropContainer::startDragging()
        @returns                    true if this component wants to receive the other callbacks regarging this
                                    type of object; if it returns false, no other callbacks will be made.
    */
    virtual bool isInterestedInDragSource( const std::string& sourceDescription, NxWidget* sourceComponent ) = 0;

    /** Callback to indicate that something is being dragged over this component.

        This gets called when the user moves the mouse into this component while dragging
        something.

        Use this callback as a trigger to make your component repaint itself to give the
        user feedback about whether the item can be dropped here or not.

        @param sourceDescription    the description string passed into DragAndDropContainer::startDragging()
        @param sourceComponent      the component that was passed into DragAndDropContainer::startDragging()
        @param x                    the mouse x position, relative to this component
        @param y                    the mouse y position, relative to this component
        @see itemDragExit
    */
    virtual void itemDragEnter( const std::string& sourceDescription, NxWidget* sourceComponent, int x, int y );

    /** Callback to indicate that the user is dragging something over this component.

        This gets called when the user moves the mouse over this component while dragging
        something. Normally overriding itemDragEnter() and itemDragExit() are enough, but
        this lets you know what happens in-between.

        @param sourceDescription    the description string passed into DragAndDropContainer::startDragging()
        @param sourceComponent      the component that was passed into DragAndDropContainer::startDragging()
        @param x                    the mouse x position, relative to this component
        @param y                    the mouse y position, relative to this component
    */
    virtual void itemDragMove ( const std::string& sourceDescription, NxWidget* sourceComponent, int x, int y );

    /** Callback to indicate that something has been dragged off the edge of this component.

        This gets called when the user moves the mouse out of this component while dragging
        something.

        If you've used itemDragEnter() to repaint your component and give feedback, use this
        as a signal to repaint it in its normal state.

        @param sourceDescription    the description string passed into DragAndDropContainer::startDragging()
        @param sourceComponent      the component that was passed into DragAndDropContainer::startDragging()
        @see itemDragEnter
    */
    virtual void itemDragExit( const std::string& sourceDescription, NxWidget * sourceComponent );

    /** Callback to indicate that the user has dropped something onto this component.

        When the user drops an item this get called, and you can use the description to
        work out whether your object wants to deal with it or not.

        Note that after this is called, the itemDragExit method may not be called, so you should
        clean up in here if there's anything you need to do when the drag finishes.

        @param sourceDescription    the description string passed into DragAndDropContainer::startDragging()
        @param sourceComponent      the component that was passed into DragAndDropContainer::startDragging()
        @param x                    the mouse x position, relative to this component
        @param y                    the mouse y position, relative to this component
    */
    virtual void itemDropped ( const std::string& sourceDescription, NxWidget* sourceComponent, int x, int y) = 0;

    /** Overriding this allows the target to tell the drag container whether to
        draw the drag image while the cursor is over it.

        By default it returns true, but if you return false, then the normal drag
        image will not be shown when the cursor is over this target.
    */
    virtual bool shouldDrawDragImageWhenOver();


 
	};

}

#endif