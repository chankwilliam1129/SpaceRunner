#pragma once
template<typename Manager>
class Singleton
{
public:
	static Manager* GetInstance()
	{
		static Manager instance;
		return &instance;
	}
};