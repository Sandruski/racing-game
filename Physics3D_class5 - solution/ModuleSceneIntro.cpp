#include "Globals.h"
#include "Application.h"
#include "ModuleSceneIntro.h"
#include "Primitive.h"
#include "PhysBody3D.h"

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

	// Colliders creation
	float mass = 0.0f;

	// ROAD
	// Road parameters
	float road_width = 12.0f;
	float road_height = 0.5f; // y axis
	float road_length = 30.0f;

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
	Cube cu = CreateCube(road_width, road_height, road_length, 0.0f, 0.0f, 0.0f, soil);
	App->physics->AddBody(cu, mass);
	Cube cu2 = CreateCube(road_width, road_height, road_length, cu.GetPos().x, cu.GetPos().y, cu.GetPos().z + cu.GetSize().z / 2.0f + road_length / 2.0f, soil);
	App->physics->AddBody(cu2, mass);
	Cylinder cy = CreateCylinder(road_width / 2.0f, road_height, cu2.GetPos().x, cu2.GetPos().y, cu2.GetPos().z + cu2.GetSize().z / 2.0f, right_angle, orthonormal_z, soil);
	App->physics->AddBody(cy, mass);
	Cube cu3 = CreateCube(road_length, road_height, road_width, cy.GetPos().x + road_length / 2.0f, cy.GetPos().y, cy.GetPos().z, soil);
	App->physics->AddBody(cu3, mass);
	Cylinder cy2 = CreateCylinder(road_width / 2.0f, road_height, cu3.GetPos().x + cu3.GetSize().x / 2.0f, cu3.GetPos().y, cu3.GetPos().z, right_angle, orthonormal_z, soil);
	App->physics->AddBody(cy2, mass);
	Cube cu4 = CreateCube(road_width, road_height, road_short_length, cy2.GetPos().x, cy2.GetPos().y, cy2.GetPos().z + road_short_length / 2.0f, soil);
	App->physics->AddBody(cu4, mass);
	Cylinder cy3 = CreateCylinder(road_width / 2.0f, road_height, cu4.GetPos().x, cu4.GetPos().y, cu4.GetPos().z + cu4.GetSize().z / 2.0f, right_angle, orthonormal_z, soil);
	App->physics->AddBody(cy3, mass);
	Cube cu5 = CreateCube(road_length, road_height, road_width, cy3.GetPos().x + road_length / 2.0f, cy3.GetPos().y, cy3.GetPos().z, soil);
	App->physics->AddBody(cu5, mass);
	Cylinder cy4 = CreateCylinder(road_width / 2.0f, road_height, cu5.GetPos().x + cu5.GetSize().x / 2.0f, cu5.GetPos().y, cu5.GetPos().z, right_angle, orthonormal_z, soil);
	App->physics->AddBody(cy4, mass);
	Cube cu6 = CreateCube(road_width, road_height, 3 * road_length, cy4.GetPos().x, cy4.GetPos().y, cy4.GetPos().z + 3 * road_length / 2.0f, soil);
	App->physics->AddBody(cu6, mass);

	// GROUND
	float ground_width = 150.0f;
	float ground_height = 15.0f; // y axis
	float ground_length = 100.0f;

	Cube ground = CreateCube(ground_width, ground_height, ground_length, cy4.GetPos().x - ground_width / 3.0f, cy4.GetPos().y - ground_height / 2, cy4.GetPos().z + cy4.radius - ground_length / 2, Pink);
	App->physics->AddBody(ground, mass);
	Cube ground1 = CreateCube(ground_width, ground_height, ground_length, ground.GetPos().x, ground.GetPos().y - ground_height, ground.GetPos().z + ground_length, Cyan);
	App->physics->AddBody(ground1, mass);
	//_ground

	// SPEED-UPS
	float speed_width = 4.0f;
	float speed_height = 0.1f; // y axis
	float speed_length = 2.0f;

	// First set
	/// It is only needed to set the position and size of the first element of the speed-up
	CreateSpeedUp(speed_width, speed_height, speed_length, 0, road_height, 0);
	CreateSpeedUp(speed_width, speed_height, speed_length, cu6.GetPos().x, road_height, cu6.GetPos().z);
	//_speed-ups
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
	// ROAD
	// Road parameters
	float road_width = 12.0f;
	float road_height = 0.5f; // y axis
	float road_length = 30.0f;

	float road_short_length = 20.0f;

	vec3 orthonormal_x(1, 0, 0);
	vec3 orthonormal_y(0, 1, 0);
	vec3 orthonormal_z(0, 0, 1);
	float right_angle = 90.0f;

	Color soil = Beige;
	//_road_parameters

	// Debug cube
	CreateCube(2.0f, 2.0f, 2.0f, 0.0f, 3.0f, 0.0f, White, true, true);
	//_debug_cube

	// Road primitives
	/// Always use last primitive created to set the position of the next primitive
	/// Notice that the position of the primitives follows a pattern
	Cube cu = CreateCube(road_width, road_height, road_length, 0.0f, 0.0f, 0.0f, soil);

	Cube cu2 = CreateCube(road_width, road_height, road_length, cu.GetPos().x, cu.GetPos().y, cu.GetPos().z + cu.GetSize().z / 2.0f + road_length / 2.0f, soil);

	Cylinder cy = CreateCylinder(road_width / 2.0f, road_height, cu2.GetPos().x, cu2.GetPos().y, cu2.GetPos().z + cu2.GetSize().z / 2.0f, right_angle, orthonormal_z, soil);

	Cube cu3 = CreateCube(road_length, road_height, road_width, cy.GetPos().x + road_length / 2.0f, cy.GetPos().y, cy.GetPos().z, soil);

	Cylinder cy2 = CreateCylinder(road_width / 2.0f, road_height, cu3.GetPos().x + cu3.GetSize().x / 2.0f, cu3.GetPos().y, cu3.GetPos().z, right_angle, orthonormal_z, soil);

	Cube cu4 = CreateCube(road_width, road_height, road_short_length, cy2.GetPos().x, cy2.GetPos().y, cy2.GetPos().z + road_short_length / 2.0f, soil);

	Cylinder cy3 = CreateCylinder(road_width / 2.0f, road_height, cu4.GetPos().x, cu4.GetPos().y, cu4.GetPos().z + cu4.GetSize().z / 2.0f, right_angle, orthonormal_z, soil);

	Cube cu5 = CreateCube(road_length, road_height, road_width, cy3.GetPos().x + road_length / 2.0f, cy3.GetPos().y, cy3.GetPos().z, soil);

	Cylinder cy4 = CreateCylinder(road_width / 2.0f, road_height, cu5.GetPos().x + cu5.GetSize().x / 2.0f, cu5.GetPos().y, cu5.GetPos().z, right_angle, orthonormal_z, soil);

	Cube cu6 = CreateCube(road_width, road_height, 3 * road_length, cy4.GetPos().x, cy4.GetPos().y, cy4.GetPos().z + 3 * road_length / 2.0f, soil);

	// GROUND
	float ground_width = 150.0f;
	float ground_height = 15.0f; // y axis
	float ground_length = 100.0f;

	Cube ground = CreateCube(ground_width, ground_height, ground_length, cy4.GetPos().x - ground_width / 3.0f, cy4.GetPos().y - ground_height / 2, cy4.GetPos().z + cy4.radius - ground_length / 2, Pink);
	
	Cube ground1 = CreateCube(ground_width, ground_height, ground_length, ground.GetPos().x, ground.GetPos().y - ground_height, ground.GetPos().z + ground_length, Cyan);
	//_ground

	// SPEED-UPS
	float speed_width = 4.0f;
	float speed_height = 0.1f; // y axis
	float speed_length = 2.0f;

	// First set
	/// It is only needed to set the position and size of the first element of the speed-up
	CreateSpeedUp(speed_width, speed_height, speed_length, 0, road_height, 0);
	CreateSpeedUp(speed_width, speed_height, speed_length, cu6.GetPos().x, road_height, cu6.GetPos().z);

	UpdateSpeedUpColors(dt);
	//_speed-ups
	//_road_primitives
	//_road

	return UPDATE_CONTINUE;
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

