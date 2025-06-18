CXX = c++
CXXFLAGS = -std=c++11 -O3 -w -Xpreprocessor -fopenmp -I/opt/homebrew/opt/libomp/include
LDFLAGS = -lomp -L/opt/homebrew/opt/libomp/lib

SOURCES = fluid.cpp lbfgs.c main.cpp optimal_transport.cpp polygon.cpp utils.cpp vector.cpp voronoi_diagram.cpp
TARGET = main

$(TARGET): $(SOURCES)
	$(CXX) $(CXXFLAGS) $(SOURCES) -o $(TARGET) $(LDFLAGS)

clean:
	rm -f $(TARGET)
