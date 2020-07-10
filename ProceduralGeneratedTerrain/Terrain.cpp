#include "Terrain.h"
#include <limits.h>
#include <array>

float Terrain::Simplex2d(int x, int z) {
    return glm::simplex(glm::vec2(x / perlinDiv, z / perlinDiv));
}

void Terrain::calculateMesh(int N, glm::vec3 ipos, float iscale) {
    delete vbo;
    delete vao;
    m_Triangles = 2 * 3 * N * N;
    N++;
    
    m_Pos = ipos;
    m_Scale = iscale;
    m_Mesh.clear();
    int index = 0;
    bool bottop = true;
    vector<vector<float>> heightMap(N, vector<float>(N, 0));
    for (unsigned int i = 0; i < N; ++i) {
        for (unsigned int j = 0; j < N; ++j) {
            heightMap[j][i] = Simplex2d(j + m_Pos.x, i + m_Pos.z);
        }
    }
    glm::vec3 normal;
    for (unsigned int i = 0; i < N; ++i) {
        for (unsigned int j = 0; j < N; ++j) {
            if (i != N - 1 && j != N - 1) {
                if (bottop) {
                    m_Mesh.push_back({ glm::vec3(j + m_Pos.x, m_Pos.y + heightMap[j][i], i + m_Pos.z), glm::vec3(0.0f, 1.0f,0.0f) }); // 0
                    m_Mesh.push_back({ glm::vec3(j + m_Pos.x + 1, m_Pos.y + heightMap[j+1][i], i + m_Pos.z), glm::vec3(0.0f, 1.0f,0.0f) }); // 1
                    m_Mesh.push_back({ glm::vec3(j + m_Pos.x, m_Pos.y + heightMap[j][i+1], i + m_Pos.z + 1), glm::vec3(0.0f, 1.0f,0.0f) }); // 2
                    m_Mesh[index + 0].normal = glm::normalize(glm::cross(m_Mesh[index + 2].pos - m_Mesh[index].pos, m_Mesh[index + 1].pos - m_Mesh[index].pos));
                    m_Mesh[index + 1].normal = m_Mesh[index].normal;
                    m_Mesh[index + 2].normal = m_Mesh[index].normal;

                    m_Mesh.push_back({ glm::vec3(j + m_Pos.x + 1, m_Pos.y + heightMap[j+1][i], i + m_Pos.z), glm::vec3(0.0f, 1.0f,0.0f) }); // 0
                    m_Mesh.push_back({ glm::vec3(j + m_Pos.x + 1, m_Pos.y + heightMap[j+1][i+1], i + m_Pos.z + 1), glm::vec3(0.0f, 1.0f,0.0f) }); // 1
                    m_Mesh.push_back({ glm::vec3(j + m_Pos.x, m_Pos.y + heightMap[j][i+1], i + m_Pos.z + 1), glm::vec3(0.0f, 1.0f,0.0f) }); // 2
                    m_Mesh[index + 3].normal = -glm::normalize(glm::cross(m_Mesh[index + 4].pos - m_Mesh[index + 3].pos, m_Mesh[index + 5].pos - m_Mesh[index + 3].pos));
                    m_Mesh[index + 4].normal = m_Mesh[index + 3].normal;
                    m_Mesh[index + 5].normal = m_Mesh[index + 3].normal;

                }
                else {
                    m_Mesh.push_back({ glm::vec3(j + m_Pos.x, m_Pos.y + heightMap[j][i], i + m_Pos.z), glm::vec3(0.0f, 1.0f,0.0f) });
                    m_Mesh.push_back({ glm::vec3(j + m_Pos.x + 1, m_Pos.y + heightMap[j+1][i+1], i + m_Pos.z + 1), glm::vec3(0.0f, 1.0f,0.0f) });
                    m_Mesh.push_back({ glm::vec3(j + m_Pos.x, m_Pos.y + heightMap[j][i+1], i + m_Pos.z + 1), glm::vec3(0.0f, 1.0f,0.0f) });
                    m_Mesh[index + 0].normal = glm::normalize(glm::cross(m_Mesh[index + 2].pos - m_Mesh[index].pos, m_Mesh[index + 1].pos - m_Mesh[index].pos));
                    m_Mesh[index + 1].normal = m_Mesh[index].normal;
                    m_Mesh[index + 2].normal = m_Mesh[index].normal;

                    m_Mesh.push_back({ glm::vec3(j + m_Pos.x, m_Pos.y + heightMap[j][i], i + m_Pos.z), glm::vec3(0.0f, 1.0f,0.0f) });
                    m_Mesh.push_back({ glm::vec3(j + m_Pos.x + 1, m_Pos.y + heightMap[j+1][i], i + m_Pos.z), glm::vec3(0.0f, 1.0f,0.0f) });
                    m_Mesh.push_back({ glm::vec3(j + m_Pos.x + 1, m_Pos.y + heightMap[j+1][i+1], i + m_Pos.z + 1), glm::vec3(0.0f, 1.0f,0.0f) });
                    m_Mesh[index + 3].normal = -glm::normalize(glm::cross(m_Mesh[index + 4].pos - m_Mesh[index + 3].pos, m_Mesh[index + 5].pos - m_Mesh[index + 3].pos));
                    m_Mesh[index + 4].normal = m_Mesh[index + 3].normal;
                    m_Mesh[index + 5].normal = m_Mesh[index + 3].normal;
                }
                index += 6;
            }
            bottop = 1 - bottop;
        }
    }
    vao = new VertexArray();
    vbo = new Buffer();
    vbo->uploadData(m_Mesh.data(), m_Mesh.size() * sizeof(Vert));

    vao->addBuffer(vbo, 0, sizeof(Vert));
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

    m_Program->setVec3("light.dir", glm::vec3(0.0f, 1.0f, 0.0f));
    m_Program->setVec3("light.ambient", glm::vec3(1.0f,1.0f, 1.0f));
    m_Program->setVec3("light.diffuse", glm::vec3(1.0f, 1.0f, 1.0f));
    m_Program->setVec3("terrainDiffuse", glm::vec3(92, 64, 51)/255.0f);
    vao->bind();
    glDrawArrays(GL_TRIANGLES, 0, m_Triangles);
}