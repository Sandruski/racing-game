#include "Globals.h"
#include "Application.h"
#include "ModuleSceneIntro.h"
#include "Primitive.h"
#include "PhysBody3D.h"

ModuleSceneIntro::ModuleSceneIntro(Application* app, bool start_enabled) : Module(app, start_enabled)
{
	name.create("scene_intro");
}

bool ModuleSceneIntro::Init(pugi::xml_node& node) 
{
	bool ret = true;

	// Map parameters
	// Road
	RW = node.child("road_width").attribute("value").as_float(); // x axis
	RH = node.child("road_height").attribute("value").as_float(); // y axis
	RL = node.child("road_length").attribute("value").as_float(); // z axis
	RL2 = node.child("road_length2").attribute("value").as_float(); // z axis

	// Ground
	GW = node.child("ground_width").attribute("value").as_float(); // x axis
	GH = node.child("ground_height").attribute("value").as_float(); // y axis
	GL = node.child("ground_length").attribute("value").as_float(); // z axis

	// Castle
	CW = node.child("castle_width").attribute("value").as_float(); // x axis
	CH = node.child("castle_height").attribute("value").as_float(); // y axis
	CL = node.child("castle_length").attribute("value").as_float(); // z axis

	// Tunnel
	TUH = node.child("tunnel_height").attribute("value").as_float(); // y axis

	// Finish line
	FLW = node.child("finish_line_width").attribute("value").as_float(); // x axis
	FLL = node.child("finish_line_length").attribute("value").as_float(); // z axis

	BR = node.child("bar_radius").attribute("value").as_float(); // radius
	BH = node.child("bar_height").attribute("value").as_float(); // y axis

	// Trees
	TRUR = node.child("trunk_radius").attribute("value").as_float(); // radius
	TRUH = node.child("trunk_height").attribute("value").as_float(); // y axis

	TRER = node.child("tree_radius").attribute("value").as_float(); // radius
	TRER2 = node.child("tree_radius2").attribute("value").as_float(); // radius
	TRER3 = node.child("tree_radius3").attribute("value").as_float(); // radius
	TREH = node.child("tree_height").attribute("value").as_float(); // y axis
	TREH2 = node.child("tree_height2").attribute("value").as_float(); // y axis
	TREH3 = node.child("tree_height3").attribute("value").as_float(); // y axis

	// Sky
	SW = node.child("sky_width").attribute("value").as_float(); // x axis
	SH = node.child("sky_height").attribute("value").as_float(); // y axis

	// Obstacles
	OR = node.child("obstacle_radius").attribute("value").as_float(); // radius
	OH = node.child("obstacle_height").attribute("value").as_float(); // y axis
	//_map_parameters

	return ret;
}

ModuleSceneIntro::~ModuleSceneIntro()
{}

