Scrum Log - Sprint 2
====================

**2015-2-25:**
* Terry:
  * Implemented concurrency of engine thought. 
  * Implemented cancelation of engine thought.
  * Improved debug logging. 
* Conor:
  * Started work on move ordering and make and unmake move.
* Darragh:
  * Worked on evaluation. 

**2015-2-26:**
* Terry:
  * Continued work on concurency.  
* Conor:
  * Refactored move generation and introduced new MoveList class.
* Darragh:
  * Added positional considerations to new Evaluation class. 

**2015-2-28:**
* Terry:
  * Added basic clock
  * Refactored build process
* Conor:
  * Continued work on new MoveList class and make and unmake.

  **2015-3-1:**
* Terry:
  * Added ability to flip board
  * Fixed crucial bug with clock
* Conor:
  * Finished make and unmake.
  * Implemented basic move ordering

  **2015-3-2:**
* Terry:
  * Move advanced clock features. 
* Conor:
  * Debuged make and unmake

  **2015-3-3:**
* Terry:
  * Added basic engine output
* Conor:
  * Researched Transposition tables.
* Darragh:
  * Implemented threads.

  **2015-3-4:**
* Terry:
  * Additional work on engine output
* Conor:
  * Researched zobrist hashing
* Darragh:
  * Converted unit tests to work with make and unmake.

  **2015-3-5:**
* Terry:
  * Finihed engine output
* Conor:
  * Started work on zobrist hashing
* Darragh:
  * Additional unit tests
 
  **2015-3-6:**
* Conor:
  * Finished zobrist hashing
  * Started work on transposition tables
* Darragh:
  * Wrote preft function
 
  **2015-3-7:**
* Terry:
  * Implemented piece promotion
  * Fixed bug with queenside castling
* Conor:
  * Changed replacement policy of transposition tables after testing
* Darragh:
  * Wrote helper functions in movelist for the perft function

  **2015-3-8:**
* Terry:
  * Finished engine outputing format
* Conor:
  * Testing of transpostion tables
* Darragh:
  * Implemented additional UCI commands

  **2015-3-9:**
* Conor:
  * Attempted returning score from TT table.
* Darragh:
  * Output principal variation
 
  **2015-3-10:**
* Terry:
  * Investigation of click and drag movement. 
* Darragh:
  * Cleaned up search class.

  **2015-3-11:**
* Terry:
  * Added ability to select a single square on the board
* Darragh:
  * Added more documentation
  * Define statments for code clarity
  * Output moves to mate correctly
  * Added nodes output

  **2015-3-12:**
* Terry:
  * Worked on click and click movement
* Conor:
  * Started work on new move represenation

  **2015-3-13:**
* Terry:
  * Finished click and click movement
* Conor:
  * Changed move representation from mov struct to u64 to save time and space
 
  **2015-3-14:**
* Conor:
  * Wrote new less expensive inCheck function
* Darragh:
  * Fixed unit tests
 
  **2015-3-15:**
* Conor:
  * Debugged new inCheck function. Now passes all unit tests.
* Darragh:
  * Modified unit tests to work with new move format
 
  **2015-3-16:**
* Terry:
  * Worked on material difference display.
* Conor:
  * Added new CaptureList class for qsearch
* Darragh:
  * Worked on qsearch
