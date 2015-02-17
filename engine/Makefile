all: main.cpp
	g++ ./Source/*.cpp -std=c++11 -o ./tictacnode_ai
	
test: tests.cpp
  g++ -o unitTests ./UnitTests/tests.cpp Board.cpp Search.cpp UCI.cpp -lboost_system -lboost_unit_test_framework
  
doc: Doxyfile
	doxygen
