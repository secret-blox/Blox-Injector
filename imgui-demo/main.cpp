#include "gui.h"

#include <thread>

int __stdcall wWinMain(
	HINSTANCE instance,
	HINSTANCE previousInstance,
	PWSTR arguments,
	int commandShow
) {
	gui::CreateHWindow("hello", "InjectorClass");
	gui::CreateDevice();
	gui::CreateImGui();

	while (gui::exit) {
		gui::BeginRender();
		gui::Render();
		gui::EndRender();

		Sleep(10);
	}

	gui::DestroyImGui();
	gui::DestroyDevice();
	gui::DestroyHWindow();

	return EXIT_SUCCESS;
}