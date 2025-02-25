CXX = g++
CXXFLAGS = -Wall -g -Wextra
LIBS = -lz 
TARGET = tcpclient

SOURCES = $(wildcard *.cpp) \
          $(wildcard Classes/*.cpp) \
          $(wildcard Classes/*/*.cpp) \
          $(wildcard Packets/*.cpp) \
          $(wildcard Classes/*/*/*.cpp) \
          $(wildcard Packets/*/*/*.cpp)

OBJECTS = $(SOURCES:.cpp=.o)

all: $(TARGET)

$(TARGET): $(OBJECTS)
	$(CXX) $(CXXFLAGS) -o $(TARGET) $(OBJECTS) $(LIBS)

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	rm -f $(TARGET) $(OBJECTS)

.PHONY: all clean
