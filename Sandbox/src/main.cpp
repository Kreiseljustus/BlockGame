#include <algorithm>
#include <iostream>

#include <Window.h>
#include <Input.h>

#include <ResourceManager.h>
#include <stb_image.h>
#include <bits/this_thread_sleep.h>
#include <Rendering/Renderer.h>
#include <Rendering/Backends/OpenGLBackend.h>

#include <Rendering/PrimitiveProvider.h>

using namespace Engine::Rendering;
using namespace Engine;

struct Enemy {
	Transform t;
	float timeAlive;
	bool alive = true;
};

bool CheckAABBCollision(const Transform& a, const Transform& b) {
	float aHalfX = a.scale.x * 0.5f;
	float aHalfY = a.scale.y * 0.5f;
	float bHalfX = b.scale.x * 0.5f;
	float bHalfY = b.scale.y * 0.5f;

	bool overlapX = std::abs(a.position.x - b.position.x) < (aHalfX + bHalfX);
	bool overlapY = std::abs(a.position.y - b.position.y) < (aHalfY + bHalfY);

	return overlapX && overlapY;
}

struct Player {
	Transform t;
	float timeAlive = 0;
	bool alive = true;
};

int main(int arc, char* argv[]) {
	if (!glfwInit()) {
		std::cout << "Failed to initialize GLFW" << std::endl;
	}


	std::vector<Enemy> enemies;
	const int maxTimeAlivePerEnemy = 10;
	const float enemySpeed = 2.0f;

	float baseSpawnTime = 3.0f;
	float minSpawnTime = 0.15;
	float decayRate = 0.10f;

	float totalGameTime = 0.0f;
	float currentWaveTime = 0.0f;

	Player p;
	p.t.position = {0, -2, 0.5};
	p.t.rotation = {0,0,0};
	p.t.scale = {.25,.25,.25};

	float pSpeed = 2.0f;

	volatile bool useOpenGL = true;

	WindowProperties props;
	props.Width = 800;
	props.Height = 600;
	props.Hidden = false;
	props.Title = "Block Game";

	Window window = Window(props);
	window.create();

	ResourceManager::LoadShader("assets/shaders/BasicVertexShader.glsl", "assets/shaders/BasicFragmentShader.glsl", "basic");
	ShaderHandle test_Shader_handle = ResourceManager::GetShader("basic");

	TextureParameters tParams;
	int channels;
	tParams.imageData = stbi_load("assets/textures/test2.jpg", &tParams.width, &tParams.height, &channels, 4);

	auto backend = useOpenGL ? std::make_unique<OpenGLBackend>() : nullptr;
	Renderer renderer = Renderer(std::move(backend));

	TextureHandle tHandle = renderer.CreateTexture(tParams);
	MeshHandle quad = renderer.CreateMesh(GetUnitQuad());

	Input input;
	window.setInputHandler(&input);
	window.setKeyCallback(Input::keyCallback);
	window.setResizeCallback(Input::resizeCallback);

	Camera camera;
	camera.projection = ProjectionType::Perspective;
	camera.position = {0,0,5};
	camera.orthoSize = 3.0f;
	camera.aspect = static_cast<float>(window.getFrameBufferSize().x) / static_cast<float>(window.getFrameBufferSize().y);

	double lastTime = glfwGetTime();

	srand(rand() * time(nullptr));

	while (!window.shouldClose()) {
		double now = glfwGetTime();
		auto deltaTime = static_cast<float>(now - lastTime);
		lastTime = now;
		totalGameTime += deltaTime;

		std::string title = "Block Game ";
		title.append(std::to_string(1.0f/deltaTime));
		glfwSetWindowTitle(window.getWindow(), title.c_str());

		float spawnInterval = std::max(minSpawnTime, baseSpawnTime * std::exp(-decayRate * totalGameTime));

		glfwPollEvents();

		if (input.IsKeyDown(GLFW_KEY_A)) {
			p.t.position.x -= deltaTime * pSpeed;
		}
		if (input.IsKeyDown(GLFW_KEY_D)) {
			p.t.position.x += deltaTime * pSpeed;
		}
		if (input.IsKeyDown(GLFW_KEY_W)) p.t.position.y += deltaTime * pSpeed;
		if (input.IsKeyDown(GLFW_KEY_S)) p.t.position.y -= deltaTime * pSpeed;

		const float boundX = 2.75f * camera.aspect;
		const float boundY = 2.15f * camera.aspect;

		p.t.position.x = std::clamp(p.t.position.x, -boundX, boundX);
		p.t.position.y = std::clamp(p.t.position.y, -boundY, boundY);

		p.timeAlive += deltaTime;

		camera.aspect = static_cast<float>(window.getFrameBufferSize().x) / static_cast<float>(window.getFrameBufferSize().y);

		currentWaveTime += deltaTime;

		if (currentWaveTime >= spawnInterval) {
			currentWaveTime = 0;
			Enemy e;
			int range = 5;
			float x = ((rand() / static_cast<float>(RAND_MAX)) * 2.0f - 1.0f) * range;
			e.t.position = {x, 5, 0.5f};
			e.t.rotation = {0,0,0};
			e.t.scale = {1,1,1};
			e.alive = true;
			e.timeAlive = 0;
			enemies.push_back(e);
		}

		for (int e = 0; e < enemies.size(); e++) {
			enemies[e].timeAlive += deltaTime;
			if (enemies[e].timeAlive >= maxTimeAlivePerEnemy) {
				enemies.at(e).alive = false;
			}
			enemies.at(e).t.position.y -= enemySpeed * deltaTime;
			if (CheckAABBCollision(enemies.at(e).t, p.t)) {
				glfwSetWindowShouldClose(window.getWindow(), true);
			}
		}

		enemies.erase(
	std::remove_if(enemies.begin(), enemies.end(), [](const Enemy& e) { return !e.alive; }),
	enemies.end()
		);

		renderer.Begin(camera);
		for (Enemy& e : enemies) {
			if (e.alive) {
				renderer.Submit(quad, {test_Shader_handle, {0}}, e.t);
			}
		}

		renderer.Submit(quad, {test_Shader_handle, tHandle}, p.t);
		renderer.End();

		glfwSwapBuffers(window.getWindow());
	}

	std::cout << "GAME OVER!" << std::endl;
	std::cout << "Score: " << p.timeAlive << std::endl;

	glfwTerminate();

	return 0;
}
