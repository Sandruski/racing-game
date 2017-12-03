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
	PolygonPlane p;
	p.color = GrassGreen;
	p.Scale(100, 0, 100);
	p.Render();

	Cube c(12, 0.5f, 40);
	c.color = Grey;
	c.Render();

	Cube c2(12, 0.5f, 40);
	c2.SetPos(c.GetPos().x, c.GetPos().y, c.GetPos().z + c.GetSize().z / 2 + c2.GetSize().z / 2);

	c2.Render();

	//c.size = vec3(100, 1, 1);

	return UPDATE_CONTINUE;
}

void ModuleSceneIntro::OnCollision(PhysBody3D* body1, PhysBody3D* body2)
{
}

