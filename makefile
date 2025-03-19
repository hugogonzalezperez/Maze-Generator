CXX = g++
CXXFLAGS = -g -c -Iinclude/ -std=c++20 $(shell sdl2-config --cflags)
LDFLAGS = -g
LDLIBS = $(shell sdl2-config --libs)

SRC = src/main.cc 
OBJS = $(SRC:.cc=.o)
EXECUTABLE = bin/maze

.PHONY: all clean

all: $(EXECUTABLE)

$(EXECUTABLE): $(OBJS)
	mkdir -p bin
	$(CXX) $(LDFLAGS) $^ -o $@ $(LDLIBS)
	rm -f $(OBJS)
	@echo ✅ All done!

%.o: %.cc
	$(CXX) $(CXXFLAGS) -o $@ $<

clean:
	rm -f $(EXECUTABLE)
	rm -rf bin
	rm -f $(OBJS)
	@echo 🧹 Clean done!