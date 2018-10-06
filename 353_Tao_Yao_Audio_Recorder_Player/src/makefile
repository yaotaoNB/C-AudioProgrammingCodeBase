CC=g++ -std=c++14 -g

PA_INCLUDE=-I../api/portaudio/include
PA=-L../api/portaudio/lib -lportaudio
SNDFILE_INCLUDE=-I../api/libsndfile/usr/local/include
SNDFILE=-L../api/libsndfile/usr/local/lib -lsndfile

IDIR=../include
CFLAGS=-I$(IDIR)
_DEPS = audio_player.hpp
DEPS = $(patsubst %,$(IDIR)/%,$(_DEPS))
ODIR=./obj
_ODIR=./obj/%.o
_OBJ = audio_player.o main.o
OBJ = $(patsubst %,$(ODIR)/%,$(_OBJ))

#preprocessor=>translation_units=>compiler=>.obj(we don't need the library for now, only the header of library)
$(ODIR)/%.o: %.cpp $(DEPS)
	$(CC) -o $@ -c $< $(CFLAGS) $(PA_INCLUDE) $(SNDFILE_INCLUDE)

#still need to include portaudio header and static lib during linking *.o files, the linker here will link .obj with our library
audio_recorder_player: $(OBJ)
	$(CC) -o $@ $^ $(CFLAGS) $(PA) $(SNDFILE)

clean:
	rm -f ./audio_recorder_player ./obj/*.o ../audio_files/*.wav *~ core

#To compile the program after install the 2 API, here's a reference compile command (change the library and include path to the path in your own computer), when comple it, cd shell path to ./src folder:
#g++ -std=c++14 -g -I../include -I../api/portaudio/include -I../api/libsndfile/usr/local/include -L../api/portaudio/lib -lportaudio -L../api/libsndfile/usr/local/lib -lsndfile *.cpp -o audio_recorder_player