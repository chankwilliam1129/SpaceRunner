#include "audio_source.h"

void AudioSource::Update()
{
	for (auto& it : Data)
	{		
		if (saveVolume != it.second->volume)
		{
			saveVolume = it.second->volume;
			SoundManager::GetInstance()->Data[it.first]->SetVolume(it.second->volume);
		}

		if (savePan != it.second->pan)
		{
			savePan = it.second->pan;
			SoundManager::GetInstance()->Data[it.first]->SetPan(it.second->pan);
		}
	}
}

void AudioSource::ImGuiUpdate()
{
#ifdef USE_IMGUI
	if (ImGui::TreeNode("AudioSource"))
	{
		for (auto& it : Data)
		{
			if (ImGui::TreeNode(it.first.c_str()))
			{
				if (ImGui::Button("play / stop"))
				{
					if (it.second->isPlay) { Stop(it.first); }
					else { Play(it.first); }
				}
				ImGui::Checkbox("isLoop", &it.second->isLoop);
				ImGui::DragFloat("volume", &it.second->volume, 0.01f, 0.0f, 1.0f);
				ImGui::DragFloat("pan", &it.second->pan, 0.01f, -1.0f, 1.0f);
				ImGui::SetNextTreeNodeOpen(false, ImGuiSetCond_Once);
				ImGui::TreePop();
			}
		}	
		ImGui::TreePop();
	}
#endif
}

void AudioSource::Instance(std::string name, bool isLoop, float volume, float pan)
{
	Data[name] = new AudioSourceData();
	Data[name]->isLoop = isLoop;
	Data[name]->volume = volume;
	Data[name]->pan    = pan;
}

void AudioSource::Play(std::string name)
{
	Data[name]->isPlay = true;
	SoundManager::GetInstance()->Data[name]->Play(Data[name]->isLoop);
}

void AudioSource::Stop(std::string name)
{
	Data[name]->isPlay = false;
	SoundManager::GetInstance()->Data[name]->Stop();
}

void AudioSource::SetVolume(std::string name, float volume)
{
	Data[name]->volume = volume;
}

void AudioSource::SetPan(std::string name, float pan)
{
	Data[name]->pan = pan;
}