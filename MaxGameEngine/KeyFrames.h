#pragma once
#include <string>
#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtx/quaternion.hpp>

enum class EasingType
{
	Linear,
	EaseIn,
	EaseOut,
	EaseInOut
};

struct sPositionKeyFrame
{
	sPositionKeyFrame(const glm::vec3& position, double time, EasingType type = EasingType::Linear)
		: mPosition(position)
		, mTime(time)
		, mEaseType(type)
	{ }

	glm::vec3 mPosition;
	EasingType mEaseType;
	double mTime;
};

struct sScaleKeyFrame
{
	sScaleKeyFrame(const glm::vec3& scale, double time, EasingType type = EasingType::Linear)
		: mScale(scale)
		, mTime(time)
		, mEaseType(type)
	{ }

	glm::vec3 mScale;
	EasingType mEaseType;
	double mTime;
};

struct sRotationKeyFrame
{
	sRotationKeyFrame(const glm::quat& rotation, double time, EasingType type = EasingType::Linear)
		: mRotation(rotation)
		, mTime(time)
		, mEaseType(type)
	{ }

	glm::quat mRotation;
	EasingType mEaseType;
	double mTime;
};