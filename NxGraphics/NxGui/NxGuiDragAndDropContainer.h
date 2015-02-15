#ifndef __NXGUIDRAGDROPCONTAINER_H__
#define __NXGUIDRAGDROPCONTAINER_H__

#include "NxGuiDragAndDropTarget.h"

namespace Nx {
	class DragImageComponent;
	class NxGuiManager;
	class NxGraphics_Export NxDragAndDropContainer
	{

public:
		NxDragAndDropContainer( NxGuiManager * manager );

		/** Destructor. */
		virtual ~NxDragAndDropContainer();

    //==============================================================================
    /** Begins a drag-and-drop operation.

        This starts a drag-and-drop operation - call it when the user drags the
        mouse in your drag-source component, and this object will track mouse
        movements until the user lets go of the mouse button, and will send
        appropriate messages to DragAndDropTarget objects that the mouse moves
        over.

        findParentDragContainerFor() is a handy method to call to find the
        drag container to use for a component.

        @param sourceDescription    a string to use as the description of the thing being
                                    dragged - this will be passed to the objects that might be
                                    dropped-onto so they can decide if they want to handle it or
                                    not
        @param sourceComponent      the component that is being dragged
        @param dragImage            the image to drag around underneath the mouse. If this is
                                    zero, a snapshot of the sourceComponent will be used instead. An
                                    image passed-in will be deleted by this object when no longer
                                    needed.
        @param allowDraggingToOtherJuceWindows   if true, the dragged component will appear as a desktop
                                    window, and can be dragged to DragAndDropTargets that are the
                                    children of components other than this one.
    */
    void startDragging (const std::string& sourceDescription,
                        NxWidget* sourceComponent,
                        /*Image* dragImage = 0,*/
                        const bool allowDraggingToOtherJuceWindows = false);

	void stopDragging();

    /** Returns true if something is currently being dragged. */
    bool isDragAndDropActive() const;

    /** Returns the description of the thing that's currently being dragged.

        If nothing's being dragged, this will return an empty string, otherwise it's the
        string that was passed into startDragging().

        @see startDragging
    */
    const std::string getCurrentDragDescription() const;

    /** Utility to find the DragAndDropContainer for a given Component.

        This will search up this component's parent hierarchy looking for the first
        parent component which is a DragAndDropContainer.

        It's useful when a component wants to call startDragging but doesn't know
        the DragAndDropContainer it should to use.

        Obviously this may return 0 if it doesn't find a suitable component.
    */
    static NxDragAndDropContainer* findParentDragContainerFor (NxWidget* childComponent);

protected:
    /** Override this if you want to be able to perform an external drag a set of files
        when the user drags outside of this container component.

        This method will be called when a drag operation moves outside the Juce-based window,
        and if you want it to then perform a file drag-and-drop, add the filenames you want
        to the array passed in, and return true.

        @param dragSourceDescription        the description passed into the startDrag() call when the drag began
        @param dragSourceComponent          the component passed into the startDrag() call when the drag began
        @param files                        on return, the filenames you want to drag
        @param canMoveFiles                 on return, true if it's ok for the receiver to move the files; false if
                                            it must make a copy of them (see the performExternalDragDropOfFiles()
                                            method)
        @see performExternalDragDropOfFiles
    */
    virtual bool shouldDropFilesWhenDraggedExternally (const std::string& dragSourceDescription,
                                                       NxWidget* dragSourceComponent,
                                                       vector<std::string>& files,
                                                       bool& canMoveFiles);

	private:
		 friend class DragImageComponent;
		DragImageComponent* dragImageComponent;
		std::string currentDragDesc;

		 NxGuiManager * mManager ;


 
	};

}

#endif