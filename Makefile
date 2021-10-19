
CXX = g++-10

sources = source.cpp lexing.cpp parsing.cpp ast.cpp typing.cpp runtime.cpp runtime_variables.cpp \
          runtime_jitting.cpp runtime_modules.cpp basically.cpp

objects = $(sources:.cpp=.o)

headers = $(sources:.cpp=.h)

executable = basically

pch_src = basically.h
pch = $(pch_src).gch

libs = -lgccjit

CXXFLAGS = -std=c++20 -fdiagnostics-color=always -g -O0



.PHONY: all clean


all: $(executable)

clean:
	rm -f $(objects) $(pch) $(executable)


$(executable): $(pch) $(objects)
	$(CXX) $(CXXFLAGS) $(objects) $(libs) -o $(executable)

$(pch): $(headers)
	$(CXX) $(CXXFLAGS) -x c++-header -c $(pch_src) -o $(pch)

source.o: source.cpp $(pch)
	$(CXX) $(CXXFLAGS) -c $(*).cpp -o $(*).o

lexing.o: lexing.cpp $(pch)
	$(CXX) $(CXXFLAGS) -c $(*).cpp -o $(*).o

parsing.o: parsing.cpp $(pch)
	$(CXX) $(CXXFLAGS) -c $(*).cpp -o $(*).o

ast.o: ast.cpp $(pch)
	$(CXX) $(CXXFLAGS) -c $(*).cpp -o $(*).o

typing.o: typing.cpp $(pch)
	$(CXX) $(CXXFLAGS) -c $(*).cpp -o $(*).o

runtime.o: runtime.cpp $(pch)
	$(CXX) $(CXXFLAGS) -c $(*).cpp -o $(*).o

runtime_variables.o: runtime_variables.cpp $(pch)
	$(CXX) $(CXXFLAGS) -c $(*).cpp -o $(*).o

runtime_jitting.o: runtime_jitting.cpp $(pch)
	$(CXX) $(CXXFLAGS) -c $(*).cpp -o $(*).o

runtime_modules.o: runtime_modules.cpp $(pch)
	$(CXX) $(CXXFLAGS) -c $(*).cpp -o $(*).o

basically.o: basically.cpp $(pch)
	$(CXX) $(CXXFLAGS) -c $(*).cpp -o $(*).o
