#include "GamePad.h"

#include <Windows.h>
#include <math.h>
#include <Xinput.h>

#pragma comment (lib, "Xinput.lib")

// ----- �X�V -----
void GamePad::Update()
{
	axisLx_ = axisLy_ = 0.0f;
	axisRx_ = axisRy_ = 0.0f;
	triggerL_ = triggerR_ = 0.0f;

	GamePadButton newButtonState = 0;

	// ----- �{�^�����擾 -----
	XINPUT_STATE xinputState;
	if (XInputGetState(slot_, &xinputState) == ERROR_SUCCESS)
	{
		//XINPUT_CAPABILITIES caps;
		//XInputGetCapabilities(m_slot, XINPUT_FLAG_GAMEPAD, &caps);
		XINPUT_GAMEPAD& pad = xinputState.Gamepad;

		if (pad.wButtons & XINPUT_GAMEPAD_DPAD_UP)					newButtonState |= BTN_UP;
		if (pad.wButtons & XINPUT_GAMEPAD_DPAD_RIGHT)				newButtonState |= BTN_RIGHT;
		if (pad.wButtons & XINPUT_GAMEPAD_DPAD_DOWN)				newButtonState |= BTN_DOWN;
		if (pad.wButtons & XINPUT_GAMEPAD_DPAD_LEFT)				newButtonState |= BTN_LEFT;
		if (pad.wButtons & XINPUT_GAMEPAD_A)						newButtonState |= BTN_A;
		if (pad.wButtons & XINPUT_GAMEPAD_B)						newButtonState |= BTN_B;
		if (pad.wButtons & XINPUT_GAMEPAD_X)						newButtonState |= BTN_X;
		if (pad.wButtons & XINPUT_GAMEPAD_Y)						newButtonState |= BTN_Y;
		if (pad.wButtons & XINPUT_GAMEPAD_START)					newButtonState |= BTN_START;
		if (pad.wButtons & XINPUT_GAMEPAD_BACK)						newButtonState |= BTN_BACK;
		if (pad.wButtons & XINPUT_GAMEPAD_LEFT_THUMB)				newButtonState |= BTN_LEFT_THUMB;
		if (pad.wButtons & XINPUT_GAMEPAD_RIGHT_THUMB)				newButtonState |= BTN_RIGHT_THUMB;
		if (pad.wButtons & XINPUT_GAMEPAD_LEFT_SHOULDER)			newButtonState |= BTN_LEFT_SHOULDER;
		if (pad.wButtons & XINPUT_GAMEPAD_RIGHT_SHOULDER)			newButtonState |= BTN_RIGHT_SHOULDER;
		if (pad.bLeftTrigger > XINPUT_GAMEPAD_TRIGGER_THRESHOLD)	newButtonState |= BTN_LEFT_TRIGGER;
		if (pad.bRightTrigger > XINPUT_GAMEPAD_TRIGGER_THRESHOLD)	newButtonState |= BTN_RIGHT_TRIGGER;

		if ((pad.sThumbLX <  XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE && pad.sThumbLX > -XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE) &&
			(pad.sThumbLY <  XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE && pad.sThumbLY > -XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE))
		{
			pad.sThumbLX = 0;
			pad.sThumbLY = 0;
		}

		if ((pad.sThumbRX <  XINPUT_GAMEPAD_RIGHT_THUMB_DEADZONE && pad.sThumbRX > -XINPUT_GAMEPAD_RIGHT_THUMB_DEADZONE) &&
			(pad.sThumbRY <  XINPUT_GAMEPAD_RIGHT_THUMB_DEADZONE && pad.sThumbRY > -XINPUT_GAMEPAD_RIGHT_THUMB_DEADZONE))
		{
			pad.sThumbRX = 0;
			pad.sThumbRY = 0;
		}

		triggerL_ = static_cast<float>(pad.bLeftTrigger) / 255.0f;
		triggerR_ = static_cast<float>(pad.bRightTrigger) / 255.0f;
		axisLx_ = static_cast<float>(pad.sThumbLX) / static_cast<float>(0x8000);
		axisLy_ = static_cast<float>(pad.sThumbLY) / static_cast<float>(0x8000);
		axisRx_ = static_cast<float>(pad.sThumbRX) / static_cast<float>(0x8000);
		axisRy_ = static_cast<float>(pad.sThumbRY) / static_cast<float>(0x8000);
	}
	else
	{
#if 0
		// XInput�œ��͏�񂪎擾�o���Ȃ������ꍇ��WindowsAPI�Ŏ擾����
		JOYINFOEX joy_info;
		joy_info.dwSize = sizeof(JOYINFOEX);
		joy_info.dwFlags = JOY_RETURNALL;	// �S�Ă̏����擾

		if (joyGetPosEx(m_slot, &joy_info) == JOYERR_NOERROR)
		{
			// ���iID���`�F�b�N����PS4�R���g���[���[�����Ή�����
			static const WORD PS4_PID = 1476;

			JOYCAPS joy_caps;
			if (joyGetDevCaps(m_slot, &joy_caps, sizeof(JOYCAPS)) == JOYERR_NOERROR)
			{
				// �\���L�[
				if (joy_info.dwPOV != 0xFFFF)
				{
					static const int pov_bit[8] =
					{
						BTN_UP,					// ��
						BTN_RIGHT | BTN_UP,		// �E��
						BTN_RIGHT,				// �E
						BTN_RIGHT | BTN_DOWN,	// �E��
						BTN_DOWN,				// ��
						BTN_LEFT | BTN_DOWN,	// ����
						BTN_LEFT,				// ��
						BTN_LEFT | BTN_UP		// ����
					};
					int angle = joy_info.dwPOV / 4500;
					new_button_state |= pov_bit[angle];
				}
				if (joy_caps.wPid == PS4_PID)
				{
					// �{�^�����
					if (joy_info.dwButtons & JOY_BUTTON1)  new_button_state |= BTN_Y;
					if (joy_info.dwButtons & JOY_BUTTON2)  new_button_state |= BTN_B;
					if (joy_info.dwButtons & JOY_BUTTON3)  new_button_state |= BTN_A;
					if (joy_info.dwButtons & JOY_BUTTON4)  new_button_state |= BTN_X;
					if (joy_info.dwButtons & JOY_BUTTON5)  new_button_state |= BTN_LEFT_SHOULDER;
					if (joy_info.dwButtons & JOY_BUTTON6)  new_button_state |= BTN_RIGHT_SHOULDER;
					if (joy_info.dwButtons & JOY_BUTTON7)  new_button_state |= BTN_LEFT_TRIGGER;
					if (joy_info.dwButtons & JOY_BUTTON8)  new_button_state |= BTN_RIGHT_TRIGGER;
					if (joy_info.dwButtons & JOY_BUTTON9)  new_button_state |= BTN_BACK;
					if (joy_info.dwButtons & JOY_BUTTON10) new_button_state |= BTN_START;
					if (joy_info.dwButtons & JOY_BUTTON11) new_button_state |= BTN_LEFT_THUMB;
					if (joy_info.dwButtons & JOY_BUTTON12) new_button_state |= BTN_RIGHT_THUMB;
					//if (joyInfo.dwButtons & JOY_BUTTON13) new_button_state |= BTN_?;	// PS
					//if (joyInfo.dwButtons & JOY_BUTTON14) new_button_state |= BTN_?;	// Touch

					// ���X�e�B�b�N
					axisLx_ = static_cast<int>(joy_info.dwXpos - 0x7FFF) / static_cast<float>(0x8000);
					axisLy_ = -static_cast<int>(joy_info.dwYpos - 0x7FFF) / static_cast<float>(0x8000);

					// �E�X�e�B�b�N
					axisRx_ = static_cast<int>(joy_info.dwZpos - 0x7FFF) / static_cast<float>(0x8000);
					axisRy_ = -static_cast<int>(joy_info.dwRpos - 0x7FFF) / static_cast<float>(0x8000);

					// LR�g���K�[
					m_trigger_l = static_cast<float>(joy_info.dwVpos) / static_cast<float>(0xFFFF);
					m_trigger_r = static_cast<float>(joy_info.dwUpos) / static_cast<float>(0xFFFF);

					if (axisLx_ > -0.25f && axisLx_ < 0.25f) axisLx_ = 0.0f;
					if (axisRx_ > -0.25f && axisRx_ < 0.25f) axisRx_ = 0.0f;
				}
			}
		}
#endif
	}

	// �L�[�{�[�h�ŃG�~�����[�V����
	{
		float lx = 0.0f;
		float ly = 0.0f;
		float rx = 0.0f;
		float ry = 0.0f;
		if (GetAsyncKeyState('W') & 0x8000) ly = 1.0f;
		if (GetAsyncKeyState('A') & 0x8000) lx = -1.0f;
		if (GetAsyncKeyState('S') & 0x8000) ly = -1.0f;
		if (GetAsyncKeyState('D') & 0x8000) lx = 1.0f;
		if (GetAsyncKeyState('I') & 0x8000) ry = 1.0f;
		if (GetAsyncKeyState('J') & 0x8000) rx = -1.0f;
		if (GetAsyncKeyState('K') & 0x8000) ry = -1.0f;
		if (GetAsyncKeyState('L') & 0x8000) rx = 1.0f;
		if (GetAsyncKeyState(' ') & 0x8000) newButtonState |= BTN_A;
		if (GetAsyncKeyState('X') & 0x8000) newButtonState |= BTN_B;
		if (GetAsyncKeyState('C') & 0x8000) newButtonState |= BTN_X;
		if (GetAsyncKeyState('V') & 0x8000) newButtonState |= BTN_Y;
		if (GetAsyncKeyState(VK_UP) & 0x8000)	newButtonState |= BTN_UP;
		if (GetAsyncKeyState(VK_RIGHT) & 0x8000)	newButtonState |= BTN_RIGHT;
		if (GetAsyncKeyState(VK_DOWN) & 0x8000)	newButtonState |= BTN_DOWN;
		if (GetAsyncKeyState(VK_LEFT) & 0x8000)	newButtonState |= BTN_LEFT;

#if 1
		if (newButtonState & BTN_UP)    ly = 1.0f;
		if (newButtonState & BTN_RIGHT) lx = 1.0f;
		if (newButtonState & BTN_DOWN)  ly = -1.0f;
		if (newButtonState & BTN_LEFT)  lx = -1.0f;
#endif

		if (lx >= 1.0f || lx <= -1.0f || ly >= 1.0f || ly <= -1.0)
		{
			float power = ::sqrtf(lx * lx + ly * ly);
			axisLx_ = lx / power;
			axisLy_ = ly / power;
		}

		if (rx >= 1.0f || rx <= -1.0f || ry >= 1.0f || ry <= -1.0)
		{
			float power = ::sqrtf(rx * rx + ry * ry);
			axisRx_ = rx / power;
			axisRy_ = ry / power;
		}
	}



	// �{�^�����̍X�V
	{
		buttonState_[1] = buttonState_[0];	// �X�C�b�`����
		buttonState_[0] = newButtonState;

		buttonDown_ = ~buttonState_[1] & newButtonState;	// �������u��
		buttonUp_ = ~newButtonState & buttonState_[1];	// �������u��
	}

}
