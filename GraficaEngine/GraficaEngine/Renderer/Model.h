#pragma once
#include <vector>
#include <string>
#include <iostream>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include "Shader.h"
#include "Mesh.h"
#include "../Utils/DebugLog.h"
#include "Texture.h"

namespace Engine
{
    class Model
    {
    private:
        std::vector<Mesh> _meshes;

        void _loadModel(std::string path);
        void _processNode(aiNode* node, const aiScene* scene);
        Mesh _processMesh(aiMesh* mesh, const aiScene* scene);
        std::vector<Texture> _loadMaterialTextures(
            aiMaterial* mat,
            aiTextureType type,
            std::string typeName);

        Material _loadMaterial(aiMaterial* material);
    public:
        Model(char* path);
        Model(Model*);
        void draw(Shader& shader) const;
        std::vector<Mesh> getMeshes() const;
    };
}
