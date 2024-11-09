// Model.cpp
// Created by Vlad M on 04.11.2024.

#include "model.h"

#include <GL/glew.h>
#include "../mesh/mesh.h"
#include "../texture/texture.h"

Model::Model() = default;

Model::~Model() {
    ClearModel();
}

void Model::LoadModel(const std::string &filename) {
    Assimp::Importer importer;
    const aiScene *scene = importer.ReadFile(
        filename,
        aiProcess_Triangulate |
        aiProcess_FlipUVs |
        aiProcess_GenSmoothNormals |
        aiProcess_JoinIdenticalVertices
    );

    if (!scene) {
        printf("Failed to load model: %s with error: %s\n", filename.c_str(), importer.GetErrorString());
        return;
    }

    LoadNode(scene->mRootNode, scene);
    LoadMaterials(scene);
}

void Model::RenderModel() {
    for (size_t i = 0; i < meshList.size(); i++) {
        unsigned int materialIndex = meshToTexture[i];
        if (materialIndex < textureList.size() && textureList[materialIndex]) {
            textureList[materialIndex]->UseTexture();
        }

        meshList[i]->RenderMesh();
    }
}

void Model::ClearModel() {
    for (auto &mesh : meshList) {
        if (mesh) {
            delete mesh;
            mesh = nullptr;
        }
    }
    meshList.clear();

    for (auto &texture : textureList) {
        if (texture) {
            delete texture;
            texture = nullptr;
        }
    }

    textureList.clear();
    meshToTexture.clear();
}

void Model::LoadNode(aiNode *node, const aiScene *scene) {
    for (unsigned int i = 0; i < node->mNumMeshes; i++) {
        LoadMesh(scene->mMeshes[node->mMeshes[i]], scene);
    }

    for (unsigned int i = 0; i < node->mNumChildren; i++) {
        LoadNode(node->mChildren[i], scene);
    }
}

void Model::LoadMesh(aiMesh *mesh, const aiScene *scene) {
    std::vector<GLfloat> vertices;
    std::vector<unsigned int> indices;

    for (unsigned int i = 0; i < mesh->mNumVertices; i++) {
        vertices.push_back(mesh->mVertices[i].x);
        vertices.push_back(mesh->mVertices[i].y);
        vertices.push_back(mesh->mVertices[i].z);

        if (mesh->mTextureCoords[0]) {
            vertices.push_back(mesh->mTextureCoords[0][i].x);
            vertices.push_back(mesh->mTextureCoords[0][i].y);
        } else {
            vertices.push_back(0.0f);
            vertices.push_back(0.0f);
        }

        vertices.push_back(mesh->mNormals[i].x);
        vertices.push_back(mesh->mNormals[i].y);
        vertices.push_back(mesh->mNormals[i].z);
    }

    for (unsigned int i = 0; i < mesh->mNumFaces; i++) {
        aiFace face = mesh->mFaces[i];
        for (unsigned int j = 0; j < face.mNumIndices; j++) {
            indices.push_back(face.mIndices[j]);
        }
    }

    Mesh *newMesh = new Mesh();
    newMesh->CreateMesh(vertices.data(), indices.data(), vertices.size(), indices.size());
    meshList.push_back(newMesh);
    meshToTexture.push_back(mesh->mMaterialIndex);
}

void Model::LoadMaterials(const aiScene *scene) {
    textureList.resize(scene->mNumMaterials, nullptr);

    for (unsigned int i = 0; i < scene->mNumMaterials; i++) {
        aiMaterial *material = scene->mMaterials[i];
        if (!material) continue;

        aiString diffuseTexturePath;
        if (material->GetTextureCount(aiTextureType_DIFFUSE) > 0 &&
            material->GetTexture(aiTextureType_DIFFUSE, 0, &diffuseTexturePath) == AI_SUCCESS) {

            // Extract filename and create texture path
            std::string filename = diffuseTexturePath.C_Str();
            int idx = filename.find_last_of("\\/");
            filename = filename.substr(idx + 1);
            std::string texPath = "textures/" + filename;

            // Load the texture
            Texture *newTexture = new Texture(texPath.c_str());
            if (!newTexture->LoadTexture()) {
                printf("Failed to load texture: %s\n", texPath.c_str());
                delete newTexture;
                newTexture = nullptr;
            }
            textureList[i] = newTexture;
        }

        // If no texture was loaded, use a default plain texture
        if (!textureList[i]) {
            textureList[i] = new Texture("textures/plain.png");
            if (!textureList[i]->LoadTextureAlpha()) {
                printf("Failed to load default texture: textures/plain.png\n");
                delete textureList[i];
                textureList[i] = nullptr;
            }
        }
    }
}
