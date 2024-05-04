#pragma once

#include <string>
#include <vector>

#include "sModelDrawInfo.h"
#include "cMesh.h"

struct sAnimationCommandInfoInQueue
{
	std::string mModelName;
	std::string mAnimationName;
	
	float mTotalAnimationDuration = 0.f;
	float mIncrementFrameRate = 0.f;
	float mAnimationTime = 0.f;

	bool bAnimationCompleted = false;

	cMesh* mMeshModel;
	sModelDrawInfo* mModelInfo;
	AnimationInfo mAnimationToBePlayed;
};

class cAnimationCommands
{
public:
	std::vector <sAnimationCommandInfoInQueue*> animationInfoInQueueList;

	void AddAnimation(std::string meshName, std::string animationName, float frameRateIncrement, float animationDuration, 
		cMesh* meshModel, sModelDrawInfo* modelInfo, AnimationInfo& animationToBePlayed);
	void DeleteAnimation(sAnimationCommandInfoInQueue* animationCommandInfoInQueue);
};

