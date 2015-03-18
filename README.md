Chess
=====

A chess engine and chess GUI in C++ and C#, respectively.

## Third-Party Software ##

For testing the engine and the GUI, we recommend the following third-party programs:
* **GUIs:**
	* Arena (Windows)
	* Xboard (Linux & OSX)
* **Engines:**
	* Stockfish 6 (Windows, Linux, OSX): https://stockfishchess.org/
	* Komodo 5.1 (Windows, Linux, OSX): http://komodochess.com/pub/Komodo_5.1r2.zip
	* Critter 1.6a (Windows, Linux, OSX): http://www.vlasak.biz/critter/
	
## Items for Product Backlog ##

* **Engine:**
	* Support for threefold repetition and 50 move rule.
	* More considerations for calculation in UCI: time, movestogo, nodes to search ect.
	* Pondering
	* Killer move heuristic
	* Null move heuristic
	* Parallel Search (?)
	* Magic Bitboards
	* Endgame tablebases
	* Opening books
	* Code profiling/optimisation (Is there anything which could be more efficient, move generation ect?)
	* Analysis of branching factor, nodes searched ect. Can pruning be improved?
	* Move probabilities (Not playing the same game all the time)

* **GUI:**

	* Loading multiple engines
	* Play engines against each other
	* Chess 960
	* Ability to set engine strength
	* Make engine play move in response to a user move (i.e. “game” mode)
	* Toolbar below menu bar for quick access to common functions (new game, flip board, etc.)
	* Analysis mode, get an engine to analyse a position
	* Variation boards
