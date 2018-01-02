#pragma once
#include "RenderComponent.h"
#include "PhysicsComponent.h"
#include "InputComponent.h"
#include "SpaceObject.h"
#include <vector>
#include <type_traits>
#include <future>
#include "Vector2D.h"
#include "Vector3d.h"
#include <atomic>
#include "amp_physics.h"
#include "Screen.h"
#include "Intersect.h"
#include <map>
using namespace std;
using namespace GameLib;

class GravityGame
{

private:

	typedef int FPS_Start;
	typedef unsigned int FramesPerSecond;

	static bool isRunning;
	static bool paused;
	FPS_Start start;
	static Screen theScreen;
	Vector2D translation;
	static atomic<unsigned int> selected;
	static bool left;
	static bool right;
	static const FramesPerSecond FPS = 1;
	static const int MAX_COMPONENTS = 500;
	static Vector2D mouseLoc;
	static Vector2D viewPoint;
	// stubs
	void addObjects(Vector3d translation, const SpaceObject& centerPoint);
	// end of stubs
	template<typename t> void loadComponent(vector<t>& vec)
	{
		if (!is_base_of<Component, t>::value)
			throw invalid_argument("Must be a Component type.");

		for (int i = 0; i < MAX_COMPONENTS; ++i)
		{
			vec.emplace_back();
		}
	}

	//inline bool setFPS()
	//{
	//	if (1000 / FPS >(SDL_GetTicks() - start))
	//	{
	//		//SDL_Delay(1000 / FPS - (SDL_GetTicks() - start));
	//		return true;
	//	}

	//	start = SDL_GetTicks();
	//	return false;
	//}


public:

	static vector<SpaceObject> spaceObjects;
	static vector<RenderComponent> renderComponents;
	static vector<PhysicsComponent> physicsComponents;
	static std::unique_ptr< concurrency::array<amp_physics::amp_physics_component, 1> > amp_physicsComponents;
	static vector<size_t> lightPositions;

	GravityGame();
	~GravityGame();

	void init()
	{
		theScreen.initializeScreen();
	}

	void play()
	{
		selected = 0;
		left = false;
		right = false;
		int keyDown = 0;
		isRunning = true;
		paused = false;
		glutDisplayFunc(renderObjects);
		glutTimerFunc(33, timerFunc, 33);
		glutSpecialFunc(keyCallBack);
		glutKeyboardFunc(keyCallBack);
		glutMouseFunc(mouseCallBack);
		glutMotionFunc(moveMouse);
		glutMainLoop();
	}

	static void moveMouse(int x, int y)
	{
			mouseLoc.x = x;
			mouseLoc.y = y;
	}

