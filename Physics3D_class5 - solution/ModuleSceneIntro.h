#pragma once
#include "Module.h"
#include "p2DynArray.h"
#include "Globals.h"
#include "Primitive.h"

struct PhysBody3D;
struct PhysMotor3D;
struct Primitive;

class ModuleSceneIntro : public Module
{
public:
	ModuleSceneIntro(Application* app, bool start_enabled = true);
	~ModuleSceneIntro();

	bool Init(pugi::xml_node& node);
	bool Start();
	update_status Update(float dt);
	bool CleanUp();

	void OnCollision(PhysBody3D* body1, PhysBody3D* body2);

	Cube CreateCube(float sizeX = 1.0f, float sizeY = 1.0f, float sizeZ = 1.0f, float posX = 0.0f, float posY = 0.0f, float posZ = 0.0f, Color color = White, bool draw = true, bool collider = true, float mass = 0.0f, float angle = 0.0f, vec3 u = (0, 0, 0));
	Cylinder CreateCylinder(float radius = 1.0f, float height = 1.0f, float posX = 0.0f, float posY = 0.0f, float posZ = 0.0f, float angle = 0.0f, vec3 u = (0, 0, 0), Color color = White, bool draw = true, bool collider = true, float mass = 0.0f);
	
	void CreateSpeedUp(float sizeX = 1.0f, float sizeY = 1.0f, float sizeZ = 1.0f, float posX = 0.0f, float posY = 0.0f, float posZ = 0.0f, uint num = 4, bool add_z = true, float angle = 0.0f, vec3 u = (0, 0, 0));
	void CreateFinishLine(float sizeX = 1.0f, float sizeY = 1.0f, float sizeZ = 1.0f, float posX = 0.0f, float posY = 0.0f, float posZ = 0.0f);
	
	void CreateLimits(Primitive* p, bool pos[4], bool inv_col[4], bool inv_pos[4], vec4 lim = { 0.0f,0.0f,0.0f,0.0f });


	void UpdateSpeedUpColors(float dt);
	void UpdateSpeedUpIndex(uint &index);

public:
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
	uint index_a = 0, index_b = 1, index_c = 2, index_d = 3;

	Cube cu6; // 2nd speed-up position reference
	Cube cu7;
	Cube cu9;
	Cube cu12;
	Cube ground1;

	// Senors 
	Cube s, h, t, n;
	PhysBody3D* sensor;
	PhysBody3D* sensor3;
	PhysBody3D* sensor4;
	PhysBody3D* sensor5;
	uint checkpoints_index = 0;

	// Hinges
	PhysBody3D* bodyA;
	PhysBody3D* bodyA2;
	Cube rotating_cu;
	Cube rotating_cu2;

	// Blit primitives
	p2List<Cube> cubes;
	p2List<Cylinder> cylinders;
};