// Load assets
bool ModuleSceneIntro::Start()
{
	LOG("Loading Intro assets");
	bool ret = true;

	//Music
	//App->audio->PlayMusic("Music/OGG/P5.ogg");

	App->camera->Move(vec3(1.0f, 1.0f, 0.0f));
	App->camera->LookAt(vec3(0, 0, 0));

	// Sensors
	s.size = vec3(4, 2, 8); // First sensor, dont touTh
	s.SetPos(0, 1.5f, 3); // First sensor, dont touTh

	h.size = vec3(10, 20, 8); // Third sensor, need to be fixed
	h.SetPos(60, 1.5f, 240); // Third sensor, need to be fixed

	t.size = vec3(14, 20, 7); // forth sensor, need to be fixed
	t.SetPos(125, -4, 369); // forth sensor, need to be fixed

	n.size = vec3(32, 20, 9); // fifth sensor, need to be fixed
	n.SetPos(277, 0.5f, 287); // fifth sensor, need to be fixed

	sensor = App->physics->AddBody(s, 0.0f);
	sensor->SetAsSensor(true);
	sensor->collision_listeners.add(this);

	sensor3 = App->physics->AddBody(h, 0.0f);
	sensor3->SetAsSensor(true);
	sensor3->collision_listeners.add(this);

	sensor4 = App->physics->AddBody(t, 0.0f);
	sensor4->SetAsSensor(true);
	sensor4->collision_listeners.add(this);

	sensor5 = App->physics->AddBody(n, 0.0f);
	sensor5->SetAsSensor(true);
	sensor5->collision_listeners.add(this);

	// Checkpoint sensors
	ch1.size = vec3(10, 4, 10);
	ch1.SetPos(60, 1.5f, 65);

	checkpoint1 = App->physics->AddBody(ch1, 0.0f);
	checkpoint1->SetAsSensor(true);
	checkpoint1->collision_listeners.add(this);

	ch2.size = vec3(40, 20, 40);
	ch2.SetPos(60, 1.5f, 300);

	checkpoint2 = App->physics->AddBody(ch2, 0.0f);
	checkpoint2->SetAsSensor(true);
	checkpoint2->collision_listeners.add(this);

	ch3.size = vec3(40, 20, 40);
	ch3.SetPos(277, 0.5f, 267);

	checkpoint3 = App->physics->AddBody(ch3, 0.0f);
	checkpoint3->SetAsSensor(true);
	checkpoint3->collision_listeners.add(this);

	ch4.size = vec3(40, 60, 40);
	ch4.SetPos(277, 0, 70);

	checkpoint4 = App->physics->AddBody(ch4, 0.0f);
	checkpoint4->SetAsSensor(true);
	checkpoint4->collision_listeners.add(this);

	ch5.size = vec3(40, 20, 40);
	ch5.SetPos(338, 0, -55);

	checkpoint5 = App->physics->AddBody(ch5, 0.0f);
	checkpoint5->SetAsSensor(true);
	checkpoint5->collision_listeners.add(this);

	loopCompletedCube.size = vec3(10, 30, 3);
	loopCompletedCube.SetPos(0, 0, 1);

	loopCompleted = App->physics->AddBody(loopCompletedCube, 0.0f);
	loopCompleted->SetAsSensor(true);
	loopCompleted->collision_listeners.add(this);
	//_checkpoint_sensors
	//_sensors

	// Map parameters
	orthonormal_x = { 1.0f, 0.0f, 0.0f };
	orthonormal_y = { 0.0f, 1.0f, 0.0f };
	orthonormal_z = { 0.0f, 0.0f, 1.0f };

	// Tunnel
	TUW = RW / 2.0f;

	// Hinges
	HcyrotW = RW / 2.0f; // x axis
	HcyrealW = RW / 8.0f; // x axis
	HcyH = 50.0f * RH; // y axis

	HcuW = RW / 2.0f; // x axis
	HcuH = 2.0f * RH; // y axis
	HcuL = RL + RW; // z axis

	// Colors
	road_col = Beige;
	water_col = Cyan;
	ground_col = Pink;
	castle_col = White;
	tower_col = Pink;
	hinge_col = Orange;
	finish_line_col = White;
	tree_col = GreenYellow;
	trunk_col = Orange;
	sky_col = SkyBlue;

	s_active_col = IndianRed;
	s_dead_col = White;
	obstacle_col = s_active_col;
	//_map_parameters

	// Road
	// 1st section
	Cube cu = CreateCube(vec3(RW, RH, RL), vec3_zero, road_col);
	Cube cu2 = CreateCube(vec3(RW, RH, RL), vec3(cu.GetPos().x, cu.GetPos().y, cu.GetPos().z + cu.GetSize().z / 2.0f + RL / 2.0f), road_col);
	Cylinder cy = CreateCylinder(RW / 2.0f, RH, vec3(cu2.GetPos().x, cu2.GetPos().y, cu2.GetPos().z + cu2.GetSize().z / 2.0f), road_col);
	Cube cu3 = CreateCube(vec3(RL, RH, RW), vec3(cy.GetPos().x + RL / 2.0f, cy.GetPos().y, cy.GetPos().z), road_col);
	Cylinder cy2 = CreateCylinder(RW / 2.0f, RH, vec3(cu3.GetPos().x + cu3.GetSize().x / 2.0f, cu3.GetPos().y, cu3.GetPos().z), road_col);
	Cube cu4 = CreateCube(vec3(RW, RH, RL2), vec3(cy2.GetPos().x, cy2.GetPos().y, cy2.GetPos().z + RL2 / 2.0f), road_col);
	Cylinder cy3 = CreateCylinder(RW / 2.0f, RH, vec3(cu4.GetPos().x, cu4.GetPos().y, cu4.GetPos().z + cu4.GetSize().z / 2.0f), road_col);
	Cube cu5 = CreateCube(vec3(RL, RH, RW), vec3(cy3.GetPos().x + RL / 2.0f, cy3.GetPos().y, cy3.GetPos().z), road_col);
	Cylinder cy4 = CreateCylinder(RW / 2.0f, RH, vec3(cu5.GetPos().x + cu5.GetSize().x / 2.0f, cu5.GetPos().y, cu5.GetPos().z), road_col);
	cu6 = CreateCube(vec3(RW, RH, 6.0f * RL), vec3(cy4.GetPos().x, cy4.GetPos().y, cy4.GetPos().z + (6.0f * RL) / 2.0f), road_col);
	cu7 = CreateCube(vec3(RW, RH, RL / 2.0f), vec3(cu6.GetPos().x, cu6.GetPos().y, cu6.GetPos().z + cu6.GetSize().z / 2.0f), road_col, -20.0f, orthonormal_x);

	// Ground and water
	Cube ground = CreateCube(vec3(GW, GH, GL), vec3(cy4.GetPos().x - GW / 3.0f, cy4.GetPos().y - GH / 2.0f, cy4.GetPos().z + cy4.radius - GL / 2.0f), ground_col);
	Cube water = CreateCube(vec3(2.0f * GW, GH, 3.0f * GL + 15.0f), vec3(ground.GetPos().x, ground.GetPos().y - GH, ground.GetPos().z + GL), water_col);
	ground2 = CreateCube(vec3(2.0f * GW, GH / 2.0f, 2.0f * GL), vec3(water.GetPos().x + water.GetSize().x / 2.0f, ground.GetPos().y - ground.GetSize().y / 2.0f, water.GetPos().z + water.GetSize().z / 2.0f + (2.0f * GL) / 2.0f), ground_col);
	//_ground_and_water
	//_1st_section

	// 2nd section
	Cube cu8 = CreateCube(vec3(3.0f * RW, RH, 3.0f * RL), vec3(cu7.GetPos().x, ground2.GetPos().y + ground2.GetSize().y / 2.0f + RH / 2.0f, ground2.GetPos().z - ground2.GetSize().z / 2.0f + (3.0f * RL) / 2.0f), road_col);
	Cylinder cy5 = CreateCylinder((3.0f * RW) / 2.0f, RH, vec3(cu8.GetPos().x, cu8.GetPos().y, cu8.GetPos().z + cu8.GetSize().z / 2.0f), road_col);
	cu9 = CreateCube(vec3(4.0f * RL, RH, 3.0f * RW), vec3(cy5.GetPos().x + (4.0f * RL) / 2.0f, cy5.GetPos().y, cy5.GetPos().z), road_col);
	Cylinder cy6 = CreateCylinder((3.0f * RW) / 2.0f, RH, vec3(cu9.GetPos().x + cu9.GetPos().x / 2.0f, cu9.GetPos().y, cu9.GetPos().z), road_col);
	Cube cu10 = CreateCube(vec3(4.0f * RL, RH, 3.0f * RW), vec3(cy6.GetPos().x, cy6.GetPos().y, cy6.GetPos().z), road_col, 20.0f, orthonormal_z);
	Cube cu11 = CreateCube(vec3(2.0f * RL, RH, 3.0f * RW), vec3(cu10.GetPos().x + cu10.GetSize().x - (2.0f * RL) / 2.0f - 4.0f, cu10.GetPos().y + cu10.GetSize().z * tan(DEGTORAD * 30.0f) - 0.4f, cu10.GetPos().z), road_col);
	Cylinder cy7 = CreateCylinder(RW / 2.0f, 50.0f * RH, vec3(cu11.GetPos().x + cu11.GetSize().x / 3.0f - (RW / 2.0f), cu11.GetPos().y + cu11.GetSize().y / 2.0f - (50.0f * RH) / 2.0f, cu11.GetPos().z), road_col);
	cu12 = CreateCube(vec3(3.0f * RW, RH, 4.0f * RL), vec3(cu11.GetPos().x + cu11.GetSize().x / 2.0f - (3.0f * RW) / 2.0f, cy6.GetPos().y, cu11.GetPos().z - (4.0f * RL) / 2.0f - cu11.GetSize().z / 2.0f + 4.0f), road_col, -20.0f, orthonormal_x);
	Cube cu13 = CreateCube(vec3(3.0f * RW, RH, 4.0f * RL), vec3(cu12.GetPos().x, cu12.GetPos().y - cu11.GetSize().z * tan(DEGTORAD * 30.0f) + 0.4f, cu12.GetPos().z - cu12.GetSize().z / 2.0f + 4.0f), road_col, -20.0f, orthonormal_x);
	Cube cu14 = CreateCube(vec3(3.0f * RW, RH, 2.0f * RL), vec3(cu13.GetPos().x, cu13.GetPos().y, cu13.GetPos().z - cu13.GetSize().z / 2.0f + (2.0f * RL) / 2.0f), road_col);
	Cube cu15 = CreateCube(vec3(3.0f * RW, RH, 2.0f * RL), vec3(cu14.GetPos().x, cu14.GetPos().y, cu14.GetPos().z - 1.2f * cu14.GetSize().z), road_col);
	Cube cu16 = CreateCube(vec3(3.0f * RW, RH, 2.0f * RL), vec3(cu15.GetPos().x, cu15.GetPos().y, cu15.GetPos().z - 1.2f * cu15.GetSize().z), road_col);

	// Castle
	Cube castle = CreateCube(vec3(90.0f, 90.0f, 130.0f), vec3(water.GetPos().x + water.GetSize().x / 2.0f + 90.0f / 3.0f, water.GetPos().y + water.GetSize().y, water.GetPos().z), castle_col);
	Cube castle1 = CreateCube(vec3(30.0f, 80.0f, 30.0f), vec3(castle.GetPos().x, castle.GetPos().y + castle.GetSize().y / 2.0f, castle.GetPos().z), castle_col);
	Cube castle2 = CreateCube(vec3(20.0f, 50.0f, 20.0f), vec3(castle1.GetPos().x, castle1.GetPos().y + castle1.GetSize().y / 2.0f, castle1.GetPos().z), castle_col);
	CreateCone(1.5f * CW, 1.5f * CH, vec3(castle2.GetPos().x, castle2.GetPos().y + castle2.GetSize().y / 2.0f + (1.5f * CH) / 2.0f, castle2.GetPos().z), tower_col);
	Cube castle3 = CreateCube(vec3(CW, CH, CL), vec3(castle.GetPos().x + castle.GetSize().x / 2.0f, castle.GetPos().y + castle.GetSize().y / 2.0f - 40.0f / 5.0f, castle.GetPos().z + castle.GetSize().z / 2.0f), castle_col);
	CreateCone(CW, CH, vec3(castle3.GetPos().x, castle3.GetPos().y + castle3.GetSize().y / 2.0f + CH / 2.0f, castle3.GetPos().z), tower_col);
	Cube castle4 = CreateCube(vec3(CW, CH, CL), vec3(castle.GetPos().x - castle.GetSize().x / 2.0f, castle.GetPos().y + castle.GetSize().y / 2.0f - 40.0f / 5.0f, castle.GetPos().z - castle.GetSize().z / 2.0f), castle_col);
	CreateCone(CW, CH, vec3(castle4.GetPos().x, castle4.GetPos().y + castle4.GetSize().y / 2.0f + CH / 2.0f, castle4.GetPos().z), tower_col);
	Cube castle5 = CreateCube(vec3(CW, CH, CL), vec3(castle.GetPos().x - castle.GetSize().x / 2.0f, castle.GetPos().y + castle.GetSize().y / 2.0f - 40.0f / 5.0f, castle.GetPos().z + castle.GetSize().z / 2.0f), castle_col);
	CreateCone(CW, CH, vec3(castle5.GetPos().x, castle5.GetPos().y + castle5.GetSize().y / 2.0f + CH / 2.0f, castle5.GetPos().z), tower_col);
	Cube castle6 = CreateCube(vec3(CW, CH, CL), vec3(castle.GetPos().x + castle.GetSize().x / 2.0f, castle.GetPos().y + castle.GetSize().y / 2.0f - 40.0f / 5.0f, castle.GetPos().z - castle.GetSize().z / 2.0f), castle_col);
	CreateCone(CW, CH, vec3(castle6.GetPos().x, castle6.GetPos().y + castle6.GetSize().y / 2.0f + CH / 2.0f, castle6.GetPos().z), tower_col);
	//_castle

	// Ground and water
	Cube ground3 = CreateCube(vec3(GW, GH / 2.0f, GL - 20.0f), vec3(ground2.GetPos().x, ground2.GetPos().y, ground2.GetPos().z - ground2.GetSize().z / 2.0f), ground_col, 60.0f, orthonormal_y);
	Cube ground4 = CreateCube(vec3(castle.GetSize().x, 3.0f * GH, GL), vec3(castle.GetPos().x, ground2.GetPos().y + ground2.GetSize().y / 2.0f - (3.0f * GH) / 2.0f, ground2.GetPos().z - ground2.GetSize().z / 2.0f - GL / 2.0f), ground_col);
	Cube ground5 = CreateCube(vec3(castle.GetSize().x, GH, GL), vec3(cu14.GetPos().x - cu14.GetSize().x / 2.0f + castle.GetSize().x / 2.0f, cu14.GetPos().y - cu14.GetSize().y / 2.0f - GH / 2.0f, ground4.GetPos().z + ground4.GetSize().z / 2.0f - GL / 2.0f), ground_col);
	Cube ground6 = CreateCube(vec3(castle.GetSize().x, GH, cu15.GetSize().z), vec3(cu15.GetPos().x - cu15.GetSize().x / 2.0f + castle.GetSize().x / 2.0f, cu15.GetPos().y - cu15.GetSize().y / 2.0f - GH / 2.0f, cu15.GetPos().z), ground_col);
	Cube ground7 = CreateCube(vec3(3.0f * castle.GetSize().x + 30.0f, 2.0f * GH, 5.0f * cu16.GetSize().z), vec3(cu16.GetPos().x - cu16.GetSize().x / 2.0f + (3.0f * castle.GetSize().x + 30.0f) / 2.0f, cu16.GetPos().y - cu16.GetSize().y / 2.0f - (2.0f * GH) / 2.0f, cu16.GetPos().z + cu16.GetSize().z / 2.0f - (5.0f * cu16.GetSize().z) / 2.0f), ground_col);
	Cube ground8 = CreateCube(vec3(GW, 5.0f * GH, ground2.GetSize().z), vec3(ground5.GetPos().x + ground5.GetSize().x / 2.0f - GW / 2.0f, ground2.GetPos().y - ground2.GetSize().y / 2.0f - (5.0f * GH) / 2.0f, ground5.GetPos().z + ground5.GetSize().z / 2.0f + ground2.GetSize().z / 2.0f), ground_col);
	Cube ground9 = CreateCube(vec3(GW / 5.0f, 4.0f * GH, ground2.GetSize().z), vec3(water.GetPos().x + water.GetSize().x / 2.0f + (GW / 5.0f) / 2.0f, water.GetPos().y + water.GetSize().y / 2.0f - (4.0f * GH) / 2.0f, castle.GetPos().z - castle.GetSize().z / 2.0f - ground2.GetSize().z / 2.0f), ground_col);
	Cube ground10 = CreateCube(vec3(2.0f * GW, 4.0f * GH, ground2.GetSize().z / 4.0f), vec3(ground9.GetPos().x - ground9.GetSize().x / 2.0f - (2.0f * GW) / 2.0f, ground9.GetPos().y, ground9.GetPos().z - ground9.GetSize().z / 2.0f + (ground2.GetSize().z / 4.0f) / 2.0f), ground_col);
	Cube water2 = CreateCube(vec3(2.0f * ground5.GetSize().x, ground5.GetSize().y, 4.0f * ground5.GetSize().z), vec3(ground5.GetPos().x + ground5.GetSize().x / 2.0f - (2.0f * ground5.GetSize().x) / 2.0f, ground5.GetPos().y - ground5.GetSize().y, ground5.GetPos().z + ground5.GetSize().z / 2.0f - (4.0f * ground5.GetSize().z) / 2.0f), water_col);
	Cube water3 = CreateCube(vec3(3.0f * ground5.GetSize().x + 50.0f, 2.0f * ground5.GetSize().y, 2.0f * ground5.GetSize().z), vec3(water.GetPos().x - water.GetSize().x / 2.0f + (3.0f * ground5.GetSize().x + 50.0f) / 2.0f, water.GetPos().y - water.GetSize().y / 2.0f - (2.0f * ground5.GetSize().y) / 2.0f, water2.GetPos().z - water2.GetSize().z / 2.0f + (2.0f * ground5.GetSize().z) / 2.0f), water_col);
	Cube water4 = CreateCube(vec3((ground7.GetPos().x + ground7.GetSize().x / 2.0f) - (water.GetPos().x - water.GetSize().x / 2.0f), 4.0f * ground5.GetSize().y, ground5.GetSize().z), vec3(water.GetPos().x - water.GetSize().x / 2.0f + ((ground7.GetPos().x + ground7.GetSize().x / 2.0f) - (water.GetPos().x - water.GetSize().x / 2.0f)) / 2.0f, water2.GetPos().y - water2.GetSize().y / 2.0f - (4.0f * ground5.GetSize().y) / 2.0f, water2.GetPos().z - water2.GetSize().z / 2.0f - ground5.GetSize().z / 2.0f), water_col);
	Cube water5 = CreateCube(vec3(ground7.GetSize().x, water4.GetSize().y, ground7.GetSize().z), vec3(water4.GetPos().x + water4.GetSize().x / 2.0f - ground7.GetSize().x / 2.0f, water4.GetPos().y, ground7.GetPos().z), water_col);
	Cube water6 = CreateCube(vec3(water.GetSize().x, water.GetSize().y, ground2.GetSize().z), vec3(water.GetPos().x, water.GetPos().y + ground2.GetSize().y / 2.0f - water.GetSize().y / 2.0f, ground2.GetPos().z), water_col);
	Cube water7 = CreateCube(vec3(water.GetSize().x, water.GetSize().y, (ground2.GetPos().z + ground2.GetSize().z / 2.0f) - (ground7.GetPos().z - ground7.GetSize().z / 2.0f)), vec3(ground7.GetPos().x - ground7.GetSize().x / 2.0f + water.GetSize().x / 2.0f, water3.GetPos().y - water3.GetSize().y / 2.0f - water.GetSize().y / 2.0f, ground2.GetPos().z + ground2.GetSize().z / 2.0f - ((ground2.GetPos().z + ground2.GetSize().z / 2.0f) - (ground7.GetPos().z - ground7.GetSize().z / 2.0f)) / 2.0f), water_col);
	//_ground_and_water
	//_2nd_section

	// 3rd section
	Cylinder cy8 = CreateCylinder((3.0f * RW) / 2.0f, RH, vec3(cu16.GetPos().x, cu16.GetPos().y, cu16.GetPos().z - cu16.GetSize().z / 2.0f), road_col);
	Cube cu17 = CreateCube(vec3(2.0f * RL, RH, 3.0f * RW), vec3(cy8.GetPos().x + (2.0f * RL) / 2.0f, cy8.GetPos().y, cy8.GetPos().z), road_col);
	Cylinder cy9 = CreateCylinder((3.0f * RW) / 2.0f, RH, vec3(cu17.GetPos().x + cu17.GetSize().x / 2.0f, cu17.GetPos().y, cu17.GetPos().z), road_col);
	cu18 = CreateCube(vec3(3.0f * RW, RH, 6.0f * RL), vec3(cy9.GetPos().x, cy9.GetPos().y, cy9.GetPos().z - (6.0f * RL) / 2.0f), road_col);
	Cylinder cy10 = CreateCylinder((3.0f * RW) / 2.0f, RH, vec3(cu18.GetPos().x, cu18.GetPos().y, cu18.GetPos().z - cu18.GetSize().z / 2.0f), road_col);
	Cube cu19 = CreateCube(vec3(4.0f * RL, RH, 3.0f * RW), vec3(cy10.GetPos().x + (4.0f * RL) / 2.0f, cy10.GetPos().y, cy10.GetPos().z), road_col);
	Cylinder cy11 = CreateCylinder((3.0f * RW) / 2.0f, RH, vec3(cu19.GetPos().x + cu19.GetSize().x / 2.0f, cu19.GetPos().y, cu19.GetPos().z), road_col);
	Cube cu20 = CreateCube(vec3(3.0f * RW, RH, 3.0f * RL), vec3(cy11.GetPos().x, cy11.GetPos().y, cy11.GetPos().z + (3.0f * RL) / 2.0f), road_col);
	Cylinder cy12 = CreateCylinder((3.0f * RW) / 2.0f, RH, vec3(cu20.GetPos().x, cu20.GetPos().y, cu20.GetPos().z + cu20.GetSize().z / 2.0f), road_col);
	cu21 = CreateCube(vec3(RL, RH, 3.0f * RW), vec3(cy12.GetPos().x - RL / 2.0f, cy12.GetPos().y, cy12.GetPos().z), road_col);
	Cube cu22 = CreateCube(vec3(4.0f * RL + RL / 2.0f + RW / 2.0f, RH, 3.0f * RW), vec3(cu21.GetPos().x - cu21.GetSize().x / 2.0f, cu21.GetPos().y, cu21.GetPos().z), road_col, -20.0f, orthonormal_z);

	// Tunnel
	float TUL = 6.0f * RL - 2.0f * cy9.radius;

	Cube tunnel_wall = CreateCube(vec3(TUW, TUH, TUL), vec3(cu17.GetPos().x + (2.0f * RL) / 2.0f - cy9.radius - TUW / 2.0f, cu17.GetPos().y, cy9.GetPos().z - cy9.radius - TUL / 2.0f), road_col);
	Cube tunnel_wall2 = CreateCube(vec3(TUW, TUH, TUL), vec3(cu17.GetPos().x + (2.0f * RL) / 2.0f + cy9.radius + TUW / 2.0f, cu17.GetPos().y, cy9.GetPos().z - cy9.radius - TUL / 2.0f), road_col);
	Cube tunnel_wall3 = CreateCube(vec3((tunnel_wall2.GetPos().x + tunnel_wall2.GetSize().x / 2.0f) - (tunnel_wall.GetPos().x - tunnel_wall.GetSize().x / 2.0f), CW / 4.0f, TUL), vec3(tunnel_wall.GetPos().x + tunnel_wall.GetSize().x / 2.0f + cu18.GetSize().x / 2.0f, tunnel_wall.GetPos().y + tunnel_wall.GetSize().y / 2.0f + (CW / 4.0f) / 2.0f, tunnel_wall.GetPos().z), road_col);
	//_tunnel
	//_3rd_section

	// 4th section
	cu23 = CreateCube(vec3(2.0f * RL, RH, RW / 2.0f + 1.0f), vec3(tunnel_wall3.GetPos().x - tunnel_wall3.GetSize().x / 2.0f - (2.0f * RL) / 2.0f, tunnel_wall3.GetPos().y + tunnel_wall3.GetSize().y / 2.0f - RH / 2.0f, tunnel_wall3.GetPos().z + RW / 2.0f + 0.5f), road_col);
	cu24 = CreateCube(vec3(2.0f * RL, RH, RW / 2.0f + 1.0f), vec3(tunnel_wall3.GetPos().x - tunnel_wall3.GetSize().x / 2.0f - (2.0f * RL) / 2.0f, tunnel_wall3.GetPos().y + tunnel_wall3.GetSize().y / 2.0f - RH / 2.0f, tunnel_wall3.GetPos().z - RW / 2.0f - 0.5f), road_col);
	Cube cu25 = CreateCube(vec3(RL, RH, cu23.GetPos().z - (cu24.GetPos().z - cu24.GetSize().z)), vec3((cu24.GetPos().x - cu24.GetSize().x / 2.0f) - RL / 2.0f, tunnel_wall3.GetPos().y + tunnel_wall3.GetSize().y / 2.0f - RH / 2.0f, tunnel_wall3.GetPos().z), road_col);
	Cube cu26 = CreateCube(vec3((cu25.GetPos().x - cu25.GetSize().x / 2.0f) - (ground9.GetPos().x + ground9.GetSize().x / 2.0f) + 2.0f, RH, cu25.GetSize().z), vec3(cu25.GetPos().x - cu25.GetSize().x / 2.0f - ((cu25.GetPos().x - cu25.GetSize().x / 2.0f) - (ground9.GetPos().x + ground9.GetSize().x / 2.0f)) / 2.0f, cu25.GetPos().y - ((cu25.GetPos().x - cu25.GetSize().x / 2.0f) - (ground9.GetPos().x + ground9.GetSize().x / 2.0f)) / 2.0f * tan(DEGTORAD * 12.0f), cu25.GetPos().z), road_col, 12.0f, orthonormal_z);
	Cube cu27 = CreateCube(vec3(RL, RH, cu26.GetSize().z), vec3(ground9.GetPos().x, ground9.GetPos().y + ground9.GetSize().y / 2.0f + RH / 2.0f, cu26.GetPos().z), road_col);
	Cylinder cy13 = CreateCylinder(RW, RH, vec3(cu27.GetPos().x - cu27.GetSize().x, cu27.GetPos().y - 10.0f * cu27.GetSize().y, cu27.GetPos().z), road_col);
	Cylinder cy14 = CreateCylinder(RW / 3.0f, 25.0f * RH, vec3(cy13.GetPos().x, cy13.GetPos().y + cy13.height / 2.0f - (25.0f * RH) / 2.0f, cy13.GetPos().z), road_col);
	Cylinder cy15 = CreateCylinder(RW, RH, vec3(cy13.GetPos().x - cu27.GetSize().x, cy13.GetPos().y, cy13.GetPos().z), road_col);
	Cylinder cy16 = CreateCylinder(RW / 3.0f, 25.0f * RH, vec3(cy15.GetPos().x, cy15.GetPos().y + cy15.height / 2.0f - (25.0f * RH) / 2.0f, cy15.GetPos().z), road_col);
	Cube cu28 = CreateCube(vec3(RL, RH, RW), vec3(cy16.GetPos().x - cu27.GetSize().x, cy15.GetPos().y, cy16.GetPos().z), road_col);
	Cylinder cy18 = CreateCylinder(RW / 2.0f, RH, vec3(cu28.GetPos().x - cu28.GetSize().x / 2.0f, cu28.GetPos().y, cu28.GetPos().z), road_col);
	Cylinder cy19 = CreateCylinder(RW / 3.0f, 25.0f * RH, vec3(cy18.GetPos().x, cy18.GetPos().y + cy18.height / 2.0f - (25.0f * RH) / 2.0f, cy18.GetPos().z), road_col);
	Cube cu29 = CreateCube(vec3(RW, RH, RL), vec3(cy18.GetPos().x, cy18.GetPos().y, cy18.GetPos().z - RL / 2.0f), road_col);
	Cylinder cy20 = CreateCylinder(RW / 2.0f, RH, vec3(cu29.GetPos().x, cu29.GetPos().y, cu29.GetPos().z - cu29.GetSize().z / 2.0f), road_col);
	Cylinder cy21 = CreateCylinder(RW / 3.0f, 25.0f * RH, vec3(cy20.GetPos().x, cy20.GetPos().y + cy20.height / 2.0f - (25.0f * RH) / 2.0f, cy20.GetPos().z), road_col);
	Cube cu30 = CreateCube(vec3((cu29.GetPos().x - cu29.GetSize().x) - (cu.GetPos().x - cu.GetSize().x), RH, RW), vec3(cy20.GetPos().x - ((cu29.GetPos().x - cu29.GetSize().x) - (cu.GetPos().x - cu.GetSize().x)) / 2.0f, cy20.GetPos().y, cy20.GetPos().z), road_col);
	Cylinder cy22 = CreateCylinder(RW / 2.0f, RH, vec3(cu30.GetPos().x - cu30.GetSize().x / 2.0f, cu30.GetPos().y, cu30.GetPos().z), road_col);
	Cylinder cy23 = CreateCylinder(RW / 3.0f, 25.0f * RH, vec3(cy22.GetPos().x, cy22.GetPos().y + cy22.height / 2.0f - (25.0f * RH) / 2.0f, cy22.GetPos().z), road_col);
	Cube cu31 = CreateCube(vec3(RW, RH, cy22.radius), vec3(cy22.GetPos().x, cy22.GetPos().y, cy22.GetPos().z + cy22.radius / 2.0f), road_col);
	Cube cu32 = CreateCube(vec3(RW, RH, RL + 2.0f * RW + 0.5f), vec3(cu31.GetPos().x, cu31.GetPos().y - ((RL + 2.0f * RW + 0.5f) / 2.0f) * tan(DEGTORAD * - 21.0f) - 0.6f, cu31.GetPos().z + cu31.GetSize().z / 2.0f + (RL + 2.0f * RW + 0.5f) / 2.0f - 2.1f), road_col, -21.0f, orthonormal_x);
	Cube cu33 = CreateCube(vec3(RW, RH, RL), vec3(cu.GetPos().x, cu.GetPos().y, ground.GetPos().z - ground.GetSize().z / 2.0f + RL / 2.0f), road_col);
	//_4th_section

	// Hinges (rotating elements)
	rotating_cu = CreateCube(vec3(HcuW, HcuH, HcuL), vec3(cu6.GetPos().x + 20.0f, cu6.GetPos().y + 2.0f * HcuH, cu6.GetPos().z - HcuL / 2.0f), hinge_col, 0.0f, vec3(0.0f,0.0f,0.0f), 0.0f, false, false);
	bodyA = App->physics->AddBody(rotating_cu, 10000.0f);
	Cylinder rotating_cy = CreateCylinder(HcyrotW, HcyH, vec3(rotating_cu.GetPos().x, rotating_cu.GetPos().y - HcyH / 2.0f, rotating_cu.GetPos().z), hinge_col, true, 0.0f, vec3_zero, 0.0f, false, false);
	PhysBody3D* bodyB = App->physics->AddBody(rotating_cy, 10000.0f);
	Cylinder real_cy = CreateCylinder(HcyrealW, HcyH, vec3(rotating_cu.GetPos().x, rotating_cu.GetPos().y - HcyH / 2.0f, rotating_cu.GetPos().z), hinge_col);

	rotating_cu2 = CreateCube(vec3(HcuW, HcuH, HcuL), vec3(cu6.GetPos().x - 20.0f, cu6.GetPos().y + 2.0f * HcuH, cu6.GetPos().z + HcuL / 2.0f), hinge_col, 0.0f, vec3_zero, 0.0f, false, false);
	bodyA2 = App->physics->AddBody(rotating_cu2, 10000.0f);
	Cylinder rotating_cy2 = CreateCylinder(HcyrotW, HcyH, vec3(rotating_cu2.GetPos().x, rotating_cu2.GetPos().y - HcyH / 2.0f, rotating_cu2.GetPos().z), hinge_col, true, 0.0f, vec3_zero, 0.0f, false, false);
	PhysBody3D* bodyB2 = App->physics->AddBody(rotating_cy2, 10000.0f);
	Cylinder real_cy2 = CreateCylinder(HcyrealW, HcyH, vec3(rotating_cu2.GetPos().x, rotating_cu2.GetPos().y - HcyH / 2.0f, rotating_cu2.GetPos().z), hinge_col);

	bodyA->GetBody()->setLinearFactor(btVector3(0, 0, 0));
	bodyA2->GetBody()->setLinearFactor(btVector3(0, 0, 0));
	bodyB->GetBody()->setAngularFactor(btVector3(0, 0, 0));
	bodyB2->GetBody()->setAngularFactor(btVector3(0, 0, 0));

	App->physics->AddConstraintHinge(*bodyA, *bodyB, vec3(0.0f, 0.0f, 0.0f), vec3(0.0f, 0.0f, 0.0f), vec3(0.0f, -1.0f, 0.0f), vec3(0.0f, 0.0f, 0.0f), true, true);
	App->physics->AddConstraintHinge(*bodyA2, *bodyB2, vec3(0.0f, 0.0f, 0.0f), vec3(0.0f, 0.0f, 0.0f), vec3(0.0f, 1.0f, 0.0f), vec3(0.0f, 0.0f, 0.0f), true, true);

	// Finish line
	CreateFinishLine(vec3(FLW, RH, FLL), vec3(FLW / 2.0f - cu.GetSize().x / 2.0f, 0.1f, 0.0f));

	Cylinder bar = CreateCylinder(BR, BH, vec3(cu.GetPos().x - cu.GetSize().x, cu.GetPos().y - cu.GetSize().y, cu.GetPos().z), finish_line_col);
	Cylinder bar2 = CreateCylinder(BR, BH, vec3(cu.GetPos().x + cu.GetSize().x, cu.GetPos().y - cu.GetSize().y, cu.GetPos().z), finish_line_col);
	Cube finish_line = CreateCube(vec3(bar2.GetPos().x - bar.GetPos().x, BH / 6.0f, RH), vec3(cu.GetPos().x, cu.GetPos().y - cu.GetSize().y + BH / 2.0f, cu.GetPos().z), finish_line_col);
	//_finish_line

	// Trees
	// ground
	Cylinder trunk = CreateCylinder(TRUR, TRUH, vec3(ground.GetPos().x - ground.GetSize().x / 3.0f, ground.GetPos().y + ground.GetSize().y / 2.0f, ground.GetPos().z + ground.GetSize().z / 3.0f), trunk_col);
	CreateCone(TRER, TREH, vec3(trunk.GetPos().x, trunk.GetPos().y + trunk.height / 2.0f + TREH / 2.0f, trunk.GetPos().z), tree_col);
	Cylinder trunk2 = CreateCylinder(TRUR, TRUH, vec3(ground.GetPos().x - ground.GetSize().x / 4.0f, ground.GetPos().y + ground.GetSize().y / 2.0f, ground.GetPos().z - ground2.GetSize().z / 15.0f), trunk_col);
	CreateCone(TRER, TREH, vec3(trunk2.GetPos().x, trunk2.GetPos().y + trunk2.height / 2.0f + TREH / 2.0f, trunk2.GetPos().z), tree_col);
	Cylinder trunk3 = CreateCylinder(TRUR, TRUH, vec3(ground.GetPos().x - ground.GetSize().x / 3.0f - ground.GetSize().x / 15.0f, ground.GetPos().y + ground.GetSize().y / 2.0f, ground.GetPos().z - ground2.GetSize().z / 6.0f), trunk_col);
	CreateCone(TRER2, TREH2, vec3(trunk3.GetPos().x, trunk3.GetPos().y + trunk3.height / 2.0f + TREH2 / 2.0f, trunk3.GetPos().z), tree_col);
	Cylinder trunk4 = CreateCylinder(TRUR, TRUH, vec3(ground.GetPos().x + ground.GetSize().x / 3.0f, ground.GetPos().y + ground.GetSize().y / 2.0f, ground.GetPos().z - ground2.GetSize().z / 8.0f), trunk_col);
	CreateCone(TRER2, TREH, vec3(trunk4.GetPos().x, trunk4.GetPos().y + trunk4.height / 2.0f + TREH / 2.0f, trunk4.GetPos().z), tree_col);
	
	// ground2
	Cylinder trunk5 = CreateCylinder(TRUR, TRUH, vec3(ground2.GetPos().x - ground2.GetSize().x / 3.0f, ground2.GetPos().y + ground2.GetSize().y / 2.0f, ground2.GetPos().z + ground2.GetSize().z / 3.0f), trunk_col);
	CreateCone(TRER, TREH, vec3(trunk5.GetPos().x, trunk5.GetPos().y + trunk5.height / 2.0f + TREH / 2.0f, trunk5.GetPos().z), tree_col);
	Cylinder trunk6 = CreateCylinder(TRUR, TRUH, vec3(ground2.GetPos().x - ground2.GetSize().x / 3.0f - ground2.GetSize().x / 10.0f, ground2.GetPos().y + ground2.GetSize().y / 2.0f, ground2.GetPos().z + ground2.GetSize().z / 4.0f), trunk_col);
	CreateCone(TRER3, TREH3, vec3(trunk6.GetPos().x, trunk6.GetPos().y + trunk6.height / 2.0f + TREH3 / 2.0f, trunk6.GetPos().z), tree_col);

	// ground4
	Cylinder trunk7 = CreateCylinder(TRUR, TRUH, vec3(ground4.GetPos().x + ground4.GetSize().x / 5.0f, ground4.GetPos().y + ground4.GetSize().y / 2.0f, ground4.GetPos().z), trunk_col);
	CreateCone(TRER, TREH, vec3(trunk7.GetPos().x, trunk7.GetPos().y + trunk7.height / 2.0f + TREH / 2.0f, trunk7.GetPos().z), tree_col);
	Cylinder trunk8 = CreateCylinder(TRUR, TRUH, vec3(ground4.GetPos().x - ground4.GetSize().x / 4.0f, ground4.GetPos().y + ground4.GetSize().y / 2.0f, ground4.GetPos().z + ground4.GetSize().z / 5.0f), trunk_col);
	CreateCone(TRER2, TREH2, vec3(trunk8.GetPos().x, trunk8.GetPos().y + trunk8.height / 2.0f + TREH2 / 2.0f, trunk8.GetPos().z), tree_col);
	Cylinder trunk9 = CreateCylinder(TRUR, TRUH, vec3(ground4.GetPos().x - ground4.GetSize().x / 3.0f, ground4.GetPos().y + ground4.GetSize().y / 2.0f, ground4.GetPos().z - ground4.GetSize().z / 4.0f), trunk_col);
	CreateCone(TRER3, TREH3, vec3(trunk9.GetPos().x, trunk9.GetPos().y + trunk9.height / 2.0f + TREH3 / 2.0f, trunk9.GetPos().z), tree_col);
	Cylinder trunk10 = CreateCylinder(TRUR, TRUH, vec3(ground4.GetPos().x, ground4.GetPos().y + ground4.GetSize().y / 2.0f, ground4.GetPos().z + ground4.GetPos().z / 6.0f), trunk_col);
	CreateCone(TRER3, TREH, vec3(trunk10.GetPos().x, trunk10.GetPos().y + trunk10.height / 2.0f + TREH / 2.0f, trunk10.GetPos().z), tree_col);

	// ground7
	Cylinder trunk11 = CreateCylinder(TRUR, TRUH, vec3(ground7.GetPos().x + ground7.GetSize().x / 3.0f, ground7.GetPos().y + ground7.GetSize().y / 2.0f, ground7.GetPos().z + ground7.GetSize().z / 3.0f), trunk_col);
	CreateCone(TRER, TREH, vec3(trunk11.GetPos().x, trunk11.GetPos().y + trunk11.height / 2.0f + TREH / 2.0f, trunk11.GetPos().z), tree_col);
	Cylinder trunk12 = CreateCylinder(TRUR, TRUH, vec3(ground7.GetPos().x + ground7.GetSize().x / 4.0f, ground7.GetPos().y + ground7.GetSize().y / 2.0f, ground7.GetPos().z + ground7.GetSize().z / 3.0f + ground7.GetSize().z / 10.0f), trunk_col);
	CreateCone(TRER3, TREH2, vec3(trunk12.GetPos().x, trunk12.GetPos().y + trunk12.height / 2.0f + TREH2 / 2.0f, trunk12.GetPos().z), tree_col);
	Cylinder trunk13 = CreateCylinder(TRUR, TRUH, vec3(ground7.GetPos().x + ground7.GetSize().x / 5.0f, ground7.GetPos().y + ground7.GetSize().y / 2.0f, ground7.GetPos().z + ground7.GetSize().z / 4.0f + ground7.GetSize().z / 8.0f), trunk_col);
	CreateCone(TRER3, TREH3, vec3(trunk13.GetPos().x, trunk13.GetPos().y + trunk13.height / 2.0f + TREH3 / 2.0f, trunk13.GetPos().z), tree_col);
	Cylinder trunk14 = CreateCylinder(TRUR, TRUH, vec3(ground7.GetPos().x + ground7.GetSize().x / 3.0f + ground7.GetSize().x / 10.0f, ground7.GetPos().y + ground7.GetSize().y / 2.0f, ground7.GetPos().z + ground7.GetSize().z / 3.0f - ground7.GetSize().z / 8.0f), trunk_col);
	CreateCone(TRER, TREH2, vec3(trunk14.GetPos().x, trunk14.GetPos().y + trunk14.height / 2.0f + TREH2 / 2.0f, trunk14.GetPos().z), tree_col);
	Cylinder trunk15 = CreateCylinder(TRUR, TRUH, vec3(ground7.GetPos().x + ground7.GetSize().x / 5.0f + ground7.GetSize().x / 10.0f, ground7.GetPos().y + ground7.GetSize().y / 2.0f, ground7.GetPos().z + ground7.GetSize().z / 3.0f - ground7.GetSize().z / 5.0f), trunk_col);
	CreateCone(TRER2, TREH, vec3(trunk15.GetPos().x, trunk15.GetPos().y + trunk15.height / 2.0f + TREH / 2.0f, trunk15.GetPos().z), tree_col);
	Cylinder trunk16 = CreateCylinder(TRUR, TRUH, vec3(ground7.GetPos().x + ground7.GetSize().x / 5.0f + ground7.GetSize().x / 5.0f, ground7.GetPos().y + ground7.GetSize().y / 2.0f, ground7.GetPos().z + ground7.GetSize().z / 3.0f - ground7.GetSize().z / 4.0f), trunk_col);
	CreateCone(TRER, TREH, vec3(trunk16.GetPos().x, trunk16.GetPos().y + trunk16.height / 2.0f + TREH / 2.0f, trunk16.GetPos().z), tree_col);
	Cylinder trunk17 = CreateCylinder(TRUR, TRUH, vec3(ground7.GetPos().x + ground7.GetSize().x / 10.0f, ground7.GetPos().y + ground7.GetSize().y / 2.0f, ground7.GetPos().z + ground7.GetSize().z / 4.5f), trunk_col);
	CreateCone(TRER, TREH, vec3(trunk17.GetPos().x, trunk17.GetPos().y + trunk17.height / 2.0f + TREH / 2.0f, trunk17.GetPos().z), tree_col);
	//_trees

	// Sky
	float SL = (ground2.GetPos().z + ground2.GetSize().z / 2.0f) - (water4.GetPos().z - water4.GetSize().z / 2.0f);

	Cube sky_wall = CreateCube(vec3(SW, SH, SL), vec3(water.GetPos().x - water.GetSize().x / 2.0f, ground.GetPos().y, water4.GetPos().z - water4.GetSize().z / 2.0f + SL / 2.0f), sky_col);
	Cube sky_wall2 = CreateCube(vec3(SL, SH, SW), vec3(sky_wall.GetPos().x + SL / 2.0f, sky_wall.GetPos().y, sky_wall.GetPos().z + sky_wall.GetSize().z / 2.0f), sky_col);
	Cube sky_wall3 = CreateCube(vec3(SW, SH, SL), vec3(sky_wall2.GetPos().x + sky_wall2.GetSize().x / 2.0f, sky_wall2.GetPos().y, sky_wall2.GetPos().z - SL / 2.0f), sky_col);
	Cube sky_wall4 = CreateCube(vec3(SL, SH, SW), vec3(sky_wall3.GetPos().x - SL / 2.0f, sky_wall3.GetPos().y, sky_wall3.GetPos().z - sky_wall3.GetSize().z / 2.0f), sky_col);
	Cube sky_ceiling = CreateCube(vec3(SL, SW, SL), vec3(sky_wall.GetPos().x + SL / 2.0f, sky_wall.GetPos().y + sky_wall.GetSize().y / 2.0f, sky_wall.GetPos().z), sky_col);
	//_sky

	// Obstacles
	Cylinder obs = CreateCylinder(OR, OH, vec3(cu8.GetPos().x - cu8.GetSize().x / 4.0f, cu8.GetPos().y + cu8.GetSize().y / 2.0f + OH / 2.0f, cu8.GetPos().z - cu8.GetSize().z / 4.0f), obstacle_col);
	Cylinder obs2 = CreateCylinder(OR, OH, vec3(cu8.GetPos().x + cu8.GetSize().x / 4.0f, cu8.GetPos().y + cu8.GetSize().y / 2.0f + OH / 2.0f, cu8.GetPos().z + cu8.GetSize().z / 4.0f), obstacle_col);
	Cylinder obs3 = CreateCylinder(OR, OH, vec3(cy5.GetPos().x, cy5.GetPos().y + cy5.height / 2.0f + OH / 2.0f, cy5.GetPos().z), obstacle_col);
	Cylinder obs4 = CreateCylinder(OR, OH, vec3(cu9.GetPos().x + cu9.GetSize().x / 3.0f, cu9.GetPos().y + cu9.GetSize().y / 2.0f + OH / 2.0f, cu9.GetPos().z + cu9.GetSize().z / 4.0f), obstacle_col);
	Cylinder obs5 = CreateCylinder(OR, OH, vec3(cu9.GetPos().x + cu9.GetSize().x / 3.0f, cu9.GetPos().y + cu9.GetSize().y / 2.0f + OH / 2.0f, cu9.GetPos().z - cu9.GetSize().z / 4.0f), obstacle_col);
	Cylinder obs6 = CreateCylinder(OR, OH, vec3(cy7.GetPos().x, cy7.GetPos().y + cy7.height / 2.0f + OH / 2.0f, cy7.GetPos().z), obstacle_col);
	Cylinder obs7 = CreateCylinder(OR, OH, vec3(cu15.GetPos().x, cu15.GetPos().y + cu15.GetSize().y / 2.0f + OH / 2.0f, cu15.GetPos().z), obstacle_col);
	Cylinder obs8 = CreateCylinder(OR, OH, vec3(cu17.GetPos().x, cu17.GetPos().y + cu17.GetSize().y / 2.0f + OH / 2.0f, cu17.GetPos().z), obstacle_col);
	Cylinder obs9 = CreateCylinder(OR, OH, vec3(cu18.GetPos().x, cu18.GetPos().y + cu18.GetSize().y / 2.0f + OH / 2.0f, cu18.GetPos().z), obstacle_col);
	Cylinder obs10 = CreateCylinder(OR, OH, vec3(cu20.GetPos().x + cu20.GetSize().x / 5.0f, cu20.GetPos().y + cu20.GetSize().y / 2.0f + OH / 2.0f, cu20.GetPos().z + cu20.GetSize().z / 5.0f), obstacle_col);
	Cylinder obs11 = CreateCylinder(OR, OH, vec3(cu20.GetPos().x - cu20.GetSize().x / 5.0f, cu20.GetPos().y + cu20.GetSize().y / 2.0f + OH / 2.0f, cu20.GetPos().z - cu20.GetSize().z / 5.0f), obstacle_col);
	Cylinder obs12 = CreateCylinder(OR, OH, vec3(cu25.GetPos().x + cu25.GetSize().x / 4.0f, cu25.GetPos().y + cu25.GetSize().y / 2.0f + OH / 2.0f, cu23.GetPos().z), obstacle_col);
	Cylinder obs13 = CreateCylinder(OR, OH, vec3(cy18.GetPos().x, cy18.GetPos().y + cy18.height / 2.0f + OH / 2.0f, cy18.GetPos().z), obstacle_col);
	//_obstacles

	// Road limits
	bool pos[4] = { false, false, true, true };
	bool inv_col[4] = { false,false,false,false };
	bool inv_pos[4] = { false,false,false,false };
	uint i = 0;

	// From start to hinges
	CreateLimits((Primitive*)&cu, pos, inv_col, inv_pos);

	inv_col[2] = true;
	inv_col[3] = true;
	CreateLimits((Primitive*)&cu2, pos, inv_col, inv_pos, vec4(0.0f,0.0f, cu3.GetSize().z / 2.0f, cu3.GetSize().z / 2.0f));

	pos[i] = true;
	pos[++i] = false;
	pos[++i] = false;
	pos[++i] = true;
	i = 0;
	inv_col[2] = false;
	inv_col[0] = true;
	CreateLimits((Primitive*)&cy, pos, inv_col, inv_pos);

	pos[i] = true;
	pos[++i] = true;
	pos[++i] = false;
	pos[++i] = false;
	i = 0;
	inv_pos[1] = true;
	inv_col[0] = false;
	inv_col[1] = true;
	inv_col[3] = false;
	CreateLimits((Primitive*)&cu3, pos, inv_col, inv_pos, vec4(cu2.GetSize().x / 2.0f, cu2.GetSize().x / 2.0f,0.0f,0.0f));

	pos[i] = false;
	pos[++i] = true;
	pos[++i] = true;
	pos[++i] = false;
	i = 0;
	inv_pos[1] = false;
	inv_col[2] = true;
	CreateLimits((Primitive*)&cy2, pos, inv_col, inv_pos);

	pos[i] = false;
	pos[++i] = false;
	pos[++i] = true;
	pos[++i] = true;
	i = 0;
	inv_pos[3] = true;
	inv_col[1] = false;
	inv_col[2] = false;
	inv_col[3] = true;
	CreateLimits((Primitive*)&cu4, pos, inv_col, inv_pos, vec4(0.0f,0.0f, cu3.GetSize().z / 2.0f, cu3.GetSize().z / 2.0f));

	pos[i] = false;
	pos[++i] = false;
	pos[++i] = false;
	pos[++i] = true;
	i = 0;
	inv_pos[3] = false;
	CreateLimits((Primitive*)&cy3, pos, inv_col, inv_pos);

	pos[i] = false;
	pos[++i] = true;
	pos[++i] = false;
	pos[++i] = false;
	i = 0;
	inv_pos[1] = true;
	inv_col[1] = true;
	inv_col[3] = false;
	CreateLimits((Primitive*)&cu5, pos, inv_col, inv_pos, vec4(0.0f, cu4.GetSize().x / 2.0f,0.0f,0.0f));

	pos[i] = false;
	pos[++i] = true;
	pos[++i] = true;
	pos[++i] = false;
	i = 0;
	inv_pos[1] = false;
	inv_col[2] = true;
	CreateLimits((Primitive*)&cy4, pos, inv_col, inv_pos);

	// From hinges to tunnel
	pos[i] = false;
	pos[++i] = false;
	pos[++i] = true;
	pos[++i] = true;
	i = 0;
	inv_col[1] = false;
	inv_col[2] = false;
	CreateLimits((Primitive*)&cu8, pos, inv_col, inv_pos, vec4(0.0f,0.0f, cu9.GetSize().z / 2.0f,0.0f));

	pos[i] = true;
	pos[++i] = false;
	pos[++i] = false;
	pos[++i] = true;
	i = 0;
	CreateLimits((Primitive*)&cy5, pos, inv_col, inv_pos);

	pos[i] = true;
	pos[++i] = true;
	pos[++i] = false;
	pos[++i] = false;
	i = 0;
	inv_pos[1] = true;
	inv_col[1] = true;
	CreateLimits((Primitive*)&cu9, pos, inv_col, inv_pos, vec4(0.0f, cu8.GetSize().x / 2.0f,0.0f,0.0f));

	pos[i] = true;
	pos[++i] = false;
	pos[++i] = true;
	pos[++i] = false;
	i = 0;
	inv_pos[1] = false;
	inv_col[1] = false;
	inv_col[2] = true;
	CreateLimits((Primitive*)&cu11, pos, inv_col, inv_pos);

	Cube aux_cu = CreateCube(vec3(cu12.GetSize().x, cu12.GetSize().y, cu12.GetPos().z - (ground2.GetPos().z - ground2.GetSize().z / 2.0f)), vec3(cu12.GetPos().x, cu12.GetPos().y, cu12.GetPos().z - (cu12.GetPos().z - (ground2.GetPos().z - ground2.GetSize().z / 2.0f)) / 2.0f), road_col, 0.0f, vec3(0.0f,0.0f,0.0f), 0.0f, false, false);
	pos[i] = false;
	pos[++i] = false;
	pos[++i] = true;
	pos[++i] = true;
	i = 0;
	inv_col[2] = false;
	CreateLimits((Primitive*)&aux_cu, pos, inv_col, inv_pos);

	pos[i] = false;
	pos[++i] = false;
	pos[++i] = true;
	pos[++i] = false;
	i = 0;
	CreateLimits((Primitive*)&cu14, pos, inv_col, inv_pos);

	CreateLimits((Primitive*)&cu15, pos, inv_col, inv_pos);

	inv_pos[2] = true;
	CreateLimits((Primitive*)&cu16, pos, inv_col, inv_pos, vec4(0.0f,0.0f, cu17.GetSize().z / 2.0f,0.0f));

	pos[i] = false;
	pos[++i] = true;
	pos[++i] = false;
	pos[++i] = false;
	i = 0;
	inv_pos[2] = false;
	inv_col[1] = true;
	CreateLimits((Primitive*)&cy8, pos, inv_col, inv_pos);

	pos[i] = true;
	pos[++i] = true;
	pos[++i] = false;
	pos[++i] = false;
	i = 0;
	inv_pos[0] = true;
	inv_col[1] = false;
	CreateLimits((Primitive*)&cu17, pos, inv_col, inv_pos, vec4(cu18.GetSize().x / 2.0f, cu18.GetSize().x / 2.0f,0.0f,0.0f));

	pos[i] = true;
	pos[++i] = false;
	pos[++i] = true;
	pos[++i] = false;
	i = 0;
	inv_pos[0] = false;
	inv_col[2] = true;
	CreateLimits((Primitive*)&cy9, pos, inv_col, inv_pos);

	// From tunnel to start
	pos[i] = false;
	pos[++i] = true;
	pos[++i] = false;
	pos[++i] = true;
	i = 0;
	inv_col[1] = true;
	inv_col[2] = false;
	CreateLimits((Primitive*)&cy10, pos, inv_col, inv_pos);

	pos[i] = false;
	pos[++i] = true;
	pos[++i] = false;
	pos[++i] = false;
	i = 0;
	inv_col[1] = false;
	CreateLimits((Primitive*)&cu19, pos, inv_col, inv_pos);

	Cube aux_cu2 = CreateCube(vec3(cu19.GetSize().x - cy10.radius - cy11.radius, cu19.GetSize().y, cu19.GetSize().z), vec3(cu19.GetPos().x, cu19.GetPos().y, cu19.GetPos().z), road_col, 0.0f, vec3_zero, 0.0f, false, false);
	pos[i] = true;
	pos[++i] = false;
	pos[++i] = false;
	pos[++i] = false;
	i = 0;
	CreateLimits((Primitive*)&aux_cu2, pos, inv_col, inv_pos);

	pos[i] = false;
	pos[++i] = true;
	pos[++i] = true;
	pos[++i] = false;
	i = 0;
	CreateLimits((Primitive*)&cy11, pos, inv_col, inv_pos);

	pos[i] = false;
	pos[++i] = false;
	pos[++i] = true;
	pos[++i] = false;
	i = 0;
	inv_col[2] = true;
	CreateLimits((Primitive*)&cu20, pos, inv_col, inv_pos);

	Cube aux_cu3 = CreateCube(vec3(cu20.GetSize().x, cu20.GetSize().y, cu20.GetSize().z - cy11.radius - cy12.radius), vec3(cu20.GetPos().x, cu20.GetPos().y, cu20.GetPos().z), road_col, 0.0f, vec3_zero, 0.0f, false, false);
	pos[i] = false;
	pos[++i] = false;
	pos[++i] = false;
	pos[++i] = true;
	i = 0;
	inv_col[2] = false;
	inv_col[3] = true;
	CreateLimits((Primitive*)&aux_cu3, pos, inv_col, inv_pos);
	
	pos[i] = true;
	pos[++i] = false;
	pos[++i] = true;
	pos[++i] = false;
	i = 0;
	inv_col[2] = true;
	inv_col[3] = false;
	CreateLimits((Primitive*)&cy12, pos, inv_col, inv_pos);

	pos[i] = true;
	pos[++i] = true;
	pos[++i] = false;
	pos[++i] = false;
	i = 0;
	inv_col[2] = false;
	CreateLimits((Primitive*)&cu21, pos, inv_col, inv_pos, vec4(0.0f, cu20.GetSize().x / 2.0f,0.0f,0.0f));

	Cube aux_cu4 = CreateCube(vec3(tunnel_wall3.GetSize().x, tunnel_wall3.GetSize().y, cu21.GetSize().z), vec3(tunnel_wall3.GetPos().x, tunnel_wall3.GetPos().y + tunnel_wall3.GetSize().y, tunnel_wall3.GetPos().z), road_col, 0.0f, vec3_zero, 0.0f, false, false);
	CreateLimits((Primitive*)&aux_cu4, pos, inv_col, inv_pos);

	Cube aux_cu5 = CreateCube(vec3(aux_cu4.GetSize().x, aux_cu4.GetSize().y, aux_cu4.GetSize().z), vec3(aux_cu4.GetPos().x + 2.0f, aux_cu4.GetPos().y, aux_cu4.GetPos().z), road_col, 0.0f, vec3_zero, 0.0f, false, false);
	pos[i] = false;
	pos[++i] = false;
	pos[++i] = false;
	pos[++i] = true;
	i = 0;
	inv_col[3] = true;
	CreateLimits((Primitive*)&aux_cu5, pos, inv_col, inv_pos, vec4(0.0f,0.0f,0.0f, tunnel_wall3.GetSize().x / 2.0f + cu23.GetSize().z / 2.0f));

	inv_pos[3] = true;
	CreateLimits((Primitive*)&aux_cu5, pos, inv_col, inv_pos, vec4(0.0f, 0.0f, 0.0f, tunnel_wall3.GetSize().x / 2.0f + cu23.GetSize().z / 2.0f));

	Cube aux_cu6 = CreateCube(vec3(aux_cu5.GetSize().x, aux_cu5.GetSize().y, (cu23.GetPos().z - cu23.GetSize().z) - cu24.GetPos().z), vec3(aux_cu5.GetPos().x, aux_cu5.GetPos().y, aux_cu5.GetPos().z), road_col, 0.0f, vec3_zero, 0.0f, false, false);
	inv_pos[3] = false;
	CreateLimits((Primitive*)&aux_cu6, pos, inv_col, inv_pos);

	pos[i] = true;
	pos[++i] = true;
	pos[++i] = false;
	pos[++i] = false;
	i = 0;
	inv_col[3] = false;
	CreateLimits((Primitive*)&cu27, pos, inv_col, inv_pos);

	pos[i] = false;
	pos[++i] = false;
	pos[++i] = true;
	pos[++i] = true;
	i = 0;
	inv_col[2] = true;
	inv_col[3] = true;
	CreateLimits((Primitive*)&cu33, pos, inv_col, inv_pos);
	//_road_limits
	//_road

	return ret;
}

