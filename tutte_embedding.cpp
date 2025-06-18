// This is the implementation for Lab 9 on Tutte Embedding
#define _CRT_SECURE_NO_WARNINGS 1
#include <iostream>
#include <map>
#include <set>
#include "headers/utils.hpp"

// Class from the given Pastebin (https://pastebin.com/CAgp9r15)
// --Class reused from RayTracer Project--
class TriangleIndices
{
public:
    TriangleIndices(int vtxi = -1, int vtxj = -1, int vtxk = -1, int ni = -1, int nj = -1, int nk = -1,
                    int uvi = -1, int uvj = -1, int uvk = -1, int group = -1, bool added = false)
        : vtxi(vtxi), vtxj(vtxj), vtxk(vtxk), uvi(uvi), uvj(uvj), uvk(uvk), ni(ni), nj(nj), nk(nk), group(group) {};
    int vtxi, vtxj, vtxk; // indices within the vertex coordinates array
    int uvi, uvj, uvk;    // indices within the uv coordinates array
    int ni, nj, nk;       // indices within the normals array
    int group;            // face group
};

// --Class reused from RayTracer Project--
class TriangleMesh
{
public:
    TriangleMesh() {};

    void readOBJ(const char *obj);
    void write_obj(const char *filename);
    void tutteEmbedding();

    std::vector<TriangleIndices> indices;
    std::vector<Vector> vertices, normals, uvs, vertexcolors;
};

void TriangleMesh::tutteEmbedding()
{ // Code based on Section 4.6.1, Algorithm 5 (p. 108-109)

    // Line 1 of Algorithm 4 - AI helped
    std::map<int, std::set<int>> adjacency;
    std::map<std::pair<int, int>, int> edgeCounts;
    for (const TriangleIndices &triangle : indices)
    {
        int triIndices[3] = {triangle.vtxi, triangle.vtxj, triangle.vtxk};
        for (size_t i = 0; i < 3; ++i)
        {
            int u = triIndices[i];
            int v = triIndices[(i + 1) % 3];
            adjacency[u].insert(v);
            adjacency[v].insert(u);
            if (u > v)
                std::swap(u, v);
            edgeCounts[{u, v}]++;
        }
    }

    std::set<int> boundaryVertexSet;
    std::map<int, std::vector<int>> boundaryAdjacency;
    for (const std::pair<std::pair<int, int>, int> &edge : edgeCounts)
    {
        if (edge.second == 1)
        {
            int u = edge.first.first, v = edge.first.second;
            boundaryVertexSet.insert(u);
            boundaryVertexSet.insert(v);
            boundaryAdjacency[u].push_back(v);
            boundaryAdjacency[v].push_back(u);
        }
    }

    std::vector<int> boundary;
    int startVertex = *boundaryVertexSet.begin(), currentVertex = startVertex, prevVertex = -1;
    while (true)
    {
        boundary.push_back(currentVertex);
        const std::vector<int> &neighbors = boundaryAdjacency[currentVertex];
        int nextVertex = -1;
        for (int neighbor : neighbors)
        {
            if (neighbor != prevVertex)
            {
                nextVertex = neighbor;
                break;
            }
        }
        prevVertex = currentVertex;
        currentVertex = nextVertex;
        if (currentVertex == startVertex)
            break;
    }

    // Line 2 of Algorithm 4
    double s = 0.;
    for (size_t i = 0; i < boundary.size(); ++i)
        s += (vertices[boundary[i]] - vertices[boundary[(i + 1) % boundary.size()]]).norm();

    // Line 3-8 of Algorithm 4
    double cs = 0.;
    std::vector<Vector> v(vertices.size());
    for (size_t i = 0; i < boundary.size(); ++i)
    {
        double theta = 2 * M_PI * cs / s;
        v[boundary[i]] = Vector(cos(theta), sin(theta));
        cs += (vertices[boundary[(i + 1) % boundary.size()]] - vertices[boundary[i]]).norm();
    }

    // Optional : Harmonic mapping by replacing the graph Laplacian by the cotangent Laplacian - AI helped
    // std::map<std::pair<int, int>, double> cotanWeights;
    // for (const TriangleIndices &tri : indices)
    // {
    //     Vector vertexI = vertices[tri.vtxi], vertexJ = vertices[tri.vtxj], vertexK = vertices[tri.vtxk];
    //     Vector ij = vertexJ - vertexI, ik = vertexK - vertexI, ji = vertexI - vertexJ,
    //            jk = vertexK - vertexJ, ki = vertexI - vertexK, kj = vertexJ - vertexK;

    //     double angleK = acos(dot(ki, kj) / (ki.norm() * kj.norm()));
    //     double angleJ = acos(dot(ji, jk) / (ji.norm() * jk.norm()));
    //     double angleI = acos(dot(ij, ik) / (ij.norm() * ik.norm()));

    //     cotanWeights[{std::min(tri.vtxi, tri.vtxj), std::max(tri.vtxi, tri.vtxj)}] += 1. / tan(angleK);
    //     cotanWeights[{std::min(tri.vtxi, tri.vtxk), std::max(tri.vtxi, tri.vtxk)}] += 1. / tan(angleJ);
    //     cotanWeights[{std::min(tri.vtxj, tri.vtxk), std::max(tri.vtxj, tri.vtxk)}] += 1. / tan(angleI);
    // }

    // Line 9-13 of Algorithm 4
    for (size_t iter = 0; iter < 1000; ++iter)
    {
        std::vector<Vector> temp = v;
        // Line 10-11 of Algorithm 4
        for (size_t i = 0; i < vertices.size(); ++i)
        {
            if (boundaryVertexSet.count(i) == 0) // all interior vertex indices i
            {
                // Version without optional harmoninc mapping
                // If you add the optinal part, you also have to modify this section
                Vector sumNeighbors;
                const std::set<int> &neighbors = adjacency[i];
                for (size_t j : neighbors)
                    sumNeighbors += v[j];
                if (!neighbors.empty())
                    temp[i] = sumNeighbors / neighbors.size();
            }
        }

        // Line 12-13 of Algorithm 4
        for (int i : boundaryVertexSet) // all boundary vertex indices i
            temp[i] = v[i];
        v = temp;
    }
    this->vertices = v;
}

