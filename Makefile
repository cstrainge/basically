
CXX = g++-10

sources = source.cpp token.cpp ast.cpp parse.cpp typing.cpp module.cpp module_loader.cpp basicly.cpp
objects = $(sources:.cpp=.o)
headers = $(sources:.cpp=.h)

executable = basicly

pch_src = basicly.h
pch = $(pch_src).gch

libs = -lgccjit

CXXFLAGS = -std=c++20 -fdiagnostics-color=always -g -O0



.PHONY: all clean release debug

all: $(executable)

clean:
	rm -f $(objects) $(pch) $(executable)


$(executable): $(pch) $(objects)
	$(CXX) $(CXXFLAGS) $(objects) $(libs) -o $(executable)


$(pch): $(headers)
	$(CXX) $(CXXFLAGS) -x c++-header -o $(pch) -c $(pch_src)


source.o: source.cpp $(pch)
	$(CXX) $(CXXFLAGS) -c $(*).cpp -o $(*).o

token.o: token.cpp $(pch)
	$(CXX) $(CXXFLAGS) -c $(*).cpp -o $(*).o

ast.o: ast.cpp $(pch)
	$(CXX) $(CXXFLAGS) -c $(*).cpp -o $(*).o

parse.o: parse.cpp $(pch)
	$(CXX) $(CXXFLAGS) -c $(*).cpp -o $(*).o

typing.o: typing.cpp $(pch)
	$(CXX) $(CXXFLAGS) -c $(*).cpp -o $(*).o

module.o: module.cpp $(pch)
	$(CXX) $(CXXFLAGS) -c $(*).cpp -o $(*).o

module_loader.o: module_loader.cpp $(pch)
	$(CXX) $(CXXFLAGS) -c $(*).cpp -o $(*).o

basicly.o: basicly.cpp $(pch)
	$(CXX) $(CXXFLAGS) -c $(*).cpp -o $(*).o