// Load assets
bool ModuleSceneIntro::CleanUp()
{
	LOG("Unloading Intro scene");

	cubes.clear();
	cylinders.clear();
	cones.clear();

	return true;
}

// Update
update_status ModuleSceneIntro::Update(float dt)
{
	if (App->input->GetKey(SDL_SCANCODE_TAB) == KEY_DOWN && indexMusic != MAX_MUSIC)
		indexMusic += 1;
	else if (App->input->GetKey(SDL_SCANCODE_TAB) == KEY_DOWN && indexMusic == MAX_MUSIC)
		indexMusic = 0;

	if (indexMusic == 0 && !song1_active)
	{
		App->audio->PlayMusic("Music/OGG/P5.ogg", 1.0f);
		song1_active = true;
		song4_active = false;
	}
	else if (indexMusic == 1 && !song2_active)
	{
		App->audio->PlayMusic("MUSIC/OGG/GHOSTBUSTERS.ogg", 1.0f);
		song1_active = false;
		song2_active = true;
	}
	else if (indexMusic == 2 && !song3_active)
	{
		App->audio->PlayMusic("MUSIC/OGG/LETITGO.ogg", 1.0f);

		song2_active = false;
		song3_active = true;
	}
	else if (indexMusic == 3 && !song4_active)
	{
		App->audio->PlayMusic("MUSIC/OGG/POKEMON.ogg", 1.0f);

		song3_active = false;
		song4_active = true;
	}

	if (minutes >= 0) {
		seconds -= 1.0f * dt;

		if (seconds <= 0)
		{
			seconds = 60.0f;
			minutes -= 1;
		}
	}
	else
	{
		endTime = true;
		winCondition = 2;
	}

	// Update sensors
	sensor->GetTransform(&s.transform);
	s.Render();

	sensor3->GetTransform(&h.transform);
	h.Render();

	sensor4->GetTransform(&t.transform);
	h.Render();

	sensor5->GetTransform(&n.transform);
	h.Render();

	// Blit cubes
	p2List_item<Cube>* cubes_it = cubes.getFirst();

	while (cubes_it != nullptr) {
		cubes_it->data.Render();

		cubes_it = cubes_it->next;
	}

	// Blit cylinders
	p2List_item<Cylinder>* cylinders_it = cylinders.getFirst();

	while (cylinders_it != nullptr) {
		cylinders_it->data.Render();

		cylinders_it = cylinders_it->next;
	}

	// Blit cones
	p2List_item<Cone>* cones_it = cones.getFirst();

	while (cones_it != nullptr) {
		cones_it->data.Render();

		cones_it = cones_it->next;
	}

	// Update hinges
	rotating_cu.Render();
	rotating_cu2.Render();

	mat4x4 transform;
	bodyA->GetTransform(transform.M);
	rotating_cu.transform = transform;
	bodyA2->GetTransform(transform.M);
	rotating_cu2.transform = transform;

	// Create and update speed-ups
	float speed_width = 4.0f;
	float speed_height = 0.1f; // y axis
	float speed_length = 2.0f;

	float RH = 0.5f; // y axis

	/// It is only needed to set the size and the position of the first element of the speed-up
	CreateSpeedUp(vec3(speed_width, speed_height, speed_length), vec3(0.0f, RH / 2.0f, 10.0f));
	//
	CreateSpeedUp(vec3(speed_length, speed_height, 2.0f * speed_width), vec3(cu7.GetPos().x - cu7.GetSize().x / 2.0f + speed_length / 2.0f, RH / 2.0f, cu7.GetPos().z), 3, false, -20.0f, orthonormal_x);
	CreateSpeedUp(vec3(speed_length, speed_height, 2.0f * speed_width), vec3(cu7.GetPos().x + speed_length / 2.0f, RH / 2.0f, cu7.GetPos().z), 3, false, -20.0f, orthonormal_x);
	//
	CreateSpeedUp(vec3(2.0f * speed_length, speed_height, 2.0f * speed_width), vec3(cu9.GetPos().x, cu9.GetPos().y + cu9.GetSize().y / 2.0f, cu9.GetPos().z), 4, false);
	//
	CreateSpeedUp(vec3(2.0f * speed_length, speed_height, 2.0f * speed_width), vec3(cu12.GetPos().x - cu12.GetSize().x / 2.0f + (2.0f * speed_length) / 2.0f, ground2.GetPos().y + ground2.GetSize().y / 2.0f, cu12.GetPos().z - 2.0f * speed_width), 3, false);
	CreateSpeedUp(vec3(2.0f * speed_length, speed_height, 2.0f * speed_width), vec3(cu12.GetPos().x - 2.0f * speed_length, ground2.GetPos().y + ground2.GetSize().y / 2.0f, cu12.GetPos().z - 2.0f * speed_width), 3, false);
	CreateSpeedUp(vec3(2.0f * speed_length, speed_height, 2.0f * speed_width), vec3(cu12.GetPos().x + 2.0f * 2.0f * speed_length, ground2.GetPos().y + ground2.GetSize().y / 2.0f, cu12.GetPos().z - 2.0f * speed_width), 3, false);
	//
	CreateSpeedUp(vec3(2.0f * speed_width, speed_height, 2.0f * speed_length), vec3(cu18.GetPos().x - cu18.GetSize().x / 5.0f, cu18.GetPos().y + cu18.GetSize().y / 2.0f, cu18.GetPos().z - cu18.GetSize().z / 5.0f));
	CreateSpeedUp(vec3(2.0f * speed_width, speed_height, 2.0f * speed_length), vec3(cu18.GetPos().x + cu18.GetSize().x / 5.0f, cu18.GetPos().y + cu18.GetSize().y / 2.0f, cu18.GetPos().z + cu18.GetSize().z / 5.0f));
	//
	CreateSpeedUp(vec3(2.0f * speed_width, speed_height, 2.0f * speed_length), vec3(cu21.GetPos().x - cu21.GetSize().x / 2.0f + (2.0f * speed_width) / 2.0f, cu21.GetPos().y + cu21.GetSize().y / 2.0f, cu21.GetPos().z - cu21.GetSize().z / 2.0f + (2.0f * speed_length) / 2.0f), 3);
	CreateSpeedUp(vec3(2.0f * speed_width, speed_height, 2.0f * speed_length), vec3(cu21.GetPos().x - cu21.GetSize().x / 2.0f + (2.0f * speed_width) / 2.0f, cu21.GetPos().y + cu21.GetSize().y / 2.0f, cu21.GetPos().z + cu21.GetSize().z / 2.0f - 2.0f * (2.0f * speed_length) - (2.0f * speed_length) / 2.0f), 3);
	CreateSpeedUp(vec3(2.0f * speed_width, speed_height, 2.0f * speed_length), vec3(cu21.GetPos().x - cu21.GetSize().x / 2.0f + (2.0f * speed_width) / 2.0f, cu21.GetPos().y + cu21.GetSize().y / 2.0f, cu21.GetPos().z - (2.0f * speed_length)), 3);
	//
	CreateSpeedUp(vec3(speed_length, speed_height, speed_width), vec3(cu23.GetPos().x + cu23.GetSize().x / 2.0f, cu23.GetPos().y + cu23.GetSize().y / 2.0f, cu23.GetPos().z), 4, false);
	CreateSpeedUp(vec3(speed_length, speed_height, speed_width), vec3(cu24.GetPos().x + cu24.GetSize().x / 2.0f, cu24.GetPos().y + cu24.GetSize().y / 2.0f, cu24.GetPos().z), 4, false);
	//
	UpdateSpeedUpColors(dt);

	return UPDATE_CONTINUE;
}

