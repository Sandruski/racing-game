#include "Globals.h"
#include "Application.h"
#include "ModuleSceneIntro.h"
#include "Primitive.h"
#include "PhysBody3D.h"

#define RW  12.0f
#define RH  0.5f
#define RL  30.0f

ModuleSceneIntro::ModuleSceneIntro(Application* app, bool start_enabled) : Module(app, start_enabled)
{
	name.create("scene_intro");
}

bool ModuleSceneIntro::Init(pugi::xml_node& node) 
{
	bool ret = true;

	float trying = node.child("trying").attribute("value").as_float();

	return ret;
}

ModuleSceneIntro::~ModuleSceneIntro()
{}

// Load assets
bool ModuleSceneIntro::Start()
{
	LOG("Loading Intro assets");
	bool ret = true;

	App->camera->Move(vec3(1.0f, 1.0f, 0.0f));
	App->camera->LookAt(vec3(0, 0, 0));

	// Sensors
	s.size = vec3(4, 2, 8); // First sensor, dont touch
	s.SetPos(0, 1.5f, 3); // First sensor, dont touch

	h.size = vec3(10, 2, 8); // Third sensor, need to be fixed
	h.SetPos(60, 1.5f, 240); // Third sensor, need to be fixed

	t.size = vec3(14, 2, 7); // forth sensor, need to be fixed
	t.SetPos(125, -4, 369); // forth sensor, need to be fixed

	n.size = vec3(32, 2, 9); // fifth sensor, need to be fixed
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
	//_sensors

	// Road
	// Road parameters
	float road_width = RW;
	float road_height = RH; // y axis
	float road_length = RL;

	float road_short_length = 20.0f;

	vec3 orthonormal_x(1, 0, 0);
	vec3 orthonormal_y(0, 1, 0);
	vec3 orthonormal_z(0, 0, 1);
	float right_angle = 90.0f;

	Color soil = Beige;
	//_road_parameters

	// Road primitives
	/// Always use last primitive created to set the position of the next primitive
	/// Notice that the position of the primitives follows a pattern

	// First section
	Cube cu = CreateCube(road_width, road_height, road_length, 0.0f, 0.0f, 0.0f, soil);
	Cube cu2 = CreateCube(road_width, road_height, road_length, cu.GetPos().x, cu.GetPos().y, cu.GetPos().z + cu.GetSize().z / 2.0f + road_length / 2.0f, soil);
	Cylinder cy = CreateCylinder(road_width / 2.0f, road_height, cu2.GetPos().x, cu2.GetPos().y, cu2.GetPos().z + cu2.GetSize().z / 2.0f, right_angle, orthonormal_z, soil);
	Cube cu3 = CreateCube(road_length, road_height, road_width, cy.GetPos().x + road_length / 2.0f, cy.GetPos().y, cy.GetPos().z, soil);
	Cylinder cy2 = CreateCylinder(road_width / 2.0f, road_height, cu3.GetPos().x + cu3.GetSize().x / 2.0f, cu3.GetPos().y, cu3.GetPos().z, right_angle, orthonormal_z, soil);
	Cube cu4 = CreateCube(road_width, road_height, road_short_length, cy2.GetPos().x, cy2.GetPos().y, cy2.GetPos().z + road_short_length / 2.0f, soil);
	Cylinder cy3 = CreateCylinder(road_width / 2.0f, road_height, cu4.GetPos().x, cu4.GetPos().y, cu4.GetPos().z + cu4.GetSize().z / 2.0f, right_angle, orthonormal_z, soil);
	Cube cu5 = CreateCube(road_length, road_height, road_width, cy3.GetPos().x + road_length / 2.0f, cy3.GetPos().y, cy3.GetPos().z, soil);
	Cylinder cy4 = CreateCylinder(road_width / 2.0f, road_height, cu5.GetPos().x + cu5.GetSize().x / 2.0f, cu5.GetPos().y, cu5.GetPos().z, right_angle, orthonormal_z, soil);
	cu6 = CreateCube(road_width, road_height, 6 * road_length, cy4.GetPos().x, cy4.GetPos().y, cy4.GetPos().z + (6 * road_length) / 2.0f, soil);
	cu7 = CreateCube(road_width, road_height, road_length / 2.0f, cu6.GetPos().x, cu6.GetPos().y, cu6.GetPos().z + cu6.GetSize().z / 2.0f, soil, true, true, 0.0f, -20, orthonormal_x);

	// Ground
	float ground_width = 150.0f;
	float ground_height = 15.0f; // y axis
	float ground_length = 100.0f;

	Cube ground = CreateCube(ground_width, ground_height, ground_length, cy4.GetPos().x - ground_width / 3.0f, cy4.GetPos().y - ground_height / 2.0f, cy4.GetPos().z + cy4.radius - ground_length / 2.0f, Pink);
	Cube water = CreateCube(2.0f * ground_width, ground_height, 3.0f * ground_length + 15.0f, ground.GetPos().x, ground.GetPos().y - ground_height, ground.GetPos().z + ground_length, Cyan);
	ground1 = CreateCube(2.0f * ground_width, ground_height / 2.0f, 2.0f * ground_length, water.GetPos().x + water.GetSize().x / 2.0f, ground.GetPos().y - ground.GetSize().y / 2.0f, water.GetPos().z + water.GetSize().z / 2.0f + (2.0f * ground_length) / 2.0f, Pink);
	//_ground
	//_first_section

	// Second section
	Cube cu8 = CreateCube(3.0f * road_width, road_height, 3.0f * road_length, cu7.GetPos().x, ground1.GetPos().y + ground1.GetSize().y / 2.0f + road_height / 2.0f, ground1.GetPos().z - ground1.GetSize().z / 2.0f + (3.0f * road_length) / 2.0f, soil);
	Cylinder cy5 = CreateCylinder((3.0f * road_width) / 2.0f, road_height, cu8.GetPos().x, cu8.GetPos().y, cu8.GetPos().z + cu8.GetSize().z / 2.0f, right_angle, orthonormal_z, soil);
	cu9 = CreateCube(4.0f * road_length, road_height, 3.0f * road_width, cy5.GetPos().x + (4.0f * road_length) / 2.0f, cy5.GetPos().y, cy5.GetPos().z, soil);
	Cylinder cy6 = CreateCylinder((3.0f * road_width) / 2.0f, road_height, cu9.GetPos().x + cu9.GetPos().x / 2.0f, cu9.GetPos().y, cu9.GetPos().z, right_angle, orthonormal_z, soil);
	Cube cu10 = CreateCube(4.0f * road_length, road_height, 3.0f * road_width, cy6.GetPos().x, cy6.GetPos().y, cy6.GetPos().z, soil, true, true, 0.0f, 20, orthonormal_z);
	Cube cu11 = CreateCube(2.0f * road_length, road_height, 3.0f * road_width, cu10.GetPos().x + cu10.GetSize().x - (2.0f * road_length) / 2.0f - 4.0f, cu10.GetPos().y + cu10.GetSize().z * tan(DEGTORAD * 30) - 0.4f, cu10.GetPos().z, soil);
	Cylinder cy7 = CreateCylinder(road_width / 2.0f, 50.0f * road_height, cu11.GetPos().x + cu11.GetSize().x / 3.0f - (road_width / 2.0f), cu11.GetPos().y + cu11.GetSize().y / 2.0f - (50.0f * road_height) / 2.0f, cu11.GetPos().z, right_angle, orthonormal_z, soil);
	cu12 = CreateCube(3.0f * road_width, road_height, 4.0f * road_length, cu11.GetPos().x + cu11.GetSize().x / 2.0f - (3.0f * road_width) / 2.0f, cy6.GetPos().y, cu11.GetPos().z - (4.0f * road_length) / 2.0f - cu11.GetSize().z / 2.0f + 4.0f, soil, true, true, 0.0f, -20, orthonormal_x);
	Cube cu13 = CreateCube(3.0f * road_width, road_height, 4.0f * road_length, cu12.GetPos().x, cu12.GetPos().y - cu11.GetSize().z * tan(DEGTORAD * 30) + 0.4f, cu12.GetPos().z - cu12.GetSize().z / 2.0f + 4.0f, soil, true, true, 0.0f, -20, orthonormal_x);
	Cube cu14 = CreateCube(3.0f * road_width, road_height, 2.0f * road_length, cu13.GetPos().x, cu13.GetPos().y, cu13.GetPos().z - cu13.GetSize().z / 2.0f + (2.0f * road_length) / 2.0f, soil);
	Cube cu15 = CreateCube(3.0f * road_width, road_height, 2.0f * road_length, cu14.GetPos().x, cu14.GetPos().y, cu14.GetPos().z - 1.2f * cu14.GetSize().z, soil);
	Cube cu16 = CreateCube(3.0f * road_width, road_height, 2.0f * road_length, cu15.GetPos().x, cu15.GetPos().y, cu15.GetPos().z - 1.2f * cu15.GetSize().z, soil);

	// Castle
	Cube castle = CreateCube(90.0f, 90.0f, 130.0f, water.GetPos().x + water.GetSize().x / 2.0f + 90.0f / 3.0f, water.GetPos().y + water.GetSize().y, water.GetPos().z, White);
	Cube castle1 = CreateCube(30.0f, 80.0f, 30.0f, castle.GetPos().x, castle.GetPos().y + castle.GetSize().y / 2.0f, castle.GetPos().z, White);
	Cube castle2 = CreateCube(20.0f, 50.0f, 20.0f, castle1.GetPos().x, castle1.GetPos().y + castle1.GetSize().y / 2.0f, castle1.GetPos().z, White);

	Cube tower = CreateCube(15.0f, 40.0f, 15.0f, castle.GetPos().x + castle.GetSize().x / 2.0f, castle.GetPos().y + castle.GetSize().y / 2.0f - 40.0f / 5.0f, castle.GetPos().z + castle.GetSize().z / 2.0f, White);
	Cube flag = CreateCube(1.0f, 15.0f, 1.0f, tower.GetPos().x, tower.GetPos().y + tower.GetSize().y / 2.0f, tower.GetPos().z, White);
	Cube tower2 = CreateCube(15.0f, 40.0f, 15.0f, castle.GetPos().x - castle.GetSize().x / 2.0f, castle.GetPos().y + castle.GetSize().y / 2.0f - 40.0f / 5.0f, castle.GetPos().z - castle.GetSize().z / 2.0f, White);
	Cube flag2 = CreateCube(1.0f, 15.0f, 1.0f, tower2.GetPos().x, tower2.GetPos().y + tower2.GetSize().y / 2.0f, tower2.GetPos().z, White);
	Cube tower3 = CreateCube(15.0f, 40.0f, 15.0f, castle.GetPos().x - castle.GetSize().x / 2.0f, castle.GetPos().y + castle.GetSize().y / 2.0f - 40.0f / 5.0f, castle.GetPos().z + castle.GetSize().z / 2.0f, White);
	Cube flag3 = CreateCube(1.0f, 15.0f, 1.0f, tower3.GetPos().x, tower3.GetPos().y + tower3.GetSize().y / 2.0f, tower3.GetPos().z, White);
	Cube tower4 = CreateCube(15.0f, 40.0f, 15.0f, castle.GetPos().x + castle.GetSize().x / 2.0f, castle.GetPos().y + castle.GetSize().y / 2.0f - 40.0f / 5.0f, castle.GetPos().z - castle.GetSize().z / 2.0f, White);
	Cube flag4 = CreateCube(1.0f, 15.0f, 1.0f, tower4.GetPos().x, tower4.GetPos().y + tower4.GetSize().y / 2.0f, tower4.GetPos().z, White);
	//_castle

	// Ground
	Cube ground2 = CreateCube(ground_width, ground_height / 2.0f, ground_length - 20.0f, ground1.GetPos().x, ground1.GetPos().y, ground1.GetPos().z - ground1.GetSize().z / 2.0f, Pink, true, true, 0.0f, 60, orthonormal_y);
	Cube ground3 = CreateCube(castle.GetSize().x, 3.0f * ground_height, ground_length, castle.GetPos().x, ground1.GetPos().y + ground1.GetSize().y / 2.0f - (3.0f * ground_height) / 2.0f, ground1.GetPos().z - ground1.GetSize().z / 2.0f - ground_length / 2.0f, Pink);
	Cube ground4 = CreateCube(castle.GetSize().x, ground_height, ground_length, cu14.GetPos().x - cu14.GetSize().x / 2.0f + castle.GetSize().x / 2.0f, cu14.GetPos().y - cu14.GetSize().y / 2.0f - ground_height / 2.0f, ground3.GetPos().z + ground3.GetSize().z / 2.0f - ground_length / 2.0f, Pink);
	Cube ground5 = CreateCube(castle.GetSize().x, ground_height, cu15.GetSize().z, cu15.GetPos().x - cu15.GetSize().x / 2.0f + castle.GetSize().x / 2.0f, cu15.GetPos().y - cu15.GetSize().y / 2.0f - ground_height / 2.0f, cu15.GetPos().z, Pink);
	Cube ground6 = CreateCube(3.0f * castle.GetSize().x, 2.0f * ground_height, 5.0f * cu16.GetSize().z, cu16.GetPos().x - cu16.GetSize().x / 2.0f + (3.0f * castle.GetSize().x) / 2.0f, cu16.GetPos().y - cu16.GetSize().y / 2.0f - (2.0f * ground_height) / 2.0f, cu16.GetPos().z + cu16.GetSize().z / 2.0f - (5.0f * cu16.GetSize().z) / 2.0f, Pink);
	Cube ground7 = CreateCube(ground_width, 2.0f * ground_height, ground1.GetSize().z, ground4.GetPos().x + ground4.GetSize().x / 2.0f - ground_width / 2.0f, ground1.GetPos().y - ground1.GetSize().y / 2.0f - (2.0f * ground_height) / 2.0f, ground4.GetPos().z + ground4.GetSize().z / 2.0f + ground1.GetSize().z / 2.0f, Pink);
	Cube ground8 = CreateCube(ground_width / 5.0f, 4.0f * ground_height, ground1.GetSize().z, water.GetPos().x + water.GetSize().x / 2.0f + (ground_width / 5.0f) / 2.0f, water.GetPos().y + water.GetSize().y / 2.0f - (4.0f * ground_height) / 2.0f, castle.GetPos().z - castle.GetSize().z / 2.0f - ground1.GetSize().z / 2.0f, Pink);
	Cube ground9 = CreateCube(2.0f * ground_width, 4.0f * ground_height, ground1.GetSize().z / 4.0f, ground8.GetPos().x - ground8.GetSize().x / 2.0f - (2.0f * ground_width) / 2.0f, ground8.GetPos().y, ground8.GetPos().z - ground8.GetSize().z / 2.0f + (ground1.GetSize().z / 4.0f) / 2.0f, Pink);

	Cube water2 = CreateCube(2.0f * ground4.GetSize().x, ground4.GetSize().y, 4.0f * ground4.GetSize().z, ground4.GetPos().x + ground4.GetSize().x / 2.0f - (2.0f * ground4.GetSize().x) / 2.0f, ground4.GetPos().y - ground4.GetSize().y, ground4.GetPos().z + ground4.GetSize().z / 2.0f - (4.0f * ground4.GetSize().z) / 2.0f, Cyan);
	Cube water3 = CreateCube(3.0f * ground4.GetSize().x + 50.0f, 2.0f * ground4.GetSize().y, 2.0f * ground4.GetSize().z, water.GetPos().x - water.GetSize().x / 2.0f + (3.0f * ground4.GetSize().x + 50.0f) / 2.0f, water.GetPos().y - water.GetSize().y / 2.0f - (2.0f * ground4.GetSize().y) / 2.0f, water2.GetPos().z - water2.GetSize().z / 2.0f + (2.0f * ground4.GetSize().z) / 2.0f, Cyan);
	Cube water4 = CreateCube(8.0f * ground4.GetSize().x, 4.0f * ground4.GetSize().y, ground4.GetSize().z, water.GetPos().x - water.GetSize().x / 2.0f + (8.0f * ground4.GetSize().x) / 2.0f, water2.GetPos().y - water2.GetSize().y / 2.0f - (4.0f * ground4.GetSize().y) / 2.0f, water2.GetPos().z - water2.GetSize().z / 2.0f - ground4.GetSize().z / 2.0f, Cyan);
	Cube water5 = CreateCube(ground6.GetSize().x, water4.GetSize().y, ground6.GetSize().z, water4.GetPos().x + water4.GetSize().x / 2 - ground6.GetSize().x / 2, water4.GetPos().y - water4.GetSize().y / 2, ground6.GetPos().z, Cyan);
	//_ground
	//_second_section

	// Third section
	Cylinder cy8 = CreateCylinder((3.0f * road_width) / 2.0f, road_height, cu16.GetPos().x, cu16.GetPos().y, cu16.GetPos().z - cu16.GetSize().z / 2.0f, right_angle, orthonormal_z, soil);
	Cube cu17 = CreateCube(2.0f * road_length, road_height, 3.0f * road_width, cy8.GetPos().x + (2.0f * road_length) / 2.0f, cy8.GetPos().y, cy8.GetPos().z, soil);
	Cylinder cy9 = CreateCylinder((3.0f * road_width) / 2.0f, road_height, cu17.GetPos().x + cu17.GetSize().x / 2.0f, cu17.GetPos().y, cu17.GetPos().z, right_angle, orthonormal_z, soil);
	Cube cu18 = CreateCube(3.0f * road_width, road_height, 6.0f * road_length, cy9.GetPos().x, cy9.GetPos().y, cy9.GetPos().z - (6.0f * road_length) / 2.0f, soil);
	Cylinder cy10 = CreateCylinder((3.0f * road_width) / 2.0f, road_height, cu18.GetPos().x, cu18.GetPos().y, cu18.GetPos().z - cu18.GetSize().z / 2.0f, right_angle, orthonormal_z, soil);
	Cube cu19 = CreateCube(4.0f * road_length, road_height, 3.0f * road_width, cy10.GetPos().x + (4.0f * road_length) / 2.0f, cy10.GetPos().y, cy10.GetPos().z, soil);
	Cylinder cy11 = CreateCylinder((3.0f * road_width) / 2.0f, road_height, cu19.GetPos().x + cu19.GetSize().x / 2.0f, cu19.GetPos().y, cu19.GetPos().z, right_angle, orthonormal_z, soil);
	Cube cu20 = CreateCube(3.0f * road_width, road_height, 3.0f * road_length, cy11.GetPos().x, cy11.GetPos().y, cy11.GetPos().z + (3.0f * road_length) / 2.0f, soil);
	Cylinder cy12 = CreateCylinder((3.0f * road_width) / 2.0f, road_height, cu20.GetPos().x, cu20.GetPos().y, cu20.GetPos().z + cu20.GetSize().z / 2.0f, right_angle, orthonormal_z, soil);
	Cube cu21 = CreateCube(road_length, road_height, 3.0f * road_width, cy12.GetPos().x - road_length / 2.0f, cy12.GetPos().y, cy12.GetPos().z, soil);
	Cube cu22 = CreateCube(4.0f * road_length + road_length / 2.0f + road_width / 2.0f, road_height, 3.0f * road_width, cu21.GetPos().x - cu21.GetSize().x / 2.0f, cu21.GetPos().y, cu21.GetPos().z, soil, true, true, 0.0f, -20, orthonormal_z);

	// Tunnel
	float tunnel_width = road_width / 2.0f;
	float tunnel_height = 45.0f;
	float tunnel_length = cu18.GetSize().z - 2.0f * cy9.radius;

	Cube tunnel_wall = CreateCube(tunnel_width, tunnel_height, tunnel_length, cu17.GetPos().x + (2.0f * road_length) / 2.0f - cy9.radius - tunnel_width / 2.0f, cu17.GetPos().y, cy9.GetPos().z - cy9.radius - tunnel_length / 2.0f, soil);
	Cube tunnel_wall2 = CreateCube(tunnel_width, tunnel_height, tunnel_length, cu17.GetPos().x + (2.0f * road_length) / 2.0f + cy9.radius + tunnel_width / 2.0f, cu17.GetPos().y, cy9.GetPos().z - cy9.radius - tunnel_length / 2.0f, soil);
	Cube tunnel_wall3 = CreateCube((tunnel_wall2.GetPos().x + tunnel_wall2.GetSize().x / 2.0f) - (tunnel_wall.GetPos().x - tunnel_wall.GetSize().x / 2.0f), tunnel_width / 4.0f, tunnel_length, tunnel_wall.GetPos().x + tunnel_wall.GetSize().x / 2.0f + cu18.GetSize().x / 2.0f, tunnel_wall.GetPos().y + tunnel_wall.GetSize().y / 2.0f + (tunnel_width / 4.0f) / 2.0f, tunnel_wall.GetPos().z, soil);
	//_tunnel
	//_third_section

	// Fourth section
	Cube cu23 = CreateCube(2.0f * road_length, road_height, road_width / 2.0f + 1.0f, tunnel_wall3.GetPos().x - tunnel_wall3.GetSize().x / 2.0f - (2.0f * road_length) / 2.0f, tunnel_wall3.GetPos().y + tunnel_wall3.GetSize().y / 2.0f - road_height / 2.0f, tunnel_wall3.GetPos().z + road_width / 2.0f + 0.5f, soil);
	Cube cu24 = CreateCube(2.0f * road_length, road_height, road_width / 2.0f + 1.0f, tunnel_wall3.GetPos().x - tunnel_wall3.GetSize().x / 2.0f - (2.0f * road_length) / 2.0f, tunnel_wall3.GetPos().y + tunnel_wall3.GetSize().y / 2.0f - road_height / 2.0f, tunnel_wall3.GetPos().z - road_width / 2.0f - 0.5f, soil);	
	Cube cu25 = CreateCube(road_length, road_height, cu23.GetPos().z - (cu24.GetPos().z - cu24.GetSize().z), (cu24.GetPos().x - cu24.GetSize().x / 2.0f) - road_length / 2.0f, tunnel_wall3.GetPos().y + tunnel_wall3.GetSize().y / 2.0f - road_height / 2.0f, tunnel_wall3.GetPos().z, soil);
	Cube cu26 = CreateCube((cu25.GetPos().x - cu25.GetSize().x / 2.0f) - (ground8.GetPos().x + ground8.GetSize().x / 2.0f) + 2.0f, road_height, cu25.GetSize().z, cu25.GetPos().x - cu25.GetSize().x / 2.0f - ((cu25.GetPos().x - cu25.GetSize().x / 2.0f) - (ground8.GetPos().x + ground8.GetSize().x / 2.0f)) / 2.0f, cu25.GetPos().y - ((cu25.GetPos().x - cu25.GetSize().x / 2.0f) - (ground8.GetPos().x + ground8.GetSize().x / 2.0f)) / 2.0f * tan(DEGTORAD * 12), cu25.GetPos().z, soil, true, true, 0.0f, 12, orthonormal_z);
	Cube cu27 = CreateCube(road_length, road_height, cu26.GetSize().z, ground8.GetPos().x, ground8.GetPos().y + ground8.GetSize().y / 2.0f + road_height / 2.0f, cu26.GetPos().z, soil);
	Cylinder cy13 = CreateCylinder(road_width, 2.0f * road_height, cu27.GetPos().x - cu27.GetSize().x, cu27.GetPos().y - 10.0f * cu27.GetSize().y, cu27.GetPos().z, right_angle, orthonormal_z, soil);
	Cylinder cy14 = CreateCylinder(road_width / 2.0f, 25.0f * road_height, cy13.GetPos().x, cy13.GetPos().y + cy13.height / 2.0f - (25.0f * road_height) / 2.0f, cy13.GetPos().z, right_angle, orthonormal_z, soil);
	Cylinder cy15 = CreateCylinder(cy13.radius, cy13.height, cy13.GetPos().x - cu27.GetSize().x, cy13.GetPos().y, cy13.GetPos().z, right_angle, orthonormal_z, soil);
	Cylinder cy16 = CreateCylinder(road_width / 2.0f, 25.0f * road_height, cy15.GetPos().x, cy15.GetPos().y + cy15.height / 2.0f - (25.0f * road_height) / 2.0f, cy15.GetPos().z, right_angle, orthonormal_z, soil);
	Cube cu28 = CreateCube(road_length, road_height, road_width, cy16.GetPos().x - cu27.GetSize().x, cy15.GetPos().y, cy16.GetPos().z, soil);
	Cylinder cy18 = CreateCylinder(road_width / 2.0f, road_height, cu28.GetPos().x - cu28.GetSize().x / 2.0f, cu28.GetPos().y, cu28.GetPos().z, right_angle, orthonormal_z, soil);
	Cylinder cy19 = CreateCylinder(road_width / 3.0f, 25.0f * road_height, cy18.GetPos().x, cy18.GetPos().y + cy18.height / 2.0f - (25.0f * road_height) / 2.0f, cy18.GetPos().z, right_angle, orthonormal_z, soil);
	Cube cu29 = CreateCube(road_width, road_height, road_length, cy18.GetPos().x, cy18.GetPos().y, cy18.GetPos().z - road_length / 2.0f, soil);
	Cylinder cy20 = CreateCylinder(road_width / 2.0f, road_height, cu29.GetPos().x, cu29.GetPos().y, cu29.GetPos().z - cu29.GetSize().z / 2.0f, right_angle, orthonormal_z, soil);
	Cylinder cy21 = CreateCylinder(road_width / 3.0f, 25.0f * road_height, cy20.GetPos().x, cy20.GetPos().y + cy20.height / 2.0f - (25.0f * road_height) / 2.0f, cy20.GetPos().z, right_angle, orthonormal_z, soil);
	Cube cu30 = CreateCube((cu29.GetPos().x - cu29.GetSize().x) - (cu.GetPos().x - cu.GetSize().x), road_height, road_width, cy20.GetPos().x - ((cu29.GetPos().x - cu29.GetSize().x) - (cu.GetPos().x - cu.GetSize().x)) / 2.0f, cy20.GetPos().y, cy20.GetPos().z, soil);
	Cylinder cy22 = CreateCylinder(road_width / 2.0f, road_height, cu30.GetPos().x - cu30.GetSize().x / 2.0f, cu30.GetPos().y, cu30.GetPos().z, right_angle, orthonormal_z, soil);
	Cylinder cy23 = CreateCylinder(road_width / 3.0f, 25.0f * road_height, cy22.GetPos().x, cy22.GetPos().y + cy22.height / 2.0f - (25.0f * road_height) / 2.0f, cy22.GetPos().z, right_angle, orthonormal_z, soil);
	Cube cu31 = CreateCube(road_width, road_height, cy22.radius, cy22.GetPos().x, cy22.GetPos().y, cy22.GetPos().z + cy22.radius / 2.0f, soil);

	float ramp_precision_y = 0.6f;
	float ramp_precision_z = 2.1f;

	Cube cu32 = CreateCube(road_width, road_height, road_length + 2.0f * road_width + 0.5f, cu31.GetPos().x, cu31.GetPos().y - ((road_length + 2.0f * road_width + 0.5f) / 2.0f) * tan(DEGTORAD * -21) - ramp_precision_y, cu31.GetPos().z + cu31.GetSize().z / 2.0f + (road_length + 2.0f * road_width + 0.5f) / 2.0f - ramp_precision_z, soil, true, true, 0.0f, -21, orthonormal_x);
	Cube cu33 = CreateCube(road_width, road_height, road_length, cu.GetPos().x, cu.GetPos().y, ground.GetPos().z - ground.GetSize().z / 2.0f + road_length / 2.0f, soil);
	//_fourth_section

	// Hinges (rotating elements)
	float cylinder_rotating_width = road_width / 2.0f;
	float cylinder_real_width = road_width / 8.0f;
	float cylinder_height = 50.0f * road_height; // y axis

	float cube_width = road_width / 2.0f;
	float cube_height = 2.0f * road_height;
	float cube_length = road_length + road_width;

	Color rotating_stuff = Orange;

	rotating_cu = CreateCube(cube_width, cube_height, cube_length, cu6.GetPos().x + 20.0f, cu6.GetPos().y + 2.0f * cube_height, cu6.GetPos().z - cube_length / 2.0f, rotating_stuff, false, false);
	bodyA = App->physics->AddBody(rotating_cu, 10000.0f);
	Cylinder rotating_cy = CreateCylinder(cylinder_rotating_width, cylinder_height, rotating_cu.GetPos().x, rotating_cu.GetPos().y - cylinder_height / 2.0f, rotating_cu.GetPos().z, right_angle, orthonormal_z, rotating_stuff, false, false);
	PhysBody3D* bodyB = App->physics->AddBody(rotating_cy, 10000.0f);
	Cylinder real_cy = CreateCylinder(cylinder_real_width, cylinder_height, rotating_cu.GetPos().x, rotating_cu.GetPos().y - cylinder_height / 2.0f, rotating_cu.GetPos().z, right_angle, orthonormal_z, rotating_stuff);

	rotating_cu2 = CreateCube(cube_width, cube_height, cube_length, cu6.GetPos().x - 20.0f, cu6.GetPos().y + 2.0f * cube_height, cu6.GetPos().z + cube_length / 2.0f, rotating_stuff, false, false);
	bodyA2 = App->physics->AddBody(rotating_cu2, 10000.0f);
	Cylinder rotating_cy2 = CreateCylinder(cylinder_rotating_width, cylinder_height, rotating_cu2.GetPos().x, rotating_cu2.GetPos().y - cylinder_height / 2.0f, rotating_cu2.GetPos().z, right_angle, orthonormal_z, rotating_stuff, false, false);
	PhysBody3D* bodyB2 = App->physics->AddBody(rotating_cy2, 10000.0f);
	Cylinder real_cy2 = CreateCylinder(cylinder_real_width, cylinder_height, rotating_cu2.GetPos().x, rotating_cu2.GetPos().y - cylinder_height / 2.0f, rotating_cu2.GetPos().z, right_angle, orthonormal_z, rotating_stuff);

	bodyA->GetBody()->setLinearFactor(btVector3(0, 0, 0));
	bodyA2->GetBody()->setLinearFactor(btVector3(0, 0, 0));
	bodyB->GetBody()->setAngularFactor(btVector3(0, 0, 0));
	bodyB2->GetBody()->setAngularFactor(btVector3(0, 0, 0));

	App->physics->AddConstraintHinge(*bodyA, *bodyB, vec3(0, 0, 0), vec3(0, 0, 0), vec3(0, -1, 0), vec3(0, 0, 0), true, true);
	App->physics->AddConstraintHinge(*bodyA2, *bodyB2, vec3(0, 0, 0), vec3(0, 0, 0), vec3(0, 1, 0), vec3(0, 0, 0), true, true);

	// Finish line
	float line_width = 1.0f;
	float line_length = 1.0f;

	CreateFinishLine(line_width, road_height, line_length, line_width / 2.0f - cu.GetSize().x / 2.0f, 0.1f, 0.0f);
	//_finish_line

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

	Cube aux_cu = CreateCube(cu12.GetSize().x, cu12.GetSize().y, cu12.GetPos().z - (ground1.GetPos().z - ground1.GetSize().z / 2.0f), cu12.GetPos().x, cu12.GetPos().y, cu12.GetPos().z - (cu12.GetPos().z - (ground1.GetPos().z - ground1.GetSize().z / 2.0f)) / 2.0f, soil, false, false);
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

	Cube aux_cu2 = CreateCube(cu19.GetSize().x - cy10.radius - cy11.radius, cu19.GetSize().y, cu19.GetSize().z, cu19.GetPos().x, cu19.GetPos().y, cu19.GetPos().z, soil, false, false);
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

	Cube aux_cu3 = CreateCube(cu20.GetSize().x, cu20.GetSize().y, cu20.GetSize().z - cy11.radius - cy12.radius, cu20.GetPos().x, cu20.GetPos().y, cu20.GetPos().z, soil, false, false);
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

	Cube aux_cu4 = CreateCube(tunnel_wall3.GetSize().x, tunnel_wall3.GetSize().y, cu21.GetSize().z, tunnel_wall3.GetPos().x, tunnel_wall3.GetPos().y + tunnel_wall3.GetSize().y, tunnel_wall3.GetPos().z, soil, false, false);
	CreateLimits((Primitive*)&aux_cu4, pos, inv_col, inv_pos);

	Cube aux_cu5 = CreateCube(aux_cu4.GetSize().x, aux_cu4.GetSize().y, aux_cu4.GetSize().z, aux_cu4.GetPos().x + 2.0f, aux_cu4.GetPos().y, aux_cu4.GetPos().z, soil, false, false);
	pos[i] = false;
	pos[++i] = false;
	pos[++i] = false;
	pos[++i] = true;
	i = 0;
	inv_col[3] = true;
	CreateLimits((Primitive*)&aux_cu5, pos, inv_col, inv_pos, vec4(0.0f,0.0f,0.0f, tunnel_wall3.GetSize().x / 2.0f + cu23.GetSize().z / 2.0f));

	inv_pos[3] = true;
	CreateLimits((Primitive*)&aux_cu5, pos, inv_col, inv_pos, vec4(0.0f, 0.0f, 0.0f, tunnel_wall3.GetSize().x / 2.0f + cu23.GetSize().z / 2.0f));

	Cube aux_cu6 = CreateCube(aux_cu5.GetSize().x, aux_cu5.GetSize().y, (cu23.GetPos().z - cu23.GetSize().z) - cu24.GetPos().z, aux_cu5.GetPos().x, aux_cu5.GetPos().y, aux_cu5.GetPos().z, soil, false, false);
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

	//_road_primitives
	//_road

	return ret;
}

