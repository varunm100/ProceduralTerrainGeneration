#pragma once
#include "Buffer.h"
#include <vector>
#include "VertexArray.h"
#include "Shader.h"

class Terrain
{
private:
	std::vector<Vert> m_Mesh;
    std::vector<unsigned int> m_Indices;
    glm::vec3 m_Pos;

    Shader* m_Program;
    VertexArray* vao;
    Buffer* vbo;
    Buffer* ibo;

    int m_Triangles;
    float m_Scale;
public:
    Terrain(Shader* iprogram) : m_Program { iprogram }  { }
    ~Terrain() {
        delete vao;
        delete vbo;
        delete ibo;
    }

    void calculateMesh(int N, glm::vec3 ipos, float iscale);
    void render(glm::mat4& camView);
};

