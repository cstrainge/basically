
CXX = g++-10

sources = source.cpp token.cpp ast.cpp parse.cpp type.cpp basicly.cpp
objects = $(sources:.cpp=.o)
headers = $(sources:.cpp=.h) std_inc.h

executable = basicly

pch_src = basicly.h
pch = $(pch_src).gch

libs = -lgccjit

cxx_flags = -std=c++20 -fdiagnostics-color=always -g -O0



.PHONY: all clean release debug

all: $(executable)

clean:
	rm -f $(objects) $(pch) $(executable) $(executable)-d



$(executable): $(pch) $(objects)
	$(CXX) $(cxx_flags) $(objects) $(libs) -o $(executable)



$(pch): $(headers)
	$(CXX) $(cxx_flags) -x c++-header -o $(pch) -c $(pch_src)



source.o: source.cpp $(pch)
	$(CXX) $(cxx_flags) -c $(*).cpp -o $(*).o

token.o: token.cpp $(pch)
	$(CXX) $(cxx_flags) -c $(*).cpp -o $(*).o

ast.o: ast.cpp $(pch)
	$(CXX) $(cxx_flags) -c $(*).cpp -o $(*).o

parse.o: parse.cpp $(pch)
	$(CXX) $(cxx_flags) -c $(*).cpp -o $(*).o

type.o: type.cpp $(pch)
	$(CXX) $(cxx_flags) -c $(*).cpp -o $(*).o

basicly.o: basicly.cpp $(pch)
	$(CXX) $(cxx_flags) -c $(*).cpp -o $(*).o
