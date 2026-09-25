#pragma once

#include<vector>
#include"../../Common/Vector2.h"

class KeyManager
{
private:
	static KeyManager* ins;

	KeyManager();
	~KeyManager() {};
public:
	static void CreateIns(void) { if (ins == nullptr) { ins = new KeyManager(); ins->Init(); } }
	static KeyManager& GetIns(void) { return *ins; }
	static void DeleteIns(void) { if (ins != nullptr) { ins->Release(); delete ins; ins = nullptr; } }

	static constexpr int MAX_CONTROLLER_COUNT = 4;

	void Update(void);

	enum class KEY_TYPE
	{
		NON = -1,

		//ÉvÉåÉCÉÑÅ[ÇÃëÄçÏ
		EVADE,
		PARRY,
		GUARD,
		LIGHTATTACK,
		HEAVYATTACK,
		FINISH,
		BREAK,
		ITEM,
		//--------------

		MOVE_FRONT,
		MOVE_BACK,
		MOVE_RIGHT,
		MOVE_LEFT,
		MOVE_UP,
		MOVE_DOWN,

		MOVE_RIGHT_SET,
		MOVE_LEFT_SET,

		CAMERA_MOVE_FRONT,
		CAMERA_MOVE_BACK,
		CAMERA_MOVE_RIGHT,
		CAMERA_MOVE_LEFT,
		CAMERA_MOVE_UP,
		CAMERA_MOVE_DOWN,

		CAMERA_ROTATE_UP,
		CAMERA_ROTATE_DOWN,
		CAMERA_ROTATE_RIGHT,
		CAMERA_ROTATE_LEFT,

		UP,
		DOWN,
		RIGHT,
		LEFT,
		A,
		B,
		X,
		Y,
		PAUSE,
		ENTER,
		SPACE,
		SHIFT,
		TAB,
		DEBUG_KEY_0,

		MOUSE_LEFT,
		MOUSE_RIGHT,

		DEBUG,

		MAX,
	};

	struct KEY_INFO {
		bool prev = false,
			now = false,
			up = false,
			down = false;
	};

	const KEY_INFO GetInfo(KEY_TYPE k, int controllerIdx = 0) { return keyInfo[controllerIdx][(int)k]; }

	const Vector2F GetRightStickVec(int controllerIdx = 0);

private:
	void Init(void);
	void Release(void);

	KEY_INFO keyInfo[MAX_CONTROLLER_COUNT][(int)KEY_TYPE::MAX];

	std::vector<int>keyboardFormat[(int)KEY_TYPE::MAX];
	std::vector<int>controllerButtonFormat[(int)KEY_TYPE::MAX];
	std::vector<int>mouse[(int)KEY_TYPE::MAX];

	enum class CONTROLLER_OTHERS
	{
		LEFTSTICK_UP,
		LEFTSTICK_DOWN,
		LEFTSTICK_RIGHT,
		LEFTSTICK_LEFT,

		RIGHTSTICK_UP,
		RIGHTSTICK_DOWN,
		RIGHTSTICK_RIGHT,
		RIGHTSTICK_LEFT,

		LEFT_TRIGGER,
		RIGHT_TRIGGER,
	};

	bool ControllerOthersInput(const CONTROLLER_OTHERS& input, int padNo);
	std::vector<CONTROLLER_OTHERS>controllerOthersFormat[(int)KEY_TYPE::MAX];
};

using KEY = KeyManager;
using KEY_TYPE = KEY::KEY_TYPE;