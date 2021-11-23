#include "transform_vibrate.h"

void PositionVibrate::Start()
{
}

void PositionVibrate::Update()
{
	if (!isTurnOn) { return; }
	if (timeCounter <= 0)
	{
		VibrateEnd();
		return;
	}

	Vector3 result = { 0,0,0 };

	result = (offsetMax - offsetMin) * sinf((time - timeCounter) / frequency) + (offsetMin + offsetMax) / 2.0f;

	result *= (time - (time - timeCounter)) / time * -1.0f;

	offset = result;

	timeCounter -= (1 / 60.0f);
}

void PositionVibrate::VibrateEnd()
{
	isTurnOn = false;
	offset = { 0,0,0 };
	time = 0;
	timeCounter = 0;
}

void PositionVibrate::SetVibrate(Vector3 offsetMax, Vector3 offsetMin, float time, float frequency)
{
	this->offsetMax = offsetMax;
	this->offsetMin = offsetMin;
	this->time = time;
	this->frequency = frequency;
	if (frequency < 0.01f)
	{
		frequency = 0.01f;
	}
	timeCounter = time;
	isTurnOn = true;
}

void PositionVibrate::ImGuiUpdate()
{
	static Vector3 Max, Min;
	static float Time, Frequency;
#ifdef USE_IMGUI
	if (ImGui::TreeNode("PositionVibrate"))
	{
		ImGui::DragFloat3("Max", &Max.x, 1.0f);
		ImGui::DragFloat3("Min", &Min.x, 1.0f);
		ImGui::DragFloat("Time", &Time, 0.01f);
		ImGui::DragFloat("Frequency", &Frequency, 0.01f);

		if (ImGui::Button("Confirm"))
		{
			SetVibrate(Max, Min, Time, Frequency);
		}

		ImGui::TreePop();
	}
	ImGui::SetNextTreeNodeOpen(false, ImGuiSetCond_Once);
#endif
}

void ScaleVibrate::Start()
{
}

void ScaleVibrate::Update()
{
	if (!isTurnOn)return;
	if (timeCounter <= 0)
	{
		VibrateEnd();
		return;
	}

	Vector3 result = { 0,0,0 };
	
	result = (offsetMax - offsetMin) * 0.5f * (sinf((time - timeCounter) / frequency)) + (offsetMax + offsetMin) * 0.5f;

	result = (result - Vector3(1, 1, 1)) * (time - (time - timeCounter)) / time * -1.0f;

	result += Vector3(1, 1, 1);

	offset = result;

	timeCounter -= (1 / 60.0f);
}

void ScaleVibrate::VibrateEnd()
{
	isTurnOn = false;
	offset = { 1,1,1 };
	time = 0;
	timeCounter = 0;
}

void ScaleVibrate::SetVibrate(Vector3 offsetMax, Vector3 offsetMin, float time, float frequency)
{
	this->offsetMax = offsetMax;
	this->offsetMin = offsetMin;
	this->time = time;
	this->frequency = frequency;
	if (frequency < 0.01f)
	{
		frequency = 0.01f;
	}
	timeCounter = time;
	isTurnOn = true;
}

void ScaleVibrate::ImGuiUpdate()
{
	static Vector3 Max = { 1,1,1 };
	static Vector3 Min = { 1,1,1 };
	static float Time, Frequency;

#ifdef USE_IMGUI
	if (ImGui::TreeNode("ScaleVibrate"))
	{
		ImGui::DragFloat3("Max", &Max.x, 1.0f);
		ImGui::DragFloat3("Min", &Min.x, 1.0f);
		ImGui::DragFloat("Time", &Time, 0.01f);
		ImGui::DragFloat("Frequency", &Frequency, 0.01f);

		if (ImGui::Button("Confirm"))
		{
			SetVibrate(Max, Min, Time, Frequency);
		}

		ImGui::TreePop();
	}
	ImGui::SetNextTreeNodeOpen(false, ImGuiSetCond_Once);
#endif
}