#pragma once
#include <Windows.h>
#include <iostream>
#include <string>
#include <fstream>

#include "gui.h"

namespace injector {
	inline HHOOK hookHandle;
	inline bool injected = false;

	inline bool consoleAllocated = false;
}

int inject();
void unloadDll();
void checkConsoleAllocation();