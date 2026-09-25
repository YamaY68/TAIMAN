#include "KeyManager.h"
#include <DxLib.h>
#include <cmath> // sqrtf用

KeyManager* KeyManager::ins = nullptr;

KeyManager::KeyManager() :
	keyInfo(),
	keyboardFormat(),
	controllerButtonFormat()
{
}

void KeyManager::Init(void)
{
	// キーボードを割り振るとき
#define SET_KEYBOARD(type,key)keyboardFormat[(int)type].emplace_back(key)

	// コントローラーのボタンを割り振るとき
#define SET_C_BUTTON(type,key)controllerButtonFormat[(int)type].emplace_back(key)

	// コントローラーのボタン以外(スティックやトリガーなど)を割り振るとき
#define SET_C_OTHERS(type,key)controllerOthersFormat[(int)type].emplace_back(key)

#define SET_MOUSE(type,key)mouse[(int)type].emplace_back(key)


	SET_KEYBOARD(KEY_TYPE::MOVE_FRONT, KEY_INPUT_W);
	SET_C_OTHERS(KEY_TYPE::MOVE_FRONT, CONTROLLER_OTHERS::LEFTSTICK_UP);

	SET_KEYBOARD(KEY_TYPE::MOVE_BACK, KEY_INPUT_S);
	SET_C_OTHERS(KEY_TYPE::MOVE_BACK, CONTROLLER_OTHERS::LEFTSTICK_DOWN);

	SET_KEYBOARD(KEY_TYPE::MOVE_RIGHT, KEY_INPUT_D);
	SET_C_OTHERS(KEY_TYPE::MOVE_RIGHT, CONTROLLER_OTHERS::LEFTSTICK_RIGHT);

	SET_KEYBOARD(KEY_TYPE::MOVE_LEFT, KEY_INPUT_A);
	SET_C_OTHERS(KEY_TYPE::MOVE_LEFT, CONTROLLER_OTHERS::LEFTSTICK_LEFT);


	SET_KEYBOARD(KEY_TYPE::MOVE_UP, KEY_INPUT_UP);
	SET_KEYBOARD(KEY_TYPE::MOVE_DOWN, KEY_INPUT_DOWN);
	SET_KEYBOARD(KEY_TYPE::MOVE_RIGHT_SET, KEY_INPUT_RIGHT);
	SET_KEYBOARD(KEY_TYPE::MOVE_LEFT_SET, KEY_INPUT_LEFT);

	SET_KEYBOARD(KEY_TYPE::SPACE, KEY_INPUT_SPACE);

	SET_KEYBOARD(KEY_TYPE::CAMERA_MOVE_FRONT, KEY_INPUT_I);
	SET_KEYBOARD(KEY_TYPE::CAMERA_MOVE_BACK, KEY_INPUT_K);
	SET_KEYBOARD(KEY_TYPE::CAMERA_MOVE_RIGHT, KEY_INPUT_L);
	SET_KEYBOARD(KEY_TYPE::CAMERA_MOVE_LEFT, KEY_INPUT_J);
	SET_KEYBOARD(KEY_TYPE::CAMERA_MOVE_UP, KEY_INPUT_U);
	SET_KEYBOARD(KEY_TYPE::CAMERA_MOVE_DOWN, KEY_INPUT_O);

	SET_KEYBOARD(KEY_TYPE::CAMERA_ROTATE_UP, KEY_INPUT_UP);
	SET_KEYBOARD(KEY_TYPE::CAMERA_ROTATE_DOWN, KEY_INPUT_DOWN);
	SET_KEYBOARD(KEY_TYPE::CAMERA_ROTATE_RIGHT, KEY_INPUT_RIGHT);
	SET_KEYBOARD(KEY_TYPE::CAMERA_ROTATE_LEFT, KEY_INPUT_LEFT);

	SET_KEYBOARD(KEY_TYPE::PAUSE, KEY_INPUT_ESCAPE);
	SET_C_BUTTON(KEY_TYPE::PAUSE, XINPUT_BUTTON_START);

	SET_KEYBOARD(KEY_TYPE::UP, KEY_INPUT_UP);
	SET_KEYBOARD(KEY_TYPE::DOWN, KEY_INPUT_DOWN);
	SET_KEYBOARD(KEY_TYPE::RIGHT, KEY_INPUT_RIGHT);
	SET_KEYBOARD(KEY_TYPE::LEFT, KEY_INPUT_LEFT);
	SET_KEYBOARD(KEY_TYPE::TAB, KEY_INPUT_TAB);

	SET_C_BUTTON(KEY_TYPE::A, XINPUT_BUTTON_A);
	SET_C_BUTTON(KEY_TYPE::B, XINPUT_BUTTON_B);
	SET_C_BUTTON(KEY_TYPE::X, XINPUT_BUTTON_X);
	SET_C_BUTTON(KEY_TYPE::Y, XINPUT_BUTTON_Y);


	SET_KEYBOARD(KEY_TYPE::ENTER, KEY_INPUT_RETURN);
	SET_KEYBOARD(KEY_TYPE::ENTER, KEY_INPUT_SPACE);
	SET_KEYBOARD(KEY_TYPE::ENTER, KEY_INPUT_J);
	SET_C_BUTTON(KEY_TYPE::ENTER, XINPUT_BUTTON_B);
	SET_C_BUTTON(KEY_TYPE::ENTER, XINPUT_BUTTON_A);
	SET_KEYBOARD(KEY::KEY_TYPE::SHIFT,KEY_INPUT_LSHIFT);

	SET_KEYBOARD(KEY_TYPE::DEBUG_KEY_0, KEY_INPUT_0);

	SET_MOUSE(KEY_TYPE::MOUSE_LEFT, MOUSE_INPUT_LEFT);
	SET_MOUSE(KEY_TYPE::MOUSE_RIGHT, MOUSE_INPUT_RIGHT);

	SET_KEYBOARD(KEY_TYPE::DEBUG, KEY_INPUT_0);

	//プレイヤーの操作--------------------------
	SET_KEYBOARD(KEY_TYPE::EVADE, KEY_INPUT_K);
	SET_KEYBOARD(KEY_TYPE::PARRY, KEY_INPUT_J);
	SET_KEYBOARD(KEY_TYPE::GUARD, KEY_INPUT_L);
	SET_KEYBOARD(KEY_TYPE::LIGHTATTACK, KEY_INPUT_J);
	SET_KEYBOARD(KEY_TYPE::HEAVYATTACK, KEY_INPUT_H);
	SET_KEYBOARD(KEY_TYPE::FINISH, KEY_INPUT_J);
	SET_KEYBOARD(KEY_TYPE::FINISH, KEY_INPUT_H);
	SET_KEYBOARD(KEY_TYPE::BREAK, KEY_INPUT_H);
	SET_KEYBOARD(KEY_TYPE::ITEM, KEY_INPUT_I);

}

