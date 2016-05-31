#Modify Makefile for your instalation of SDL


#what compiler we are using
CC=g++ 
#the path to SDL config
SDLPATH=/usr/bin
#SDLPATH=/usr/local/bin

all: hppd


doubleup.o: doubleup.cpp
	$(CC) doubleup.cpp -c `${SDLPATH}/sdl-config --cflags`

hppd: doubleup.o
	$(CC) doubleup.o -o hppd `${SDLPATH}/sdl-config --static-libs`
	
	
clean:
	rm -f *.o *~

