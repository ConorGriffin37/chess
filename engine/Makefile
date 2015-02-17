all: ./Source/main.cpp
	g++ ./Source/*.cpp -std=c++11 -o Saruman
	
test: ./UnitTests/tests.cpp
	g++ -o unitTests ./UnitTests/tests.cpp ./Source/Board.cpp ./Source/Search.cpp ./Source/UCI.cpp  -std=c++11 -lboost_system -lboost_unit_test_framework
  
doc: Doxyfile
	doxygen