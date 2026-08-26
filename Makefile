CXX = g++
CXXFLAGS = -Wall -Wextra -O2 -std=c++11
TARGET = teste_inifiles
SOURCE = teste_inifiles.c

.PHONY: all clean run

all: $(TARGET)

$(TARGET): $(SOURCE) inifiles.h
	$(CXX) $(CXXFLAGS) $(SOURCE) -o $(TARGET)

run: $(TARGET)
	./$(TARGET)

clean:
	rm -f $(TARGET)