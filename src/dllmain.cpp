// dllmain.cpp : Defines the entry point for the DLL application.
#include "core/Main.h"
#include <GTA/Controls/Keyboard.h>

static SimpleNoclip simpleNoclip;

#ifdef _DEBUG
static bool g_EnableDebugConsole = true;
#else
static bool g_EnableDebugConsole = false;
#endif

void ScriptMain() {
    if (g_EnableDebugConsole) {
        Utility::CreateDebugConsole();
    }

    simpleNoclip.Init();

    while (true) {
        simpleNoclip.Update();
        WAIT(0);
    }
}

BOOL APIENTRY DllMain(HMODULE hInstance, DWORD reason, LPVOID lpReserved) {
	switch (reason) {
	case DLL_PROCESS_ATTACH:
		scriptRegister(hInstance, ScriptMain);
		keyboardHandlerRegister(GTA::Controls::OnKeyboardMessage);
		break;
	case DLL_PROCESS_DETACH:
		if (g_EnableDebugConsole) {
			Utility::CloseDebugConsole();
		}

		simpleNoclip.OnShutdown();
		scriptUnregister(hInstance);
		keyboardHandlerUnregister(GTA::Controls::OnKeyboardMessage);
		break;
	}

	return TRUE;
}
