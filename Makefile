all: seam

CFLAGS=-fPIC -g -Wall `pkg-config --cflags opencv`
LIBS = `pkg-config --libs opencv`
INCLUDE = -I/usr/local/include/boost/ -I/home/nick/seamcarving/inc -I/home/nick/seamcarving/common

seam:  *.cpp src/*.cpp common/*.cpp
	$(CXX) $(INCLUDE) $(CFLAGS) $? -o $@  $(LIBS)

%.o: %.cpp
	$(CXX) -c $(CFLAGS) $< -o $@

clean:
	rm -rf *.o seam

