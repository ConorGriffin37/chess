Scrum Log - Sprint 3
====================

**2015-03-25:**
* Terry:
  * Added ability for users to set engine depth
* Conor:
  * Started work on branching factor analysis
* Darragh:
  * Added support for searchmoves, nodes and movetime commands in UCI.

**2015-03-26:**
* Terry:
  * Added support for multiple game modes : 
   * Singleplayer
   * Multiplayer
   * Engine vs Engine (not yet supported)
  * GUI will now tell engine to move automatically in single player mode (if an engine is loaded)
* Conor:
  * Added outputting of effective branching factor
* Darragh:
  * Started on implementation of threefold repetition
 
**2015-03-30:**
* Terry:
  * Added analyse mode.
  * Fixed bug with cancelling the engine process.
* Conor:
  * Added killer move heuristic
* Darragh:
  * Implemented 50 move rule

**2015-03-31:**
* Terry:
  * Added ability to load 2 engines and play them against each other
  * Fixed several bugs
* Conor:
  * Added null move heuristic
  * Fixed bugs with threefold repetition
* Darragh:
  * Implemented threefold repetition
  * Ran code profiling tools and generated visualistation for discussion
  * Updated unit tests to work with changes made to the engine
