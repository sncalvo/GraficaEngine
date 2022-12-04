#include "Model.h"
#include "../Utils/AssimpGLMHelpers.h"

#include <iostream>

#include <assimp/version.h>

#include "ResourceManager.h"

namespace Engine
{
    Model::Model(char* path)
    {
        _loadModel(path);
    }

    Model::Model(Model* otherModel)
    {
        _meshes = otherModel->getMeshes();

        for (auto renderer : otherModel->getMeshRenderers())
        {
            _meshRenderers.push_back(renderer->clone());
        }
        for (auto renderer : otherModel->getShadowRenderers())
        {
            _shadowRenderers.push_back(renderer->clone());
        }
    }

    void Model::_loadModel(std::string path)
    {
        Assimp::Importer import;
        const aiScene* scene = import.ReadFile(path, aiProcess_Triangulate | aiProcess_FlipUVs);

        if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
        {
            DebugLog::error("ERROR::ASSIMP::" + std::string(import.GetErrorString()));
            return;
        }

        _processNode(scene->mRootNode, scene);
    }

    void Model::_processNode(aiNode* node, const aiScene* scene)
    {
        for (unsigned int i = 0; i < node->mNumMeshes; i++)
        {
            aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
            _meshes.push_back(_processMesh(mesh, scene));
        }
 
        for (unsigned int i = 0; i < node->mNumChildren; i++)
        {
            _processNode(node->mChildren[i], scene);
        }
    }

    void Model::SetVertexBoneData(Vertex& vertex, int boneID, float weight) {
        for (int i = 0; i < MAX_BONE_WEIGHTS; ++i)
        {
            if (vertex.m_BoneIDs[i] < 0)
            {
                vertex.m_Weights[i] = weight;
                vertex.m_BoneIDs[i] = boneID;
                break;
            }
        }
    }

    void Model::ExtractBoneWeightForVertices(std::vector<Vertex>& vertices, aiMesh* mesh, const aiScene* scene) {
        for (int boneIndex = 0; boneIndex < mesh->mNumBones; ++boneIndex)
        {
            int boneID = -1;
            std::string boneName = mesh->mBones[boneIndex]->mName.C_Str();
            if (m_BoneInfoMap.find(boneName) == m_BoneInfoMap.end())
            {
                BoneInfo newBoneInfo;
                newBoneInfo.id = m_BoneCounter;
                newBoneInfo.offset = AssimpGLMHelpers::ConvertMatrixToGLMFormat(
                    mesh->mBones[boneIndex]->mOffsetMatrix);
                m_BoneInfoMap[boneName] = newBoneInfo;
                boneID = m_BoneCounter;
                m_BoneCounter++;
            }
            else
            {
                boneID = m_BoneInfoMap[boneName].id;
            }
            assert(boneID != -1);
            auto weights = mesh->mBones[boneIndex]->mWeights;
            int numWeights = mesh->mBones[boneIndex]->mNumWeights;

            for (int weightIndex = 0; weightIndex < numWeights; ++weightIndex)
            {
                int vertexId = weights[weightIndex].mVertexId;
                float weight = weights[weightIndex].mWeight;
                assert(vertexId <= vertices.size());
                SetVertexBoneData(vertices[vertexId], boneID, weight);
            }
        }
    }

    std::shared_ptr<Mesh> Model::_processLOW(std::string modelName)
    {
        std::string fileName = "Assets/Models/" + modelName + "_LOW.obj";
        Assimp::Importer import;
        const aiScene* scene = import.ReadFile(fileName, aiProcess_Triangulate | aiProcess_FlipUVs);

        if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
        {
            // Silenced since was not helpful
            // DebugLog::warning("WARNING::LOW::ASSIMP::" + std::string(import.GetErrorString()));
            return nullptr;
        }

        if (scene->mRootNode->mNumChildren > 0)
        {
            aiNode* node = scene->mRootNode->mChildren[0];

            if (node->mNumMeshes == 0)
            {
                return nullptr;
            }

            aiMesh* mesh = scene->mMeshes[node->mMeshes[0]];

            auto [vertices, indices, center] = _processMeshBuffers(mesh, scene);
            auto newMesh = std::make_shared<Mesh>(vertices, indices, center);

            return newMesh;
        }

        return nullptr;
    }

