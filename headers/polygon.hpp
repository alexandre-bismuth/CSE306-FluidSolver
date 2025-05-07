#ifndef POLYGON_HPP
#define POLYGON_HPP

class Polygon
{
public:
    void addVertex(Vector vertex) { vertices.push_back(vertex); }
    std::vector<Vector> vertices;
};

#endif