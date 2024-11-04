#pragma once

#include <vector>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

class Texture;
class Mesh;

class Model {
public:
    Model();
    ~Model();

    void LoadModel(const std::string & filename);
    void RenderModel();
    void ClearModel();

private:
    void LoadNode(aiNode *node, const aiScene *scene);
    void LoadMesh(aiMesh *mesh, const aiScene *scene);
    void LoadMaterials(const aiScene *scene);

    std::vector<Mesh*> meshList;
    std::vector<Texture*> textureList;
    std::vector<unsigned int> meshToTexture;
};
