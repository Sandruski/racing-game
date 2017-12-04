#pragma once
#include "Module.h"
#include "p2DynArray.h"
#include "Globals.h"
#include "Primitive.h"

#define MAX_SNAKE 2

struct PhysBody3D;
struct PhysMotor3D;
struct Primitive;

class ModuleSceneIntro : public Module
{
public:
	ModuleSceneIntro(Application* app, bool start_enabled = true);
	~ModuleSceneIntro();

	bool Start();
	update_status Update(float dt);
	bool CleanUp();

	void OnCollision(PhysBody3D* body1, PhysBody3D* body2);

	Cube CreateCube(float sizeX = 1.0f, float sizeY = 1.0f, float sizeZ = 1.0f, float posX = 0.0f, float posY = 0.0f, float posZ = 0.0f, Color color = White, bool axis = false, bool wire = false);
	Cylinder CreateCylinder(float radius = 1.0f, float height = 1.0f, float posX = 0.0f, float posY = 0.0f, float posZ = 0.0f, float angle = 0.0f, vec3 u = (0, 0, 0), Color color = White, bool axis = false, bool wire = false);
	void CreateSpeedUp(float sizeX = 1.0f, float sizeY = 1.0f, float sizeZ = 1.0f, float posX = 0.0f, float posY = 0.0f, float posZ = 0.0f);

	void UpdateSpeedUpColors(float dt);
	void UpdateSpeedUpIndex(uint &index);

public:
	/*
	PhysBody3D* pb_snake[MAX_SNAKE];
	Sphere s_snake[MAX_SNAKE];

	PhysBody3D* pb_snake2[MAX_SNAKE];
	Sphere s_snake2[MAX_SNAKE];
	*/

	PhysBody3D* pb_chassis;
	Cube p_chassis;

	PhysBody3D* pb_wheel;
	Cylinder p_wheel;

	PhysBody3D* pb_wheel2;
	Cylinder p_wheel2;

	PhysMotor3D* left_wheel;
	PhysMotor3D* right_wheel;

	float max_speed_time = 0.0f;
	float speed_time_count = 0.0f;

	// Speed-up parameters
	Color colors[5] = { PalePink, Pink, DeepPink, Violet, Purple };
	bool a_updated = false, b_updated = false, c_updated = false, d_updated = false;
	uint index_a = 0;
	uint index_b = 1;
	uint index_c = 2;
	uint index_d = 3;
	//_speed-up_parameters
};
