CXX = g++

SOURCES = main.cpp framebuffer.cpp figuras.cpp utilidades.cpp
OBJECTS = $(SOURCES:.cpp=.o)
EXECUTABLE = to04

all: $(EXECUTABLE)

$(EXECUTABLE): $(OBJECTS)
	$(CXX) $(OBJECTS) -o $(EXECUTABLE) $(LIBS)

%.o: %.cpp
	$(CXX) $< -o $@

clean:
	rm -f $(OBJECTS) $(EXECUTABLE)

run: $(EXECUTABLE)
	./$(EXECUTABLE)

.PHONY: all clean run help
