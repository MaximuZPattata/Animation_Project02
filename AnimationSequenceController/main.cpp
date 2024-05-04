#include "pch.h"
#include "cMesh.h"
#include "cJsonReader.h"
#include "cHiResTimer.h"
#include "cAnimationSystem.h"

#include <cControlGameEngine.h>

static void error_callback(int error, const char* description)
{
    fprintf(stderr, "ERROR : %s\n", description);
}

//---------------------------Global Objects-----------------------------------------------

GLFWwindow* window;
cControlGameEngine gameEngine;
cJsonReader jsonReader;
sCameraDetailsFromFile camDetails;
sCubeMapDetailsFromFile cubeMapDetails;
cHiResTimer* timer = new cHiResTimer(60);

std::vector<sModelDetailsFromFile> modelDetailsList;
std::vector<sLightDetailsFromFile> lightDetailsList;
std::vector<sPhysicsDetailsFromFile> physicsDetailsList;

const float POSITION_OFFSET = 0.0f;

int main()
{
    //-----------------------------------Initialize Window--------------------------------------

    int result = 0;

    glfwSetErrorCallback(error_callback);

    if (!glfwInit())
        exit(EXIT_FAILURE);

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 2);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);

    window = glfwCreateWindow(1200, 800, "Game World", NULL, NULL);

    if (!window)
    {
        glfwTerminate();
        exit(EXIT_FAILURE);
    }

    //------------------------------Input key and Cursor initialize-----------------------------

    glfwSetKeyCallback(window, key_callback);

    glfwSetCursorPosCallback(window, mouse_callback);

    glfwMakeContextCurrent(window);
    gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
    glfwSwapInterval(1);

    //--------------------------------Initialize Game Engine----------------------------------------

    result = gameEngine.InitializeGameEngine();

    if (result != 0)
        return -1;

    //--------------------------------Loading Models, Lights and initial camera position from Json file---------------------------------------------

    bool jsonresult = jsonReader.ReadScene("SceneDescription.json", modelDetailsList, cubeMapDetails, physicsDetailsList, lightDetailsList, camDetails);

    if (jsonresult)
    {
        std::cout << "File read successfully !" << std::endl;

        std::string modelName;
        int lightId;
        float angleDegrees;

        // Loading Models
        for (int index = 0; index < modelDetailsList.size(); index++)
        {
            modelName = modelDetailsList[index].modelName;

            //---------------------------Load Models with position----------------------------------------------------------------------

            gameEngine.LoadModelsInto3DSpace(modelDetailsList[index].modelFilePath, modelName, modelDetailsList[index].modelPosition.x + POSITION_OFFSET,
                modelDetailsList[index].modelPosition.y + POSITION_OFFSET, modelDetailsList[index].modelPosition.z + POSITION_OFFSET);

            //---------------------------------Rotate Models-----------------------------------------------------------------------------

            if (modelDetailsList[index].modelOrientation.x != 0.f)
            {
                angleDegrees = modelDetailsList[index].modelOrientation.x;
                gameEngine.RotateMeshModel(modelName, angleDegrees, 1.f, 0.f, 0.f);
            }
            if (modelDetailsList[index].modelOrientation.y != 0.f)
            {
                angleDegrees = modelDetailsList[index].modelOrientation.y;
                gameEngine.RotateMeshModel(modelName, angleDegrees, 0.f, 1.f, 0.f);
            }
            if (modelDetailsList[index].modelOrientation.z != 0.f)
            {
                angleDegrees = modelDetailsList[index].modelOrientation.z;
                gameEngine.RotateMeshModel(modelName, angleDegrees, 0.f, 0.f, 1.f);
            }

            //------------------------------------Scale Models-----------------------------------------------------------------------------

            gameEngine.ScaleModel(modelName, modelDetailsList[index].modelScaleValue);

            //-----------------------------------Model Wireframe---------------------------------------------------------------------------

            if (modelDetailsList[index].wireframeModeOn)
                gameEngine.TurnWireframeModeOn(modelName);

            //----------------------------------Model Mesh Light---------------------------------------------------------------------------

            if (modelDetailsList[index].meshLightsOn)
                gameEngine.TurnMeshLightsOn(modelName);

            //-------------------------------------Model Bones-----------------------------------------------------------------------------

            if (modelDetailsList[index].applyBones)
                gameEngine.ApplyBonesToMesh(modelName, true);

            //----------------------------------Cube Map Details-----------------------------------------------------------------------------

            if (modelDetailsList[index].isSkyBox)
            {
                if (!cubeMapDetails.cubeMapName.empty())
                {
                    result = gameEngine.AddCubeMap(modelName, cubeMapDetails.cubeMapName, cubeMapDetails.filePathPosX, cubeMapDetails.filePathNegX,
                        cubeMapDetails.filePathPosY, cubeMapDetails.filePathNegY, cubeMapDetails.filePathPosZ,
                        cubeMapDetails.filePathNegZ);
                    if (result != 0)
                        return -1;
                }
                else
                {
                    std::cout << "Cube Properties wasnt updated or added in the json file" << std::endl;

                    return -1;
                }
            }
            else
            {
                //------------------------------------Color Models-----------------------------------------------------------------------------

                if (modelDetailsList[index].manualColors)
                {
                    gameEngine.UseManualColors(modelName, true);
                    gameEngine.ChangeColor(modelName, modelDetailsList[index].modelColorRGB.x, modelDetailsList[index].modelColorRGB.y, modelDetailsList[index].modelColorRGB.z);
                }

                //------------------------------------Add Textures------------------------------------------------------------------------------

                if (modelDetailsList[index].useTextures)
                {
                    gameEngine.UseTextures(modelName, true);

                    for (int textureIndex = 0; textureIndex < modelDetailsList[index].textureNameList.size(); textureIndex++)
                    {
                        if (modelDetailsList[index].blendTexture[textureIndex])
                            gameEngine.AddTexturesToTheMix(modelName, modelDetailsList[index].textureFilePathList[textureIndex],
                                modelDetailsList[index].textureNameList[textureIndex], modelDetailsList[index].textureRatiosList[textureIndex]);
                        else
                            gameEngine.AddTexturesToOverlap(modelName, modelDetailsList[index].textureFilePathList[textureIndex],
                                modelDetailsList[index].textureNameList[textureIndex]);

                        if (modelDetailsList[index].useDiscardMaskTexture[textureIndex])
                            gameEngine.AddDiscardMaskTexture(modelName, modelDetailsList[index].textureNameList[textureIndex],
                                modelDetailsList[index].discardMaskTextureFilePath[textureIndex]);
                    }
                }
            }

            //-----------------------------------Adding Lights--------------------------------------------------------------------------------

            for (int index = 0; index < lightDetailsList.size(); index++)
            {
                lightId = lightDetailsList[index].lightId;

                gameEngine.CreateLight(lightId, lightDetailsList[index].lightPosition.x + POSITION_OFFSET, lightDetailsList[index].lightPosition.y + POSITION_OFFSET, lightDetailsList[index].lightPosition.z + POSITION_OFFSET);
                gameEngine.ChangeLightType(lightId, lightDetailsList[index].lightType);
                gameEngine.ChangeLightIntensity(lightId, lightDetailsList[index].linearAttenuation, lightDetailsList[index].quadraticAttenuation);
                gameEngine.ChangeLightDirection(lightId, lightDetailsList[index].lightDirection.x, lightDetailsList[index].lightDirection.y, lightDetailsList[index].lightDirection.z);
                gameEngine.ChangeLightColour(lightId, lightDetailsList[index].lightColorRGB.r, lightDetailsList[index].lightColorRGB.g, lightDetailsList[index].lightColorRGB.b);
                gameEngine.ChangeLightAngle(lightId, lightDetailsList[index].innerAngle, lightDetailsList[index].outerAngle);

                if (lightDetailsList[index].lightOn)
                    gameEngine.TurnOffLight(lightId, false);
                else
                    gameEngine.TurnOffLight(lightId, true);
            }

            //--------------------------Loading Initial Camera Position-----------------------------------------------------------------------

            gameEngine.MoveCameraPosition(camDetails.initialCameraPosition.x + POSITION_OFFSET, camDetails.initialCameraPosition.y + POSITION_OFFSET, camDetails.initialCameraPosition.z + POSITION_OFFSET);
        }
    }
    else
        return -1;

    //-------------------------Setting Animation Sequences----------------------------------

    gameEngine.PlayAnimation("DancingVampire01", "mixamorig_Hips", 5.0f);
    gameEngine.PlayAnimation("DancingVampire", "Hips", 5.5f);
    gameEngine.PlayAnimation("DancingVampire01", "mixamorig_Hips", 6.0f);
    gameEngine.PlayAnimation("DancingVampire", "Hips", 6.0f);
    gameEngine.PlayAnimation("DancingVampire01", "mixamorig_Hips", 7.0f);
    gameEngine.PlayAnimation("DancingVampire", "Hips", 6.5f);
    gameEngine.PlayAnimation("DancingVampire01", "mixamorig_Hips", 8.0f);
    gameEngine.PlayAnimation("DancingVampire", "Hips", 7.0f);
    gameEngine.PlayAnimation("DancingVampire01", "mixamorig_Hips", 9.0f);
    gameEngine.PlayAnimation("DancingVampire", "Hips", 7.5f);

    //-------------------------------Frame loop---------------------------------------------

    const double LARGEST_DELTA_TIME = 1.0f / 30.0f;
    double lastTime = glfwGetTime();
    float ONE_SEC_COUNT = 0.0f;
    float FPS = 0.0f;

    float frameRate = 0.f;
    bool animationDone = false;

    while (!glfwWindowShouldClose(window))
    {
        //----------------------Frame Rate Count---------------------------------------------

        gameEngine.deltaTime = timer->getFrameTime();

        /*ONE_SEC_COUNT += gameEngine.deltaTime;

        if (ONE_SEC_COUNT >= 0.99f)
        {
            std::cout << "FPS : " << FPS << " | SECONDS_TAKEN : " << ONE_SEC_COUNT << std::endl;
            FPS = 1.0f;
            ONE_SEC_COUNT = 0.0f;
        }
        else
            FPS++;*/

        if(!animationDone)
            frameRate += 7.5f;

        if (frameRate >= 3433.f)
        {
            frameRate = 1000.f;
            animationDone = true;
        }

        //------------------Run Animations--------------------------------------------------

        gameEngine.UpdateAllAnimations();

        //--------------------Run Engine----------------------------------------------------

        gameEngine.RunGameEngine(window);
    }

    glfwDestroyWindow(window);

    glfwTerminate();

    exit(EXIT_SUCCESS);

    std::cout << "Window closed !" << std::endl;

    return 0;
}