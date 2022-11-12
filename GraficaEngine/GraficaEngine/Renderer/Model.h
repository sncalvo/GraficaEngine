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
#include "MeshRenderer.h"
#include "ShadowRenderer.h"

namespace Engine
{
    class Model
    {
    private:
        std::vector<std::shared_ptr<Mesh>> _meshes;
        std::vector<std::shared_ptr<MeshRenderer>> _meshRenderers;
        std::vector<std::shared_ptr<ShadowRenderer>> _shadowRenderers;

        void _loadModel(std::string path);
        void _processNode(aiNode* node, const aiScene* scene);
        std::shared_ptr<Mesh> _processMesh(aiMesh* mesh, const aiScene* scene);
        std::vector<Texture> _loadMaterialTextures(
            aiMaterial* mat,
            aiTextureType type,
            std::string typeName);

        Material _loadMaterial(aiMaterial* material);
    public:
        Model(char* path);
        Model(Model*);
        std::vector<std::shared_ptr<Mesh>> getMeshes() const;
        // TODO: Probably we don't want model to be the source of the renderers. We should have a Manager of resources that creates this
        std::vector<std::shared_ptr<MeshRenderer>> getMeshRenderers() const {
            return _meshRenderers;
        }
        std::vector<std::shared_ptr<ShadowRenderer>> getShadowRenderers() const {
            return _shadowRenderers;
        }
    };
}
