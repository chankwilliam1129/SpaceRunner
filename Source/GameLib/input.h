#pragma once
#include <Windows.h>
#include <DirectXMath.h>
#include <Xinput.h>
#include "vector.h"

//�}�E�X�֘A
class Mouse
{
public:
	enum Botton
	{
		LEFT,
		RIGHT,
		MIDDLE,
		MAX,
	};

private:
	bool isPush[Botton::MAX];
	bool isRelease[Botton::MAX];
	bool isPushTrigger[Botton::MAX];
	bool isReleaseTrigger[Botton::MAX];
	LPPOINT p;

public:
	static UINT GetState(UINT msg);

	Vector2 GetPosition();

	void SetIsPush(const Botton botton, const bool flg) { isPush[botton] = flg; }
	void SetIsRelease(const Botton botton, const bool flg) { isRelease[botton] = flg; }

	void SetIsPushTrigger(const Botton botton, const bool flg) { isPushTrigger[botton] = flg; }
	void SetIsReleaseTrigger(const Botton botton, const bool flg) { isReleaseTrigger[botton] = flg; }

	void SetBottonDownFlg(const Botton botton);
	void SetBottonUpFlg(const Botton botton);

	bool Push(const Botton botton);				//�}�E�X��������Ă�����		true
	bool PushTrigger(const Botton botton);		//�}�E�X�������ꂽ�u��		true
	bool ReleaseTrigger(const Botton botton);	//�}�E�X�������ꂽ�u��		true
	bool None(const Botton botton);				//�}�E�X��������ĂȂ�������	true
};
extern Mouse mouse;

//�L�[�{�[�h�֘A
class KeyBoard
{
private:
	static const int keyMax = 256;

	BYTE key[keyMax];
	BYTE key_prev[keyMax];
	void GetKeyState();

public:
	KeyBoard() :key(), key_prev() {};
	~KeyBoard() {};

	KeyBoard& operator=(const KeyBoard& org)
	{
		for (int i = 0; i < keyMax; i++) 
		{
			key[i] = org.key[i];
		}
		return *this;
	}

	static void GetState();	                //�S���̃L�[�̏�Ԃ��擾���Ă���

	bool Push(const int _key);				//�L�[��������Ă�����	  true
	bool PushTrigger(const int _key);		//�L�[�������ꂽ�u��		  true
	bool ReleaseTrigger(const int _key);	//�L�[�������ꂽ�u��		  true
	bool None(const int _key);				//�L�[��������ĂȂ�������	  true
};
extern KeyBoard kb;

////�Q�[���p�b�h�֘A
//class GamePad
//{
//private:
//	static const int ControllerMax = 4;
//
//	struct ControllerState
//	{
//		XINPUT_STATE state;			//�p�b�h��id
//		XINPUT_VIBRATION vibration;	//�p�b�h�̐U�����x
//		bool is_connected;			//�p�b�h���ڑ�����Ă�����	true
//	};
//
//	ControllerState gamePad[ControllerMax];
//
//	WORD buttons[ControllerMax];
//	WORD buttons_prev[ControllerMax];
//
//	struct bTrigger
//	{
//		BYTE left;
//		BYTE right;
//	};
//
//	bTrigger trg[ControllerMax];
//	bTrigger trg_prev[ControllerMax];
//
//	HRESULT CheckControllerState();	//�p�b�h���ڑ�����Ă��邩���ׂ�
//
//	float GetDegreeAngle(const float x, const float y);
//	float GetRadianAngle(const float x, const float y);
//
//	bool CheckRange(const SHORT x, const SHORT y);
//
//public:
//	GamePad() :buttons(), buttons_prev(), gamePad(), trg(), trg_prev() {};
//	~GamePad() {};
//
//	GamePad& operator=(const GamePad& org)
//	{
//		for (int i = 0; i < ControllerMax; i++) {
//			gamePad[i] = org.gamePad[i];
//		}
//
//		return *this;
//	}
//
//	enum PlayerID
//	{
//		_1P,
//		_2P,
//		_3P,
//		_4P,
//		PLAYER_MAX
//	};
//
//	//�R���g���[���[�� �g���K�[(TRIGGER) �̏�Ԃ𒲂ׂ邽�� 
//	enum TriggerType
//	{
//		PUSH,
//		PUSH_TRIGGER,
//		RELEASE_TRIGGER,
//		NONE,
//	};
//
//	static SHORT thres_hold;	//�p�b�h�̔�������͈͂̐ݒ肷�邽�߂̒l
//	static void GetState();		//�p�b�h�̏�Ԃ��擾���Ă���
//
//	bool CheckLStickRange(const PlayerID player);		//���̃p�b�h�̔�������͈͂����ׂ�
//	bool CheckRStickRange(const PlayerID player);		//�E�̃p�b�h�̔�������͈͂����ׂ�
//
//	float GetLStickDegreeAngle(const PlayerID player);	//�x(degree) �ō��̃p�b�h�̊p�x�𒲂ׂ�
//	float GetLStickRadianAngle(const PlayerID player);	//���W�A�� �ō��̃p�b�h�̊p�x�𒲂ׂ�
//
//	float GetRStickDegreeAngle(const PlayerID player);	//�x(degree) �ŉE�̃p�b�h�̊p�x�𒲂ׂ�
//	float GetRStickRadianAngle(const PlayerID player);	//���W�A�� �ŉE�̃p�b�h�̊p�x�𒲂ׂ�
//
//	Vector2 GetLStickVector(const PlayerID player);
//	Vector2 GetRStickVector(const PlayerID player);
//
//	void SetVibration(const float motor1_speed, const float motor2_speed, const int player);	//�p�b�h�̐U���̑��x�̐ݒ�	�ŏ� 0.0f, �ő� 100.0f 
//	void ResetVibration(const PlayerID player);													//�p�b�h�̐U���̑��x�� 0.0f �ɂ���
//
//	bool LTrigger(const TriggerType type, const PlayerID player);	//�p�b�h�̍��̃g���K�[(TRIGGER) �̏�Ԃ𒲂ׂ�
//	bool RTrigger(const TriggerType type, const PlayerID player);	//�p�b�h�̉E�̃g���K�[(TRIGGER) �̏�Ԃ𒲂ׂ�
//
//	bool Push(const int botton, const PlayerID player);				//�{�^����������Ă�����		true
//	bool PushTrigger(const int botton, const PlayerID player);		//�{�^���������ꂽ�u��		true
//	bool ReleaseTrigger(const int botton, const PlayerID player);	//�{�^���������ꂽ�u��		true
//	bool None(const int botton, const PlayerID player);				//�{�^����������ĂȂ�������	true
//};
//extern GamePad pad;