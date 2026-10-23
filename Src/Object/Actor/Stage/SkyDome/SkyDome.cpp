#include "SkyDome.h"
#include"../../../../Utility/AsoUtility.h"

SkyDome::SkyDome(void)
{
}

void SkyDome::SubLoad(void)
{
	transform_.modelId = MV1LoadModel("Data/Model/SkyDome/sky.mv1");
	transform_.scl = VGet(30, 30, 30);
}

void SkyDome::SubInit(void)
{
}

void SkyDome::SubUpdate(void)
{
	static float rot_ = 0;
	rot_ += 0.01f;
	transform_.quaRot = Quaternion::Euler(VGet(0.0f, AsoUtility::Deg2RadF(rot_), 0.0f));
	transform_.Update();
}

void SkyDome::SubDraw(void)
{

}

void SkyDome::SubRelease(void)
{
}