// Function based on the given Pastebin (https://pastebin.com/CAgp9r15)
// --Function reused from RayTracer Project--
void TriangleMesh::readOBJ(const char *obj)
{ // N.B : My formatter changed the appearance of the function
    char matfile[255];
    char grp[255];

    FILE *f;
    f = fopen(obj, "r");
    int curGroup = -1;
    while (!feof(f))
    {
        char line[255];
        if (!fgets(line, 255, f))
            break;

        std::string linetrim(line);
        linetrim.erase(linetrim.find_last_not_of(" \r\t") + 1);
        strcpy(line, linetrim.c_str());

        if (line[0] == 'u' && line[1] == 's')
        {
            sscanf(line, "usemtl %[^\n]\n", grp);
            curGroup++;
        }

        if (line[0] == 'v' && line[1] == ' ')
        {
            Vector vec;

            Vector col;
            if (sscanf(line, "v %lf %lf %lf %lf %lf %lf\n", &vec[0], &vec[1], &vec[2], &col[0], &col[1], &col[2]) == 6)
            {
                col[0] = std::min(1., std::max(0., col[0]));
                col[1] = std::min(1., std::max(0., col[1]));
                col[2] = std::min(1., std::max(0., col[2]));

                vertices.push_back(vec);
                vertexcolors.push_back(col);
            }
            else
            {
                sscanf(line, "v %lf %lf %lf\n", &vec[0], &vec[1], &vec[2]);
                vertices.push_back(vec);
            }
        }
        if (line[0] == 'v' && line[1] == 'n')
        {
            Vector vec;
            sscanf(line, "vn %lf %lf %lf\n", &vec[0], &vec[1], &vec[2]);
            normals.push_back(vec);
        }
        if (line[0] == 'v' && line[1] == 't')
        {
            Vector vec;
            sscanf(line, "vt %lf %lf\n", &vec[0], &vec[1]);
            uvs.push_back(vec);
        }
        if (line[0] == 'f')
        {
            TriangleIndices t;
            int i0, i1, i2, i3;
            int j0, j1, j2, j3;
            int k0, k1, k2, k3;
            int nn;
            t.group = curGroup;

            char *consumedline = line + 1;
            int offset;

            nn = sscanf(consumedline, "%u/%u/%u %u/%u/%u %u/%u/%u%n", &i0, &j0, &k0, &i1, &j1, &k1, &i2, &j2, &k2, &offset);
            if (nn == 9)
            {
                if (i0 < 0)
                    t.vtxi = vertices.size() + i0;
                else
                    t.vtxi = i0 - 1;
                if (i1 < 0)
                    t.vtxj = vertices.size() + i1;
                else
                    t.vtxj = i1 - 1;
                if (i2 < 0)
                    t.vtxk = vertices.size() + i2;
                else
                    t.vtxk = i2 - 1;
                if (j0 < 0)
                    t.uvi = uvs.size() + j0;
                else
                    t.uvi = j0 - 1;
                if (j1 < 0)
                    t.uvj = uvs.size() + j1;
                else
                    t.uvj = j1 - 1;
                if (j2 < 0)
                    t.uvk = uvs.size() + j2;
                else
                    t.uvk = j2 - 1;
                if (k0 < 0)
                    t.ni = normals.size() + k0;
                else
                    t.ni = k0 - 1;
                if (k1 < 0)
                    t.nj = normals.size() + k1;
                else
                    t.nj = k1 - 1;
                if (k2 < 0)
                    t.nk = normals.size() + k2;
                else
                    t.nk = k2 - 1;
                indices.push_back(t);
            }
            else
            {
                nn = sscanf(consumedline, "%u/%u %u/%u %u/%u%n", &i0, &j0, &i1, &j1, &i2, &j2, &offset);
                if (nn == 6)
                {
                    if (i0 < 0)
                        t.vtxi = vertices.size() + i0;
                    else
                        t.vtxi = i0 - 1;
                    if (i1 < 0)
                        t.vtxj = vertices.size() + i1;
                    else
                        t.vtxj = i1 - 1;
                    if (i2 < 0)
                        t.vtxk = vertices.size() + i2;
                    else
                        t.vtxk = i2 - 1;
                    if (j0 < 0)
                        t.uvi = uvs.size() + j0;
                    else
                        t.uvi = j0 - 1;
                    if (j1 < 0)
                        t.uvj = uvs.size() + j1;
                    else
                        t.uvj = j1 - 1;
                    if (j2 < 0)
                        t.uvk = uvs.size() + j2;
                    else
                        t.uvk = j2 - 1;
                    indices.push_back(t);
                }
                else
                {
                    nn = sscanf(consumedline, "%u %u %u%n", &i0, &i1, &i2, &offset);
                    if (nn == 3)
                    {
                        if (i0 < 0)
                            t.vtxi = vertices.size() + i0;
                        else
                            t.vtxi = i0 - 1;
                        if (i1 < 0)
                            t.vtxj = vertices.size() + i1;
                        else
                            t.vtxj = i1 - 1;
                        if (i2 < 0)
                            t.vtxk = vertices.size() + i2;
                        else
                            t.vtxk = i2 - 1;
                        indices.push_back(t);
                    }
                    else
                    {
                        nn = sscanf(consumedline, "%u//%u %u//%u %u//%u%n", &i0, &k0, &i1, &k1, &i2, &k2, &offset);
                        if (i0 < 0)
                            t.vtxi = vertices.size() + i0;
                        else
                            t.vtxi = i0 - 1;
                        if (i1 < 0)
                            t.vtxj = vertices.size() + i1;
                        else
                            t.vtxj = i1 - 1;
                        if (i2 < 0)
                            t.vtxk = vertices.size() + i2;
                        else
                            t.vtxk = i2 - 1;
                        if (k0 < 0)
                            t.ni = normals.size() + k0;
                        else
                            t.ni = k0 - 1;
                        if (k1 < 0)
                            t.nj = normals.size() + k1;
                        else
                            t.nj = k1 - 1;
                        if (k2 < 0)
                            t.nk = normals.size() + k2;
                        else
                            t.nk = k2 - 1;
                        indices.push_back(t);
                    }
                }
            }

            consumedline = consumedline + offset;

            while (true)
            {
                if (consumedline[0] == '\n')
                    break;
                if (consumedline[0] == '\0')
                    break;
                nn = sscanf(consumedline, "%u/%u/%u%n", &i3, &j3, &k3, &offset);
                TriangleIndices t2;
                t2.group = curGroup;
                if (nn == 3)
                {
                    if (i0 < 0)
                        t2.vtxi = vertices.size() + i0;
                    else
                        t2.vtxi = i0 - 1;
                    if (i2 < 0)
                        t2.vtxj = vertices.size() + i2;
                    else
                        t2.vtxj = i2 - 1;
                    if (i3 < 0)
                        t2.vtxk = vertices.size() + i3;
                    else
                        t2.vtxk = i3 - 1;
                    if (j0 < 0)
                        t2.uvi = uvs.size() + j0;
                    else
                        t2.uvi = j0 - 1;
                    if (j2 < 0)
                        t2.uvj = uvs.size() + j2;
                    else
                        t2.uvj = j2 - 1;
                    if (j3 < 0)
                        t2.uvk = uvs.size() + j3;
                    else
                        t2.uvk = j3 - 1;
                    if (k0 < 0)
                        t2.ni = normals.size() + k0;
                    else
                        t2.ni = k0 - 1;
                    if (k2 < 0)
                        t2.nj = normals.size() + k2;
                    else
                        t2.nj = k2 - 1;
                    if (k3 < 0)
                        t2.nk = normals.size() + k3;
                    else
                        t2.nk = k3 - 1;
                    indices.push_back(t2);
                    consumedline = consumedline + offset;
                    i2 = i3;
                    j2 = j3;
                    k2 = k3;
                }
                else
                {
                    nn = sscanf(consumedline, "%u/%u%n", &i3, &j3, &offset);
                    if (nn == 2)
                    {
                        if (i0 < 0)
                            t2.vtxi = vertices.size() + i0;
                        else
                            t2.vtxi = i0 - 1;
                        if (i2 < 0)
                            t2.vtxj = vertices.size() + i2;
                        else
                            t2.vtxj = i2 - 1;
                        if (i3 < 0)
                            t2.vtxk = vertices.size() + i3;
                        else
                            t2.vtxk = i3 - 1;
                        if (j0 < 0)
                            t2.uvi = uvs.size() + j0;
                        else
                            t2.uvi = j0 - 1;
                        if (j2 < 0)
                            t2.uvj = uvs.size() + j2;
                        else
                            t2.uvj = j2 - 1;
                        if (j3 < 0)
                            t2.uvk = uvs.size() + j3;
                        else
                            t2.uvk = j3 - 1;
                        consumedline = consumedline + offset;
                        i2 = i3;
                        j2 = j3;
                        indices.push_back(t2);
                    }
                    else
                    {
                        nn = sscanf(consumedline, "%u//%u%n", &i3, &k3, &offset);
                        if (nn == 2)
                        {
                            if (i0 < 0)
                                t2.vtxi = vertices.size() + i0;
                            else
                                t2.vtxi = i0 - 1;
                            if (i2 < 0)
                                t2.vtxj = vertices.size() + i2;
                            else
                                t2.vtxj = i2 - 1;
                            if (i3 < 0)
                                t2.vtxk = vertices.size() + i3;
                            else
                                t2.vtxk = i3 - 1;
                            if (k0 < 0)
                                t2.ni = normals.size() + k0;
                            else
                                t2.ni = k0 - 1;
                            if (k2 < 0)
                                t2.nj = normals.size() + k2;
                            else
                                t2.nj = k2 - 1;
                            if (k3 < 0)
                                t2.nk = normals.size() + k3;
                            else
                                t2.nk = k3 - 1;
                            consumedline = consumedline + offset;
                            i2 = i3;
                            k2 = k3;
                            indices.push_back(t2);
                        }
                        else
                        {
                            nn = sscanf(consumedline, "%u%n", &i3, &offset);
                            if (nn == 1)
                            {
                                if (i0 < 0)
                                    t2.vtxi = vertices.size() + i0;
                                else
                                    t2.vtxi = i0 - 1;
                                if (i2 < 0)
                                    t2.vtxj = vertices.size() + i2;
                                else
                                    t2.vtxj = i2 - 1;
                                if (i3 < 0)
                                    t2.vtxk = vertices.size() + i3;
                                else
                                    t2.vtxk = i3 - 1;
                                consumedline = consumedline + offset;
                                i2 = i3;
                                indices.push_back(t2);
                            }
                            else
                            {
                                consumedline = consumedline + 1;
                            }
                        }
                    }
                }
            }
        }
    }
    fclose(f);
}

// Code from Slack - https://csc3s006ep2024-2025.slack.com/archives/C08J0R6UQK0/p1748447707594079
void TriangleMesh::write_obj(const char *filename)
{

    FILE *f = fopen(filename, "w+");
    for (int i = 0; i < vertices.size(); i++)
        fprintf(f, "v %3.5f %3.5f %3.5f\n", vertices[i][0], vertices[i][1], vertices[i][2]);
    for (int i = 0; i < indices.size(); i++)
        fprintf(f, "f %u %u %u\n", indices[i].vtxi + 1, indices[i].vtxj + 1, indices[i].vtxk + 1);
    fclose(f);
}

int main()
{
    TriangleMesh mesh;
    mesh.readOBJ("goethe_mask.obj");
    mesh.tutteEmbedding();
    mesh.write_obj("tutte_embedding.obj");
    return 0;
}

// To execute, run: g++ -std=c++11 -O3 tutte_embedding.cpp utils.cpp vector.cpp polygon.cpp -o tutte_embedding