#include "sModelDrawInfo.h"

unsigned int sModelDrawInfo::m_nextUniqueID = sModelDrawInfo::FIRST_UNIQUE_ID;

#define PRINT_SPACES(x) {int s = x; while(s--> 0) printf(" ");}

sModelDrawInfo::sModelDrawInfo()
{
	this->VAO_ID = 0;

	this->VertexBufferID = 0;
	this->VertexBuffer_Start_Index = 0;
	this->numberOfVertices = 0;

	this->IndexBufferID = 0;
	this->IndexBuffer_Start_Index = 0;
	this->numberOfIndices = 0;
	this->numberOfTriangles = 0;

	this->pVertices = 0;
	this->pIndices = 0;

	this->maxExtents_XYZ = glm::vec3(0.0f);
	this->minExtents_XYZ = glm::vec3(0.0f);
	this->deltaExtents_XYZ = glm::vec3(0.0f);

	this->maxExtent = 0.0f;

	this->m_UniqueID = sModelDrawInfo::m_nextUniqueID;
	sModelDrawInfo::m_nextUniqueID++;

	return;
}

unsigned int sModelDrawInfo::getUniqueID(void)
{
	return this->m_UniqueID;
}

void sModelDrawInfo::calcExtents(void)
{
	if (this->pVertices == NULL)
		return;

	sVertexWithBone* pCurrentVert = &(this->pVertices[0]);

	this->minExtents_XYZ.x = pCurrentVert->x;
	this->minExtents_XYZ.y = pCurrentVert->y;
	this->minExtents_XYZ.z = pCurrentVert->z;

	this->maxExtents_XYZ.x = pCurrentVert->x;
	this->maxExtents_XYZ.y = pCurrentVert->y;
	this->maxExtents_XYZ.z = pCurrentVert->z;

	for (unsigned int vertIndex = 0; vertIndex != this->numberOfVertices; vertIndex++)
	{
		sVertexWithBone* pCurrentVert = &(this->pVertices[vertIndex]);

		if (pCurrentVert->x < this->minExtents_XYZ.x) { this->minExtents_XYZ.x = pCurrentVert->x; }
		if (pCurrentVert->y < this->minExtents_XYZ.y) { this->minExtents_XYZ.y = pCurrentVert->y; }
		if (pCurrentVert->z < this->minExtents_XYZ.z) { this->minExtents_XYZ.z = pCurrentVert->z; }

		if (pCurrentVert->x > this->maxExtents_XYZ.x) { this->maxExtents_XYZ.x = pCurrentVert->x; }
		if (pCurrentVert->y > this->maxExtents_XYZ.y) { this->maxExtents_XYZ.y = pCurrentVert->y; }
		if (pCurrentVert->z > this->maxExtents_XYZ.z) { this->maxExtents_XYZ.z = pCurrentVert->z; }
	}

	this->deltaExtents_XYZ.x = this->maxExtents_XYZ.x - this->minExtents_XYZ.x;
	this->deltaExtents_XYZ.y = this->maxExtents_XYZ.y - this->minExtents_XYZ.y;
	this->deltaExtents_XYZ.z = this->maxExtents_XYZ.z - this->minExtents_XYZ.z;

	this->maxExtent = this->deltaExtents_XYZ.x;
	if (this->maxExtent < this->deltaExtents_XYZ.y) { this->maxExtent = this->deltaExtents_XYZ.y; }
	if (this->maxExtent < this->deltaExtents_XYZ.z) { this->maxExtent = this->deltaExtents_XYZ.z; }

	return;
}

void sModelDrawInfo::AssimpToGLM(const aiMatrix4x4& a, glm::mat4& g)
{
	g[0][0] = a.a1; g[0][1] = a.b1; g[0][2] = a.c1; g[0][3] = a.d1;
	g[1][0] = a.a2; g[1][1] = a.b2; g[1][2] = a.c2; g[1][3] = a.d2;
	g[2][0] = a.a3; g[2][1] = a.b3; g[2][2] = a.c3; g[2][3] = a.d3;
	g[3][0] = a.a4; g[3][1] = a.b4; g[3][2] = a.c4; g[3][3] = a.d4;
}

Node* sModelDrawInfo::CreateAnimNode(aiNode* node)
{
	Node* newNode = new Node(node->mName.C_Str());
	
	AssimpToGLM(node->mTransformation, newNode->Transformation);
	
	return newNode;
}

Node* sModelDrawInfo::GenerateBoneHierarchy(aiNode* assimpNode, const int depth)
{
	Node* node = CreateAnimNode(assimpNode);
	
	for (int i = 0; i < assimpNode->mNumChildren; ++i)
		node->Children.emplace_back(GenerateBoneHierarchy(assimpNode->mChildren[i], depth + 1));

	return node;
}
