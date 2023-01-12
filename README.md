To build and run simply run these command in the terminal at this directory. (not including tilde)

~ g++ ECGraphicViewImp.cpp ECController.h ECObserver.h ECGraphicViewImp.h test-gv.cpp -lallegro -lallegro_font -lallegro_primitives -lallegro_image -lallegro_main -std=c++14

~ ./a.out

TO RUN WITH SAVE FILE USE THE FOLLOWING BELOW
~ ./a.out test.txt


--- Features Implemented (Part I) ---
(i) Features that work
    - Can switch between edit and insert mode
    - Can create multiple rectangle on the canvas by clicking the mouse, dragging it, then letting go.
    - When clicking a shape, the shape becomes selected, turning blue.
    - Re-click on the shape to change it back to black, meaning unselected.
    - While in edit mode with a selected shape, when you drag your mouse, the shape follows your mouse and when you click again the shape places itself in the position.
    - While in edit mode, when you press d with a selected shape, it deletes that shape.
    - Undo and Redo works for all of the features mentioned above.

(ii) Features that don't work.
    - None


--- Features Implemented (Part II) ---
(i) Features that work
    - Ability to toggle between Rectangle/Ellipse using G button in insertion mode.
    - Ability to toggle between Fill Mode, allowing to create filled shapes.
    - Allow users to select multiple shapes by holding down the ctrl button while clicking on respective shapes.
    - Users can move selected shape(s) by using the arrow keys.
    - Allow the ability to group selected shapes into a composite shape by pressing g.
        - If there are more than 1 shape selected it groups them into a composite shape.
        - If only one shape is selected and it is a composite shape, it will ungroup that composite shape one level at a time.
        - If only one shape is selected and it is NOT a composite shape, then it will do nothing.
        - After pressing g, all selected shapes are deselected.
    - Implemented the ability to save/load canvases.
        - Can save/load all shapes including, Rectangle, Ellipse, Composite, both filled Rectangle and Ellipse.
        - file that are passed in through the command line will first be loaded before showing the canvas.
        - After closing the canvas, the information from the canvas will be saved onto the file that was initially passed through the command line.
        - If an empty file is passed through the command line, there is no loading done.

(ii) Known Bugs
    - Mouse does not move with shape when using arrows.




    

