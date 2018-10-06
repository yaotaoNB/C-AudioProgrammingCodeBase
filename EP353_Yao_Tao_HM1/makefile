writecsd: main.o writeCsd.o
	g++ -std=c++14 main.o writeCsd.o -o writecsd -I.

main.o: main.cpp
	g++ -std=c++14 -c main.cpp -I.

writeCsd.o: writeCsd.cpp
	g++ -std=c++14 -c writeCsd.cpp -I.

clean:
	rm *.o writecsd *.csd