// Generic functions
Cube ModuleSceneIntro::CreateCube(vec3 size, vec3 pos, Color color, float angle, vec3 u, float mass, bool draw, bool collider)
{
	Cube cu(size.x, size.y, size.z);
	cu.SetPos(pos.x, pos.y, pos.z);
	cu.color = color;

	if (angle != 0)
		cu.SetRotation(angle, vec3(u.x, u.y, u.z));

	if (collider)
		App->physics->AddBody(cu, mass);

	if (draw)
		cubes.add(cu);

	return cu;
}

Cylinder ModuleSceneIntro::CreateCylinder(float radius, float height, vec3 pos, Color color, bool flip, float angle, vec3 u, float mass, bool draw, bool collider)
{
	Cylinder cy(radius, height);
	cy.SetPos(pos.x, pos.y, pos.z);
	cy.color = color;

	if (flip)
		cy.SetRotation(90.0f, vec3(0.0f, 0.0f, 1.0f));

	if (angle != 0.0f)
		cy.SetRotation(angle, vec3(u.x, u.y, u.z));

	if (collider)
		App->physics->AddBody(cy, mass);

	if (draw)
		cylinders.add(cy);

	return cy;
}

Cone ModuleSceneIntro::CreateCone(float radius, float height, vec3 pos, Color color, bool flip, float angle, vec3 u, float mass, bool draw, bool collider)
{
	Cone co(radius, height);
	co.SetPos(pos.x, pos.y, pos.z);
	co.color = color;

	if (flip)
		co.SetRotation(90.0f, vec3(0.0f, 0.0f, 1.0f));

	if (angle != 0.0f)
		co.SetRotation(angle, vec3(u.x, u.y, u.z));

	if (collider)
		App->physics->AddBody(co, mass);

	if (draw)
		cones.add(co);

	return co;
}

