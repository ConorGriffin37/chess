Scrum Log - Sprint 1
====================

**2015-2-3:**
* Terry:
	* Set up git repo.
	* Began working on GUI board representation.
	* Added Piece and Square classes.
	* *Next:* Create a Board class to tie the Pieces and Squares together.

**2015-2-4:**
* Terry:
	* Added a Board class.
	* *Next:* Begin working on FEN parsing for the GUI.

**2015-2-5:**
* Terry:
	* Added basic FEN parser.
	* *Next:* Begin working on move validation

**2015-2-8:**
* Terry:
	* Implemented backend for move validation.
	* *Next:* Begin working on UCI.
* Conor:
	* Started working on bitboard representation.
	* Started working on FEN parser.
* Darragh:
	* Started working on UCI protocol.

**2015-2-10:**
* Terry:
	* Wrote a wrapper for C#'s builtin Process class for interfacing with the engine.
* Conor:
	* Finished basic FEN parser.
	* Finished board representation.
	* Began move generation.
* Darragh:
	* Implemented basic UCI.
	* Implemented basic search.

**2015-2-11:**
* Terry:
	* Implemented UCI commands necessary for this sprint.
	* Fixed errors with FEN parsing.
	* Added ability for Board to output a FEN string.
	* *Next:* Begin working on graphical elements.
* Conor:
	* Continued working on move generation.
* Darragh:
	* Fixed bugs in the search algorithm.
	* Wrote unit tests for existing functionality.

**2015-2-12:**
* Terry:
	* Began working on implementing graphical elements.
* Conor:
	* Finished move generation.
	* *Next:* Begin working on dynamic move processing.
* Darragh:
	* Created engine main file.
	* Updated UCI.

**2015-2-13:**
* Terry:
	* Implemented graphics for:
		* Loading an engine.
		* Importing a FEN string.
		* Displaying a board background (no pieces).
* Conor:
	* Wrote dynamic move processing.
* Darragh:
	* Fixed bugs in the search algorithm.
**Note:** at this point we have a working initial version of the engine, 
which is capable of interfacing with a GUI and playing a game.

**2015-2-14:**
* Terry:
	* Began working on drawing pieces to the screen.
* Conor:
	* Debugged the board and evaluation functions.
* Darragh:
	* Fine-tuned the search algorithm.

**2015-2-15:**
* Terry:
	* Added piece drawing.
	* *Next:* Enable user to input moves.
* Conor & Darragh:
	* Fixed error in the rootAlphaBeta function.
	* Other minor bugfixes.

**2015-2-16:**
* Terry:
	* Added ability for user to input moves.
	* Added more functions for checking the legality of moves.
	* Added checking for checkmate and stalemate, as well as GUI adjudications.
* Conor:
	* Minor bug fixes.
	* Improved code formatting.
* Darragh:
	* Wrote unit tests.

**2015-2-17:**
* Terry:
	* Added button to reset board to starting position.
	* Added button to instruct the engine to make a move.
	* Wrote documentation.
	* Wrote unit tests.
* Conor & Darragh:
	* Wrote makefiles.
	* Wrote documentation.
	* Enabled Doxygen support.
