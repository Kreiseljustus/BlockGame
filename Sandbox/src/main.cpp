
#include "Core/Application.h"
#include "minecraft/BlockGame.h"
#include "Rendering/Backends/OpenGLBackend.h"

#include <Rendering/PrimitiveProvider.h>

//TODO: Crash when ImGuiLayer exists anywhre in code

using namespace Engine;
using namespace Engine::Core;

int main() {
	Engine::Core::Application app{Engine::Core::ApplicationSpecification()};
	app.PushLayer(new BlockGame());
	app.Run();
}
