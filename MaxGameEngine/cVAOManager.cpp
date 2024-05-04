#include "cVAOManager.h"

#include "../OpenGLCommon.h"

#include <glm/vec3.hpp>
#include <glm/vec4.hpp>

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include <vector>
#include <sstream>
#include <fstream>

void cVAOManager::setBasePath(std::string basePathWithoutSlash)
{
    this->m_basePathWithoutSlash = basePathWithoutSlash;
    return;
}

bool cVAOManager::LoadModelIntoVAO(std::string friendlyName, std::string fileName, sModelDrawInfo& drawInfo, unsigned int shaderProgramID, bool bIsDynamicBuffer)
{
    drawInfo.meshFileName = fileName;

    drawInfo.friendlyName = friendlyName;

    std::string fileAndPath = this->m_basePathWithoutSlash + "/" + fileName;

    if (!this->m_LoadTheFile(fileAndPath, drawInfo))
        return false;

    glGenVertexArrays(1, &(drawInfo.VAO_ID));

    glBindVertexArray(drawInfo.VAO_ID);

    glGenBuffers(1, &(drawInfo.VertexBufferID));

    glBindBuffer(GL_ARRAY_BUFFER, drawInfo.VertexBufferID);

   /* glBufferData(GL_ARRAY_BUFFER, sizeof(sVertex) * drawInfo.numberOfVertices,
        (GLvoid*)drawInfo.pVertices, (bIsDynamicBuffer ? GL_DYNAMIC_DRAW : GL_STATIC_DRAW));*/

    // Adding this
    glBufferData(GL_ARRAY_BUFFER, sizeof(sVertexWithBone) * drawInfo.numberOfVertices,
        (GLvoid*)drawInfo.pVertices, (bIsDynamicBuffer ? GL_DYNAMIC_DRAW : GL_STATIC_DRAW));

    glGenBuffers(1, &(drawInfo.IndexBufferID));

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, drawInfo.IndexBufferID);
   
    glBufferData(GL_ELEMENT_ARRAY_BUFFER,
        sizeof(unsigned int) * drawInfo.numberOfIndices,
        (GLvoid*)drawInfo.pIndices,
        GL_STATIC_DRAW);

    GLint vpos_location = glGetAttribLocation(shaderProgramID, "vPos");
    GLint vcol_location = glGetAttribLocation(shaderProgramID, "vCol");
    GLint vNormal_location = glGetAttribLocation(shaderProgramID, "vNormal");
    GLint vTextureCoords_location = glGetAttribLocation(shaderProgramID, "vTextureCoords");
    // Adding this
    GLint vBoneIds_location = glGetAttribLocation(shaderProgramID, "vBoneIds");
    // Adding this
    GLint vBoneWeights_location = glGetAttribLocation(shaderProgramID, "vBoneWeights");

    glEnableVertexAttribArray(vpos_location);
    glVertexAttribPointer(vpos_location, 4, GL_FLOAT, GL_FALSE, sizeof(sVertexWithBone), (void*)offsetof(sVertexWithBone, x));

    glEnableVertexAttribArray(vcol_location);
    glVertexAttribPointer(vcol_location, 4, GL_FLOAT, GL_FALSE, sizeof(sVertexWithBone), (void*)offsetof(sVertexWithBone, r));

    glEnableVertexAttribArray(vNormal_location);
    glVertexAttribPointer(vNormal_location, 4, GL_FLOAT, GL_FALSE, sizeof(sVertexWithBone), (void*)offsetof(sVertexWithBone, nx));

    glEnableVertexAttribArray(vTextureCoords_location);
    glVertexAttribPointer(vTextureCoords_location, 2, GL_FLOAT, GL_FALSE, sizeof(sVertexWithBone), (void*)offsetof(sVertexWithBone, u));
    
    // Adding this
    glEnableVertexAttribArray(vBoneIds_location);
    glVertexAttribPointer(vBoneIds_location, 4, GL_INT, GL_FALSE, sizeof(sVertexWithBone), (void*)offsetof(sVertexWithBone, boneId));
    
    // Adding this
    glEnableVertexAttribArray(vBoneWeights_location);
    glVertexAttribPointer(vBoneWeights_location, 4, GL_FLOAT, GL_FALSE, sizeof(sVertexWithBone), (void*)offsetof(sVertexWithBone, boneWeight));

    glBindVertexArray(0);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

    glDisableVertexAttribArray(vpos_location);
    glDisableVertexAttribArray(vcol_location);
    glDisableVertexAttribArray(vNormal_location);
    glDisableVertexAttribArray(vTextureCoords_location);
    // Adding this
    glDisableVertexAttribArray(vBoneIds_location);
    // Adding this
    glDisableVertexAttribArray(vBoneWeights_location);

    this->m_map_ModelName_to_VAOID[drawInfo.friendlyName] = drawInfo;

    return true;
}

