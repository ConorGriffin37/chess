Unit Testing
=====

##Running the unit tests

Unit tests use the boost unit testing library, install boost before running the unit tests

Place the tests.cpp in the same directory as the source of the engine and compile it with the following command:
g++ -o unitTests tests.cpp Board.cpp Search.cpp UCI.cpp -lboost_system -lboost_unit_test_framework

And run it like so ./unitTests
