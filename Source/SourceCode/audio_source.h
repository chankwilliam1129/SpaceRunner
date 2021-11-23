#pragma once
#include "../GameLib/sound.h"
#include "component.h"

class AudioSourceData
{
public:
	float volume = { 1.0f };
	float pan    = {  .0f };
	bool isPlay  = false;
	bool isLoop  = false;
};

class AudioSource :public Component
{
	std::map<std::string, AudioSourceData*, std::less<>> Data;
	float saveVolume;
	float savePan;

public:
	AudioSource() {};
	~AudioSource() {};

public:
	void Update();
	void ImGuiUpdate();

public:
	void Instance(std::string name, bool isLoop = (false), float volume = (1.0f), float pan = (.0f));
	void Play(std::string name);
	void Stop(std::string name);
	void SetVolume(std::string name, float volume);
	void SetPan(std::string name, float pan);
};