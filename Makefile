CXX = c++
CXXFLAGS = -std=c++11 -O3 -w -Xpreprocessor -fopenmp -I/opt/homebrew/opt/libomp/include
LDFLAGS = -lomp -L/opt/homebrew/opt/libomp/lib

# svg_writer.cpp not in sources as it is included in main
SOURCES = main.cpp vector.cpp vonoroi_diagram.cpp
TARGET = main

$(TARGET): $(SOURCES)
	$(CXX) $(CXXFLAGS) $(SOURCES) -o $(TARGET) $(LDFLAGS)

clean:
	rm -f $(TARGET)
