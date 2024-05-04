#pragma once
#include "cMesh.h"
#include "cPhysics.h"
#include "cLightManager.h"
#include "cLightHelper.h"
#include "cVAOManager.h"
#include "cShaderManager.h"
#include "cCubeMap.h"
#include "cAABB.h"
#include "TextureManager/cBasicTextureManager.h"
#include "cAnimationSystem.h"

class cControlGameEngine
{
private:

    int gSelectedLight = 0;
    int meshListIndex = 0;
    int lightListIndex = 0;
    int textureUnitIndex = 0;

    bool bAnimationReversed = false;
    bool bTransparencyMeshAvailable = false;
    bool bAABBGenerated = false;

    glm::vec3 cameraEye = glm::vec3(0.0, 0.0f, 0.0f);
    glm::vec3 cameraTarget = glm::vec3(0.0f, 0.0f, -1.0f);
    glm::vec3 upVector = glm::vec3(0.0f, 1.0f, 0.0f);

    GLuint shaderProgramID = 0;

    cShaderManager* mShaderManager = NULL;
    cVAOManager* mVAOManager = NULL;
    cPhysics* mPhysicsManager = NULL;
    cLightManager* mLightManager = NULL;
    cBasicTextureManager* mTextureManager = NULL;
    cCubeMap* cubeMapManager = NULL;
    cAnimationSystem* mAnimationManager = NULL;

    std::vector < sPhysicsProperties* > PhysicsModelList;
    std::vector < cMesh* > TotalMeshList;
    std::vector < sModelDrawInfo* > MeshDrawInfoList;
    std::vector < cAABB* > AABBList;

    //std::map< unsigned int, cAABB* > AABBIdMap;

    cShaderManager::cShader vertexShader;
    cShaderManager::cShader fragmentShader;

    int InitializeShader();

    void DrawObject(cMesh* currentMesh, glm::mat4 matModel, GLuint shaderProgramID);

    cMesh* g_pFindMeshByFriendlyName(std::string friendlyNameToFind);

    sModelDrawInfo* g_pFindModelInfoByFriendlyName(std::string friendlyNameToFind);

    sPhysicsProperties* FindPhysicalModelByName(std::string modelName);

    void CalculateBoneMatrices(cMesh* pCurrentMesh, Node* node, const glm::mat4& parentTransformationMatrix, sModelDrawInfo* modelInfo);

public:

    double deltaTime = 0.0f;
    
    float yaw = -90.0f;   // Vertical axis(Left and Right)
    float pitch = 0.0f;   // Horizontal axis(Up and Down)
    float previousX = 0.0f, previousY = 0.0f;

    bool mouseMoved = true;
    bool enableMouseMovement = false;

    //-------------------COMMON FUNCTIONS--------------------------------------------------

    float getRandomFloat(float num1, float num2);

    //-------------------CAMERA CONTROLS---------------------------------------------------

    void MoveCameraPosition(float translate_x, float translate_y, float translate_z);
    void MoveCameraTarget(float translate_x, float translate_y, float translate_z);

    glm::vec3 GetCurrentCameraPosition();
    glm::vec3 GetCurrentCameraTarget();

    //-------------------MESH CONTROLS-----------------------------------------------------

    void LoadModelsInto3DSpace(std::string filePath, std::string modelName, float initial_x, float initial_y, float initial_z);

    void ChangeColor(std::string modelName, float r, float g, float b);

    void UseManualColors(std::string modelName, bool useColor);

    void ScaleModel(std::string modelName, float scale_value);

    void MoveModel(std::string modelName, float translate_x, float translate_y, float translate_z);

    glm::vec3 GetModelPosition(std::string modelName);

    float GetModelScaleValue(std::string modelName);

    void RotateMeshModel(std::string modelName, float angleDegrees, float rotate_x, float rotate_y, float rotate_z);

    void RotateMeshModelUsingQuaternion(std::string modelName, glm::quat rotationQuaternion);

    bool IsVisibilityOn(std::string modelName);

    void TurnVisibilityOn(std::string modelName);

    void TurnWireframeModeOn(std::string modelName);

    void TurnMeshLightsOn(std::string modelName);

    void DeleteMesh(std::string modelName);

    void ShiftToNextMeshInList();

    void ShiftToPreviousMeshInList();

    cMesh* GetCurrentModelSelected();

    void SortMeshesBasedOnTransparency();

    void MakeMeshAABB(std::string modelName, bool isAABB);

    void ToggleMeshBoneWeightColor(std::string modelName);

    //-------------------BONE CONTROLS-----------------------------------------------------

    void ApplyBonesToMesh(std::string modelName, bool applyBones);

