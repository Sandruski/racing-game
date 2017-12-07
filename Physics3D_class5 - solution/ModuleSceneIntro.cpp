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
	Cube cu2 = CreateCube(road_width, road_height, road_length, cu.GetPos().x, cu.GetPos().y, cu.GetPos().z + cu.GetSize().z / 2.0f + road_length / 2.0f, soil);
	Cylinder cy = CreateCylinder(road_width / 2.0f, road_height, cu2.GetPos().x, cu2.GetPos().y, cu2.GetPos().z + cu2.GetSize().z / 2.0f, right_angle, orthonormal_z, soil);
	Cube cu3 = CreateCube(road_length, road_height, road_width, cy.GetPos().x + road_length / 2.0f, cy.GetPos().y, cy.GetPos().z, soil);
	Cylinder cy2 = CreateCylinder(road_width / 2.0f, road_height, cu3.GetPos().x + cu3.GetSize().x / 2.0f, cu3.GetPos().y, cu3.GetPos().z, right_angle, orthonormal_z, soil);
	Cube cu4 = CreateCube(road_width, road_height, road_short_length, cy2.GetPos().x, cy2.GetPos().y, cy2.GetPos().z + road_short_length / 2.0f, soil);
	Cylinder cy3 = CreateCylinder(road_width / 2.0f, road_height, cu4.GetPos().x, cu4.GetPos().y, cu4.GetPos().z + cu4.GetSize().z / 2.0f, right_angle, orthonormal_z, soil);
	Cube cu5 = CreateCube(road_length, road_height, road_width, cy3.GetPos().x + road_length / 2.0f, cy3.GetPos().y, cy3.GetPos().z, soil);
	Cylinder cy4 = CreateCylinder(road_width / 2.0f, road_height, cu5.GetPos().x + cu5.GetSize().x / 2.0f, cu5.GetPos().y, cu5.GetPos().z, right_angle, orthonormal_z, soil);
	cu6 = CreateCube(road_width, road_height, 6 * road_length, cy4.GetPos().x, cy4.GetPos().y, cy4.GetPos().z + (6 * road_length) / 2.0f, soil);

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
	Cylinder real_cy2 = CreateCylinder(cylinder_real_width, cylinder_height, rotating_cu.GetPos().x, rotating_cu.GetPos().y - cylinder_height / 2.0f, rotating_cu.GetPos().z, right_angle, orthonormal_z, rotating_stuff);

	bodyA->GetBody()->setLinearFactor(btVector3(0, 0, 0));
	bodyA2->GetBody()->setLinearFactor(btVector3(0, 0, 0));
	bodyB->GetBody()->setLinearFactor(btVector3(0, 0, 0));
	bodyB->GetBody()->setAngularFactor(btVector3(0, 0, 0));
	bodyB2->GetBody()->setLinearFactor(btVector3(0, 0, 0));
	bodyB2->GetBody()->setAngularFactor(btVector3(0, 0, 0));

	App->physics->AddConstraintHinge(*bodyA, *bodyB, vec3(0, 0, 0), vec3(0, 0, 0), vec3(0, -1, 0), vec3(0, 0, 0), true, true);
	App->physics->AddConstraintHinge(*bodyA2, *bodyB2, vec3(0, 0, 0), vec3(0, 0, 0), vec3(0, 1, 0), vec3(0, 0, 0), true, true);

	// Ground
	float ground_width = 150.0f;
	float ground_height = 15.0f; // y axis
	float ground_length = 100.0f;

	Cube ground = CreateCube(ground_width, ground_height, ground_length, cy4.GetPos().x - ground_width / 3.0f, cy4.GetPos().y - ground_height / 2, cy4.GetPos().z + cy4.radius - ground_length / 2, Pink);
	Cube ground1 = CreateCube(ground_width, ground_height, ground_length, ground.GetPos().x, ground.GetPos().y - ground_height, ground.GetPos().z + ground_length, Cyan);
	//_ground

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

	/// It is only needed to set the position and size of the first element of the speed-up
	CreateSpeedUp(speed_width, speed_height, speed_length, 0, road_height, 0);
	CreateSpeedUp(speed_width, speed_height, speed_length, cu6.GetPos().x, road_height, cu6.GetPos().z);

	UpdateSpeedUpColors(dt);

	return UPDATE_CONTINUE;
}

// Specific functions
void ModuleSceneIntro::CreateSpeedUp(float sizeX, float sizeY, float sizeZ, float posX, float posY, float posZ)
{
	Cube speed_a = CreateCube(sizeX, sizeY, sizeZ, posX, posY, posZ, colors[index_d], false, false);
	speed_a.Render();
	Cube speed_b = CreateCube(sizeX, sizeY, sizeZ, speed_a.GetPos().x, posY, speed_a.GetPos().z + speed_a.GetSize().z, colors[index_c], false, false);
	speed_b.Render();
	Cube speed_c = CreateCube(sizeX, sizeY, sizeZ, speed_b.GetPos().x, posY, speed_b.GetPos().z + speed_b.GetSize().z, colors[index_b], false, false);
	speed_c.Render();
	Cube speed_d = CreateCube(sizeX, sizeY, sizeZ, speed_c.GetPos().x, posY, speed_c.GetPos().z + speed_c.GetSize().z, colors[index_a], false, false);
	speed_d.Render();
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
Cube ModuleSceneIntro::CreateCube(float sizeX, float sizeY, float sizeZ, float posX, float posY, float posZ, Color color, bool draw, bool collider, float mass)
{
	Cube cu(sizeX, sizeY, sizeZ);
	cu.SetPos(posX, posY, posZ);
	cu.color = color;

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