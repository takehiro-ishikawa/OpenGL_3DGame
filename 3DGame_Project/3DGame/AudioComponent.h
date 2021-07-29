#pragma once
#include "Component.h"
#include "AudioSystem.h"
#include <vector>
#include <string>

#pragma region プロトタイプ宣言
class Actor;
class SoundEvent;
#pragma endregion

class AudioComponent : public Component
{
public:
	AudioComponent(Actor* owner, int updateOrder = 200);
	~AudioComponent();

	void Update(float deltaTime) override;
	void OnUpdateWorldTransform() override;

	SoundEvent PlayEvent(const std::string& name);
	void StopAllEvents();

private:
	std::vector<SoundEvent> mEvents2D;
	std::vector<SoundEvent> mEvents3D;
};