void ModuleSceneIntro::CreateSpeedUp(float sizeX, float sizeY, float sizeZ, float posX, float posY, float posZ)
{
	Cube speed_a = CreateCube(sizeX, sizeY, sizeZ, posX, posY, posZ, colors[index_d]);
	Cube speed_b = CreateCube(sizeX, sizeY, sizeZ, speed_a.GetPos().x, posY, speed_a.GetPos().z + speed_a.GetSize().z, colors[index_c]);
	Cube speed_c = CreateCube(sizeX, sizeY, sizeZ, speed_b.GetPos().x, posY, speed_b.GetPos().z + speed_b.GetSize().z, colors[index_b]);
	Cube speed_d = CreateCube(sizeX, sizeY, sizeZ, speed_c.GetPos().x, posY, speed_c.GetPos().z + speed_c.GetSize().z, colors[index_a]);
}

Cube ModuleSceneIntro::CreateCube(float sizeX, float sizeY, float sizeZ, float posX, float posY, float posZ, Color color, bool axis, bool wire)
{
	Cube cu(sizeX, sizeY, sizeZ);
	cu.SetPos(posX, posY, posZ);
	cu.color = color;

	cu.axis = axis;
	cu.wire = wire;

	cu.Render();

	return cu;
}

Cylinder ModuleSceneIntro::CreateCylinder(float radius, float height, float posX, float posY, float posZ, float angle, vec3 u, Color color, bool axis, bool wire)
{
	Cylinder cy(radius, height);
	cy.SetPos(posX, posY, posZ);
	cy.SetRotation(angle, vec3(u.x, u.y, u.z));
	cy.color = color;

	cy.axis = axis;
	cy.wire = wire;

	cy.Render();

	return cy;
}

void ModuleSceneIntro::OnCollision(PhysBody3D* body1, PhysBody3D* body2)
{
}

