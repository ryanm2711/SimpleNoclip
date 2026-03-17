#pragma once
#include "UIManager.h"
#include "Settings.h"

#include <main.h>
#include <Utility/Math.h>
#include <Utility/Console.h>

class UIManager;

enum class CamViewModes : int {
	ThirdPersonClose,
	ThirdPersonMid,
	ThirdPersonFar,
	FirstPerson
};

class SimpleNoclip {
public:
	void Init();
	void Update();
	void OnShutdown();

	inline bool IsNoclipping() const {
		return m_Noclipping;
	}

	inline bool IsUFOModeEnabled() const {
		return m_UFOMode;
	}

	inline float GetSpeedModifier() const {
		return m_SpeedModifier;
	}

	inline float GetUnboostedSpeedModifier() const {
		return m_UnboostedSpeedModifier;
	}

	inline float GetMaxSpeed() const {
		return m_MaxSpeed;
	}

	inline float GetMinSpeed() const {
		return m_MinSpeed;
	}
private:
	void EnterNoclip();
	void DoNoclip();
	void ExitNoclip();

	Vector3 GetNoclipInput();
	void HandleSpeedModifier();
	void DisableControls();

	std::unique_ptr<UIManager> ptrUIManager;
	std::unique_ptr<Settings> ptrSettings;

	float m_SpeedModifier;
	float m_UnboostedSpeedModifier;
	float m_MinSpeed;
	float m_MaxSpeed;

	DWORD m_MoveForwardKey;
	DWORD m_MoveBackwardKey;
	DWORD m_MoveRightKey;
	DWORD m_MoveLeftKey;
	DWORD m_NoclipKey;
	DWORD m_UFOModeKey;
	DWORD m_BoostKey;
	DWORD m_MoveUpKey;
	DWORD m_MoveDownKey;

	bool m_Noclipping;

	bool m_UFOMode;
};