void KeyManager::Update(void)
{
	// 全てのコントローラー分ループ
	for (int p = 0; p < MAX_CONTROLLER_COUNT; p++) {
		// DxLibの入力パッド指定用ID (DX_INPUT_PAD1 + 0, + 1, + 2, + 3)
		int padNo = DX_INPUT_PAD1 + p;

		for (int i = 0; i < (int)KEY_TYPE::MAX; i++) {
			keyInfo[p][i].prev = keyInfo[p][i].now;

			bool b = false;

			// キーボードとマウスはPCに1つしかないため、1P(p == 0)の時のみ判定する
			if (p == 0) {
				for (auto& input : keyboardFormat[i]) {
					if (b) { break; }
					if (CheckHitKey(input) != 0) { b = true; }
				}
				for (auto& input : mouse[i]) {
					if (b) { break; }
					if (GetMouseInput() & input) { b = true; }
				}
			}

			// 各コントローラーのボタン入力判定
			for (auto& input : controllerButtonFormat[i]) {
				if (b) { break; }

				XINPUT_STATE state = {};
				if (GetJoypadXInputState(padNo, &state) != 0) { state = {}; }

				if (state.Buttons[input] != 0) { b = true; }
			}

			// 各コントローラーのスティック・トリガー入力判定
			for (CONTROLLER_OTHERS input : controllerOthersFormat[i]) {
				if (b) { break; }

				b = ControllerOthersInput(input, padNo);
			}

			keyInfo[p][i].now = b;

			keyInfo[p][i].up = (keyInfo[p][i].prev && !keyInfo[p][i].now);
			keyInfo[p][i].down = (!keyInfo[p][i].prev && keyInfo[p][i].now);
		}
	}
}

