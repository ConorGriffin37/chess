Chess GUI
=========

GUI for the chess project.

## Dependencies ##

This engine requires the latest versions of the following software:
* The Mono Runtme
* GTK+
* NUnit
* Doxygen

## How to Install ##

From the directory in which this file is located, run the following commands:
```bash
$ ./autogen.sh && make
```

Then, to run the main program, run the following:
```bash
$ ./GUI/bin/Debug/gui.exe
```

To run the unit tests:
```bash
$ nunit-console ./Test/bin/Debug/test.dll
```

And, finally, to compile documentation:
```bash
$ doxygen Doxyfile
```
