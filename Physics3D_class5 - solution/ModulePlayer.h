#pragma once
#include "Module.h"
#include "Globals.h"
#include "p2Point.h"

struct PhysVehicle3D;

#define MAX_ACCELERATION 1500.0f
#define BOOST_ACCELERATION 1500.0f
#define TURN_DEGREES 30.0f * DEGTORAD
#define BRAKE_POWER 100.0f
#define IM mat4x4(1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f)

class ModulePlayer : public Module
{
public:
	ModulePlayer(Application* app, bool start_enabled = true);
	virtual ~ModulePlayer();

	bool Init(pugi::xml_node& node);
	bool Start();
	update_status Update(float dt);
	bool CleanUp();

public:

	PhysVehicle3D* vehicle = nullptr;
	float turn;
	float acceleration;
	float brake;
	vec3 start_pos = { 0.0f,0.0f,0.0f };
	vec3 position = { 0.0f,0.0f,0.0f };
	float max_speed = 0.0f;
	float friction = 0.0f;

	// Speed-ups
	bool speedupX = false;
	bool speedupZ = false;
	bool speedupZnegative = false;
	bool speedupXnegative = false;
	bool finished = false;
	float speed_positive_x = 0.0f;
	float speed_negative_x = 0.0f;
	float speed_positive_z = 0.0f;
	float speed_negative_z = 0.0f;

	// Vehicle jump
	bool cantJump = false;
	float timerJump = 0.0f;
	float seconds_to_jump = 0.0f;
	float jump_force = 0.0f;

	// FX
	uint FX_index[2] = { 0,0 };
	p2SString FX1_path;
	p2SString FX2_path;

	// Checkpoints
	vec3 checkpoint1_pos = { 0.0f,0.0f,0.0f };
	vec3 checkpoint2_pos = { 0.0f,0.0f,0.0f };
	vec3 checkpoint3_pos = { 0.0f,0.0f,0.0f };
	vec3 checkpoint4_pos = { 0.0f,0.0f,0.0f };
	vec3 checkpoint5_pos = { 0.0f,0.0f,0.0f };
	vec3 win_pos = { 0.0f,0.0f,0.0f };
	vec3 lose_pos = { 0.0f,0.0f,0.0f };
};