void KeyManager::Release(void)
{
	for (auto& input : keyboardFormat) { input.clear(); }
	for (auto& input : controllerButtonFormat) { input.clear(); }
	for (auto& input : controllerOthersFormat) { input.clear(); }
	for (auto& input : mouse) { input.clear(); }
}

// padNo 引数を追加して、特定のパッドから取得するように変更
bool KeyManager::ControllerOthersInput(const CONTROLLER_OTHERS& input, int padNo)
{
	XINPUT_STATE state = {};
	if (GetJoypadXInputState(padNo, &state) != 0) { state = {}; }

	short lenge = 10000;

	switch (input)
	{
	case KeyManager::CONTROLLER_OTHERS::LEFTSTICK_UP:
		if (state.ThumbLY > lenge) { return true; }
		break;
	case KeyManager::CONTROLLER_OTHERS::LEFTSTICK_DOWN:
		if (state.ThumbLY < -lenge) { return true; }
		break;
	case KeyManager::CONTROLLER_OTHERS::LEFTSTICK_RIGHT:
		if (state.ThumbLX > lenge) { return true; }
		break;
	case KeyManager::CONTROLLER_OTHERS::LEFTSTICK_LEFT:
		if (state.ThumbLX < -lenge) { return true; }
		break;

	case KeyManager::CONTROLLER_OTHERS::RIGHTSTICK_UP:
		if (state.ThumbRY > lenge) { return true; }
		break;
	case KeyManager::CONTROLLER_OTHERS::RIGHTSTICK_DOWN:
		if (state.ThumbRY < -lenge) { return true; }
		break;
	case KeyManager::CONTROLLER_OTHERS::RIGHTSTICK_RIGHT:
		if (state.ThumbRX > lenge) { return true; }
		break;
	case KeyManager::CONTROLLER_OTHERS::RIGHTSTICK_LEFT:
		if (state.ThumbRX < -lenge) { return true; }
		break;

	case KeyManager::CONTROLLER_OTHERS::LEFT_TRIGGER:
		if (state.LeftTrigger > 0) { return true; }
		break;
	case KeyManager::CONTROLLER_OTHERS::RIGHT_TRIGGER:
		if (state.RightTrigger > 0) { return true; }
		break;
	default:
		return false;
	}

	return false;
}

const Vector2F KeyManager::GetRightStickVec(int controllerIdx)
{
	XINPUT_STATE state = {};
	int padNo = DX_INPUT_PAD1 + controllerIdx;

	if (GetJoypadXInputState(padNo, &state) != 0) { return { 0.0f,0.0f }; }
	short lenge = 10000;

	Vector2F vec = { (abs(state.ThumbRX) > lenge) ? (float)state.ThumbRX : 0.0f,(abs(state.ThumbRY) > lenge) ? (float)-state.ThumbRY : 0.0f };

	// Vector2Fのオペレータオーバーロード依存を避けるため成分ごとに比較
	if (vec.x == 0.0f && vec.y == 0.0f) { return{ 0.0f,0.0f }; }

	return vec / sqrtf(vec.x * vec.x + vec.y * vec.y);
}