void ModuleSceneIntro::CreateLimits(Primitive* p, bool pos[4], bool inv_col[4], bool inv_pos[4], vec4 lim)
{
	vec3 position = p->GetPos();
	vec3 size = vec3(0.0f, 0.0f, 0.0f);

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
	Color c1, c2, color;

	// Cube direction of creation
	float increment_x = 0.0f, increment_z = 0.0f;
	int dir = 0;

	for (uint i = 0; i < 4; ++i) {

		if (pos[i]) {

			// Update cube color code
			if (inv_col[i]) {
				c1 = Cyan;
				c2 = Orange;
			}
			else {
				c1 = Orange;
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
					color = c1;

				if (i == 0 || i == 1)
					x = increment_x;
				else if (i == 2 || i == 3)
					z = increment_z;

				CreateCube(cu_base, cu_height, cu_base, x, (position.y - size.y / 2.0f) + cu_height / 2.0f, z, color);

				increment_z += dir * cu_base;
				increment_x += dir * cu_base;
			}
		}
	}
}

// Load assets
bool ModuleSceneIntro::CleanUp()
{
	LOG("Unloading Intro scene");

	return true;
}

// Update
update_status ModuleSceneIntro::Update(float dt)
{
	// Update sensors
	sensor->GetTransform(&s.transform);
	s.Render();

	sensor3->GetTransform(&h.transform);
	h.Render();

	sensor4->GetTransform(&t.transform);
	h.Render();

	sensor5->GetTransform(&n.transform);
	h.Render();

	// Debug cube
	Cube debug_cube = CreateCube(2.0f, 2.0f, 2.0f, 0.0f, 3.0f, 0.0f, White, false, false);
	debug_cube.axis = true;
	debug_cube.wire = true;

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

	float road_height = 0.5f; // y axis

	vec3 orthonormal_x(1, 0, 0);
	vec3 orthonormal_y(0, 1, 0);
	vec3 orthonormal_z(0, 0, 1);
	float right_angle = 90.0f;

	/// It is only needed to set the position and size of the first element of the speed-up
	CreateSpeedUp(speed_width, speed_height, speed_length, 0.0f, road_height / 2.0f, 0.0f);

	CreateSpeedUp(speed_length, speed_height, 2.0f * speed_width, cu7.GetPos().x - cu7.GetSize().x / 2.0f + speed_length / 2.0f, road_height / 2.0f, cu7.GetPos().z, 3, false, -20, orthonormal_x);
	CreateSpeedUp(speed_length, speed_height, 2.0f * speed_width, cu7.GetPos().x + speed_length / 2.0f, road_height / 2.0f, cu7.GetPos().z, 3, false, -20, orthonormal_x);

	CreateSpeedUp(2.0f * speed_length, speed_height, 2.0f * speed_width, cu9.GetPos().x, cu9.GetPos().y + cu9.GetSize().y / 2.0f, cu9.GetPos().z, 4, false);

	CreateSpeedUp(2.0f * speed_length, speed_height, 2.0f * speed_width, cu12.GetPos().x - cu12.GetSize().x / 2.0f + (2.0f * speed_length) / 2.0f, ground1.GetPos().y + ground1.GetSize().y / 2.0f, cu12.GetPos().z - 2.0f * speed_width, 3, false);
	CreateSpeedUp(2.0f * speed_length, speed_height, 2.0f * speed_width, cu12.GetPos().x - 2.0f * speed_length, ground1.GetPos().y + ground1.GetSize().y / 2.0f, cu12.GetPos().z - 2.0f * speed_width, 3, false);
	CreateSpeedUp(2.0f * speed_length, speed_height, 2.0f * speed_width, cu12.GetPos().x + 2.0f * 2.0f * speed_length, ground1.GetPos().y + ground1.GetSize().y / 2.0f, cu12.GetPos().z - 2.0f * speed_width, 3, false);

	UpdateSpeedUpColors(dt);

	return UPDATE_CONTINUE;
}

