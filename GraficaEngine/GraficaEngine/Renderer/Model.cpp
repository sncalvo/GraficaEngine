#include "Model.h"

#include <iostream>

namespace Engine
{
    Model::Model(char* path)
    {
        _loadModel(path);
    }

    Model::Model(Model* otherModel)
    {
        _meshes = otherModel->getMeshes();
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

    Mesh Model::_processMesh(aiMesh* mesh, const aiScene* scene)
    {
        std::vector<Vertex> vertices;
        std::vector<unsigned int> indices;
        std::vector<Texture> textures;

        for (unsigned int i = 0; i < mesh->mNumVertices; i++)
        {
            Vertex vertex;
            glm::vec3 vector;
            vector.x = mesh->mVertices[i].x;
            vector.y = mesh->mVertices[i].y;
            vector.z = mesh->mVertices[i].z;
            vertex.position = vector;
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
        aiMaterial* meshMaterial = scene->mMaterials[mesh->mMaterialIndex];

        textures = _loadMaterialTextures(meshMaterial, aiTextureType_DIFFUSE, "texture_diffuse");
        Material material = _loadMaterial(meshMaterial);
        // textures.insert(textures.end(), diffuseMaps.begin(), diffuseMaps.end());
    
        return Mesh(vertices, indices, textures, material);
    }

    std::vector<Texture> Model::_loadMaterialTextures(aiMaterial* mat, aiTextureType type, std::string typeName)
    {
        std::vector<Texture> textures;
        aiString texturePath;

        mat->GetTexture(type, 0, &texturePath);

        if (texturePath.length > 0)
        {
            Texture texture(texturePath.C_Str(), typeName);
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


    void Model::draw(Shader& shader) const
    {
        for (unsigned int i = 0; i < _meshes.size(); i++)
        {
            _meshes[i].draw(shader);
        }
    }

    std::vector<Mesh> Model::getMeshes() const
    {
        return _meshes;
    }
}
