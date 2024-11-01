#pragma once

#include <GL/glew.h>

class Mesh {
    public:
      Mesh();
      ~Mesh();

      void CreateMesh(GLfloat *vertices, unsigned int *indices, unsigned int numberOfVertices, unsigned int numberOfIndices);
      void RenderMesh();
      void ClearMesh();

    private:
      GLuint vaoId, vboId, iboId;
      GLsizei indexCount;
};

