#include "Main.h"

#include <GTA/Controls/Controls.h>
#include <iostream>
#include <natives.h>
#include <algorithm>

void SimpleNoclip::Init()
{
	ptrSettings = std::make_unique<Settings>("SimpleNoclip.ini");
	ptrSettings->Load();

	ptrUIManager = std::make_unique<UIManager>(*this);

	// Load settings to script
	m_SpeedModifier = ptrSettings->GetStartingSpeedModifier();
	m_UnboostedSpeedModifier = m_SpeedModifier;
	m_MaxSpeed = ptrSettings->GetMaxNoclipSpeed();
	m_MinSpeed = ptrSettings->GetMinNoclipSpeed();

	m_NoclipKey = ptrSettings->GetNoclipKey(); // N key
	m_UFOModeKey = ptrSettings->GetUFOModeKey();
	m_PrecisionModeKey = ptrSettings->GetPrecisionModeKey();
	m_HideHUDKey = ptrSettings->GetHideHUDKey();
	m_BoostKey = ptrSettings->GetBoostKey();
	m_MoveForwardKey = ptrSettings->GetMoveForwardKey();
	m_MoveBackwardKey = ptrSettings->GetMoveBackwardKey();
	m_MoveRightKey = ptrSettings->GetMoveRightKey();
	m_MoveLeftKey = ptrSettings->GetMoveLeftKey();
	m_MoveUpKey = ptrSettings->GetMoveUpKey();
	m_MoveDownKey = ptrSettings->GetMoveDownKey();

	m_Noclipping = false;
	m_UFOMode = false;
}

void SimpleNoclip::Update()
{
	if (GTA::Controls::IsKeyJustUp(m_NoclipKey)) {
		m_Noclipping = !m_Noclipping;

		if (m_Noclipping) {
			EnterNoclip();
		}
		else {
			ExitNoclip();
		}
	}

	if (m_Noclipping) {
		if (GTA::Controls::IsKeyJustUp(m_UFOModeKey)) {
			m_UFOMode = !m_UFOMode;
		}

		if (GTA::Controls::IsKeyJustUp(m_HideHUDKey)) {
			m_HideHUD = !m_HideHUD;
		}

		HandleSpeedModifier();
		DoNoclip();

		if (!IsHUDHidden()) {
			ptrUIManager->Render();
		}
		else {
			HUD::HIDE_HUD_AND_RADAR_THIS_FRAME();
		}
	}
}

void SimpleNoclip::OnShutdown()
{
	if (IsNoclipping()) {
		ExitNoclip();
	}
}

// Used to set things up like camera
void SimpleNoclip::EnterNoclip()
{
	Player playerID = PLAYER::PLAYER_ID();
	Ped playerPed = PLAYER::PLAYER_PED_ID();
	std::cout << "Enter noclip setup!" << "\n";

	CAM::SET_FOLLOW_PED_CAM_VIEW_MODE(4); // First person
	ENTITY::FREEZE_ENTITY_POSITION(playerPed, true);
	ENTITY::SET_ENTITY_VISIBLE(playerPed, false, false);
	ENTITY::SET_ENTITY_COLLISION(playerPed, false, false);

	PLAYER::SET_POLICE_IGNORE_PLAYER(playerID, true);
	PLAYER::SET_EVERYONE_IGNORE_PLAYER(playerID, true);
}

Vector3 SimpleNoclip::GetNoclipInput() {
	Vector3 inputVec{};
	if (GTA::Controls::IsKeyDown(m_MoveForwardKey)) { // W key (forward)
		inputVec.y = 1;
	}
	else if (GTA::Controls::IsKeyDown(m_MoveBackwardKey)) { // S key (backwards
		inputVec.y = -1;
	}

	if (GTA::Controls::IsKeyDown(m_MoveRightKey)) { // D key (right)
		inputVec.x = 1;
	}
	else if (GTA::Controls::IsKeyDown(m_MoveLeftKey)) { // A key (left)
		inputVec.x = -1;
	}

	// Moving up and down vertically
	if (GTA::Controls::IsKeyDown(m_MoveUpKey)) {
		inputVec.z = 1;
	}
	else if (GTA::Controls::IsKeyDown(m_MoveDownKey)) {
		inputVec.z = -1;
	}

	return inputVec;
}

Vector3 GetForwardVector(Vector3 vec, bool ufoMode) {
	float heading = Utility::Math::DegreesToRadians(vec.z);
	float pitch{ 0.0f };
	if (!ufoMode) {
		pitch = sin(Utility::Math::DegreesToRadians(vec.x));
	}
	return Vector3{-sin(heading), cos(heading), pitch };
}

Vector3 GetRightVector(Vector3 vec, bool ufoMode) {
	Vector3 forward = GetForwardVector(vec, ufoMode);
	return Vector3{ forward.y, -forward.x, 0.0f };
}

