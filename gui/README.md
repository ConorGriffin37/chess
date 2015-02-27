Chess GUI
=========

GUI for the chess project. Currently runs on Linux, Windows, and OSX (unstable).

## Dependencies ##

This engine requires the latest versions of the following software:
* The Mono Runtme
* GTK+
* MonoDevelop/Visual Studio
* Doxygen

## How to Install ##

To install, open up GUI.sln in MonoDevelop or Visual Studio and build within the IDE. If you 
wish to run tests, you must build with the "Debug" option selected instead of "Release".

Then, to run the main program, run the following:
```bash
$ cd <chess_dir_location>/gui/GUI/bin/Debug/
$ ./GUI.exe
```

To run the unit tests:
```bash
$ cd <chess_dir_location>/gui/Test/bin/Debug/
$ nunit-console ./Test.dll
```

And, finally, to compile documentation:
```bash
$ cd <chess_dir_location>/gui/
$ doxygen Doxyfile
```