	static void mouseCallBack(int button, int state, int x, int y) {

		moveMouse(x, y);
		if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN) {
			left = true;
		}
		else if(left && button == GLUT_LEFT_BUTTON && state == GLUT_UP)
			left = false;
		if (button == GLUT_RIGHT_BUTTON && state == GLUT_DOWN){
			right = true;
		}
		else if (right && button == GLUT_RIGHT_BUTTON && state == GLUT_UP){
			right = false;
		}
   }

	static void keyCallBack(unsigned char key, int x, int y)
	{
		switch (key) 
		{
			case 27: // ASCII CANCEL (ESCAPE KEY)
				isRunning = false;
				break;
				
			case 112:
				paused = !paused;
				break;
			case 'd':
				theScreen.camera->roll(0.1f);
				break; 
			case 'a':
				theScreen.camera->roll(-0.1f);
				break;
			case 'w':
				theScreen.camera->pitch(0.1f);
				break;
			case 's':
				theScreen.camera->pitch(-0.1f);
				break;
			case 'e':
				theScreen.camera->yaw(0.1f);
				break;
			case 'q':
				theScreen.camera->yaw(-0.1f);
				break;
		}
	}

	static void keyCallBack(int key, int x, int y)
	{
		const int MOVEMENT_SPEED = 30;
		switch (key)
		{
		case GLUT_KEY_UP:       // Up: increase y speed
			GravityGame::theScreen.camera->translate(0, 0, MOVEMENT_SPEED); break;
		case GLUT_KEY_DOWN:     // Down: decrease y speed
			GravityGame::theScreen.camera->translate(0, 0, -MOVEMENT_SPEED); break;
		case GLUT_KEY_LEFT:
			GravityGame::theScreen.camera->translate(MOVEMENT_SPEED, 0, 0);
			break;
		case GLUT_KEY_RIGHT:
			GravityGame::theScreen.camera->translate(-MOVEMENT_SPEED,0,0); break;
		}
	}

	static void selectPreviousObject(){
		if (GravityGame::selected == 0)
			GravityGame::selected = GravityGame::spaceObjects.size() - 1;
		else
			--GravityGame::selected;
	}

	static void selectNextObject(){
		if (GravityGame::selected == GravityGame::spaceObjects.size()-1)
			GravityGame::selected = 0;
		else
			++GravityGame::selected;
	}

	static void timerFunc(int millis)
	{
		if (!isRunning){
			glutDestroyWindow(GravityGame::theScreen.getScreenId());
			exit(0);
		}
		glutPostRedisplay();
		glutTimerFunc(millis, timerFunc, millis);
	}

	static void asyncUpdate()
	{
		vector<future<void>> f;

		for (auto& p : physicsComponents)
			f.push_back(async(&PhysicsComponent::Update2, &p, &physicsComponents));

		for(auto& fut : f)
			fut.get();
	}

	static void amp_update(){
		using namespace concurrency;
		//concurrency::array<amp_physics::amp_physics_component,1> amp_physics_array(physicsComponents.size(), physicsComponents.begin(), physicsComponents.end());
		concurrency::array<amp_physics::amp_physics_component, 1>& amp_physics_array = *amp_physicsComponents;
		parallel_for_each(amp_physics_array.extent,
			[=, &amp_physics_array](index<1> idx) restrict(amp){ amp_physics_array[idx].Update(amp_physics_array.data(), amp_physics_array.extent[0]); });
		
		parallel_for_each(amp_physics_array.extent,
			[=, &amp_physics_array](index<1> idx) restrict(amp){ amp_physics_array[idx].move(); });

		concurrency::copy(amp_physics_array, begin(physicsComponents));
	}

	static void updateComponents()
	{
		amp_update();
	}

	static void renderObjects()
	{
		if (!paused){
			updateComponents();
			theScreen.rotation += 0.01f;
		}
		if (left) {
			left = false;
			glm::vec4 viewport = theScreen.glViewPort();
			glm::vec3 mouseLocation{ mouseLoc.x, viewport[3] - mouseLoc.y, 0.1f };
			glReadPixels(mouseLocation.x, mouseLocation.y, 1, 1, GL_DEPTH_COMPONENT, GL_FLOAT, &mouseLocation.z);
			const glm::mat4& view = theScreen.camera->view();
			const glm::vec3& cameraPosition = theScreen.camera->position();
			glm::vec3 worldCoordinates = glm::unProject(mouseLocation, view, theScreen.projection, viewport);
			glm::vec3 diff = worldCoordinates - cameraPosition;
			glm::Ray ray{ cameraPosition, diff };
			std::map<float, glm::vec3> intersectPoints;
			for (auto& p : physicsComponents) {
				glm::vec3 objectLocation{ p.location.x, p.location.y, p.location.z };
				glm::Sphere sp{ objectLocation, p.radius };
				if (glm::intersects(ray, sp)) {
					float distance = glm::distance(cameraPosition, objectLocation);
					intersectPoints[distance] = objectLocation;
				}
			}
			if (intersectPoints.size()) {
				auto location = -intersectPoints.begin()->second;
				location = glm::normalize(cameraPosition - location) * 500.0f + location;
				theScreen.camera->setPosition(location);
			}
		}
		theScreen.refresh();
		for (auto& o : spaceObjects)
		{
			theScreen.render(o);
		}
		theScreen.update();
	}
	
	static PhysicsComponent& getPhysicsComponent(const SpaceObject& s)
	{
		return physicsComponents[s.physicsComponent];
	}

	static RenderComponent& getRenderComponent(const SpaceObject& s)
	{
		return renderComponents[s.renderComponent];
	}

	static SpaceObject& createPlanet(Vector3d location, Vector3d velocity, unsigned int mass)
	{
		const std::array<float, 3> color = createColorFromRGB(65, 86, 197);
		float radius =50;
		physicsComponents.push_back(PhysicsComponent(location, velocity, mass, radius));
		renderComponents.push_back(RenderComponent(Shape(30, radius), color));
		spaceObjects.push_back( SpaceObject (physicsComponents.size()-1, renderComponents.size()-1 ) );
		return *(spaceObjects.end()-1);
	}

	static SpaceObject& createSatellite(Vector3d location, Vector3d velocity, int mass)
	{
		const std::array<float, 3> color = createColorFromRGB(244, 164, 96);
		float radius = 10;
		physicsComponents.push_back(PhysicsComponent(location, velocity, mass, radius));
		renderComponents.push_back(RenderComponent(Shape(30, radius), color));
		spaceObjects.push_back(SpaceObject(physicsComponents.size() - 1, renderComponents.size() - 1));
		return *(spaceObjects.end()-1);
	}

	static SpaceObject& createStar(Vector3d location, Vector3d velocity, int mass)
	{
		const std::array<float, 3> color = createColorFromRGB(255, 255, 0);
		float radius = 100;
		physicsComponents.push_back(PhysicsComponent(location, velocity, mass, radius));
		renderComponents.push_back(RenderComponent(Shape(30, radius), color));
		spaceObjects.push_back(SpaceObject(physicsComponents.size() - 1, renderComponents.size() - 1));
		lightPositions.push_back(spaceObjects.size()-1);
		return *(spaceObjects.end() - 1);
	}

	static const std::array<float, 3> createColorFromRGB(float r, float g, float b){
		r = r / 255;
		g = g / 255;
		b = b / 255;
		return std::array<float, 3>{{ r, g, b }};
	}
	static Vector3d randomizeLocation(int minDistance, int maxDistance, const Vector3d& centerPoint){
		int distance = (rand() % (maxDistance - (minDistance - 1)) + minDistance);
		Vector3d location(rand()*randomizeSign(), rand()*randomizeSign(), rand()*randomizeSign());
		location = centerPoint - location;
		location.setLength(distance);
		return location+centerPoint;
	}

	static int randomizeSign(){
		return (rand() % 2) == 1 ? 1 : -1;
	}
};

