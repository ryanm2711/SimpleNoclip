#pragma once
#include "Main.h"

// Forward decelaration of class
class SimpleNoclip;

class UIManager {
public:
	UIManager(SimpleNoclip &noclip) : m_NoclipSystem(noclip) {};
	void Render();
private:
	SimpleNoclip& m_NoclipSystem;
};