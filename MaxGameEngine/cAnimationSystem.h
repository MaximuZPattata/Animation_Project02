#pragma once
#include <string>
#include <iostream>
#include "cMesh.h"
#include "sModelDrawInfo.h"
#include "cAnimationCommands.h"

//struct sAnimation
//{
//	sAnimation(sModelDrawInfo* meshModelInfo) : modelInfo(meshModelInfo), mOverallAnimationTime(modelInfo->AnimationInfoList[0].Duration)
//	{ }
//
//	sModelDrawInfo* modelInfo;
//
//	std::vector<sPositionKeyFrame> mPositionKeyFrames;
//	std::vector<sScaleKeyFrame> mScaleKeyFrames;
//	std::vector<sRotationKeyFrame> mRotationKeyFrames;
//
//	int mCurrentPositionKeyFrame = 0;
//	double mTime = 0.f;
//	float mOverallAnimationTime = 0.f;
//
//	bool IsDone()
//	{
//		if (mTime >= mPositionKeyFrames[mPositionKeyFrames.size() - 1].mTime && mTime >= mRotationKeyFrames[mRotationKeyFrames.size() - 1].mTime
//				&& mTime >= mScaleKeyFrames[mScaleKeyFrames.size() - 1].mTime)
//			return true;
//
//		return false;
//	}
//};

class cAnimationSystem
{
public:
	cAnimationSystem();
	~cAnimationSystem();

	//std::vector<sAnimation*> mAnimationList;
	//std::vector<sKeyFrameEvent*> mKeyFrameEventList;

	//bool isAnimationPaused();
	//bool isAnimationReversed();
	//bool CheckForwardAnimationCompletion();
	//bool CheckReverseAnimationCompletion();
	//void PauseAnimation();
	//void PlayAnimation();
	//void ReverseAnimationFlow();
	//void NormalAnimationFlow();
	//void ChangeAnimationSpeed(float speed);
	//void ResetNormalAnimationFlags();
	//void ResetReverseAnimationFlags();
	//void ResetAnimationTime();
	void RunBoneAnimation(cMesh* animMesh, AnimationInfo& animationDetails, Node& nodeAnim, float deltaTime);
	void UpdateAnimations();
	bool AddAnimationCommand(cMesh* animMesh, sModelDrawInfo* animModel, std::string animationName, float frameRateIncrement);

private:
	/*float mAnimationSpeed = 1.f;
	bool mPauseAnimation = false;
	bool mReverseAnimation = false;
	bool mPositionAnimationComplete = false;
	bool mRotationAnimationComplete = false;
	bool mScaleAnimationComplete = false;
	bool mReversePositionAnimationComplete = false;
	bool mReverseRotationAnimationComplete = false;
	bool mReverseScaleAnimationComplete = false;*/

	cAnimationCommands* mAnimationCommandsManager = NULL;

	glm::mat4 AnimationTransformations(NodeAnimation* nodeAnim, float dt);
	float FindValueAfterEasingEffect(double startKeyFrameTime, double endKeyFrameTime, EasingType easeType, float dt);
};
