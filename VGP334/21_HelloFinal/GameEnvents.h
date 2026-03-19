#pragma once

#include <NardaEngine/Inc/SpEngine.h>


class GameEvents
{
public:
	using Callback = std::function<void()>;

	static void SubscribeThunder(const Callback& callback)
	{
		mThunderCallbacks.push_back(callback);
	}

	static void BroadcastThunder()
	{
		for (auto& cb : mThunderCallbacks)
		{
			cb();
		}
	}

private:
	inline static std::vector<Callback> mThunderCallbacks;
};