#-I./include specify the directory for header files.
#-L./lib Add this directory to the list of directories to be searched for `-l', .a file all starts with lib + library_name + .a
#-l_libraryName is short cut of lib, it's the .a file (libportaudio.a)
CC=g++ -std=c++14
#include the portaudio library
PA=-I../portaudio/include -L../portaudio/lib -lportaudio
#inclde dir
IDIR=../include
#include our custom headers
CFLAGS=-I$(IDIR)
_DEPS = waveGen.h
DEPS = $(patsubst %,$(IDIR)/%,$(_DEPS))
ODIR=./obj
_ODIR=./obj/%.o
_OBJ = waveGen.o main.o
OBJ = $(patsubst %,$(ODIR)/%,$(_OBJ))

$(ODIR)/%.o: %.cpp $(DEPS)
	$(CC) -o $@ -c $< $(CFLAGS) $(PA)

#still need to include portaudio header and static lib during linking *.o files
wavegen: $(OBJ)
	$(CC) -o $@ $^ $(CFLAGS) $(PA)

clean:
	rm -f ./wavegen ./waveeg ./obj/*.o *~ core