
#include "Core/Application.h"
#include "minecraft/BlockGame.h"
#include "audioVisualizer/AVLayer.h"

//TODO: Crash when ImGuiLayer exists anywhre in code

using namespace Engine;
using namespace Engine::Core;

int main() {
	Application app{ApplicationSpecification()};
	app.PushLayer(new AVLayer());
	app.Run();
}
