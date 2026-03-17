#include "UIManager.h"
#include <natives.h>
#include <string>
#include <GTA/UI/Hud.h>

void UIManager::Render()
{
	float noclipSpeed = m_NoclipSystem.GetSpeedModifier();
	std::string msg = "Speed: " + std::to_string(noclipSpeed);
	std::string ufoModeMsg = "UFO Mode: " + std::to_string(m_NoclipSystem.IsUFOModeEnabled());

	static const float ALIGNED_WITH_RADAR_X = 0.015f;
	static const float ABOVE_RADAR_Y = 0.75f;
	static const float TEXT_SEPARATOR_Y = 0.05f;

	GTA::UI::ShowText(msg, ALIGNED_WITH_RADAR_X, ABOVE_RADAR_Y);
	GTA::UI::ShowText(ufoModeMsg, ALIGNED_WITH_RADAR_X, ABOVE_RADAR_Y - TEXT_SEPARATOR_Y);
}
