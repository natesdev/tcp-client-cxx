CXX = g++
CXXFLAGS = -Wall -g -Wextra 
LIBS = -lz -lmongocxx -lbsoncxx
TARGET = showdownclient

SOURCES = $(wildcard *.cpp) \
          $(wildcard Classes/*.cpp) \
          $(wildcard Classes/*/*.cpp) \
          $(wildcard Packets/*.cpp) \
          $(wildcard Classes/*/*/*.cpp) \
          $(wildcard Packets/*/*/*.cpp) 


OBJECTS = $(SOURCES:.cpp=.o)

all: $(TARGET)
$(TARGET): $(OBJECTS)
	$(CXX) $(CXXFLAGS) $(OBJECTS) -o $(TARGET) $(LIBS)

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	rm -f $(TARGET) $(OBJECTS)

.PHONY: all clean 