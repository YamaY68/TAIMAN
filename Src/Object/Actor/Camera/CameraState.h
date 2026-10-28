#pragma once
#include <DxLib.h>
#include "../../../Common/Quaternion.h"

enum class CAMERA_KIND
{
	NONE = 0,
	DEBUG,
	PLAYER_FOLLOW,
	LOCK_ON,
	EVENT,

};

struct CameraState
{
	VECTOR position = { 0.0f, 100.0f, -300.0f };
	Quaternion rotation = Quaternion::Identity();
	VECTOR target = { 0.0f, 50.0f, 0.0f };
	VECTOR upVector = { 0.0f, 1.0f, 0.0f };
	float fovRad = 60.0f * (DX_PI_F / 180.0f);
	float nearClip = 10.0f;
	float farClip = 20000.0f; 
	CAMERA_KIND kind_=CAMERA_KIND::NONE;

	static CameraState Lerp(const CameraState& a, const CameraState& b, float t)
	{
		CameraState result;

		result.position = VAdd(a.position, VScale(VSub(b.position, a.position), t));
		result.target = VAdd(a.target, VScale(VSub(b.target, a.target), t));

		result.rotation = Quaternion::Slerp(a.rotation, b.rotation, t);

		result.fovRad = a.fovRad + (b.fovRad - a.fovRad) * t;
		result.nearClip = a.nearClip + (b.nearClip - a.nearClip) * t;
		result.farClip = a.farClip + (b.farClip - a.farClip) * t;
		return result;
	}
};