// Specific functions for the game
void ModuleSceneIntro::CreateSpeedUp(vec3 size, vec3 pos, uint num, bool flip, float angle, vec3 u)
{
	Cube speed_a = CreateCube(size, pos, colors[index_d], 0.0f, vec3_zero, 0.0f, false, false);
	if (angle != 0)
		speed_a.SetRotation(angle, u);
	speed_a.Render();

	if (flip) {
		Cube speed_b = CreateCube(size, vec3(speed_a.GetPos().x, pos.y, speed_a.GetPos().z + speed_a.GetSize().z), colors[index_c], 0.0f, vec3(0.0f,0.0f,0.0f), 0.0f, false, false);
		if (angle != 0)
			speed_b.SetRotation(angle, u);
		speed_b.Render();

		Cube speed_c = CreateCube(size, vec3(speed_b.GetPos().x, pos.y, speed_b.GetPos().z + speed_b.GetSize().z), colors[index_b], 0.0f, vec3_zero, 0.0f, false, false);
		if (angle != 0)
			speed_c.SetRotation(angle, u);
		speed_c.Render();

		if (num == 4) {
			Cube speed_d = CreateCube(size, vec3(speed_c.GetPos().x, pos.y, speed_c.GetPos().z + speed_c.GetSize().z), colors[index_a], 0.0f, vec3_zero, 0.0f, false, false);
			if (angle != 0)
				speed_d.SetRotation(angle, u);
			speed_d.Render();
		}
	}
	else {
		Cube speed_b = CreateCube(size, vec3(speed_a.GetPos().x + speed_a.GetSize().x, pos.y, speed_a.GetPos().z), colors[index_c], 0.0f, vec3_zero, 0.0f, false, false);
		if (angle != 0)
			speed_b.SetRotation(angle, u);
		speed_b.Render();

		Cube speed_c = CreateCube(size, vec3(speed_b.GetPos().x + speed_b.GetSize().x, pos.y, speed_b.GetPos().z), colors[index_b], 0.0f, vec3_zero, 0.0f, false, false);
		if (angle != 0)
			speed_c.SetRotation(angle, u);
		speed_c.Render();

		if (num == 4) {
			Cube speed_d = CreateCube(size, vec3(speed_c.GetPos().x + speed_c.GetSize().x, pos.y, speed_c.GetPos().z), colors[index_a], 0.0f, vec3_zero, 0.0f, false, false);
			if (angle != 0)
				speed_d.SetRotation(angle, u);
			speed_d.Render();
		}
	}
}