    //-----------------ANIMATION CONTROLS--------------------------------------------------

    void UpdateAllAnimations();

    void PlayAnimation(std::string modelName, std::string animationName, float frameRateIncrement);

    //------------------TEXTURE CONTROLS--------------------------------------------------

    void UseTextures(std::string modelName, bool applyTexture);

    void ApplyHeightMap(std::string modelName, std::string textureName, bool applyHeightMap);

    void AddTexturesToTheMix(std::string modelName, std::string texturePath, std::string textureName, float textureRatio);

    void AddTexturesToOverlap(std::string modelName, std::string texturePath, std::string textureName);

    void AddDiscardMaskTexture(std::string modelName, std::string textureName, std::string discardMaskTexturePath);

    void ChangeTextureRatios(std::string modelName, std::vector <float> textureRatio);

    void AdjustHeightMapScale(std::string modelName, std::string textureName, float height);

    void AdjustHeightMapUVPos(std::string modelName, std::string textureName, glm::vec2 UVOffsetPos);

    void RemoveBlackAreasInHeightMap(std::string modelName, std::string textureName, bool removeBlackAreas);

    void RemoveColoredAreasHeightMap(std::string modelName, std::string textureName, bool removeColoredAreas);

    void ApplyDiscardMaskTexture(std::string modelName, std::string textureName, bool applyDiscardMask);

    void AdjustAlphaTransparency(std::string modelName, float alphaTransparencyLevel);

    void UseDiscardMaskTexture(GLuint shaderProgramID, GLint bUseDiscardMaskTexture_UL, std::string texturePath, int textureUnit);

    //-----------------CUBE MAP CONTROLS--------------------------------------------------

    bool AddCubeMap(std::string modelName, std::string skyBoxName, std::string filePathPosX, std::string filePathNegX, std::string filePathPosY, std::string filePathNegY,
                    std::string filePathPosZ, std::string filePathNegZ);
    void DeleteCubeMap();

    void DrawSkyBox(cMesh* skyBox);

    //-------------------AABB CONTROLS----------------------------------------------------

    void CreateSceneAABBs(glm::vec3 startingPoint, glm::ivec3 cubesCount, float cubeSize, std::string meshFilePath);
    
    void AddVerticesToAABB(std::string modelName);
    
    void AddTrianglesToAABB(std::string modelName);

    //-------------------LIGHT CONTROLS---------------------------------------------------

    void CreateLight(int lightId, float initial_x, float initial_y, float initial_z);

    void TurnOffLight(int lightId, bool turnOff);

    void PositionLight(int lightId, float translate_x, float translate_y, float translate_z);

    void ChangeLightIntensity(int lightId, float linearAttentuation, float quadraticAttentuation);

    void ChangeLightType(int lightId, float lightType);

    void ChangeLightAngle(int lightId, float innerAngle, float outerAngle);

    void ChangeLightDirection(int lightId, float direction_x, float direction_y, float direction_z);

    void ChangeLightColour(int lightId, float color_r, float color_g, float color_b);

    float GetLightLinearAttenuation(int lightId);

    float GetLightQuadraticAttenuation(int lightId);

    float GetLightType(int lightId);

    float GetLightInnerAngle(int lightId);

    float GetLightOuterAngle(int lightId);

    float IsLightOn(int lightId);

    glm::vec3 GetLightPosition(int lightId);

    glm::vec3 GetLightDirection(int lightId);

    glm::vec3 GetLightColor(int lightId);

    void ShiftToNextLightInList();

    int GetCurrentLightSelected();

    //------------------PHYSICS CONTROLS---------------------------------------------------

    void ComparePhysicalAttributesWithOtherModels();

    void MakePhysicsHappen(sPhysicsProperties* physicsModel, std::string Model2, std::string collisionType);

    void ChangeModelPhysicsPosition(std::string modelName, float newPositionX, float newPositionY, float newPositionZ);

    void AddSpherePhysicsToMesh(std::string modelName, std::string physicsMeshType, float objectRadius);

    void AddPlanePhysicsToMesh(std::string modelName, std::string physicsMeshType);

    void ChangeModelPhysicsVelocity(std::string modelName, glm::vec3 velocityChange);

    void ChangeModelPhysicsAcceleration(std::string modelName, glm::vec3 accelerationChange);

    int ChangeModelPhysicalMass(std::string modelName, float mass);

    void ResetPosition(sPhysicsProperties* physicsModel);

    void PhysicsEulerIntegration(std::string modelName);

    void ComparePhysicsObjectWithAABBs(std::string modelName);

    //-------------------ENGINE CONTROLS---------------------------------------------------

    int InitializeGameEngine();

    void RunGameEngine(GLFWwindow* window);
};

