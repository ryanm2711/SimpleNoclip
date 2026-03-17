#pragma once

#include <Utility/ScriptSettings.h>

static const wchar_t* KEYBINDINGS_SECTION = L"Keybindings";
static const wchar_t* GENERAL_SECTION = L"General";

class Settings : public Utility::ScriptSettings {
public:
	Settings(std::string iniPath) : ScriptSettings(iniPath) {
		m_StartingSpeedModifier = 10.0f;
		m_SpeedBoostMultiplier = 2.0f;
		m_MinNoclipSpeed = 1.0f;
		m_MaxNoclipSpeed = 1000.0f;
		m_NoclipKey = 0x4E; // N key by default
		m_UFOModeToggleKey = 0x47; // G key by default

		// Movement
		m_MoveForwardKey = 0x57; // W key
		m_MoveBackwardsKey = 0x53; // S key
		m_MoveRightKey = 0x44; // D key
		m_MoveLeftKey = 0x41; // A key
		m_MoveUpKey = VK_SPACE;
		m_MoveDownKey = VK_CONTROL;
		m_BoostKey = VK_SHIFT;

		RegisterSetting<float>(GENERAL_SECTION, L"StartingSpeedModifier", m_StartingSpeedModifier, &m_StartingSpeedModifier, L"# Default starting speed when the script initialises.");
		RegisterSetting<float>(GENERAL_SECTION, L"SpeedBoostMultiplier", m_SpeedBoostMultiplier, &m_SpeedBoostMultiplier, L"# How much faster do you want to go when holding boost key.");
		RegisterSetting<float>(GENERAL_SECTION, L"MinNoclipSpeed", m_MinNoclipSpeed, &m_MinNoclipSpeed, L"# Minimum noclip speed that you can go.");
		RegisterSetting<float>(GENERAL_SECTION, L"MaxNoclipSpeed", m_MaxNoclipSpeed, &m_MaxNoclipSpeed, L"# Maximum noclip speed that you can go. (I have only tested up to 1000.0, if too fast you may crash.)");

		RegisterSetting<DWORD>(KEYBINDINGS_SECTION, L"NoclipKey", m_NoclipKey, &m_NoclipKey, L"# Key to go in & out of noclip mode. (Default: N)");
		RegisterSetting<DWORD>(KEYBINDINGS_SECTION, L"UFOModeToggleKey", m_UFOModeToggleKey, &m_UFOModeToggleKey, L"# Toggles UFO mode, meaning the camera does not influence moving vertically. (Default: G)");
		
		RegisterSetting<DWORD>(KEYBINDINGS_SECTION, L"MoveForwardsKey", m_MoveForwardKey, &m_MoveForwardKey, L"# The key that moves you forward when noclipped. (Default: W)");
		RegisterSetting<DWORD>(KEYBINDINGS_SECTION, L"MoveBackwardsKey", m_MoveBackwardsKey, &m_MoveBackwardsKey, L"# The key that moves you backwards when noclipped. (Default: S)");
		RegisterSetting<DWORD>(KEYBINDINGS_SECTION, L"MoveRightKey", m_MoveRightKey, &m_MoveRightKey, L"# The key that moves you right when noclipped. (Default: D)");
		RegisterSetting<DWORD>(KEYBINDINGS_SECTION, L"MoveLeftKey", m_MoveLeftKey, &m_MoveLeftKey, L"# The key that moves you left when noclipped. (Default: A)");
		RegisterSetting<DWORD>(KEYBINDINGS_SECTION, L"MoveUpKey", m_MoveUpKey, &m_MoveUpKey, L"# The key that moves you up vertically when noclipped. (Default: SPACE)");
		RegisterSetting<DWORD>(KEYBINDINGS_SECTION, L"MoveDownKey", m_MoveDownKey, &m_MoveDownKey, L"# The key that moves you down vertically when noclipped. (Default: CTRL)");
		RegisterSetting<DWORD>(KEYBINDINGS_SECTION, L"BoostKey", m_MoveDownKey, &m_MoveDownKey, L"# The key that moves you faster when held down. (Default: SHIFT)");
	};

	float GetStartingSpeedModifier() const;
	float GetMinNoclipSpeed() const;
	float GetMaxNoclipSpeed() const;

	DWORD GetBoostKey() const;
	DWORD GetNoclipKey() const;
	DWORD GetUFOModeKey() const;
	DWORD GetMoveForwardKey() const;
	DWORD GetMoveBackwardKey() const;
	DWORD GetMoveRightKey() const;
	DWORD GetMoveLeftKey() const;
	DWORD GetMoveUpKey() const;
	DWORD GetMoveDownKey() const;
private:
	float m_StartingSpeedModifier;
	float m_SpeedBoostMultiplier;
	float m_MinNoclipSpeed;
	float m_MaxNoclipSpeed;
	
	DWORD m_MoveForwardKey;
	DWORD m_MoveBackwardsKey;
	DWORD m_MoveRightKey;
	DWORD m_MoveLeftKey;
	DWORD m_MoveUpKey;
	DWORD m_MoveDownKey;
	DWORD m_NoclipKey;
	DWORD m_UFOModeToggleKey;
	DWORD m_BoostKey;
};