void ModuleSceneIntro::UpdateSpeedUpColors(float dt)
{
	speed_time_count += 10.0f * dt;

	if (speed_time_count >= 0.0f && !a_updated) {
		UpdateSpeedUpIndex(index_a);
		a_updated = true;
	}
	else if (speed_time_count >= 0.25f && !b_updated) {
		UpdateSpeedUpIndex(index_b);
		b_updated = true;
	}
	else if (speed_time_count >= 0.5f && !c_updated) {
		UpdateSpeedUpIndex(index_c);
		c_updated = true;
	}
	else if (speed_time_count >= 0.75f && !d_updated) {
		UpdateSpeedUpIndex(index_d);
		d_updated = true;
	}

	if (speed_time_count >= 1.0f) {
		speed_time_count = 0.0f;

		a_updated = false;
		b_updated = false;
		c_updated = false;
		d_updated = false;
	}
}

void ModuleSceneIntro::UpdateSpeedUpIndex(uint &index)
{
	if (index == 4)
		index = 0;
	else
		++index;
}

void ModuleSceneIntro::CreateFinishLine(vec3 size, vec3 pos)
{
	Color c, c2;

	for (int i = 0; i < 3; ++i) {

		if (i % 2 != 0) {
			c = Black;
			c2 = White;
		}
		else {
			c = White;
			c2 = Black;
		}

		for (int j = 0; j < 6; ++j) {
			CreateCube(size, vec3(pos.x + (j * 2 + 0) * size.x, pos.y, pos.z + i * size.z), c, 0.0f, vec3(0.0f,0.0f,0.0f), 0.0f, true, false);
			CreateCube(size, vec3(pos.x + (j * 2 + 1) * size.x, pos.y, pos.z + i * size.z), c2, 0.0f, vec3_zero, 0.0f, true, false);
		}
	}
}

