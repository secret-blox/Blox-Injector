#include "injector.h"
#include <cstdarg>
#include <cstdio> 
#include <iostream>

void logInfo(const char* format, ...) {
	if (true) {
		const int BUFFER_SIZE = 1024;
		char buffer[BUFFER_SIZE];

		va_list args;
		va_start(args, format);

		// Capture the length of the formatted string
		int length = vsnprintf(buffer, sizeof(buffer), format, args);

		va_end(args);

		if (length > 0) {
			HANDLE consoleHandle = GetStdHandle(STD_OUTPUT_HANDLE);
			DWORD charsWritten;
			WriteConsole(consoleHandle, buffer, length, &charsWritten, NULL);
		}
	}
}

int inject() {
	HWND processWindow = FindWindow(NULL, "Roblox");
	if (processWindow == NULL) {
		MessageBox(NULL, "Roblox is not opened!", "ERROR", MB_OK);
		return EXIT_FAILURE;
	}

	logInfo("Window Handle: %p \n", processWindow);

	DWORD processId = NULL;
	DWORD threadProcessId = GetWindowThreadProcessId(processWindow, &processId);
	if (threadProcessId == NULL) {
		if (gui::debugMode) {
			MessageBox(NULL, "Injection failed!", "ERROR", MB_OK);
		}
		else {
			MessageBox(NULL, "Could not get thread id of the window!", "ERROR", MB_OK);
		}
		return EXIT_FAILURE;
	}

	logInfo("Thread Process ID: %d \n", threadProcessId);

	// Replace with your DLL path niggas
	HMODULE dllToInject = LoadLibraryEx("C:\\Users\\Pixeluted\\source\\repos\\RobloxInjector\\x64\\Release\\target.dll", NULL, DONT_RESOLVE_DLL_REFERENCES);
	if (dllToInject == NULL) {
		MessageBox(NULL, "Could not load DLL!", "ERROR", MB_OK);
		return EXIT_FAILURE;
	}

	logInfo("DLL Handle: %p\n", dllToInject);

	HOOKPROC nextHookAddress = (HOOKPROC)GetProcAddress(dllToInject, "NextHook");
	if (nextHookAddress == NULL) {
		if (gui::debugMode) {
			MessageBox(NULL, "Invalid DLL!", "ERROR", MB_OK);
		}
		else {
			MessageBox(NULL, "Nexthook is not defined in target DLL!", "ERROR", MB_OK);
		}
		return EXIT_FAILURE;
	}

	logInfo("NextHook Address: %p\n", nextHookAddress);

	injector::hookHandle = SetWindowsHookEx(WH_GETMESSAGE, nextHookAddress, dllToInject, threadProcessId);
	if (injector::hookHandle == NULL) {
		if (gui::debugMode) {
			MessageBox(NULL, "Injection failed!", "ERROR", MB_OK);
		}
		else {
			MessageBox(NULL, "Could not set hook!", "ERROR", MB_OK);
		}
		return EXIT_FAILURE;
	}

	PostThreadMessage(threadProcessId, WM_NULL, NULL, NULL);
	injector::injected = true;
	return 0;
}

void unloadDll() {
	UnhookWindowsHookEx(injector::hookHandle);
	injector::injected = false;
}

void checkConsoleAllocation() {
	if (gui::debugMode && injector::consoleAllocated == false) {
		AllocConsole();
		injector::consoleAllocated = true;
	}
	else if (gui::debugMode == false && injector::consoleAllocated == true) {
		FreeConsole();
		injector::consoleAllocated = false;
	}
}