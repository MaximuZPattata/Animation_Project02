#include "pch.h"
//#include "cAnimationSystem.h"
//
//#include <cControlGameEngine.h>
//
//extern cControlGameEngine gameEngine;
//
//void ResetScene01Models()
//{
//	std::string modelNameForNow;
//
//	modelNameForNow = "Beholder01";
//	gameEngine.RotateMeshModel(modelNameForNow, 0.f, 0.f, 0.f, 0.f);
//
//	modelNameForNow = "Beholder02";
//	gameEngine.MoveModel(modelNameForNow, 80.0, -20.0, 0.0);
//
//	modelNameForNow = "Beholder03";
//	gameEngine.MoveModel(modelNameForNow, 100.0, -20.0, 0.0);
//
//	modelNameForNow = "Beholder04";
//	gameEngine.MoveModel(modelNameForNow, -100.0, -20.0, 0.0);
//
//	modelNameForNow = "Beholder05";
//	gameEngine.MoveModel(modelNameForNow, 90.0, -20.0, 0.0);
//
//	modelNameForNow = "Falcon";
//	gameEngine.RotateMeshModel(modelNameForNow, 0.f, 0.f, 0.f, 0.f);
//}
//
//void ResetScene02Models()
//{
//	std::string modelNameForNow = "Falcon";
//	if (!gameEngine.IsVisibilityOn(modelNameForNow))
//		gameEngine.TurnVisibilityOn(modelNameForNow);
//
//	modelNameForNow = "Beholder01";
//	if (!gameEngine.IsVisibilityOn(modelNameForNow))
//		gameEngine.TurnVisibilityOn(modelNameForNow);
//}
//
//void Scene01Sequence(cAnimationSystem& animationManager)
//{
//	//---------------------Resetting Animation Sequences-------------------------------------
//
//	animationManager.mAnimationList.clear();
//	animationManager.ResetAnimationTime();
//	if (animationManager.isAnimationReversed())
//		animationManager.NormalAnimationFlow();
//	ResetScene02Models();
//
//	//------------------------Beholder01 Animations------------------------------------------
//	
//	sAnimation* beholder01Animation = new sAnimation();
//	std::string modelNameForNow = "Beholder01";
//
//	beholder01Animation->modelName = modelNameForNow;
//	beholder01Animation->mTime = 0.0;
//	
//	beholder01Animation->mPositionKeyFrames.push_back(sPositionKeyFrame(glm::vec3(50, 0, 0), 0.0));
//	beholder01Animation->mPositionKeyFrames.push_back(sPositionKeyFrame(glm::vec3(-50, 0, 0), 12.0, EasingType::Linear));
//
//	beholder01Animation->mRotationKeyFrames.push_back(sRotationKeyFrame(0.0, glm::vec3(0.f, 0.f, 0.0f), 0.0));
//	beholder01Animation->mRotationKeyFrames.push_back(sRotationKeyFrame(90.0, glm::vec3(0.f, 1.f, 0.f), 2.0));
//	beholder01Animation->mRotationKeyFrames.push_back(sRotationKeyFrame(180.0, glm::vec3(0.f, 1.f, 0.f), 4.0));
//	beholder01Animation->mRotationKeyFrames.push_back(sRotationKeyFrame(270.0, glm::vec3(0.f, 1.f, 0.f), 6.0));
//	beholder01Animation->mRotationKeyFrames.push_back(sRotationKeyFrame(0.0, glm::vec3(0.f, 1.f, 0.f), 8.0));
//	beholder01Animation->mRotationKeyFrames.push_back(sRotationKeyFrame(90.0, glm::vec3(0.f, 1.f, 0.f), 10.0));
//	beholder01Animation->mRotationKeyFrames.push_back(sRotationKeyFrame(180.0, glm::vec3(0.f, 1.f, 0.f), 12.0));
//
//	//---------------------------Falcon Animations-------------------------------------------
//
//	sAnimation* falconAnimations = new sAnimation();
//	modelNameForNow = "Falcon";
//
//	falconAnimations->modelName = modelNameForNow;
//	falconAnimations->mTime = 0.0;
//
//	falconAnimations->mPositionKeyFrames.push_back(sPositionKeyFrame(glm::vec3(-50, 0, 0), 0.0));
//	falconAnimations->mPositionKeyFrames.push_back(sPositionKeyFrame(glm::vec3(-20, 0, 0), 1.5, EasingType::Linear));
//	falconAnimations->mPositionKeyFrames.push_back(sPositionKeyFrame(glm::vec3(0, 20, 0), 3.0, EasingType::EaseInOut));
//	falconAnimations->mPositionKeyFrames.push_back(sPositionKeyFrame(glm::vec3(25, 5, 0), 6.0, EasingType::EaseIn));
//	falconAnimations->mPositionKeyFrames.push_back(sPositionKeyFrame(glm::vec3(60, 0, 0), 10.0, EasingType::EaseOut));
//
//	falconAnimations->mRotationKeyFrames.push_back(sRotationKeyFrame(0.0, glm::vec3(0.f, 0.f, 0.0f), 0.0));
//	falconAnimations->mRotationKeyFrames.push_back(sRotationKeyFrame(0.0, glm::vec3(0.f, 0.f, 0.0f), 2.0));
//	falconAnimations->mRotationKeyFrames.push_back(sRotationKeyFrame(-60.0, glm::vec3(1.f, 0.f, 0.f), 3.0, EasingType::Linear));
//	falconAnimations->mRotationKeyFrames.push_back(sRotationKeyFrame(0.0, glm::vec3(0.f, 0.f, 0.f), 10.0, EasingType::Linear));
//	 
//	sKeyFrameEvent* easeEvent01 = new sKeyFrameEvent(false, modelNameForNow);
//	easeEvent01->mEaseType = EasingType::Linear;
//
//	sKeyFrameEvent* easeEvent02 = new sKeyFrameEvent(false, modelNameForNow);
//	easeEvent02->mEaseType = EasingType::EaseOut;
//
//	sKeyFrameEvent* easeEvent03 = new sKeyFrameEvent(false, modelNameForNow);
//	easeEvent03->mEaseType = EasingType::EaseInOut;
//
//	sKeyFrameEvent* easeEvent04 = new sKeyFrameEvent(false, modelNameForNow);
//	easeEvent04->mEaseType = EasingType::EaseIn;
//
//	//------------------------Beholder02 Animations------------------------------------------- 
//
//	sAnimation* beholder02Animation = new sAnimation();
//	modelNameForNow = "Beholder02";
//
//	beholder02Animation->modelName = modelNameForNow;
//	beholder02Animation->mTime = 0.0;
//	
//	beholder02Animation->mPositionKeyFrames.push_back(sPositionKeyFrame(glm::vec3(80, 2, 85), 0.0));
//	beholder02Animation->mPositionKeyFrames.push_back(sPositionKeyFrame(glm::vec3(-80, 2, 85), 18.0, EasingType::Linear));
//	
//	beholder02Animation->mRotationKeyFrames.push_back(sRotationKeyFrame(0.0, glm::vec3(0.f, 0.f, 0.0f), 0.0));
//	beholder02Animation->mRotationKeyFrames.push_back(sRotationKeyFrame(-90.0, glm::vec3(0.f, 1.f, 0.f), 3.0));
//	beholder02Animation->mRotationKeyFrames.push_back(sRotationKeyFrame(-180.0, glm::vec3(0.f, 1.f, 0.f), 6.0));
//	beholder02Animation->mRotationKeyFrames.push_back(sRotationKeyFrame(-270.0, glm::vec3(0.f, 1.f, 0.f), 9.0));
//	beholder02Animation->mRotationKeyFrames.push_back(sRotationKeyFrame(0.0, glm::vec3(0.f, 1.f, 0.f), 12.0));
//	beholder02Animation->mRotationKeyFrames.push_back(sRotationKeyFrame(-90.0, glm::vec3(0.f, 1.f, 0.f), 15.0));
//
//	//-----------------------Other Beholder Animations----------------------------------------
//
//	sAnimation* beholder03Animation = new sAnimation();
//	modelNameForNow = "Beholder03";
//
//	beholder03Animation->modelName = modelNameForNow;
//	beholder03Animation->mTime = 0.0;
//
//	beholder03Animation->mPositionKeyFrames.push_back(sPositionKeyFrame(glm::vec3(50, 10, -20), 0.0));
//	beholder03Animation->mPositionKeyFrames.push_back(sPositionKeyFrame(glm::vec3(-80, 10, -20), 5.0, EasingType::Linear));
//
//	beholder03Animation->mRotationKeyFrames.push_back(sRotationKeyFrame(0.0, glm::vec3(0.f, 0.f, 0.0f), 0.0));
//	beholder03Animation->mRotationKeyFrames.push_back(sRotationKeyFrame(-90.0, glm::vec3(1.f, 0.f, 0.f), 2.5));
//	beholder03Animation->mRotationKeyFrames.push_back(sRotationKeyFrame(-180.0, glm::vec3(1.f, 0.f, 0.f), 2.5));
//
//	sAnimation* beholder04Animation = new sAnimation();
//	modelNameForNow = "Beholder04";
//
//	beholder04Animation->modelName = modelNameForNow;
//	beholder04Animation->mTime = 0.0;
//
//	beholder04Animation->mPositionKeyFrames.push_back(sPositionKeyFrame(glm::vec3(40, -30, -40), 0.0));
//	beholder04Animation->mPositionKeyFrames.push_back(sPositionKeyFrame(glm::vec3(-100, -30, -40), 8.0, EasingType::Linear));
//
//	beholder04Animation->mScaleKeyFrames.push_back(sScaleKeyFrame(1.5, 0.0, EasingType::Linear));
//	beholder04Animation->mScaleKeyFrames.push_back(sScaleKeyFrame(3.5, 8.0, EasingType::Linear));
//
//	sAnimation* beholder05Animation = new sAnimation();
//	modelNameForNow = "Beholder05";
//
//	beholder05Animation->modelName = modelNameForNow;
//	beholder05Animation->mTime = 0.0;
//
//	beholder05Animation->mPositionKeyFrames.push_back(sPositionKeyFrame(glm::vec3(150, 30, -40), 0.0));
//	beholder05Animation->mPositionKeyFrames.push_back(sPositionKeyFrame(glm::vec3(-80, 30, -40), 15.0, EasingType::Linear));
//
//	beholder05Animation->mRotationKeyFrames.push_back(sRotationKeyFrame(0.0, glm::vec3(0.f, 0.f, 0.0f), 0.0));
//	beholder05Animation->mRotationKeyFrames.push_back(sRotationKeyFrame(-90.0, glm::vec3(0.f, 0.f, 1.f), 7.0));
//	beholder05Animation->mRotationKeyFrames.push_back(sRotationKeyFrame(-180.0, glm::vec3(0.f, 0.f, 1.f), 8.0));
//
//	//-----------Pushing all animations into the animation list---------------------------
//
//	animationManager.mAnimationList.push_back(beholder01Animation);
//	animationManager.mAnimationList.push_back(falconAnimations);
//	animationManager.mAnimationList.push_back(beholder02Animation);
//	animationManager.mAnimationList.push_back(beholder03Animation);
//	animationManager.mAnimationList.push_back(beholder04Animation);
//	animationManager.mAnimationList.push_back(beholder05Animation);
//
//	//-------Pushing all KeyFrame events into the KeyFrame event list---------------------
//
//	animationManager.mKeyFrameEventList.push_back(easeEvent01);
//	animationManager.mKeyFrameEventList.push_back(easeEvent02);
//	animationManager.mKeyFrameEventList.push_back(easeEvent03);
//	animationManager.mKeyFrameEventList.push_back(easeEvent04);
//
//	std::cout << std::endl;
//	std::cout << "SCENE_01 PLAYING !" << std::endl;
//	std::cout << std::endl;
//}
//
//void Scene02Sequence(cAnimationSystem& animationManager)
//{
//	//---------------------Resetting Animation Sequences-------------------------------------
//
//	animationManager.mAnimationList.clear();
//	animationManager.ResetAnimationTime();
//	if (animationManager.isAnimationReversed())
//		animationManager.NormalAnimationFlow();
//	ResetScene01Models();
//
//	//------------------------Beholder01 Animations------------------------------------------
//
//	sAnimation* beholder01Animation = new sAnimation();
//	std::string modelNameForNow = "Beholder01";
//
//	beholder01Animation->modelName = modelNameForNow;
//	beholder01Animation->mTime = 0.0;
//
//	beholder01Animation->mPositionKeyFrames.push_back(sPositionKeyFrame(glm::vec3(50, 0, 0), 0.0));
//	beholder01Animation->mPositionKeyFrames.push_back(sPositionKeyFrame(glm::vec3(35, 0, 0), 2.0, EasingType::Linear));
//	beholder01Animation->mPositionKeyFrames.push_back(sPositionKeyFrame(glm::vec3(20, 0, 0), 4.0, EasingType::Linear));
//	beholder01Animation->mPositionKeyFrames.push_back(sPositionKeyFrame(glm::vec3(5, 0, 0), 6.0, EasingType::Linear));
//
//	beholder01Animation->mRotationKeyFrames.push_back(sRotationKeyFrame(0.0, glm::vec3(0.f, 0.f, 0.0f), 0.0));
//	beholder01Animation->mRotationKeyFrames.push_back(sRotationKeyFrame(90.0, glm::vec3(0.f, 1.f, 0.f), 2.0));
//	beholder01Animation->mRotationKeyFrames.push_back(sRotationKeyFrame(180.0, glm::vec3(0.f, 1.f, 0.f), 4.0));
//	beholder01Animation->mRotationKeyFrames.push_back(sRotationKeyFrame(270.0, glm::vec3(0.f, 1.f, 0.f), 6.0));
//
//	sKeyFrameEvent* timeEvent01 = new sKeyFrameEvent(true, modelNameForNow);
//	timeEvent01->mTime = 6.0f;
//
//	//---------------------------Falcon Animations-------------------------------------------
//
//	sAnimation* falconAnimations = new sAnimation();
//	modelNameForNow = "Falcon";
//
//	falconAnimations->modelName = modelNameForNow;
//	falconAnimations->mTime = 0.0;
//
//	falconAnimations->mPositionKeyFrames.push_back(sPositionKeyFrame(glm::vec3(-50, 0, 0), 0.0));
//	falconAnimations->mPositionKeyFrames.push_back(sPositionKeyFrame(glm::vec3(-35, 0, 0), 2.0, EasingType::Linear));
//	falconAnimations->mPositionKeyFrames.push_back(sPositionKeyFrame(glm::vec3(-20, 0, 0), 4.0, EasingType::Linear));
//	falconAnimations->mPositionKeyFrames.push_back(sPositionKeyFrame(glm::vec3(-5, 0, 0), 6.0, EasingType::Linear));
//
//	sKeyFrameEvent* easeEvent01 = new sKeyFrameEvent(false, modelNameForNow);
//	easeEvent01->mEaseType = EasingType::Linear;
//
//	sKeyFrameEvent* timeEvent02 = new sKeyFrameEvent(true, modelNameForNow);
//	timeEvent02->mTime = 6.0f;
//
//	//------------------------Explosion Animation------------------------------------------- 
//
//	sAnimation* explosionAnimation = new sAnimation();
//	modelNameForNow = "Explosion";
//
//	explosionAnimation->modelName = modelNameForNow;
//	explosionAnimation->mTime = 0.0;
//
//	explosionAnimation->mScaleKeyFrames.push_back(sScaleKeyFrame(0.f, 0.f));
//	explosionAnimation->mScaleKeyFrames.push_back(sScaleKeyFrame(0.f, 6.f));
//	explosionAnimation->mScaleKeyFrames.push_back(sScaleKeyFrame(18.f, 6.5f));
//	explosionAnimation->mScaleKeyFrames.push_back(sScaleKeyFrame(0.f, 7.25f));
//
//	//-----------Pushing all animations into the animation list---------------------------
//
//	animationManager.mAnimationList.push_back(beholder01Animation);
//	animationManager.mAnimationList.push_back(falconAnimations);
//	animationManager.mAnimationList.push_back(explosionAnimation);
//
//	//-------Pushing all KeyFrame events into the KeyFrame event list---------------------
//
//	animationManager.mKeyFrameEventList.push_back(easeEvent01);
//	animationManager.mKeyFrameEventList.push_back(timeEvent01); 
//	animationManager.mKeyFrameEventList.push_back(timeEvent02);
//
//	std::cout << std::endl;
//	std::cout << "SCENE_02 PLAYING !" << std::endl;
//	std::cout << std::endl;
//}
//
//
//
