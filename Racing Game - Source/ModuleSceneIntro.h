#pragma once
#include "Module.h"
#include "p2DynArray.h"
#include "Globals.h"
#include "Primitive.h"

struct PhysBody3D;
struct PhysMotor3D;
struct Primitive;

#define MAX_MUSIC 3

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

	Cube CreateCube(vec3 size = { 1.0f,1.0f,1.0f }, vec3 pos = { 0.0f, 0.0f, 0.0f }, Color color = White, float angle = 0.0f, vec3 u = (0.0f, 0.0f, 0.0f), float mass = 0.0f, bool draw = true, bool collider = true);
	Cylinder CreateCylinder(float radius = 1.0f, float height = 1.0f, vec3 pos = { 0.0f,0.0f,0.0f }, Color color = White, bool flip = true, float angle = 0.0f, vec3 u = { 0.0f, 0.0f, 0.0f }, float mass = 0.0f, bool draw = true, bool collider = true);
	Cone CreateCone(float radius = 1.0f, float height = 1.0f, vec3 pos = { 0.0f,0.0f,0.0f }, Color color = White, bool flip = true, float angle = 0.0f, vec3 u = { 0.0f, 0.0f, 0.0f }, float mass = 0.0f, bool draw = true, bool collider = true);
	
	void CreateSpeedUp(vec3 size = { 1.0f,1.0f,1.0f }, vec3 pos = { 0.0f, 0.0f, 0.0f }, uint num = 4, bool flip = true, float angle = 0.0f, vec3 u = { 0.0f, 0.0f, 0.0f });
	void CreateFinishLine(vec3 size = { 1.0f,1.0f,1.0f }, vec3 pos = { 0.0f, 0.0f, 0.0f });
	void CreateLimits(Primitive* p, bool pos[4], bool inv_col[4], bool inv_pos[4], vec4 lim = { 0.0f,0.0f,0.0f,0.0f });
	void CreateTrees(Cube ground, vec3 variation[], uint num = 0);

	void UpdateSpeedUpColors(float dt);
	void UpdateSpeedUpIndex(uint &index);

	void RadioUpdate();
	void TimeToLoseUpdate(float dt);

public:

	// Vehicle parts
	PhysBody3D* pb_chassis = nullptr;
	Cube p_chassis;

	PhysBody3D* pb_wheel = nullptr;
	Cylinder p_wheel;

	PhysBody3D* pb_wheel2 = nullptr;
	Cylinder p_wheel2;

	PhysMotor3D* left_wheel = nullptr;
	PhysMotor3D* right_wheel = nullptr;

	float max_speed_time = 0.0f;
	float speed_time_count = 0.0f;

	// Speed-up parameters
	Color colors[5] = { PalePink, Pink, DeepPink, Violet, Purple };
	bool a_updated = false, b_updated = false, c_updated = false, d_updated = false;
	uint index_a = 0, index_b = 1, index_c = 2, index_d = 3;

	// Primitives
	Cube cu6; // 2nd speed-up position reference
	Cube cu7;
	Cube cu9;
	Cube cu12;
	Cube cu18;
	Cube cu21;
	Cube cu23;
	Cube cu24;
	Cube ground2;

	// Senors
	Cube s;
	PhysBody3D* sensors[9] = { nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr , nullptr };
	vec3 sensorPos_1, sensorPos_2, sensorPos_3, sensorPos_4, sensorPos_5, sensorPos_6, sensorPos_7, sensorPos_8, sensorPos_9;
	uint checkpoints_index = 0;

	// Checkpoints
	PhysBody3D* checkpoints[5] = { nullptr, nullptr, nullptr, nullptr, nullptr };
	vec3 checkbox_sensors_1, checkbox_sensors_2, checkbox_sensors_3, checkbox_sensors_4, checkbox_sensors_5;
	// Game parameters
	PhysBody3D* loopCompleted = nullptr;
	uint loopsCount = 0;
	int minutes = 0;
	float seconds = 0;
	bool endTime = false;
	uint winCondition = 0; //0 neutral //1 win //2 lose

	// Music
	uint indexMusic = 0;
	bool song1_active = true, song2_active = false, song3_active = false, song4_active = false;

	// Hinges
	PhysBody3D* bodyA = nullptr;
	PhysBody3D* bodyA2 = nullptr;
	Cube rotating_cu;
	Cube rotating_cu2;

	// Blit primitives
	p2List<Cube> cubes;
	p2List<Cylinder> cylinders;
	p2List<Cone> cones;

	// Map parameters
	vec3 orthonormal_x = { 0.0f,0.0f,0.0f }, orthonormal_y = { 0.0f,0.0f,0.0f }, orthonormal_z = { 0.0f,0.0f,0.0f }, vec3_zero = { 0.0f,0.0f,0.0f };
	float RW = 0.0f, RH = 0.0f, RL = 0.0f, RL2 = 0.0f;
	float GW = 0.0f, GH = 0.0f, GL = 0.0f;
	float CW = 0.0f, CH = 0.0f, CL = 0.0f;
	float TUW = 0.0f, TUH = 0.0f;
	float HcyrotW = 0.0f, HcyrealW = 0.0f, HcyH = 0.0f, HcuW = 0.0f, HcuH = 0.0f, HcuL = 0.0f;
	float FLW = 0.0f, FLL = 0.0f, BR = 0.0f, BH = 0.0f;
	float TRUR = 0.0f, TRUH = 0.0f, TRER = 0.0f, TRER2 = 0.0f, TRER3 = 0.0f, TREH = 0.0f, TREH2 = 0.0f, TREH3 = 0.0f;
	float SW = 0.0f, SH = 0.0f;
	float OR = 0.0f, OH = 0.0f;
	Color road_col, water_col, ground_col, castle_col, tower_col, hinge_col, finish_line_col, tree_col, trunk_col, sky_col, obstacle_col, s_active_col, s_dead_col;
};

