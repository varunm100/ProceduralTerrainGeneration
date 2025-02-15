#pragma once
#include "Buffer.h"
#include <vector>
#include "VertexArray.h"
#include "Shader.h"
#include <glm/gtc/noise.hpp>

class Terrain
{
private:
	std::vector<Vert> m_Mesh;
    glm::vec3 m_Pos;

    Shader* m_Program;
    VertexArray* vao;
    Buffer* vbo;

    int m_Triangles;
    float m_Scale;
    const float perlinDiv = 10.0f;
public:
    Terrain(Shader* iprogram) : m_Program { iprogram }  { }
    ~Terrain() {
        delete vao;
        delete vbo;
    }
    float Simplex2d(int x, int z);
    void calculateMesh(int N, glm::vec3 ipos, float iscale);
    void render(glm::mat4& camView);
};

