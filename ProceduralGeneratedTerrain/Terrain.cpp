#include "Terrain.h"
#include <limits.h>
#include <array>

void Terrain::calculateMesh(int N, glm::vec3 ipos, float iscale) {
    delete vbo;
    delete ibo;
    delete vao;
    m_Triangles = 2 * 3 * glm::pow(N, 2);
    N++;
    
    m_Pos = ipos;
    m_Scale = iscale;
    m_Mesh.clear();
    m_Indices.clear();
    int index = 0;
    for (unsigned int i = 0; i < N; ++i) {
        for (unsigned int j = 0; j < N; ++j) {
            if ((j != N - 1) && (i != N - 1)) {
                m_Indices.push_back(index); // wound counter-clockwise
                m_Indices.push_back(index + 1);
                m_Indices.push_back(index + N);

                m_Indices.push_back(index + 1);
                m_Indices.push_back(index + N + 1);
                m_Indices.push_back(index + N);
            }
            
            m_Mesh.push_back({ glm::vec3(j + m_Pos.x, m_Pos.y + glm::simplex(glm::vec2((float)(j + m_Pos.x) / perlinDiv, (float)(i + m_Pos.z) / perlinDiv)), i + m_Pos.z), glm::vec3(0.0f, 1.0f,0.0f) });
            
            ++index;
        }
    }
    vao = new VertexArray();
    vbo = new Buffer();
    ibo = new Buffer();
    vbo->uploadData(m_Mesh.data(), m_Mesh.size() * sizeof(Vert));
    ibo->uploadData(m_Indices.data(), m_Indices.size() * sizeof(unsigned int));

    vao->addBuffers(vbo, ibo, sizeof(Vert));
    vao->addAttrib(0, 3, offsetof(Vert, pos));
    vao->addAttrib(1, 3, offsetof(Vert, normal));
}
void Terrain::render(glm::mat4& camView) {
    m_Program->use();
    glm::mat4 model(1);
    model = glm::scale(model, glm::vec3(1) * m_Scale);
    glm::mat4 projection = glm::perspective(glm::radians(45.0f), 1920.0f/1080.0f, 0.1f, 100.0f);
    m_Program->setMat4("uMVT.model", model);
    m_Program->setMat4("uMVT.view", camView);
    m_Program->setMat4("uMVT.projection", projection);
    vao->bind();
    glDrawElements(GL_TRIANGLES,m_Triangles , GL_UNSIGNED_INT, nullptr);
}