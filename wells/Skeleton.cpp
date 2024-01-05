#include "framework.h";
#include <ctime>

SurfaceShader surfaceShader;
NormalDisplayShader normalDisplayShader;
ShadowDepthShader shadowDepthShader;
Scene * scene;
unsigned int shadowMapFBO; // Shadow map frame buffer object
unsigned int depthMap; // Shadow depth map
int SHADOW_WIDTH = 1024;
int SHADOW_HEIGHT = 1024;


void setup() {
	// Backface culling
	if (BACK_FACE_CULLING) {
		glEnable(GL_CULL_FACE);
	}
	else {
		glDisable(GL_CULL_FACE);
	}

	// Set to wireframe only
	if (WIREFRAME_ONLY) glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	else glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
}

void onInitialization() {
	srand(time(0));

	scene = new Scene();
	scene->build();

	glEnable(GL_DEPTH_TEST); // Enable Z-buffer
	glDepthMask(GL_TRUE);
	glDepthFunc(GL_LEQUAL);
	glDepthRange(0.0f, 1.0f);

	glCullFace(GL_FRONT);
	
	setup();

	surfaceShader.load("surface_vertex.glsl", "surface_fragment.glsl");
	normalDisplayShader.load("normal_display_vertex.glsl", "normal_display_fragment.glsl", "normal_display_geometry.glsl");
	//shadowDepthShader.load("shadow_depth_vertex.glsl", "shadow_depth_fragment.glsl");

	// shadows
	//glGenBuffers(1, &shadowMapFBO); // FBO for rendering depth map
	//glGenTextures(1, &depthMap);
	//glBindTexture(GL_TEXTURE_2D, depthMap);
	//glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT,
	//	SHADOW_WIDTH, SHADOW_HEIGHT, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	//glBindFramebuffer(GL_FRAMEBUFFER, shadowMapFBO); // bind shadow map frame buffer
	//glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, depthMap, 0); // attach tepth texture to the framebuffer's depth buffer
	//glDrawBuffer(GL_NONE); // explicitly tell openGL that no color data is to be rendered
	//glReadBuffer(GL_NONE); // ---- || ----
	//glBindFramebuffer(GL_FRAMEBUFFER, 0); // unbind framebuffer
}

void onDisplay() {
	glClearColor(0.38, 0.59, 0.71, 1); // blue background to mimic sky


	// Render to depth map (shadows)
	//glViewport(0, 0, SHADOW_WIDTH, SHADOW_HEIGHT);
	//glBindFramebuffer(GL_FRAMEBUFFER, shadowMapFBO);
	//glClear(GL_DEPTH_BUFFER_BIT);
	//scene->useShader(&shadowDepthShader);
	//scene->render(SHADOW_WIDTH, SHADOW_HEIGHT);
	//glBindFramebuffer(GL_FRAMEBUFFER, 0);

	// Render scene as normal with shadow mapping
	glViewport(0, 0, WINDOW_WIDTH_PX, WINDOW_HEIGHT_PX);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // clear frame buffer & depth buffer so that primitives get drawn
	//glDrawBuffer(GL_BACK);
	//glReadBuffer(GL_BACK);

	//glActiveTexture(GL_TEXTURE0);
	//glBindTexture(GL_TEXTURE_2D, depthMap);

	scene->useShader(&surfaceShader);
	scene->render(WINDOW_WIDTH_PX, WINDOW_HEIGHT_PX);
	if (DISPLAY_NORMALS) {
		scene->useShader(&normalDisplayShader);
		scene->render(WINDOW_WIDTH_PX, WINDOW_HEIGHT_PX);
	}

	glutSwapBuffers(); // exchange buffers for double buffering
}

void moveCamera(Dir dir) {
	vec3 moveVector;
	const vec3 up = scene->getPlayerCamera()->up;
	const vec3 lookDir = scene->getPlayerCamera()->lookDir;
	if (dir == Dir::up) moveVector = up;
	else if (dir == Dir::down) moveVector = -1.0f * up;
	else if (dir == Dir::forward) moveVector = lookDir;
	else if (dir == Dir::back) moveVector = -1.0f * lookDir;
	else if (dir == Dir::right) moveVector = cross(up, lookDir);
	else if (dir == Dir::left) moveVector = cross(lookDir, up);
	moveVector = 0.03f * moveVector; // set magnitude of movement

	// Translate camera
	scene->getPlayerCamera()->setPosition(scene->getPlayerCamera()->getPosition() + moveVector);

	// Translate selected well
	if (scene->getSelectedWell() != NULL) {
		scene->getSelectedWell()->setPosition(scene->getSelectedWell()->getPosition() + moveVector); // set the new position for the well
	}
}