bool cVAOManager::FindDrawInfoByModelName(std::string filename, sModelDrawInfo& drawInfo)
{
    std::map< std::string, sModelDrawInfo>::iterator itDrawInfo = this->m_map_ModelName_to_VAOID.find(filename);

    if (itDrawInfo == this->m_map_ModelName_to_VAOID.end())
        return false;

    drawInfo = itDrawInfo->second;
    return true;
}

bool cVAOManager::m_LoadTheFile(std::string fileName, sModelDrawInfo& drawInfo)
{
    Assimp::Importer importer;

    const aiScene* scene = importer.ReadFile(fileName, aiProcess_ValidateDataStructure);

    if (!scene)
        return false;

    const aiMesh* mesh = scene->mMeshes[0]; 

    //-----------------Adding Vertices-----------------------------------

    drawInfo.meshFileName = fileName;
    drawInfo.numberOfVertices = mesh->mNumVertices;
    drawInfo.pVertices = new sVertexWithBone[drawInfo.numberOfVertices];

    for (unsigned int i = 0; i < drawInfo.numberOfVertices; ++i)
    {
        const aiVector3D& vertex = mesh->mVertices[i];
        const aiVector3D& normal = mesh->mNormals[i];
        const aiColor4D& color = mesh->mColors[0][i];

        drawInfo.pVertices[i].x = vertex.x;
        drawInfo.pVertices[i].y = vertex.y;
        drawInfo.pVertices[i].z = vertex.z;

        if (mesh->HasVertexColors(0))
        {
            drawInfo.pVertices[i].r = color.r;
            drawInfo.pVertices[i].g = color.g;
            drawInfo.pVertices[i].b = color.b;
            drawInfo.pVertices[i].a = color.a;
        }

        if (mesh->HasNormals())
        {
            drawInfo.pVertices[i].nx = normal.x;
            drawInfo.pVertices[i].ny = normal.y;
            drawInfo.pVertices[i].nz = normal.z;
        }

        if (mesh->HasTextureCoords(0))
        {
            drawInfo.pVertices[i].u = mesh->mTextureCoords[0][i].x;
            drawInfo.pVertices[i].v = mesh->mTextureCoords[0][i].y;
        }

    }

    //-------------------Adding Faces------------------------------------

    drawInfo.numberOfIndices = mesh->mNumFaces * 3;
    drawInfo.pIndices = new unsigned int[drawInfo.numberOfIndices];
    drawInfo.numberOfTriangles = mesh->mNumFaces;
    drawInfo.pTriangles = new sTriangle[drawInfo.numberOfTriangles];

    for (unsigned int i = 0; i < mesh->mNumFaces; ++i)
    {
        const aiFace& face = mesh->mFaces[i];
        drawInfo.pIndices[i * 3] = face.mIndices[0];
        drawInfo.pIndices[i * 3 + 1] = face.mIndices[1];
        drawInfo.pIndices[i * 3 + 2] = face.mIndices[2];

        drawInfo.pTriangles[i].v1.x = mesh->mVertices[face.mIndices[0]].x;
        drawInfo.pTriangles[i].v1.y = mesh->mVertices[face.mIndices[0]].y;
        drawInfo.pTriangles[i].v1.z = mesh->mVertices[face.mIndices[0]].z;

        drawInfo.pTriangles[i].v2.x = mesh->mVertices[face.mIndices[1]].x;
        drawInfo.pTriangles[i].v2.y = mesh->mVertices[face.mIndices[1]].y;
        drawInfo.pTriangles[i].v2.z = mesh->mVertices[face.mIndices[1]].z;

        drawInfo.pTriangles[i].v3.x = mesh->mVertices[face.mIndices[2]].x;
        drawInfo.pTriangles[i].v3.y = mesh->mVertices[face.mIndices[2]].y;
        drawInfo.pTriangles[i].v3.z = mesh->mVertices[face.mIndices[2]].z;
    }

    //-------------------Adding Bones------------------------------------

    if (mesh->HasBones())
    {
        drawInfo.RootNode = drawInfo.GenerateBoneHierarchy(scene->mRootNode);
        drawInfo.GlobalInverseTransformation = glm::inverse(drawInfo.RootNode->Transformation);

        unsigned int numBones = mesh->mNumBones;

        for (unsigned int boneIdx = 0; boneIdx < numBones; ++boneIdx)
        {
            aiBone* bone = mesh->mBones[boneIdx];

            std::string name(bone->mName.C_Str(), bone->mName.length);
            drawInfo.BoneNameToIdMap.insert(std::pair<std::string, int>(name, drawInfo.BoneInfoList.size()));

            // Store the offset matrices
            BoneInfo info;

            drawInfo.AssimpToGLM(bone->mOffsetMatrix, info.BoneOffset);
            drawInfo.BoneInfoList.emplace_back(info);

            for (int weightIdx = 0; weightIdx < bone->mNumWeights; ++weightIdx)
            {
                aiVertexWeight& vertexWeight = bone->mWeights[weightIdx];

                sVertexWithBone& vertex = drawInfo.pVertices[vertexWeight.mVertexId];

                for (int infoIdx = 0; infoIdx < 4; ++infoIdx)
                {
                    if (vertex.boneWeight[infoIdx] <= 0.f)
                    {
                        vertex.boneId[infoIdx] = boneIdx;
                        vertex.boneWeight[infoIdx] = vertexWeight.mWeight;

                        break;
                    }
                }
            }
        }
    }

    //-----------------Adding Animations---------------------------------

    if (scene->HasAnimations())
    {
        for (unsigned int i = 0; i < scene->mNumAnimations; i++)
        {
            aiAnimation* sceneAnimation = scene->mAnimations[i];

            AnimationInfo animInfo;

            animInfo.AnimationName = sceneAnimation->mName.C_Str();
            animInfo.Duration = (float)sceneAnimation->mDuration;
            animInfo.TicksPerSecond = (float)sceneAnimation->mTicksPerSecond;
            animInfo.RootNode = drawInfo.GenerateBoneHierarchy(scene->mRootNode);

            for (unsigned int j = 0; j < sceneAnimation->mNumChannels; j++)
            {
                aiNodeAnim* sceneNodeAnim = sceneAnimation->mChannels[j];
                NodeAnimation* nodeAnim = new NodeAnimation(sceneNodeAnim->mNodeName.C_Str());

                for (unsigned int k = 0; k < sceneNodeAnim->mNumPositionKeys; k++)
                {
                    glm::vec3 pos = glm::vec3(sceneNodeAnim->mPositionKeys[k].mValue.x, sceneNodeAnim->mPositionKeys[k].mValue.y, sceneNodeAnim->mPositionKeys[k].mValue.z);

                    sPositionKeyFrame keyFrame(pos, (float)sceneNodeAnim->mPositionKeys[k].mTime);

                    nodeAnim->PositionKeys.push_back(keyFrame);
                }

                for (unsigned int k = 0; k < sceneNodeAnim->mNumRotationKeys; k++)
                {
                    glm::quat quaternionRotation = glm::quat(static_cast <float> (sceneNodeAnim->mRotationKeys[k].mValue.w), static_cast <float> (sceneNodeAnim->mRotationKeys[k].mValue.x),
                        static_cast <float> (sceneNodeAnim->mRotationKeys[k].mValue.y), static_cast <float> (sceneNodeAnim->mRotationKeys[k].mValue.z));

                    sRotationKeyFrame keyFrame(quaternionRotation, (float)sceneNodeAnim->mRotationKeys[k].mTime);

                    nodeAnim->RotationKeys.push_back(keyFrame);
                }

                for (unsigned int k = 0; k < sceneNodeAnim->mNumScalingKeys; k++)
                {
                    glm::vec3 scale = glm::vec3(sceneNodeAnim->mScalingKeys[k].mValue.x, sceneNodeAnim->mScalingKeys[k].mValue.y, sceneNodeAnim->mScalingKeys[k].mValue.z);

                    sScaleKeyFrame keyFrame(scale, (float)sceneNodeAnim->mScalingKeys[k].mTime);

                    nodeAnim->ScalingKeys.push_back(keyFrame);
                }

                animInfo.NodeAnimations.insert(std::pair<std::string, NodeAnimation*>(nodeAnim->Name, nodeAnim));
            }

            drawInfo.AnimationInfoList.push_back(animInfo);
        }
    }

    return true;
}

