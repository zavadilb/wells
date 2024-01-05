#ifndef SCENE_H
#define SCENE_H
#include <vector>
#include "game_object.h"
#include "camera.h"
#include "body.h"
#include "pink_noise_terrain.h"
#include "leaf_object.h"
#include "arrow.h"
#include "sphere.h"
#include "well.h"
#include "dynamic_object.h"

class Scene {
	std::vector<GameObject*> objects;
	std::vector<Well*> wells;
	std::vector<DynamicObject*> dynamicObjects; // objects that move
	Geometry* wellGeometry; // geometry used by wells
	Material* wellMaterial; // material used by wells
	Geometry* dynamicObjectGeometry;
	Material* dynamicObjectMaterial;
	Well* selectedWell = NULL;
	Light l;
	Camera* playerCamera;
	Body* body = NULL;
	bool simulationHappening = false;


public:
	void build();
	void render(int windowWidth, int windowHeight);
	void useShader(Shader* shader);
	void startSimulation();
	void simulate(float tstart, float tend);
	void placeWell();
	void selectWell();
	void deselectWell();
	void placeGravityObject();
	Well* getSelectedWell();

	// gravity
	vec3 getNetGravity(vec3 position, float mass); // mass in SI (kg)

	// collision detection
	vec3 supportGJK(GameObject* a, GameObject* b, vec3 dir);
	bool collisionGJK(GameObject* a, GameObject* b);

	Camera* getPlayerCamera();
	~Scene();
};
#endif // !SCENE_H


