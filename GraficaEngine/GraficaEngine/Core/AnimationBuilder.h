#pragma once

#include <map>
#include "AssimpNodeData.h"
#include "Animator.h"
#include "../Renderer/Model.h"
#include "../Utils/AssimpGLMHelpers.h"
#include "../Renderer/Bone.h"
#include "../Renderer/BoneInfo.h"

namespace Engine {
    class AnimationBuilder
    {
    public:
        AnimationBuilder() = default;

        AnimationBuilder(const std::string& animationPath, Model* model)
        {
            _animation = new Animation();
            Assimp::Importer importer;
            const aiScene* scene = importer.ReadFile(animationPath, aiProcess_Triangulate);
            assert(scene && scene->mRootNode);
            auto animation = scene->mAnimations[0];
            _animation->m_Duration = animation->mDuration;
            _animation->m_TicksPerSecond = animation->mTicksPerSecond;
            aiMatrix4x4 globalTransformation = scene->mRootNode->mTransformation;
            globalTransformation = globalTransformation.Inverse();
            ReadHeirarchyData(_animation->m_RootNode, scene->mRootNode);
            ReadMissingBones(animation, *model);
        }

        ~AnimationBuilder()
        {
        }

        Animation *getAnimation() { return _animation; }


    private:
        void ReadMissingBones(const aiAnimation* animation, Model& model)
        {
            int size = animation->mNumChannels;

            auto& boneInfoMap = model.GetBoneInfoMap();//getting m_BoneInfoMap from Model class
            int& boneCount = model.GetBoneCount(); //getting the m_BoneCounter from Model class

            //reading channels(bones engaged in an animation and their keyframes)
            for (int i = 0; i < size; i++)
            {
                auto channel = animation->mChannels[i];
                std::string boneName = channel->mNodeName.data;

                if (boneInfoMap.find(boneName) == boneInfoMap.end())
                {
                    boneInfoMap[boneName].id = boneCount;
                    boneCount++;
                }
                _animation->m_Bones.push_back(Bone(channel->mNodeName.data,
                    boneInfoMap[channel->mNodeName.data].id, channel));
            }

            _animation->m_BoneInfoMap = boneInfoMap;
        }

        void ReadHeirarchyData(AssimpNodeData& dest, const aiNode* src)
        {
            assert(src);

            dest.name = src->mName.data;
            dest.transformation = AssimpGLMHelpers::ConvertMatrixToGLMFormat(src->mTransformation);
            dest.childrenCount = src->mNumChildren;

            for (int i = 0; i < src->mNumChildren; i++)
            {
                AssimpNodeData newData;
                ReadHeirarchyData(newData, src->mChildren[i]);
                dest.children.push_back(newData);
            }
        }
        Animation *_animation;
    };
}