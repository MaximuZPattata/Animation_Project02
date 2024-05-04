#include "cAnimationCommands.h"

void cAnimationCommands::AddAnimation(std::string meshName, std::string animName, float frameRateIncrement, float animationDuration, 
	cMesh* meshModel, sModelDrawInfo* modelInfo, AnimationInfo& animationToBePlayed)
{
	sAnimationCommandInfoInQueue* newAnimationInfo = new sAnimationCommandInfoInQueue();
	
	newAnimationInfo->mAnimationName = animName;
	newAnimationInfo->mModelName = meshName;
	newAnimationInfo->mIncrementFrameRate = frameRateIncrement;
	newAnimationInfo->mTotalAnimationDuration = animationDuration;
	newAnimationInfo->mMeshModel = meshModel;
	newAnimationInfo->mModelInfo = modelInfo;
	newAnimationInfo->mAnimationToBePlayed = animationToBePlayed;

	this->animationInfoInQueueList.push_back(newAnimationInfo);
}

void cAnimationCommands::DeleteAnimation(sAnimationCommandInfoInQueue* animationInfoInQueue)
{
	this->animationInfoInQueueList.erase(std::remove(this->animationInfoInQueueList.begin(), this->animationInfoInQueueList.end(), animationInfoInQueue), this->animationInfoInQueueList.end());

	delete animationInfoInQueue;
}