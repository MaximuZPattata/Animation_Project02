#include "cAnimationSystem.h"
#include <glm/gtx/easing.hpp>

cAnimationSystem::cAnimationSystem()
{
	if(this->mAnimationCommandsManager == NULL)
		this->mAnimationCommandsManager = new cAnimationCommands();
}

cAnimationSystem::~cAnimationSystem()
{
	delete this->mAnimationCommandsManager;
}

float cAnimationSystem::FindValueAfterEasingEffect(double startKeyFrameTime, double endKeyFrameTime, EasingType easeType, float dt)
{
	float percent = 0.f;

	percent = (dt - startKeyFrameTime) / (endKeyFrameTime - startKeyFrameTime);

	float result = 0.f;

	switch (easeType)
	{
	case EasingType::Linear:
		result = percent;
		break;

	case EasingType::EaseIn:
		result = glm::sineEaseIn(percent);
		break;

	case EasingType::EaseOut:
		result = glm::sineEaseOut(percent);
		break;

	case EasingType::EaseInOut:
		result = glm::sineEaseInOut(percent);
		break;
	}

	return result;
}

glm::mat4 cAnimationSystem::AnimationTransformations(NodeAnimation* nodeAnim, float deltaTime)
{
	glm::vec3 modelPosition = glm::vec3(0.f);
	glm::quat modelRotation = glm::quat(0.f, 0.f, 0.f, 1.f);
	glm::vec3 modelScale = glm::vec3(0.f);

	//------------------------------------------------------------------------------------
	//----------------------HANDLING POSITION KEYFRAMES-----------------------------------
	//------------------------------------------------------------------------------------

	if (nodeAnim->PositionKeys.size() == 1)
		modelPosition = nodeAnim->PositionKeys[0].mPosition;
	else
	{
		int positionKeyFrameEndIndex = 1;

		for (; positionKeyFrameEndIndex < nodeAnim->PositionKeys.size(); positionKeyFrameEndIndex++)
		{
			if (nodeAnim->PositionKeys[positionKeyFrameEndIndex].mTime > deltaTime)
				break;
		}

		if (positionKeyFrameEndIndex >= nodeAnim->PositionKeys.size())
			modelPosition = nodeAnim->PositionKeys[positionKeyFrameEndIndex - 1].mPosition;
		else
		{
			int positionKeyFrameStartIndex = positionKeyFrameEndIndex - 1;

			sPositionKeyFrame positionStartKeyFrame = nodeAnim->PositionKeys[positionKeyFrameStartIndex];
			sPositionKeyFrame positionEndKeyFrame = nodeAnim->PositionKeys[positionKeyFrameEndIndex];

			float result = FindValueAfterEasingEffect(positionStartKeyFrame.mTime, positionEndKeyFrame.mTime, positionEndKeyFrame.mEaseType, deltaTime);

			glm::vec3 delta = positionEndKeyFrame.mPosition - positionStartKeyFrame.mPosition;

			modelPosition = positionStartKeyFrame.mPosition + delta * result;
		}
	}

	//------------------------------------------------------------------------------------
	//----------------------HANDLING ROTATION KEYFRAMES-----------------------------------
	//------------------------------------------------------------------------------------

	if (nodeAnim->RotationKeys.size() == 1)
		modelRotation = glm::quat(nodeAnim->RotationKeys[0].mRotation);
	else
	{
		int rotationKeyFrameEndIndex = 1;

		for (; rotationKeyFrameEndIndex < nodeAnim->RotationKeys.size(); rotationKeyFrameEndIndex++)
		{
			if (nodeAnim->RotationKeys[rotationKeyFrameEndIndex].mTime > deltaTime)
				break;
		}

		if (rotationKeyFrameEndIndex >= nodeAnim->RotationKeys.size())
			modelRotation = glm::quat(nodeAnim->RotationKeys[rotationKeyFrameEndIndex - 1].mRotation);
		else
		{
			int rotationKeyFrameStartIndex = rotationKeyFrameEndIndex - 1;

			sRotationKeyFrame rotationStartKeyFrame = nodeAnim->RotationKeys[rotationKeyFrameStartIndex];
			sRotationKeyFrame rotationEndKeyFrame = nodeAnim->RotationKeys[rotationKeyFrameEndIndex];

			float result = FindValueAfterEasingEffect(rotationStartKeyFrame.mTime, rotationEndKeyFrame.mTime, rotationEndKeyFrame.mEaseType, deltaTime);

			glm::quat startQuat = glm::quat(rotationStartKeyFrame.mRotation);
			glm::quat endQuat = glm::quat(rotationEndKeyFrame.mRotation);
			glm::quat offset = glm::slerp(startQuat, endQuat, result);

			modelRotation = glm::quat(offset);
		}
	}
	//------------------------------------------------------------------------------------
	//----------------------HANDLING SCALE KEYFRAMES--------------------------------------
	//------------------------------------------------------------------------------------

	if (nodeAnim->ScalingKeys.size() == 1)
		modelScale = nodeAnim->ScalingKeys[0].mScale;
	else
	{
		int scaleKeyFrameEndIndex = 1;

		for (; scaleKeyFrameEndIndex < nodeAnim->ScalingKeys.size(); scaleKeyFrameEndIndex++)
		{
			if (nodeAnim->ScalingKeys[scaleKeyFrameEndIndex].mTime > deltaTime)
				break;
		}

		if (scaleKeyFrameEndIndex >= nodeAnim->ScalingKeys.size())
			modelScale = nodeAnim->ScalingKeys[scaleKeyFrameEndIndex - 1].mScale;
		else
		{
			int scaleKeyFrameStartIndex = scaleKeyFrameEndIndex - 1;

			sScaleKeyFrame scaleStartKeyFrame = nodeAnim->ScalingKeys[scaleKeyFrameStartIndex];
			sScaleKeyFrame scaleEndKeyFrame = nodeAnim->ScalingKeys[scaleKeyFrameEndIndex];

			float result = FindValueAfterEasingEffect(scaleStartKeyFrame.mTime, scaleEndKeyFrame.mTime, scaleEndKeyFrame.mEaseType, deltaTime);

			glm::vec3 delta = scaleEndKeyFrame.mScale - scaleStartKeyFrame.mScale;

			modelScale = scaleStartKeyFrame.mScale + delta * result;
		}
	}
	//------------------------------------------------------------------------------------
	//-------------------------MATRIX TRANSFORMATION--------------------------------------
	//------------------------------------------------------------------------------------

	glm::mat4 matModel = glm::mat4(1.0f);

	glm::mat4 translate = glm::translate(glm::mat4(1.0f), modelPosition);
	glm::mat4 rotate = glm::mat4(modelRotation);
	glm::mat4 scale = glm::scale(glm::mat4(1.0f), modelScale);

	matModel = matModel * translate;
	matModel = matModel * rotate;
	matModel = matModel * scale;

	/*printf("\n\n------------------START----------------------------");
	
	for (int i = 0; i < nodeAnim->PositionKeys.size(); i++)
		printf("\n InitialPos[%d] :  (%f, %f, %f)\n", i, nodeAnim->PositionKeys[i].mPosition.x, nodeAnim->PositionKeys[i].mPosition.y, nodeAnim->PositionKeys[i].mPosition.z);

	printf("\n ModelPos : (%f, %f, %f)\n", modelPosition.x, modelPosition.y, modelPosition.z);
	printf("\n ModelRot : (%f, %f, %f)\n", modelRotation.x, modelRotation.y, modelRotation.z);
	printf("\n ModelScal : (%f, %f, %f)\n", modelScale.x, modelScale.y, modelScale.z);

	for (int count = 0; count < matModel.length(); count++)
		printf("\n Matrix[%d] : (%f, %f, %f)", count, matModel[count].x, matModel[count].y, matModel[count].z);

	printf("\n-------------------END----------------------------\n\n");*/

	return matModel;
}

