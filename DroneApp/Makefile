CC = g++

LDFLAGS = -lwiringPi -ljansson

all : QuadCopterX QuadCopterP HexaCopterX OctoCopterX

QuadCopterX : ;$(CC) -o QuadCopterX QuadX.cpp $(LDFLAGS)

QuadCopterP : ;$(CC) -o QuadCopterP QuadP.cpp $(LDFLAGS)

HexaCopterX : ;$(CC) -o HexaCopterX HexaX.cpp $(LDFLAGS)

OctoCopterX : ;$(CC) -o OctoCopterX OctoX.cpp $(LDFLAGS)

clean :; rm -rf QuadCopterX QuadCopterP HexaCopterX OctoCopterX
