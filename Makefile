CXX = g++
CXXFLAGS = -Wall -Wextra
LIBS = -lm

SOURCES = main.cpp framebuffer.cpp figuras.cpp utilidades.cpp
OBJECTS = $(SOURCES:.cpp=.o)
EXECUTABLE = to04_polygons

all: $(EXECUTABLE)

$(EXECUTABLE): $(OBJECTS)
	$(CXX) $(OBJECTS) -o $(EXECUTABLE) $(LIBS)

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	rm -f $(OBJECTS) $(EXECUTABLE)

run: $(EXECUTABLE)
	sudo ./$(EXECUTABLE)

.PHONY: all clean run help
