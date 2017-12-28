#include "GravityGame.h"

vector<SpaceObject> GravityGame::spaceObjects;
vector<RenderComponent> GravityGame::renderComponents;
vector<PhysicsComponent> GravityGame::physicsComponents;
std::unique_ptr< concurrency::array<amp_physics::amp_physics_component, 1> > GravityGame::amp_physicsComponents;
atomic<unsigned int> GravityGame::selected;
Screen GravityGame::theScreen;
bool GravityGame::left;
bool GravityGame::right;
Vector2D GravityGame::mouseLoc;
Vector2D GravityGame::viewPoint;
bool GravityGame::isRunning;
bool GravityGame::paused;
vector<size_t> GravityGame::lightPositions;

GravityGame::GravityGame()
{
	this->init();
	srand(time(NULL));
	isRunning = false;
	paused = false;
	start = 0;
	spaceObjects.reserve(MAX_COMPONENTS);
	renderComponents.reserve(MAX_COMPONENTS);
	physicsComponents.reserve(MAX_COMPONENTS);

	auto centerpoint = createStar(Vector3d(0, 0, 0), Vector3d(0, 0, 0), 6000);
	auto centerpoint2 = createStar(Vector3d(0, 0, -800), Vector3d(0, 0, 0), 100);
	auto centerpoint3 = createStar(Vector3d(0, -800, 0), Vector3d(0, 0, 0), 6000);
	auto centerpoint4 = createStar(Vector3d(800, 0, 0), Vector3d(0, 0, 0), 6000);
	auto centerpoint5 = createStar(Vector3d(-800, 0, 0), Vector3d(0, 0, 0), 6000);
	centerpoint2.getPhysicsComponent().orbitObject(centerpoint.getPhysicsComponent(), true);
	centerpoint3.getPhysicsComponent().orbitObject(centerpoint.getPhysicsComponent(), true);
	centerpoint4.getPhysicsComponent().orbitObject(centerpoint.getPhysicsComponent(), true);
	centerpoint5.getPhysicsComponent().orbitObject(centerpoint.getPhysicsComponent(), true);

	for (int i = 0; i < 50; ++i){
		Vector3d randomLoc = randomizeLocation(5000, 20000, centerpoint.getPhysicsComponent().location);
		addObjects(randomLoc, centerpoint);
	}
	amp_physicsComponents = std::make_unique<concurrency::array<amp_physics::amp_physics_component, 1>>(physicsComponents.size(), physicsComponents.begin(), physicsComponents.end());
}


GravityGame::~GravityGame()
{
}

void GravityGame::addObjects(Vector3d translation, const SpaceObject& centerPoint){

	//translation = randomizeLocation(5000, 6000, centerPoint.getPhysicsComponent().location);

	bool outerRotation = (rand() % 2) > 0 ? true : false;
	auto a = createStar(translation, Vector3d(0, 0, 0), rand() % 1500 + 700);
	PhysicsComponent& physicsComponent = a.getPhysicsComponent();
	physicsComponent.orbitObject(centerPoint.getPhysicsComponent(), outerRotation);
	physicsComponent.radius = 1;

	const int max = rand() % 8 + 1;
	for (int i = 0; i < max; ++i){
		int startingOuterDistance = 75;
		int minOuterDistance = startingOuterDistance * (i + 1);
		int maxOuterDistance = minOuterDistance + startingOuterDistance;
		translation = randomizeLocation(minOuterDistance, maxOuterDistance, a.getPhysicsComponent().location);
		auto planet = createPlanet(translation, Vector3d(0, 0, 0), rand() % 5 + 1);
		planet.getPhysicsComponent().orbitObject(a.getPhysicsComponent(), outerRotation);

		const int innermax = rand() % 5+ 1;
		bool innerRotation = (rand() % 2) > 0 ? true : false;
		for (int j = 0; j < innermax; ++j){
			int startingInnerDistance = 1;
			int minInnerDistance = startingInnerDistance * (j+1);
			int maxInnerDistance = minInnerDistance + startingInnerDistance;
			translation = randomizeLocation(minInnerDistance, maxInnerDistance, planet.getPhysicsComponent().location);
			auto sat = createSatellite(translation, Vector3d(0, 0, 0), 2);
			sat.getPhysicsComponent().orbitObject(planet.getPhysicsComponent(), innerRotation);
		}
	}
}
