#pragma once
#include<memory>
#include<vector>
class CameraManager;
template<typename T>

class FactoryBase
{
public:
	FactoryBase(void) = default;
	FactoryBase(CameraManager* cameramanager) {};
	virtual ~FactoryBase(void)= default;
	virtual std::vector<std::unique_ptr<T>>Create(void) = 0;
protected:
	CameraManager* cameraManager_ = nullptr;
};