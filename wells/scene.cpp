#include "scene.h"
void Scene::build() {
	playerCamera = new Camera();
	playerCamera->position = vec3(0, 1.8, 3);
	playerCamera->up = vec3(0, 1, 0);
	playerCamera->lookDir = normalize(vec3(0, 1, 0) - playerCamera->position);

	Material* bodyMaterial = new Material();
	bodyMaterial->kd = vec3(0.5f, 0.5f, 0.5f);
	bodyMaterial->ks = vec3(0.5, 0.5, 0.5);
	body = new Body(bodyMaterial, vec3(0,3,0), 2);
	body->translate(vec3(0, 3, 0));
	body->rotate(vec3(0, 0, M_PI / 4.0f));

	objects.push_back(body);

	Sphere* highVisSphere = new Sphere(0.05f, vec3(31,81,255));
	LeafObject* attachObject = new LeafObject(highVisSphere, bodyMaterial);
	attachObject->translate(vec3(0, 3, 0));

	objects.push_back(attachObject);

	l = Light();
	l.Le = vec3(1, 1, 1);
	l.La = vec3(0.2, 0.2, 0.2);
	l.wLightPos = vec4(0, 5, 0, 1);

	const float terrainW = 10.0f;
	const float terrainH = 10.0f;
	PinkNoiseTerrain* t = new PinkNoiseTerrain(terrainW, terrainH);
	Material* terrainMaterial = new Material();
	terrainMaterial->kd = vec3(0.5f, 0.5f, 0.5f);
	terrainMaterial->ks = vec3(0.1f, 0.1f, 0.1f);
	LeafObject* terrain = new LeafObject(t, terrainMaterial);
	terrain->translate(vec3(-terrainW / 2, -3, -terrainH / 2));
	objects.push_back(terrain);

	const vec3 red = vec3(1, 0, 0);
	const vec3 green = vec3(0, 1, 0);
	const vec3 blue = vec3(0, 0, 1);
	Arrow* xUnit = new Arrow(bodyMaterial, red, red);
	Arrow* yUnit = new Arrow(bodyMaterial, green, green);
	Arrow* zUnit = new Arrow(bodyMaterial, blue, blue);

	// arrow by default points towards (+y)
	xUnit->rotate(vec3(0, 0, M_PI / 2));
	// no rotation for yUnit
	zUnit->rotate(vec3(M_PI / 2, 0, 0));

	objects.push_back(xUnit);
	objects.push_back(yUnit);
	objects.push_back(zUnit);

	wellGeometry = highVisSphere; // reusing existing geometry
	wellMaterial = bodyMaterial; // reusing existing material

	dynamicObjectGeometry = new Sphere(0.025f, vec3(255, 165, 0));
	dynamicObjectMaterial = bodyMaterial; // reusing existing material
}

void Scene::useShader(Shader* shader) {
	for (auto* obj : objects) obj->setShader(shader);
}

void Scene::render(int windowWidth, int windowHeight) {
	RenderState state;

	state.wEye = playerCamera->position;
	state.V = playerCamera->getViewMatrix();
	state.P = playerCamera->getProjectionMatrix(windowWidth, windowHeight);
	state.light = l;

	// light space matrix
	vec3 lightPos = vec3(l.wLightPos.x, l.wLightPos.y, l.wLightPos.z);
	vec3 up = vec3(1, 0, 0);
	vec3 target = vec3(0, 0, 0);
	mat4 pointAtMx = PointAtMatrix(lightPos, target, up);
	const mat4 lightView = quickInverse(pointAtMx);
	const mat4 lightProjection = ProjectionMatrix(M_PI / 4, 0.01f, 20.0f, windowWidth, windowHeight);
	state.lightVP = lightView * lightProjection;

	for (auto* obj : objects) obj->draw(state);
}

void Scene::startSimulation() {
	if (simulationHappening) return;
	
	const float vStartX = 2 * getRandom() - 1; // generate random number
	body->setVelocity(vec3(vStartX, 0, 0));
	simulationHappening = true;
}