void ModuleSceneIntro::CreateLimits(Primitive* p, bool pos[4], bool inv_col[4], bool inv_pos[4], vec4 lim)
{
	vec3 position = p->GetPos();
	vec3 size = vec3_zero;

	if (p->GetType() == Primitive_Cube) {
		Cube* cu = (Cube*)p;
		size = cu->GetSize();
	}
	else if (p->GetType() == Primitive_Cylinder) {
		Cylinder* cy = (Cylinder*)p;
		float diameter = 2.0f * cy->radius;
		size = vec3(diameter, cy->height, diameter);
	}

	float cu_base = 2.0f;
	float cu_height = 2.0f;
	float x = 0.0f, z = 0.0f;

	// Cube color code
	Color c, c2, color;

	// Cube direction of creation
	float increment_x = 0.0f, increment_z = 0.0f;
	int dir = 0;

	for (uint i = 0; i < 4; ++i) {

		if (pos[i]) {

			// Update cube color code
			if (inv_col[i]) {
				c = Cyan;
				c2 = Orange;
			}
			else {
				c = Orange;
				c2 = Cyan;
			}

			// Update cube direction of creation
			if (inv_pos[i]) {
				increment_x = position.x + size.x / 2.0f - cu_base / 2.0f;
				increment_z = position.z + size.z / 2.0f - cu_base / 2.0f;
				dir = -1;
			}
			else {
				increment_x = position.x - size.x / 2.0f + cu_base / 2.0f;
				increment_z = position.z - size.z / 2.0f + cu_base / 2.0f;
				dir = 1;
			}

			// Calculate cubes per row
			uint max_cubes = 0;

			switch (i) {
			case 0:
				max_cubes = (size.x - lim.x) / cu_base;
				z = position.z + size.z / 2.0f + cu_base / 2.0f;
				break;
			case 1:
				max_cubes = (size.x - lim.y) / cu_base;
				z = position.z - size.z / 2.0f - cu_base / 2.0f;
				break;
			case 2:
				max_cubes = (size.z - lim.z) / cu_base;
				x = position.x + size.x / 2.0f + cu_base / 2.0f;
				break;
			case 3:
				max_cubes = (size.z - lim.w) / cu_base;
				x = position.x - size.x / 2.0f - cu_base / 2.0f;
				break;
			}

			// Create cubes
			for (uint j = 0; j < max_cubes; ++j) {

				if (j % 2 != 0)
					color = c2;
				else
					color = c;

				if (i == 0 || i == 1)
					x = increment_x;
				else if (i == 2 || i == 3)
					z = increment_z;

				CreateCube(vec3(cu_base, cu_height, cu_base), vec3(x, (position.y - size.y / 2.0f) + cu_height / 2.0f, z), color);

				increment_z += dir * cu_base;
				increment_x += dir * cu_base;
			}
		}
	}
}

