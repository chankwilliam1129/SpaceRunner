#pragma once
#include <DirectXMath.h>
#include <map>
#include "game_object.h"

class StageManager :public GameObjectManager, public Singleton<StageManager>
{
public:
	const static int StageSize = 16;
	const static int StageListSize = 9;

	float StageGlowStrength = 5.0f;

	float moveSpeed = 1.0f;
	float moveSpeedScale = 1.0f;
	float nowStageTimer = .0f;
	int nowStage = 0;

	int index = 0;
	int randNum = 3;
	int adjustNum = 0;

	class StageData
	{
	public:
		int data[StageSize];

		int Get(int num)
		{
			while (num >= StageSize)
			{
				num -= StageSize;
			}
			return data[num];
		}

		StageData(
			int num00, int num01, int num02, int num03, int num04, int num05, int num06, int num07,
			int num08, int num09, int num10, int num11, int num12, int num13, int num14, int num15)
		{
			data[0] = num00; data[1] = num01; data[2] = num02; data[3] = num03; data[4] = num04;
			data[5] = num05; data[6] = num06; data[7] = num07; data[8] = num08; data[9] = num09;
			data[10] = num10; data[11] = num11; data[12] = num12; data[13] = num13; data[14] = num14; data[15] = num15;
		}

		StageData(const StageData& c)
		{
			for (int i = 0; i < StageSize; i++)
			{
				data[i] = c.data[i];
			}
		}

		void Reset(
			int num00, int num01, int num02, int num03, int num04, int num05, int num06, int num07,
			int num08, int num09, int num10, int num11, int num12, int num13, int num14, int num15)
		{
			data[0] = num00; data[1] = num01; data[2] = num02; data[3] = num03; data[4] = num04;
			data[5] = num05; data[6] = num06; data[7] = num07; data[8] = num08; data[9] = num09;
			data[10] = num10; data[11] = num11; data[12] = num12; data[13] = num13; data[14] = num14; data[15] = num15;
		}
	};

	std::map<int, StageData*> StageSet;

public:
	void Initialize();
	void Update();
	void Render();

public:
	void SetStageData();
	int GetIndex();
};

class StageGroup : public Component
{
public:
	const Vector3 scale = Vector3(1.0f, 1.0f, 5.0f);
	const float size = 20.0f;
	int index = 0;
	float position = 0;


	void Start();
	void Update();

	void TransformUpdate();
	void ResetChild();
};

class Stage :public Component
{
public:
	int area = 0;
	int type = 0;

public:
	void Start();

public:
	void SetType(int type, int area);
};