//void cAnimationSystem::ReverseAnimation(cControlGameEngine gameEngine)
//{
//	glm::vec3 modelPosition = glm::vec3(0.f);
//	glm::quat modelRotation = glm::quat(0.f, 0.f, 0.f, 1.f);
//	float modelScale = 1.f;
//
//	int currentPositionKeyFrameIndex = 0;
//
//	int positionFramesCompletionCount = 0;
//	int rotationFramesCompletionCount = 0;
//	int scaleFramesCompletionCount = 0;
//
//	bool keyFrameChange = false;
//
//	std::string modelName = "";
//
//	for (int modelIterator = 0; modelIterator < mAnimationList.size(); modelIterator++)
//	{
//		modelName = mAnimationList[modelIterator]->modelName;
//
//		//------------------------------------------------------------------------------------
//		//----------------------HANDLING POSITION KEYFRAMES-----------------------------------
//		//------------------------------------------------------------------------------------
//
//		if (mAnimationList[modelIterator]->mPositionKeyFrames.size() > 0)
//		{
//			//--------------------If only one KeyFrame is set----------------------------------------
//
//			if (mAnimationList[modelIterator]->mPositionKeyFrames.size() == 1)
//				modelPosition = mAnimationList[modelIterator]->mPositionKeyFrames[0].mPosition;
//
//			else
//			{
//				//---------------Initializing values for multiple KeyFrames set-----------------------------
//
//				int KeyFrameEndIndex = mAnimationList[modelIterator]->mPositionKeyFrames.size() - 1;
//
//				// Waiting for the animation time to match the last keyframe time
//				if (mOverallAnimationtime < mAnimationList[modelIterator]->mPositionKeyFrames[KeyFrameEndIndex].mTime)
//				{
//					for (; KeyFrameEndIndex >= 0; KeyFrameEndIndex--)
//					{
//						if (mAnimationList[modelIterator]->mPositionKeyFrames[KeyFrameEndIndex].mTime < mOverallAnimationtime)
//						{
//							if (mAnimationList[modelIterator]->mCurrentPositionKeyFrame != KeyFrameEndIndex)
//							{
//								mAnimationList[modelIterator]->mCurrentPositionKeyFrame = KeyFrameEndIndex;
//								keyFrameChange = true;
//							}
//
//							break;
//						}
//					}
//
//					//--------------If KeyFrame index has reached the last KeyFrame Position-------------------
//
//					if (KeyFrameEndIndex < 0)
//					{
//						modelPosition = mAnimationList[modelIterator]->mPositionKeyFrames[KeyFrameEndIndex + 1].mPosition;
//
//						positionFramesCompletionCount += 1;
//
//						// Updating position values to the model
//						gameEngine.MoveModel(modelName, modelPosition.x, modelPosition.y, modelPosition.z);
//					}
//
//					else
//					{
//						int KeyFrameStartIndex = KeyFrameEndIndex + 1;
//						currentPositionKeyFrameIndex = KeyFrameStartIndex;
//
//						//----------------Initializing start and end KeyFrame objects--------------------------
//
//						sPositionKeyFrame startKeyFrame = mAnimationList[modelIterator]->mPositionKeyFrames[KeyFrameStartIndex];
//						sPositionKeyFrame endKeyFrame = mAnimationList[modelIterator]->mPositionKeyFrames[KeyFrameEndIndex];
//
//						//-------------Calculating position value based on Easing effect-----------------------
//
//						float result = FindValueAfterEasingEffect(startKeyFrame.mTime, endKeyFrame.mTime, startKeyFrame.mEaseType);
//
//						glm::vec3 deltaPos = startKeyFrame.mPosition - endKeyFrame.mPosition;
//
//						modelPosition = startKeyFrame.mPosition - deltaPos * result;
//
//						//------------------Updating position values to the model-----------------------------
//
//						gameEngine.MoveModel(modelName, modelPosition.x, modelPosition.y, modelPosition.z);
//					}
//				}
//			}
//		}
//
//		else
//			positionFramesCompletionCount += 1;
//
//		if(!mReversePositionAnimationComplete && positionFramesCompletionCount == mAnimationList.size())
//			mReversePositionAnimationComplete = true;
//
//		//------------------------------------------------------------------------------------
//		//----------------------HANDLING ROTATION KEYFRAMES-----------------------------------
//		//------------------------------------------------------------------------------------
//
//		if (mAnimationList[modelIterator]->mRotationKeyFrames.size() > 0)
//		{
//			//--------------------If only one KeyFrame is set------------------------------------
//
//			if (mAnimationList[modelIterator]->mRotationKeyFrames.size() == 1)
//				modelRotation = mAnimationList[modelIterator]->mRotationKeyFrames[0].mRotation;
//
//			else
//			{
//				//-------------Initializing values for multiple KeyFrames set------------------------
//
//				int KeyFrameEndIndex = mAnimationList[modelIterator]->mRotationKeyFrames.size() - 1;
//
//				// Waiting for the animation time to match the last keyframe time
//				if (mOverallAnimationtime < mAnimationList[modelIterator]->mRotationKeyFrames[KeyFrameEndIndex].mTime)
//				{
//
//					for (; KeyFrameEndIndex >= 0; KeyFrameEndIndex--)
//					{
//						if (mAnimationList[modelIterator]->mRotationKeyFrames[KeyFrameEndIndex].mTime < mOverallAnimationtime)
//							break;
//					}
//
//					//--------------If KeyFrame index has reached the last KeyFrame Position-------------------
//
//					if (KeyFrameEndIndex < 0)
//					{
//						modelRotation = mAnimationList[modelIterator]->mRotationKeyFrames[KeyFrameEndIndex + 1].mRotation;
//
//						rotationFramesCompletionCount += 1;
//
//						// Updating position values to the model
//						gameEngine.RotateMeshModelUsingQuaternion(modelName, modelRotation);
//
//					}
//
//					else
//					{
//						int KeyFrameStartIndex = KeyFrameEndIndex + 1;
//
//						//----------------Initializing start and end KeyFrame objects--------------------------
//
//						sRotationKeyFrame startKeyFrame = mAnimationList[modelIterator]->mRotationKeyFrames[KeyFrameStartIndex];
//						sRotationKeyFrame endKeyFrame = mAnimationList[modelIterator]->mRotationKeyFrames[KeyFrameEndIndex];
//
//						//-------------Calculating rotation value based on Easing effect-----------------------
//
//						float result = FindValueAfterEasingEffect(startKeyFrame.mTime, endKeyFrame.mTime, startKeyFrame.mEaseType);
//
//						float startKeyFrameRadianVal = glm::radians(startKeyFrame.mAngleDegrees);
//						float endKeyFrameRadianVal = glm::radians(endKeyFrame.mAngleDegrees);
//
//						glm::quat startKeyFrameRotationQuaternion = glm::angleAxis(startKeyFrameRadianVal, glm::vec3(startKeyFrame.mRotation.x,
//							startKeyFrame.mRotation.y, startKeyFrame.mRotation.z));
//						glm::quat endKeyFrameRotationQuaternion = glm::angleAxis(endKeyFrameRadianVal, glm::vec3(endKeyFrame.mRotation.x,
//							endKeyFrame.mRotation.y, endKeyFrame.mRotation.z));
//
//						modelRotation = glm::slerp(startKeyFrameRotationQuaternion, endKeyFrameRotationQuaternion, result);
//
//						//------------------Updating rotation values to the model-------------------------------
//
//						gameEngine.RotateMeshModelUsingQuaternion(modelName, modelRotation);
//					}
//				}
//			}
//		}
//
//		else
//			rotationFramesCompletionCount += 1;
//
//		if (!mReverseRotationAnimationComplete && rotationFramesCompletionCount == mAnimationList.size())
//			mReverseRotationAnimationComplete = true;
//
//		//------------------------------------------------------------------------------------
//		//----------------------HANDLING SCALE KEYFRAMES--------------------------------------
//		//------------------------------------------------------------------------------------
//
//		if (mAnimationList[modelIterator]->mScaleKeyFrames.size() > 0)
//		{
//			//--------------------If only one KeyFrame is set----------------------------------------
//
//			if (mAnimationList[modelIterator]->mScaleKeyFrames.size() == 1)
//				modelScale = mAnimationList[modelIterator]->mScaleKeyFrames[0].mScale;
//
//			else
//			{
//				//----------Initializing values for multiple KeyFrames set----------------------------
//
//				int KeyFrameEndIndex = mAnimationList[modelIterator]->mScaleKeyFrames.size() - 1;
//
//				// Waiting for the animation time to match the last keyframe time
//				if (mOverallAnimationtime < mAnimationList[modelIterator]->mScaleKeyFrames[KeyFrameEndIndex].mTime)
//				{
//					for (; KeyFrameEndIndex >= 0; KeyFrameEndIndex--)
//					{
//						if (mAnimationList[modelIterator]->mScaleKeyFrames[KeyFrameEndIndex].mTime < mOverallAnimationtime)
//							break;
//					}
//
//					//--------If KeyFrame index has reached the last KeyFrame Position--------------------
//
//					if (KeyFrameEndIndex < 0)
//					{
//						modelScale = mAnimationList[modelIterator]->mScaleKeyFrames[KeyFrameEndIndex + 1].mScale;
//
//						scaleFramesCompletionCount += 1;
//
//						// Updating scale value to the model
//						gameEngine.ScaleModel(modelName, modelScale);
//					}
//
//					else
//					{
//						int KeyFrameStartIndex = KeyFrameEndIndex + 1;
//
//						//----------------Initializing start and end KeyFrame objects--------------------------
//
//						sScaleKeyFrame startKeyFrame = mAnimationList[modelIterator]->mScaleKeyFrames[KeyFrameStartIndex];
//						sScaleKeyFrame endKeyFrame = mAnimationList[modelIterator]->mScaleKeyFrames[KeyFrameEndIndex];
//
//						//----------------Calculating scale value based on Easing effect-----------------------
//
//						float result = FindValueAfterEasingEffect(startKeyFrame.mTime, endKeyFrame.mTime, startKeyFrame.mEaseType);
//
//						float deltaScale = startKeyFrame.mScale - endKeyFrame.mScale;
//
//						modelScale = startKeyFrame.mScale - deltaScale * result;
//
//						//----------------------Updating scale value to the model------------------------------
//
//						gameEngine.ScaleModel(modelName, modelScale);
//					}
//				}
//			}
//	
//		}
//
//		else
//			scaleFramesCompletionCount += 1;
//
//		if (!mReverseScaleAnimationComplete && scaleFramesCompletionCount == mAnimationList.size())
//			mReverseScaleAnimationComplete = true;
//
//		//------------------------------------------------------------------------------------
//		//----------------------HANDLING KEYFRAME EVENTS--------------------------------------
//		//------------------------------------------------------------------------------------
//
//		if (keyFrameChange)
//		{
//			for (int eventIterator = 0; eventIterator < mKeyFrameEventList.size(); eventIterator++)
//			{
//				if (mKeyFrameEventList[eventIterator]->mEventTriggered)
//				{
//					if (currentPositionKeyFrameIndex >= 0 && mAnimationList[modelIterator]->mPositionKeyFrames.size() >=currentPositionKeyFrameIndex)
//						if (mKeyFrameEventList[eventIterator]->checkForEventTrigger(modelName, mOverallAnimationtime,
//							mAnimationList[modelIterator]->mPositionKeyFrames[currentPositionKeyFrameIndex], true, gameEngine))
//							break;
//				}
//			}
//		}
//	}
//}

