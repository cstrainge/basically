
CXX = g++-10
objects = source.o token.o ast.o parse.o basicly.o
pch_src = basicly.h
pch = $(pch_src).gch
libs = -lgccjit
executable = basicly

cxx_flags = -g -std=c++20 -fdiagnostics-color=always



.PHONY: all clean



all: basicly



clean:
	rm -f $(objects) $(pch) $(executable)



$(executable): $(pch) $(objects)
	$(CXX) $(cxx_flags) $(objects) $(libs) -o $(executable)



$(pch): std_inc.h basicly.h source.h token.h ast.h parse.h
	$(CXX) $(cxx_flags) -x c++-header -o $(pch) -c $(pch_src)



source.o: source.cpp $(pch)
	$(CXX) $(cxx_flags) -c source.cpp

token.o: token.cpp $(pch)
	$(CXX) $(cxx_flags) -c token.cpp

ast.o: ast.cpp $(pch)
	$(CXX) $(cxx_flags) -c ast.cpp

parse.o: parse.cpp $(pch)
	$(CXX) $(cxx_flags) -c parse.cpp

basicly.o: basicly.cpp $(pch)
	$(CXX) $(cxx_flags) -c basicly.cpp
