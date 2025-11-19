CXX = g++

SOURCES = utilidades.cpp framebuffer.cpp iguras.cpp main.cpp
OBJECTS = $(SOURCES:.cpp=.o)
EXECUTABLE = to04_polygons

all: $(EXECUTABLE)

$(EXECUTABLE): $(OBJECTS)
	$(CXX) $(OBJECTS) -o $(EXECUTABLE) $(LIBS)

%.o: %.cpp
	$(CXX) $< -o $@

clean:
	rm -f $(OBJECTS) $(EXECUTABLE)

run: $(EXECUTABLE)
	sudo ./$(EXECUTABLE)

.PHONY: all clean run help
