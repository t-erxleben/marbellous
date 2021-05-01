## Functions

* select button to open radial submenue, do it again for the next submenu
* only on menu per level is open at the same time
* you can have either painting or modeling tools <- (New image -> painting state -> modeling state -> finish)
* movement direction and intensity it done with mouse input in normal mode

* every state
	* color picker
		* different colors (<- select to change?)
	* redo
		* new canvas
		* redo step
		* back to end of painting state
* painting state:
	* color dropper
	* color rakes
	* go to modeling state
* modeling state
	* rake
		* different rakes
			* strait movement
			* oscillating movement
		* custom rake (expert)
	* export


## Tech Stack

* HTML5 + CSS3 + JS should be enough to implement the needed functions, and it should go fast.
	* because we don't need a lot of windows and views we should be fine
	* if we decide to want more we can swap to react but then the website will blow up