void ModuleSceneIntro::OnCollision(PhysBody3D* body1, PhysBody3D* body2)
{
	if (body1 == sensor && body2 == (PhysBody3D*)App->player->vehicle) {
		App->player->speedupZ = true;
	}

	else if (body1 == sensor3 && body2 == (PhysBody3D*)App->player->vehicle) {
		App->player->speedupZ = true;
	}

	else if (body1 == sensor4 && body2 == (PhysBody3D*)App->player->vehicle) {
		App->player->speedupX = true;
	}

	else if (body1 == sensor5 && body2 == (PhysBody3D*)App->player->vehicle) {
		App->player->speedupZnegative = true;
	}

	else if (body1 == checkpoint1 && body2 == (PhysBody3D*)App->player->vehicle) {
		switch (checkpoints_index) {
		case 0:
			checkpoints_index = 1;
			break;
		case 5:
			checkpoints_index = 5;
			break;
		case 10:
			checkpoints_index = 11;
			break;
		}
	}

	else if (body1 == checkpoint2 && body2 == (PhysBody3D*)App->player->vehicle) {
		switch (checkpoints_index) {
		case 1:
			checkpoints_index = 2;
			break;
		case 6:
			checkpoints_index = 7;
			break;
		case 12:
			checkpoints_index = 12;
			break;
		}
	}

	else if (body1 == checkpoint3 && body2 == (PhysBody3D*)App->player->vehicle) {
		switch (checkpoints_index) {
		case 2:
			checkpoints_index = 3;
			break;
		case 7:
			checkpoints_index = 8;
			break;
		case 12:
			checkpoints_index = 13;
			break;
		}
	}

	else if (body1 == checkpoint4 && body2 == (PhysBody3D*)App->player->vehicle) {
		switch (checkpoints_index) {
		case 3:
			checkpoints_index = 4;
			break;
		case 8:
			checkpoints_index = 9;
			break;
		case 13:
			checkpoints_index = 14;
			break;
		}
	}

	else if (body1 == checkpoint5 && body2 == (PhysBody3D*)App->player->vehicle) {
		switch (checkpoints_index) {
		case 4:
			checkpoints_index = 5;
			break;
		case 9:
			checkpoints_index = 10;
			break;
		case 14:
			checkpoints_index = 15;
			break;
		}
	}

	else if (body1 == loopCompleted && body2 == (PhysBody3D*)App->player->vehicle) {
		switch (checkpoints_index) {
		case 5:
			loopsCount = 1;
			break;
		case 10:
			loopsCount = 2;
			break;
		case 15:
			loopsCount = 3;
			if (endTime = false)
				winCondition = 1;
			break;
		}
	}
}