Chess GUI
=========

GUI for the chess project. Currently runs on Linux, Windows, and OSX (unstable).

## Dependencies ##

This engine requires the latest versions of the following software:
* The Mono Runtme
* GTK+
* NUnit
* Doxygen

## How to Install ##

If you are using Linux you can follow the commands below. Otherwise, you can open GUI.sln in 
VisualStudio or MonoDevelop and compile within the IDE.

From the directory in which this file is located, run the following commands:
```bash
$ ./autogen.sh && make
```

Then, to run the main program, run the following:
```bash
$ cd ./GUI/bin/Debug/
$ ./GUI.exe
```

To run the unit tests:
```bash
$ cd ./Test/bin/Debug/
$ nunit-console ./test.dll
```

And, finally, to compile documentation:
```bash
$ doxygen Doxyfile
```

## Troubleshooting ##

If you get an error while running `autogen.sh` stating that NUnit cannot be found then you 
can 
(assuming you have NUnit installed) run the following command to get pkg-config to recognise 
NUnit:
```bash
$ sudo ln -s /usr/lib/pkgconfig/mono-nunit.pc /usr/lib/pkgconfig/nunit.pc
```