void cAnimationSystem::RunBoneAnimation(cMesh* animMesh, AnimationInfo& animationDetails, Node& nodeAnim, float deltaTime)
{
	std::map<std::string, NodeAnimation*>::iterator animIt = animationDetails.NodeAnimations.find(nodeAnim.Name);

	if (animIt != animationDetails.NodeAnimations.end())
	{
		NodeAnimation* nodeAnimInfo = animIt->second;

		glm::mat4 boneTransform = AnimationTransformations(nodeAnimInfo, deltaTime);

		animMesh->mBoneTransformationsMap[nodeAnim.Name] = boneTransform;
	}

	for (Node* child : nodeAnim.Children)
		RunBoneAnimation(animMesh, animationDetails, *child, deltaTime);
}


bool cAnimationSystem::AddAnimationCommand(cMesh* animMesh, sModelDrawInfo* animModel, std::string animationName, float frameRateIncrement)
{
	for (AnimationInfo animationInfo : animModel->AnimationInfoList)
	{
		if (animationInfo.AnimationName == animationName)
		{
			if (this->mAnimationCommandsManager == NULL)
				this->mAnimationCommandsManager = new cAnimationCommands();

			this->mAnimationCommandsManager->AddAnimation(animMesh->friendlyName, animationName, frameRateIncrement, animationInfo.Duration, animMesh, animModel, animationInfo);
			
			return true;
		}
	}

	return false;
}

