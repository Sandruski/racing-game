#pragma once
#include "Module.h"
#include "Globals.h"
#include "p2Point.h"

struct PhysVehicle3D;

#define MAX_ACCELERATION 1500.0f
#define BOOST_ACCELERATION 1500.0f
#define TURN_DEGREES 30.0f * DEGTORAD
#define BRAKE_POWER 100.0f

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

	PhysVehicle3D* vehicle;
	float turn;
	float acceleration;
	float brake;
	vec3 position;
	float* identity;
	float* patata;
	PhysBody3D* sphere;

	bool speedupX = false;
	bool speedupZ = false;
	bool speedupZnegative = false;
	bool speedupXnegative = false;
	bool finished = false;
	bool cantJump = false;
	float timerJump = 0.0f;

	int music_index[2];
};
