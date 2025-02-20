CXX = g++
CXXFLAGS = -Wall -g -Wextra -fPIC
LIBS = -lz -lmongocxx -lbsoncxx
TARGET = libm.dll

SOURCES = $(wildcard *.cpp) \
          $(wildcard Classes/*.cpp) \
          $(wildcard Classes/*/*.cpp) \
          $(wildcard Packets/*.cpp) \
          $(wildcard Classes/*/*/*.cpp) \
          $(wildcard Packets/*/*/*.cpp)

OBJECTS = $(SOURCES:.cpp=.o)

all: $(TARGET)

$(TARGET): $(OBJECTS)
	$(CXX) $(CXXFLAGS) -shared -o $(TARGET) $(OBJECTS) $(LIBS)

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	rm -f $(TARGET) $(OBJECTS)

.PHONY: all clean
