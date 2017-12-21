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
	Shape s1;
	Shape s2;
	Vector2D translation;
	static atomic<unsigned int> selected;
	static bool left;
	static bool right;
	static const FramesPerSecond FPS = 1;
	static const int MAX_COMPONENTS = 500;
	static Vector2D mouseLoc;
	static Vector2D viewPoint;
	static bool isViewPoint;
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
		isViewPoint = false;
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
	static void rotateCamera(){
		if (right){
			viewPoint = mouseLoc - theScreen.getCenterPoint();
			//viewPoint.y = -viewPoint.y;
			viewPoint.y /= theScreen.screen_height();
			viewPoint.x /= theScreen.screen_width();
			//viewPoint = theScreen.applyMagnitude(viewPoint);
			viewPoint *= 3;
			theScreen.rotatex += viewPoint.y;
			theScreen.rotatey += viewPoint.x;
		}
	}
	static void moveObject()
	{
		if(!left)
			return;
		auto& p = physicsComponents[spaceObjects[selected].physicsComponent];
		auto c = theScreen.getCenterPoint();
		auto point = mouseLoc - c;
		point.y = -point.y;
		//point.x /= theScreen.screen_width();
		//point.y /= theScreen.screen_height();
		//p.velocity += point.Normalize() / 100.0f; 
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
				++theScreen.rotatez;
				break; 
			case 'a':
				--theScreen.rotatez;
				break;
		}
	}

	static void keyCallBack(int key, int x, int y)
	{
		switch (key)
		{
		case GLUT_KEY_UP:       // Up: increase y speed
			GravityGame::theScreen.zoom_in(); break;
		case GLUT_KEY_DOWN:     // Down: decrease y speed
			GravityGame::theScreen.zoom_out(); break;
		case GLUT_KEY_LEFT:
			selectPreviousObject(); break;
		case GLUT_KEY_RIGHT:
			selectNextObject(); break;
		}
	}

	static void selectPreviousObject(){
		isViewPoint = false;
		if (GravityGame::selected == 0)
			GravityGame::selected = GravityGame::spaceObjects.size() - 1;
		else
			--GravityGame::selected;
	}

	static void selectNextObject(){
		isViewPoint = false;
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

	// bad code not actually concurrent
	static void threadUpdate()
	{
		const int size = physicsComponents.size();
		for(int i = 0; i < size; ++i)
		{
			physicsComponents[i].Update(physicsComponents);

			if((i+1) < size)
			{
				auto &func = [&](){physicsComponents[i+1].Update(physicsComponents);};
				thread t(func);
				t.join();
			}
		}
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
		//asyncUpdate();
		amp_update();
		//for (auto& p : physicsComponents)
			//p.Update(physicsComponents);
		//for (auto& p : physicsComponents)
			//p.move();
	}

	static void renderObjects()
	{
		if (!paused){
			updateComponents();
			moveObject();
			rotateCamera();
		}
		theScreen.refresh();
		
		if (!isViewPoint){
			theScreen.centerOn(physicsComponents[selected].location);
		}
		else{
			//theScreen.centerOn(viewPoint);
		}
		for (auto& o : spaceObjects)
		{
			//physicsComponents[o.physicsComponent].move();
			auto& rc = renderComponents[o.renderComponent];
			theScreen.render(rc.getShape(),
				physicsComponents[o.physicsComponent].location, rc.getColor());
		}
		
		//theScreen.centerOn(Vector2D(1,0));
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
		float radius = mass / 100.0f;
		physicsComponents.push_back(PhysicsComponent(location, velocity, mass, radius));
		renderComponents.push_back(RenderComponent(Shape(30, radius), color));
		spaceObjects.push_back( SpaceObject (physicsComponents.size()-1, renderComponents.size()-1 ) );
		return *(spaceObjects.end()-1);
	}

	static SpaceObject& createSatellite(Vector3d location, Vector3d velocity, int mass)
	{
		const std::array<float, 3> color = createColorFromRGB(244, 164, 96);
		float radius = mass / 100.0f;
		physicsComponents.push_back(PhysicsComponent(location, velocity, mass, radius));
		renderComponents.push_back(RenderComponent(Shape(30, radius), color));
		spaceObjects.push_back(SpaceObject(physicsComponents.size() - 1, renderComponents.size() - 1));
		return *(spaceObjects.end()-1);
	}

	static SpaceObject& createStar(Vector3d location, Vector3d velocity, int mass)
	{
		const std::array<float, 3> color = createColorFromRGB(255, 255, 0);
		float radius = mass / 100.0f;
		physicsComponents.push_back(PhysicsComponent(location, velocity, mass, radius));
		renderComponents.push_back(RenderComponent(Shape(30, radius), color));
		spaceObjects.push_back(SpaceObject(physicsComponents.size() - 1, renderComponents.size() - 1));
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

