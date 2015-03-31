all: ./Source/main.cpp

	g++ ./Source/*.cpp -o Saruman -pthread -std=c++11 -O3

	

test: ./UnitTests/tests.cpp

	g++ -o Saruman_unitTests ./UnitTests/tests.cpp ./Source/Board.cpp ./Source/Search.cpp ./Source/UCI.cpp ./Source/MoveList.cpp ./Source/Evaluation.cpp ./Source/CaptureList.cpp ./Source/TranspositionTables.cpp -O3  -std=c++11 -lboost_system -lboost_unit_test_framework

  

doc: Doxyfile

	doxygen
