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
	s.size = vec3(5, 3, 1);
	s.SetPos(0, 2.5f, 20);

	g.size = vec3(5, 3, 1);
	g.SetPos(10, 5.0f, 20);

	sensor = App->physics->AddBody(s, 0.0f);
	sensor->SetAsSensor(true);
	sensor->collision_listeners.add(this);

	sensor2 = App->physics->AddBody(g, 0.0f);
	sensor2->SetAsSensor(true);
	sensor2->collision_listeners.add(this);
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
	Cylinder cy6 = CreateCylinder((3.0f * road_width) / 2.0f, road_height, cu8.GetPos().x, cu8.GetPos().y, cu8.GetPos().z + cu8.GetSize().z / 2.0f, right_angle, orthonormal_z, soil);
	cu9 = CreateCube(4.0f * road_length, road_height, 3.0f * road_width, cy6.GetPos().x + (4.0f * road_length) / 2.0f, cy6.GetPos().y, cy6.GetPos().z, soil);
	Cylinder cy7 = CreateCylinder((3.0f * road_width) / 2.0f, road_height, cu9.GetPos().x + cu9.GetPos().x / 2.0f, cu9.GetPos().y, cu9.GetPos().z, right_angle, orthonormal_z, soil);
	Cube cu10 = CreateCube(4.0f * road_length, road_height, 3.0f * road_width, cy7.GetPos().x, cy7.GetPos().y, cy7.GetPos().z, soil, true, true, 0.0f, 20, orthonormal_z);
	Cube cu11 = CreateCube(2.0f * road_length, road_height, 3.0f * road_width, cu10.GetPos().x + cu10.GetSize().x - (2.0f * road_length) / 2.0f - 4.0f, cu10.GetPos().y + cu10.GetSize().z * tan(DEGTORAD * 30) - 0.4f, cu10.GetPos().z, soil);
	Cylinder cy8 = CreateCylinder(road_width / 2.0f, 50.0f * road_height, cu11.GetPos().x + cu11.GetSize().x / 3.0f - (road_width / 2.0f), cu11.GetPos().y + cu11.GetSize().y / 2.0f - (50.0f * road_height) / 2.0f, cu11.GetPos().z, right_angle, orthonormal_z, soil);
	cu12 = CreateCube(3.0f * road_width, road_height, 4.0f * road_length, cu11.GetPos().x + cu11.GetSize().x / 2.0f - (3.0f * road_width) / 2.0f, cy7.GetPos().y, cu11.GetPos().z - (4.0f * road_length) / 2.0f - cu11.GetSize().z / 2.0f + 4.0f, soil, true, true, 0.0f, -20, orthonormal_x);
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
	
	//Cube water5 = CreateCube(ground4.GetSize().z, 4.0f * ground4.GetSize().y, 8.0f * ground4.GetSize().x, water4.GetPos().x - water4.GetSize().x / 2.0f + ground4.GetSize().z / 2.0f, water2.GetPos().y + water2.GetSize().y / 2.0f + (4.0f * ground4.GetSize().y) / 2.0f, water3.GetPos().z, Cyan);
	
	//_ground
	//_second_section

	// Third section
	Cylinder cy9 = CreateCylinder((3.0f * road_width) / 2.0f, road_height, cu16.GetPos().x, cu16.GetPos().y, cu16.GetPos().z - cu16.GetSize().z / 2.0f, right_angle, orthonormal_z, soil);
	Cube cu17 = CreateCube(2.0f * road_length, road_height, 3.0f * road_width, cy9.GetPos().x + (2.0f * road_length) / 2.0f, cy9.GetPos().y, cy9.GetPos().z, soil);
	Cylinder cy10 = CreateCylinder((3.0f * road_width) / 2.0f, road_height, cu17.GetPos().x + cu17.GetSize().x / 2.0f, cu17.GetPos().y, cu17.GetPos().z, right_angle, orthonormal_z, soil);
	Cube cu18 = CreateCube(3.0f * road_width, road_height, 6.0f * road_length, cy10.GetPos().x, cy10.GetPos().y, cy10.GetPos().z - (6.0f * road_length) / 2.0f, soil);
	Cylinder cy11 = CreateCylinder((3.0f * road_width) / 2.0f, road_height, cu18.GetPos().x, cu18.GetPos().y, cu18.GetPos().z - cu18.GetSize().z / 2.0f, right_angle, orthonormal_z, soil);
	Cube cu19 = CreateCube(4.0f * road_length, road_height, 3.0f * road_width, cy11.GetPos().x + (4.0f * road_length) / 2.0f, cy11.GetPos().y, cy11.GetPos().z, soil);
	Cylinder cy12 = CreateCylinder((3.0f * road_width) / 2.0f, road_height, cu19.GetPos().x + cu19.GetSize().x / 2.0f, cu19.GetPos().y, cu19.GetPos().z, right_angle, orthonormal_z, soil);
	Cube cu20 = CreateCube(3.0f * road_width, road_height, 3.0f * road_length, cy12.GetPos().x, cy12.GetPos().y, cy12.GetPos().z + (3.0f * road_length) / 2.0f, soil);
	Cylinder cy13 = CreateCylinder((3.0f * road_width) / 2.0f, road_height, cu20.GetPos().x, cu20.GetPos().y, cu20.GetPos().z + cu20.GetSize().z / 2.0f, right_angle, orthonormal_z, soil);
	Cube cu21 = CreateCube(road_length, road_height, 3.0f * road_width, cy13.GetPos().x - road_length / 2.0f, cy13.GetPos().y, cy13.GetPos().z, soil);
	
	Cube cu22 = CreateCube(5.0f * road_length - 0.8f, road_height, 3.0f * road_width, cu21.GetPos().x - cu21.GetSize().x / 2.0f, cu21.GetPos().y, cu21.GetPos().z, soil, true, true, 0.0f, -20, orthonormal_z);

	// Tunnel
	float tunnel_width = 5.0f;
	float tunnel_height = 50.0f; // y axis
	float tunnel_length = cu18.GetSize().z - 2.0f * cy10.radius;

	Cube tunnel_wall = CreateCube(tunnel_width, tunnel_height, tunnel_length, cu17.GetPos().x + (2.0f * road_length) / 2.0f - cy10.radius, cu17.GetPos().y, cy10.GetPos().z - cy10.radius - tunnel_length / 2.0f, soil);
	Cube tunnel_wall2 = CreateCube(tunnel_width, tunnel_height, tunnel_length, cu17.GetPos().x + (2.0f * road_length) / 2.0f + cy10.radius, cu17.GetPos().y, cy10.GetPos().z - cy10.radius - tunnel_length / 2.0f, soil);
	Cube tunnel_wall3 = CreateCube(tunnel_width - 4.0f, tunnel_height - 2.0f * tunnel_width + 1.0f, tunnel_length, tunnel_wall2.GetPos().x - (tunnel_height - 2.0f * tunnel_width + 1.0f ) / 2.0f + tunnel_width / 2.0f, tunnel_wall2.GetPos().y + tunnel_wall2.GetSize().y / 2.0f, cy10.GetPos().z - cy10.radius - tunnel_length / 2.0f, soil, true, true, 0.0f, right_angle, orthonormal_z);
	//_tunnel
	//_third_section

	// Fourth section
	Cube cu23 = CreateCube(2.0f * road_length, road_height, road_width / 2.0f, tunnel_wall3.GetPos().x - tunnel_wall3.GetSize().y / 2.0f - (2.0f * road_length) / 2.0f, tunnel_wall3.GetPos().y + tunnel_wall3.GetSize().x / 2.0f - road_height / 2.0f, tunnel_wall3.GetPos().z - 2.0f + road_width / 2.0f, soil);
	Cube cu24 = CreateCube(2.0f * road_length, road_height, road_width / 2.0f, tunnel_wall3.GetPos().x - tunnel_wall3.GetSize().y / 2.0f - (2.0f * road_length) / 2.0f, tunnel_wall3.GetPos().y + tunnel_wall3.GetSize().x / 2.0f - road_height / 2.0f, tunnel_wall3.GetPos().z + 2.0f - road_width / 2.0f, soil);

	Cube cu25 = CreateCube(2.0f * road_length, road_height, road_width + 2.0f, cu24.GetPos().x - cu24.GetSize().x, tunnel_wall3.GetPos().y + tunnel_wall3.GetSize().x / 2.0f - road_height / 2.0f, tunnel_wall3.GetPos().z, soil);
	//_fourth_section

	// Hinges (rotating elements)
	float cylinder_rotating_width = road_width / 2.0f;
	float cylinder_real_width = road_width / 8.0f;
	float cylinder_height = 50.0f * road_height; // y axis

	float cube_width = road_width / 2.0f;
	float cube_height = 2.0f * road_height;
	float cube_length = road_length + 2.0f * road_width;

	Color rotating_stuff = Orange;

	rotating_cu = CreateCube(cube_width, cube_height, cube_length, cu6.GetPos().x + 20.0f, cu6.GetPos().y + 2.0f * cube_height, cu6.GetPos().z - cube_length / 2.0f, rotating_stuff, false, false);
	bodyA = App->physics->AddBody(rotating_cu, 1000.0f);
	Cylinder rotating_cy = CreateCylinder(cylinder_rotating_width, cylinder_height, rotating_cu.GetPos().x, rotating_cu.GetPos().y - cylinder_height / 2.0f, rotating_cu.GetPos().z, right_angle, orthonormal_z, rotating_stuff, false, false);
	PhysBody3D* bodyB = App->physics->AddBody(rotating_cy, 1000.0f);
	Cylinder real_cy = CreateCylinder(cylinder_real_width, cylinder_height, rotating_cu.GetPos().x, rotating_cu.GetPos().y - cylinder_height / 2.0f, rotating_cu.GetPos().z, right_angle, orthonormal_z, rotating_stuff);

	rotating_cu2 = CreateCube(cube_width, cube_height, cube_length, cu6.GetPos().x - 20.0f, cu6.GetPos().y + 2.0f * cube_height, cu6.GetPos().z + cube_length / 2.0f, rotating_stuff, false, false);
	bodyA2 = App->physics->AddBody(rotating_cu2, 1000.0f);
	Cylinder rotating_cy2 = CreateCylinder(cylinder_rotating_width, cylinder_height, rotating_cu2.GetPos().x, rotating_cu2.GetPos().y - cylinder_height / 2.0f, rotating_cu2.GetPos().z, right_angle, orthonormal_z, rotating_stuff, false, false);
	PhysBody3D* bodyB2 = App->physics->AddBody(rotating_cy2, 1000.0f);
	Cylinder real_cy2 = CreateCylinder(cylinder_real_width, cylinder_height, rotating_cu2.GetPos().x, rotating_cu2.GetPos().y - cylinder_height / 2.0f, rotating_cu2.GetPos().z, right_angle, orthonormal_z, rotating_stuff);

	bodyA->GetBody()->setLinearFactor(btVector3(0, 0, 0));
	bodyA2->GetBody()->setLinearFactor(btVector3(0, 0, 0));
	bodyB->GetBody()->setLinearFactor(btVector3(0, 0, 0));
	bodyB->GetBody()->setAngularFactor(btVector3(0, 0, 0));
	bodyB2->GetBody()->setLinearFactor(btVector3(0, 0, 0));
	bodyB2->GetBody()->setAngularFactor(btVector3(0, 0, 0));

	App->physics->AddConstraintHinge(*bodyA, *bodyB, vec3(0, 0, 0), vec3(0, 0, 0), vec3(0, -1, 0), vec3(0, 0, 0), true, true);
	App->physics->AddConstraintHinge(*bodyA2, *bodyB2, vec3(0, 0, 0), vec3(0, 0, 0), vec3(0, 1, 0), vec3(0, 0, 0), true, true);

	// Finish line
	float line_width = 1.0f;
	float line_length = 1.0f;

	CreateFinishLine(line_width, road_height, line_length, line_width / 2.0f - cu.GetSize().x / 2.0f, 0.1f, 0.0f);
	//_finish_line
	//_road_primitives
	//_road

	return ret;
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

	sensor2->GetTransform(&g.transform);
	g.Render();

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
	CreateSpeedUp(speed_width, speed_height, speed_length, cu6.GetPos().x, road_height / 2.0f, cu6.GetPos().z);

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
	if (body1 == sensor) {
		App->player->speedup = true;
	}

	else if (body1) {
		checkpoints_index = 1;
	}
}