void onKeyboard(unsigned char key, int pX, int pY) {
	bool changed = true;
	if (key == 'w') {
		moveCamera(Dir::forward);
	}
	else if (key == 'a') {
		moveCamera(Dir::left);
	}
	else if (key == 's') {
		moveCamera(Dir::back);
	}
	else if (key == 'd') {
		moveCamera(Dir::right);
	}
	else if (key == ' ') {
		moveCamera(Dir::up);
	}
	else if (key == 'c') {
		moveCamera(Dir::down);
	}
	else if (key == 'y') {
		scene->startSimulation();
	}
	else if (key == 'n') {
		DISPLAY_NORMALS = !DISPLAY_NORMALS;
	}
	else if (key == 'f') {
		WIREFRAME_ONLY = !WIREFRAME_ONLY;
	}
	else if (key == 'b') {
		BACK_FACE_CULLING = !BACK_FACE_CULLING;
	}
	else if (key == 'g') {
		PHYSICS::g = applyTransform(PHYSICS::g, RotationMatrix(vec3(0, 0, M_PI / 2)), 0.0f);
	}
	else if (key == 'p') {
		scene->placeWell();
	}
	else {
		changed = false;
	}
	if (changed) {
		setup();
		glutPostRedisplay();
	}
}

void onKeyboardUp(unsigned char key, int pX, int pY) {}

vec2 oldPosition = vec2(0, 0);
void onMouseMotion(int pX, int pY) {
	// Convert to normalized device space
	vec2 newPosition;
	newPosition.x = 2.0f * pX / WINDOW_WIDTH_PX - 1;	// flip y axis
	newPosition.y = 1.0f - 2.0f * pY / WINDOW_HEIGHT_PX;
	if (newPosition.x >= WINDOW_WIDTH_PX || newPosition.x <= -1) return; // dragged out of bounds horizontally
	if (newPosition.y >= WINDOW_HEIGHT_PX || newPosition.y <= -1) return; // dragged out of bounds vertically
	if (oldPosition.x == newPosition.x && oldPosition.y == newPosition.y) return; // no movement, just a click


	if (oldPosition.x != 0 && oldPosition.y != 0) {
		vec2 offset = normalize(oldPosition - newPosition);

		// needed rotation around -up (=down) axis
		vec3 downAxis = normalize(- 1.0f * scene->getPlayerCamera()->up);
		const float downAmount = offset.x * (M_PI / 300);

		// needed rotation around +left axis
		vec3 leftAxis = normalize(cross(scene->getPlayerCamera()->up, scene->getPlayerCamera()->lookDir));
		const float leftAmount = offset.y * (M_PI / 300);

		// Rotate camera
		scene->getPlayerCamera()->rotateCamera(downAmount, downAxis);
		scene->getPlayerCamera()->rotateCamera(leftAmount, leftAxis);

		// Translate + rotate selected well
		if (scene->getSelectedWell() != NULL) {
			Well* selectedWell = scene->getSelectedWell();

			// Calculate new well position
			const vec3 oldWellPosition = selectedWell->getPosition();
			vec3 newWellPosition = oldWellPosition; // new position initialized as old position
			newWellPosition = applyTransform(newWellPosition, TranslateMatrix(-1.0f * scene->getPlayerCamera()->position)); // Base transform: player camera to origin
			newWellPosition = applyTransform(newWellPosition, RotationMatrix(downAmount, downAxis)); // Apply rotation around -up (=down) axis
			newWellPosition = applyTransform(newWellPosition, RotationMatrix(leftAmount, leftAxis)); // Apply rotation around +left axis
			newWellPosition = applyTransform(newWellPosition, TranslateMatrix(scene->getPlayerCamera()->position)); // Reverse base transform
			selectedWell->setPosition(newWellPosition); // set the new position for the well
			
			// Adjust orientation TODO
			vec3 cam2Old = normalize(oldWellPosition - scene->getPlayerCamera()->position);
			vec3 cam2New = normalize(newWellPosition - scene->getPlayerCamera()->position);
			float orientationAdjustmentAngle = acosf(dot(cam2Old, cam2New));
			vec3 orientationAdjustmentAxis = normalize(cross(cam2New, normalize(newWellPosition - oldWellPosition)));

			vec3 finalRotation = (orientationAdjustmentAxis * orientationAdjustmentAngle);

			//printf("\n\n");
			//orientationAdjustmentAxis.print();
			//printf("%f\n", orientationAdjustmentAngle);
			//printf("rot: ");
			//finalRotation.print();
			//printf("\n");

			selectedWell->rotate(finalRotation);
		}

		glutPostRedisplay();
	}
	oldPosition = newPosition;
}

void onMouse(int button, int state, int pX, int pY) {
	bool changed = true;
	if (button == GLUT_RIGHT_BUTTON) {
		if (state == GLUT_DOWN) {
			scene->selectWell();
		}
		else if (state == GLUT_UP) {
			scene->deselectWell();
		}
	}
	else if (button == GLUT_MIDDLE_BUTTON && state == GLUT_DOWN) {
		scene->placeGravityObject();
	}
	else if (button == 3 && state == GLUT_DOWN) {
		// scroll up 
		if (scene->getSelectedWell() != NULL) {
			scene->getSelectedWell()->translate(scene->getPlayerCamera()->lookDir * 0.02f);
		}
	}
	else if (button == 4 && state == GLUT_DOWN) {
		// scroll down 
		if (scene->getSelectedWell() != NULL) {
			scene->getSelectedWell()->translate(scene->getPlayerCamera()->lookDir * -0.02f);
		}
	}
	else {
		changed = false;
	}
	if (changed) glutPostRedisplay();
}

long lastTime = 0;
void onIdle() {
	long time = glutGet(GLUT_ELAPSED_TIME);
	if(time - lastTime >= 20) {
		scene->simulate(lastTime, time);
		lastTime = time;
		glutPostRedisplay();
	}
}