// Specific functions
void ModuleSceneIntro::CreateSpeedUp(float sizeX, float sizeY, float sizeZ, float posX, float posY, float posZ, uint num, bool add_z, float angle, vec3 u)
{
	Cube speed_a = CreateCube(sizeX, sizeY, sizeZ, posX, posY, posZ, colors[index_d], false, false);
	if (angle != 0)
		speed_a.SetRotation(angle, u);
	speed_a.Render();

	if (add_z) {
		Cube speed_b = CreateCube(sizeX, sizeY, sizeZ, speed_a.GetPos().x, posY, speed_a.GetPos().z + speed_a.GetSize().z, colors[index_c], false, false);
		if (angle != 0)
			speed_b.SetRotation(angle, u);
		speed_b.Render();

		Cube speed_c = CreateCube(sizeX, sizeY, sizeZ, speed_b.GetPos().x, posY, speed_b.GetPos().z + speed_b.GetSize().z, colors[index_b], false, false);
		if (angle != 0)
			speed_c.SetRotation(angle, u);
		speed_c.Render();

		if (num == 4) {
			Cube speed_d = CreateCube(sizeX, sizeY, sizeZ, speed_c.GetPos().x, posY, speed_c.GetPos().z + speed_c.GetSize().z, colors[index_a], false, false);
			if (angle != 0)
				speed_d.SetRotation(angle, u);
			speed_d.Render();
		}
	}
	else {
		Cube speed_b = CreateCube(sizeX, sizeY, sizeZ, speed_a.GetPos().x + speed_a.GetSize().x, posY, speed_a.GetPos().z, colors[index_c], false, false);
		if (angle != 0)
			speed_b.SetRotation(angle, u);
		speed_b.Render();

		Cube speed_c = CreateCube(sizeX, sizeY, sizeZ, speed_b.GetPos().x + speed_b.GetSize().x, posY, speed_b.GetPos().z, colors[index_b], false, false);
		if (angle != 0)
			speed_c.SetRotation(angle, u);
		speed_c.Render();

		if (num == 4) {
			Cube speed_d = CreateCube(sizeX, sizeY, sizeZ, speed_c.GetPos().x + speed_c.GetSize().x, posY, speed_c.GetPos().z, colors[index_a], false, false);
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

void ModuleSceneIntro::CreateFinishLine(float sizeX, float sizeY, float sizeZ, float posX, float posY, float posZ)
{
	Color c1 = White;
	Color c2 = Black;

	for (int i = 0; i < 3; ++i) {

		if (i % 2 != 0) {
			c1 = Black;
			c2 = White;
		}
		else {
			c1 = White;
			c2 = Black;
		}

		for (int j = 0; j < 6; ++j) {
			CreateCube(sizeX, sizeY, sizeZ, posX + (j * 2 + 0) * sizeX, posY, posZ + i * sizeZ, c1, true, false);
			CreateCube(sizeX, sizeY, sizeZ, posX + (j * 2 + 1) * sizeX, posY, posZ + i * sizeZ, c2, true, false);
		}
	}
}

// Generic functions
Cube ModuleSceneIntro::CreateCube(float sizeX, float sizeY, float sizeZ, float posX, float posY, float posZ, Color color, bool draw, bool collider, float mass, float angle, vec3 u)
{
	Cube cu(sizeX, sizeY, sizeZ);
	cu.SetPos(posX, posY, posZ);
	cu.color = color;

	if (angle != 0)
		cu.SetRotation(angle, vec3(u.x, u.y, u.z));

	if (collider)
		App->physics->AddBody(cu, mass);

	if (draw)
		cubes.add(cu);

	return cu;
}

Cylinder ModuleSceneIntro::CreateCylinder(float radius, float height, float posX, float posY, float posZ, float angle, vec3 u, Color color, bool draw, bool collider, float mass)
{
	Cylinder cy(radius, height);
	cy.SetPos(posX, posY, posZ);
	cy.SetRotation(angle, vec3(u.x, u.y, u.z));
	cy.color = color;

	if (collider)
		App->physics->AddBody(cy, mass);

	if (draw)
		cylinders.add(cy);

	return cy;
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


	else if (body1) {
		checkpoints_index = 1;
	}
}