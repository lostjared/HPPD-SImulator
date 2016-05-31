
all: hppd


doubleup.o: doubleup.cpp
	$(CXX) doubleup.cpp -c `sdl-config --cflags`

hppd: doubleup.o
	$(CXX) doubleup.o -o hppd `sdl-config --static-libs`
	
clean:
	rm -f *.o *~