void cAnimationSystem::UpdateAnimations()
{
	if (this->mAnimationCommandsManager != NULL)
	{
		std::vector < sAnimationCommandInfoInQueue* > deleteAnimationCommandsList;

		for (sAnimationCommandInfoInQueue* animationCommands : this->mAnimationCommandsManager->animationInfoInQueueList)
		{
			if (!animationCommands->bAnimationCompleted)
			{
				animationCommands->mAnimationTime += animationCommands->mIncrementFrameRate;

				this->RunBoneAnimation(animationCommands->mMeshModel, animationCommands->mAnimationToBePlayed, *animationCommands->mModelInfo->RootNode,
					animationCommands->mAnimationTime);

				if (animationCommands->mAnimationTime >= animationCommands->mTotalAnimationDuration)
					animationCommands->bAnimationCompleted = true;

				break;
			}
			else
				deleteAnimationCommandsList.push_back(animationCommands);
		}

		for (sAnimationCommandInfoInQueue* animationCommand : deleteAnimationCommandsList)
			this->mAnimationCommandsManager->DeleteAnimation(animationCommand);
	}
}

//bool cAnimationSystem::isAnimationPaused()
//{
//	return mPauseAnimation;
//}
//
//bool cAnimationSystem::isAnimationReversed()
//{
//	return mReverseAnimation;
//}
//
//void cAnimationSystem::PauseAnimation()
//{
//	mPauseAnimation = true;
//}
//
//void cAnimationSystem::PlayAnimation()
//{
//	mPauseAnimation = false;
//}
//
//void cAnimationSystem::ReverseAnimationFlow()
//{
//	if (!mReverseAnimation)
//	{
//		mAnimationSpeed = 1.f;
//		mReverseAnimation = true;
//	}
//}
//
//void cAnimationSystem::NormalAnimationFlow()
//{
//	if (mReverseAnimation)
//	{
//		mAnimationSpeed = 1.f;
//		mReverseAnimation = false;
//	}
//}
//
//void cAnimationSystem::ChangeAnimationSpeed(float speed)
//{
//	mAnimationSpeed = speed;
//}
//
//bool cAnimationSystem::CheckForwardAnimationCompletion()
//{
//	return (mPositionAnimationComplete && mRotationAnimationComplete && mScaleAnimationComplete);
//}
//
//bool cAnimationSystem::CheckReverseAnimationCompletion()
//{
//	return (mReversePositionAnimationComplete && mReverseRotationAnimationComplete && mReverseScaleAnimationComplete);
//}
//
//void cAnimationSystem::ResetNormalAnimationFlags()
//{
//	mPositionAnimationComplete = false;
//	mRotationAnimationComplete = false;
//	mScaleAnimationComplete = false;
//}
//
//void cAnimationSystem::ResetReverseAnimationFlags()
//{
//	mReversePositionAnimationComplete = false;
//	mReverseRotationAnimationComplete = false;
//	mReverseScaleAnimationComplete = false;
//}

//void cAnimationSystem::ResetAnimationTime()
//{
//	mOverallAnimationtime = 0.0f;
//}