    std::shared_ptr<Mesh> Model::_processMesh(aiMesh* mesh, const aiScene* scene) {
        std::vector<Texture*> textures;
        
        auto [vertices, indices, center] = _processMeshBuffers(mesh, scene);
        aiMaterial* meshMaterial = scene->mMaterials[mesh->mMaterialIndex];

        std::vector<glm::vec3> vertexPositions;
        for (const auto& vertex : vertices)
        {
            vertexPositions.push_back(vertex.position);
        }
        auto aabb = std::make_shared<Aabb>(vertexPositions);
        aabb->position = center;
        _aabbs.push_back(aabb);

        textures = _loadMaterialTextures(meshMaterial, aiTextureType_DIFFUSE, "texture_diffuse");
        Material material = _loadMaterial(meshMaterial);
        // textures.insert(textures.end(), diffuseMaps.begin(), diffuseMaps.end());

        ExtractBoneWeightForVertices(vertices, mesh, scene);

        // Check if LOW version exists and load it
        auto meshLow = _processLOW(mesh->mName.C_Str());

        auto newMesh = std::make_shared<Mesh>(vertices, indices, center, aabb);
        auto meshRenderer = std::make_shared<MeshRenderer>(newMesh, meshLow, material, textures);
        auto shadowRenderer = std::make_shared<ShadowRenderer>(newMesh, meshLow);

        _meshRenderers.push_back(meshRenderer);
        _shadowRenderers.push_back(shadowRenderer);
        return newMesh;
    }

    std::tuple< std::vector<Vertex>, std::vector<unsigned int>, glm::vec3> Model::_processMeshBuffers(aiMesh* mesh, const aiScene* scene)
    {
        std::vector<Vertex> vertices;
        std::vector<unsigned int> indices;
        glm::vec3 center = glm::vec3(0.f);

        for (unsigned int i = 0; i < mesh->mNumVertices; i++)
        {
            Vertex vertex;
            SetVertexBoneDataToDefault(vertex);

            glm::vec3 vector;
            vector.x = mesh->mVertices[i].x;
            vector.y = mesh->mVertices[i].y;
            vector.z = mesh->mVertices[i].z;
            vertex.position = vector;
            center += vector;

            if (mesh->HasNormals())
            {
                vector.x = mesh->mNormals[i].x;
                vector.y = mesh->mNormals[i].y;
                vector.z = mesh->mNormals[i].z;
                vertex.normal = vector;
            }
            if (mesh->mTextureCoords[0])
            {
                glm::vec2 vec;
                vec.x = mesh->mTextureCoords[0][i].x;
                vec.y = mesh->mTextureCoords[0][i].y;
                vertex.textureCoordinates = vec;
            }
            else
            {
                vertex.textureCoordinates = glm::vec2(0.0f, 0.0f);
            }
            vertices.push_back(vertex);
        }
        for (unsigned int i = 0; i < mesh->mNumFaces; i++)
        {
            aiFace face = mesh->mFaces[i];
            for (unsigned int j = 0; j < face.mNumIndices; j++)
            {
                indices.push_back(face.mIndices[j]);
            }
        }

        center /= mesh->mNumVertices;

        return { vertices, indices, center };
    }

    std::vector<Texture*> Model::_loadMaterialTextures(aiMaterial* mat, aiTextureType type, std::string typeName)
    {
        std::vector<Texture*> textures;
        aiString texturePath;

        mat->GetTexture(type, 0, &texturePath);

        if (texturePath.length > 0)
        {
            Texture* texture = ResourceManager::getInstance().createTexture(texturePath.C_Str(), typeName);
            // Texture texture(texturePath.C_Str(), typeName);
            textures.push_back(texture);
        }

        return textures;
    }

    Material Model::_loadMaterial(aiMaterial* material)
    {
        aiColor3D color;
        material->Get(AI_MATKEY_COLOR_DIFFUSE, color);
        glm::vec3 diffuse{color.r, color.g, color.b};
        glm::vec3 ambient{color.r, color.g, color.b};
        material->Get(AI_MATKEY_COLOR_SPECULAR, color);
        glm::vec3 specular{color.r, color.g, color.b};
        // Currently ignoring ambient and reasigning diffuse since looks better
        // material->Get(AI_MATKEY_COLOR_AMBIENT, color);
        float shininess;
        material->Get(AI_MATKEY_SHININESS, shininess);

        return Material{ ambient, diffuse, specular, shininess };
    }

    std::vector<std::shared_ptr<Mesh>> Model::getMeshes() const
    {
        return _meshes;
    }
}
