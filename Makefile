CXX = g++
LIBS = -lm

SOURCES = main.cpp framebuffer.cpp figuras.cpp utilidades.cpp
OBJECTS = $(SOURCES:.cpp=.o)
EXECUTABLE = to04

all: $(EXECUTABLE)

$(EXECUTABLE): $(OBJECTS)
	$(CXX) $(OBJECTS) -o $(EXECUTABLE) $(LIBS)

%.o: %.cpp
	$(CXX) -c $< -o $@

clean:
	rm -f $(OBJECTS) $(EXECUTABLE)

run: $(EXECUTABLE)
	./$(EXECUTABLE)

play:
    $(MAKE) clean
    $(MAKE) run

.PHONY: all clean run help
