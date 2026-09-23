CXX := c++
CXXFLAGS := -std=c++20 -Wall -Wextra -Wpedantic -O3 -g
TARGET := fdtd1d
SOURCE :=template.cpp

$(TARGET): $(SOURCE)
	$(CXX) $(CXXFLAGS) $(SOURCE) -o $(TARGET)

	
run: $(TARGET)
	./$(TARGET)

clean:
	$(RM) $(TARGET)

.PHONY: run clean