void Scene::simulate(float tstart, float tend) {
	const float dt = 10.0f; // dt is ”infinitesimal” (10ms)
	if (simulationHappening) {
		for (float t = tstart; t < tend; t += dt) {
			for (DynamicObject* dynObj : dynamicObjects) {
				const vec3 newNetForce = getNetGravity(dynObj->getPosition(), dynObj->getMass()); // calculate gravitational force where dynamic object is
				dynObj->setCurrentNetForce(newNetForce); // update net force
				dynObj->animate(tstart, tend); // animate
			}

			//TODO make body into dynamic object and remove this
			body->animate(tstart, tend); 
		}
	}
}

Camera* Scene::getPlayerCamera() {
	return playerCamera;
}

vec3 Scene::getNetGravity(vec3 position, float mass = 1) {
	vec3 netForce = vec3(0, 0, 0);
	for (size_t i = 0; i < wells.size(); i++)
	{
		const vec3 r = position - wells[i]->getPosition(); // well --> position
		const float distance = length(r);
		const float wellMass = wells[i]->getMass();

		vec3 force = -1.0f * PHYSICS::G * ((wellMass * mass) / distance * distance) * r; // Newton's law of universal gravitation
		
		netForce = netForce + force;
	}
	return netForce;
}

// returns a vertex on the minkowski difference of A and B using extreme points
vec3 Scene::supportGJK(GameObject* a, GameObject* b, vec3 dir) {
	return a->findFurthestPoint(dir) - b->findFurthestPoint(-dir);
}

bool Scene::collisionGJK(GameObject* a, GameObject* b) {
	// first support point in any random direction
	vec3 support = supportGJK(a, b, vec3(1, 0, 0)); 

	std::vector<vec3> simplex;
	simplex.insert(simplex.begin(), support);

	// new direction towards the origin
	vec3 direction = -support;

	while (true) {
		support = supportGJK(a, b, direction);

		if (dot(support, direction) <= 0) {
			return false; // origin is in the other direction => no collision possible
		}

		simplex.insert(simplex.begin(), support);
	}
}

void Scene::placeWell() {
	if (!wellGeometry) throw new std::runtime_error("Cannot place new well: missing base geometry (Scene::wellGeometry)!");
	if (!wellMaterial) throw new std::runtime_error("Cannot place new well: missing material (scene::wellMaterial)!");

	const vec3 offset = playerCamera->lookDir * 0.5f;
	Well* w = new Well(wellGeometry, wellMaterial, playerCamera->position + offset); // position new well in front of the player 
	objects.push_back(w); // add ptr to game objects so well gets rendered
	wells.push_back(w); // add ptr to wells so it can be handled accordingly
}

void Scene::selectWell() {
	Ray grabRay = Ray(playerCamera->position, playerCamera->lookDir);
	float closestWellDistance = -1;
	for (Well* w : wells) {
		float currentWellDistance = w->Intersect(grabRay);
		if (
			(currentWellDistance > 0) && // hit current well
			(currentWellDistance < closestWellDistance || closestWellDistance < 0)) // hit closer or first hit that we found
		{
			closestWellDistance = currentWellDistance;
			selectedWell = w;
		}
	}
}
void Scene::deselectWell() {
	selectedWell = NULL;
}

Well* Scene::getSelectedWell() {
	return selectedWell;
}

void Scene::placeGravityObject() {
	if (!dynamicObjectGeometry) throw new std::runtime_error("Cannot place new gravity object: missing base geometry (Scene::dynamicObjectGeometry)!");
	if (!dynamicObjectMaterial) throw new std::runtime_error("Cannot place new gravity object: missing material (scene::dynamicObjectMaterial)!");

	const vec3 offset = playerCamera->lookDir * 0.5f;
	DynamicObject* o = new DynamicObject(dynamicObjectGeometry, dynamicObjectMaterial, 2);
	o->setPosition(playerCamera->getPosition() + offset);
	objects.push_back(o); // add ptr to game objects so well gets rendered
	dynamicObjects.push_back(o); // add ptr to gravity objects so it can be handled accordingly
}

Scene::~Scene() {
	for (GameObject* o : objects) if (o) delete o; // delete all game objects
	if (playerCamera) delete playerCamera; // delete player camera
	if (wellGeometry) delete wellGeometry; // delete geometry that is used by wells
	if (wellMaterial) delete wellMaterial; // delete material that is used by wells
	if (dynamicObjectGeometry) delete dynamicObjectGeometry; // delete geometry that is used by dynamic objects
	if (dynamicObjectMaterial) delete dynamicObjectMaterial; // delete material that is used by dynamic objects
}
