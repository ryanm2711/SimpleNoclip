#include "Settings.h"

#pragma region Number Settings
float Settings::GetStartingSpeedModifier() const {
	return m_StartingSpeedModifier;
}

float Settings::GetMinNoclipSpeed() const {
	return m_MinNoclipSpeed;
}

float Settings::GetMaxNoclipSpeed() const {
	return m_MaxNoclipSpeed;
}
#pragma endregion

#pragma region Keybinds
DWORD Settings::GetNoclipKey() const {
	return m_NoclipKey;
}

DWORD Settings::GetUFOModeKey() const {
	return m_UFOModeToggleKey;
}

DWORD Settings::GetHideHUDKey() const
{
	return m_HideHUDKey;
}

DWORD Settings::GetPrecisionModeKey() const
{
	return m_PrecisionModeKey;
}

DWORD Settings::GetBoostKey() const
{
	return m_BoostKey;
}

DWORD Settings::GetMoveForwardKey() const {
	return m_MoveForwardKey;
}

DWORD Settings::GetMoveBackwardKey() const {
	return m_MoveBackwardsKey;
}

DWORD Settings::GetMoveRightKey() const {
	return m_MoveRightKey;
}
DWORD Settings::GetMoveLeftKey() const {
	return m_MoveLeftKey;
}
DWORD Settings::GetMoveUpKey() const
{
	return m_MoveUpKey;
}
DWORD Settings::GetMoveDownKey() const
{
	return m_MoveDownKey;
}
#pragma endregion