void SimpleNoclip::DoNoclip()
{
	DisableControls();

	Ped playerPed = PLAYER::PLAYER_PED_ID();
	
	Vector3 playerPos = ENTITY::GET_ENTITY_COORDS(playerPed, false);
	Vector3 camRot = CAM::GET_GAMEPLAY_CAM_ROT(0);

	Vector3 inputVec = GetNoclipInput();

	// Handle cam rotation and move in right direction
	Vector3 camForward = GetForwardVector(camRot, IsUFOModeEnabled()) * inputVec.y;
	Vector3 camRight = GetRightVector(camRot, IsUFOModeEnabled()) * inputVec.x;
	Vector3 moveDirection = camForward + camRight;
	moveDirection.z += inputVec.z;

	// Fixes faster movement if going diagonally
	moveDirection = moveDirection.Normalize();

	// Calculate new position from our movement direction and current pos.
	Vector3 newPosition = playerPos + (moveDirection * GetSpeedModifier() * MISC::GET_FRAME_TIME());

	ENTITY::SET_ENTITY_HEADING(playerPed, camRot.z);
	ENTITY::SET_ENTITY_COORDS_NO_OFFSET(playerPed, newPosition.x, newPosition.y, newPosition.z, false, false, false);
}

void SimpleNoclip::ExitNoclip()
{
	// Force set to false, prevent others from depending on it like UIManager
	m_Noclipping = false;

	Player playerID = PLAYER::PLAYER_ID();
	Ped playerPed = PLAYER::PLAYER_PED_ID();

	std::cout << "Exit noclip setup!" << "\n";
	CAM::SET_FOLLOW_PED_CAM_VIEW_MODE(1); // Third person

	ENTITY::FREEZE_ENTITY_POSITION(playerPed, false);
	ENTITY::SET_ENTITY_VISIBLE(playerPed, true, false);
	ENTITY::SET_ENTITY_COLLISION(playerPed, true, true);

	PLAYER::SET_POLICE_IGNORE_PLAYER(playerID, false);
	PLAYER::SET_EVERYONE_IGNORE_PLAYER(playerID, false);
}

void SimpleNoclip::HandleSpeedModifier()
{
	static const float SPEED_BOOST_MODIFIER = 2.0f;
	static const float MAX_INCREMENT_SPEED = 20.0f;
	static const float MIN_INCREMENT_SPEED = 1.0f;
	float incrementSpeed = MAX_INCREMENT_SPEED;

	float baseSpeed = GetUnboostedSpeedModifier();

	float incrementSpeedPercentage = baseSpeed / GetMaxSpeed(); // Scale increment depending on how fast/slow currently going
	incrementSpeed *= incrementSpeedPercentage;

	if (incrementSpeed < MIN_INCREMENT_SPEED) {
		incrementSpeed = MIN_INCREMENT_SPEED;
	}

	if (GTA::Controls::IsControlPressed(GTA::Controls::ControlActionSets::FrontendControls, GTA::Controls::ControlTypes::CursorScrollUp)) {
		baseSpeed += incrementSpeed;
	}
	else if (GTA::Controls::IsControlPressed(GTA::Controls::ControlActionSets::FrontendControls, GTA::Controls::ControlTypes::CursorScrollDown)) {
		baseSpeed -= incrementSpeed;
	}

	float clampedBaseSpeed = std::clamp(baseSpeed, GetMinSpeed(), GetMaxSpeed());
	m_UnboostedSpeedModifier = clampedBaseSpeed;
	m_SpeedModifier = clampedBaseSpeed;

	//Utility::Log("Is player holding down forward key: ", GTA::Controls::IsKeyDown(m_MoveForwardKey));

	if (GTA::Controls::IsKeyDown(VK_SHIFT)) {
		float boostedSpeed = GetUnboostedSpeedModifier() * SPEED_BOOST_MODIFIER;
		m_SpeedModifier = std::clamp(boostedSpeed, GetMinSpeed(), GetMaxSpeed());
	}
	if (GTA::Controls::IsKeyDown(m_PrecisionModeKey)) {
		m_SpeedModifier *= 0.1f;
		m_SpeedModifier = std::clamp(m_SpeedModifier, GetMinSpeed(), GetMaxSpeed());
	}
}

void SimpleNoclip::DisableControls()
{
	// Disable weapon wheel controls while noclipping (So we can use scroll wheel without it appearing)
	GTA::Controls::DisableControlThisFrame(GTA::Controls::ControlActionSets::PlayerControls, GTA::Controls::ControlTypes::SelectWeapon, true);

	GTA::Controls::DisableControlThisFrame(GTA::Controls::ControlActionSets::PlayerControls, GTA::Controls::ControlTypes::CharacterWheel);
	GTA::Controls::DisableControlThisFrame(GTA::Controls::ControlActionSets::PlayerControls, GTA::Controls::ControlTypes::Phone);	
	GTA::Controls::DisableControlThisFrame(GTA::Controls::ControlActionSets::PlayerControls, GTA::Controls::ControlTypes::Duck);
}
