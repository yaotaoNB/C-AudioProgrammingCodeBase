CSD=melodyGen.csd
OUT=melodygen
#SRC=./src/
#g++ -std=c++14 -o $(OUT) ./src/*.cpp -I./include
CPP14=-std=c++14
MAIN_O=./src/main.o
WRITECSD_O=./src/writeCsd.o
CHORDGEN_O=./src/chordGen.o
MAIN_CPP=./src/main.cpp
WRITECSD_CPP=./src/writeCsd.cpp
CHORDGEN_CPP=./src/chordGen.cpp
WRITECSD_H=./include/writeCsd.h
CHORDGEN_H=./include/chordGen.h
INCLUDE=-I./include

chordgen: $(MAIN_O) $(WRITECSD_O) $(CHORDGEN_O)
	g++ $(CPP14) -o $(OUT) ./src/*.o

$(MAIN_O): $(MAIN_CPP)
	g++ $(CPP14) -o $(MAIN_O) -c $(MAIN_CPP) $(INCLUDE)

$(WRITECSD_O): $(WRITECSD_CPP) $(WRITECSD_H)
	g++ $(CPP14) -o $(WRITECSD_O) -c $(WRITECSD_CPP) $(INCLUDE)

$(CHORDGEN_O): $(CHORDGEN_CPP) $(CHORDGEN_H)
	g++ $(CPP14) -o $(CHORDGEN_O) -c $(CHORDGEN_CPP) $(INCLUDE)

runcsd:
	csound -odac $(CSD)

clean:
	rm $(OUT) ./src/*.o ./csd/*.csd