#pragma once

#include <glm/glm.hpp>
#include <glm/vec3.hpp>
#include <string>
#include <vector>
#include <map>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h> 
#include "KeyFrames.h"

struct sVertex
{
	float x, y, z, w;		//vPos	
	float r, g, b, a;		//vCol	
	float nx, ny, nz, nw;	//vNormal	
	float u, v;				//vTextureCoords
};

struct sVertexWithBone
{
	sVertexWithBone()
	{
		boneId[0] = 0;
		boneId[1] = 0;
		boneId[2] = 0;
		boneId[3] = 0;
		boneWeight[0] = 0.f;
		boneWeight[1] = 0.f;
		boneWeight[2] = 0.f;
		boneWeight[3] = 0.f;

		x = y = z = w = 0.f;
		r = g = b = a = 0.f;
		nx = ny = nz = nw = 0.f;
		u = v = 0.f;
	}

	float x, y, z, w;		//vPos	
	float r, g, b, a;		//vCol	
	float nx, ny, nz, nw;	//vNormal	
	float u, v;				//vTextureCoords
	int boneId[4];			//vBoneIds
	float boneWeight[4];	//vBoneWeights
};

struct sTriangle
{
	glm::vec3 v1;
	glm::vec3 v2;
	glm::vec3 v3;
};

struct BoneInfo
{
	glm::mat4 BoneOffset = glm::mat4(1.0f);				// Offset from the parent bone/node
	glm::mat4 WorldTransformation = glm::mat4(1.0f);	// Calculated transformation used for rendering
};

// Connection Node for hierarchy
struct Node
{
	Node(const std::string& name) : Name(name) { }

	std::string Name;
	glm::mat4 Transformation = glm::mat4(1.0f);
	std::vector<Node*> Children;
};

struct NodeAnimation
{
	NodeAnimation(const std::string& name) : Name(name) { }

	std::string Name;

	std::vector<sPositionKeyFrame> PositionKeys;
	std::vector<sRotationKeyFrame> RotationKeys;
	std::vector<sScaleKeyFrame> ScalingKeys;
};

struct AnimationInfo
{
	std::string AnimationName;

	float Duration;
	float TicksPerSecond;

	Node* RootNode;

	std::map<std::string, NodeAnimation*> NodeAnimations;
};

struct sModelDrawInfo
{
	sModelDrawInfo();

	std::string meshFileName;
	std::string friendlyName;

	unsigned int VAO_ID;

	unsigned int VertexBufferID;
	unsigned int VertexBuffer_Start_Index;
	unsigned int numberOfVertices;

	unsigned int IndexBufferID;
	unsigned int IndexBuffer_Start_Index;
	unsigned int numberOfIndices;
	unsigned int numberOfTriangles;

	unsigned int* pIndices;
	
	float maxExtent;

	sVertexWithBone* pVertices;
	sTriangle* pTriangles;
	Node* RootNode;

	glm::vec3 maxExtents_XYZ;
	glm::vec3 minExtents_XYZ;
	glm::vec3 deltaExtents_XYZ;
	
	std::vector<BoneInfo> BoneInfoList;
	std::vector<AnimationInfo> AnimationInfoList;

	std::map<std::string, int> NodeNameToIdMap;
	std::map<std::string, int> BoneNameToIdMap;

	glm::mat4 GlobalInverseTransformation;

	Node* GenerateBoneHierarchy(aiNode* node, const int depth = 0);
	void AssimpToGLM(const aiMatrix4x4& a, glm::mat4& g);
	Node* CreateAnimNode(aiNode* node);
	void calcExtents(void);
	unsigned int getUniqueID(void);

private:

	unsigned int m_UniqueID;
	static const unsigned int FIRST_UNIQUE_ID = 1;
	static unsigned int m_nextUniqueID;
};
