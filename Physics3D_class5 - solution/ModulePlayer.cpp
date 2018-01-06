#include "Globals.h"
#include "Application.h"
#include "ModulePlayer.h"
#include "Primitive.h"
#include "PhysVehicle3D.h"
#include "PhysBody3D.h"
#include "ModuleSceneIntro.h"
#include "ModuleAudio.h"


ModulePlayer::ModulePlayer(Application* app, bool start_enabled) : Module(app, start_enabled), vehicle(NULL)
{
	name.create("player");

	turn = acceleration = brake = 0.0f;
}

bool ModulePlayer::Init(pugi::xml_node& node) 
{
	bool ret = true;

	return ret;
}

ModulePlayer::~ModulePlayer()
{}

// Load assets
bool ModulePlayer::Start()
{
	App->audio->LoadFx("Music/WAV/IDLE.wav");
	App->audio->LoadFx("Music/WAV/LOOP.wav");

	LOG("Loading player");

	VehicleInfo car;

	// Car properties ----------------------------------------
	car.chassis_size.Set(2, 1, 4);
	car.chassis_offset.Set(0, 1, 0);
	car.color = White;

	car.mass = 500.0f;
	car.suspensionStiffness = 15.88f;
	car.suspensionCompression = 0.83f;
	car.suspensionDamping = 0.88f;
	car.maxSuspensionTravelCm = 1000.0f;
	car.frictionSlip = 100.5;
	car.maxSuspensionForce = 6000.0f;
	
	// Wheel properties ---------------------------------------
	float connection_height = 1.2f;
	float wheel_radius = 0.5f;
	float wheel_width = 0.5f;
	float suspensionRestLength = 1.2f;

	// Don't change anything below this line ------------------

	float half_width = car.chassis_size.x*0.5f;
	float half_length = car.chassis_size.z*0.5f;
	
	vec3 direction(0,-1,0);
	vec3 axis(-1,0,0);
	
	car.num_wheels = 4;
	car.wheels = new Wheel[4];

	// FRONT-LEFT ------------------------
	car.wheels[0].connection.Set(half_width - 0.3f * wheel_width, connection_height, half_length - wheel_radius);
	car.wheels[0].direction = direction;
	car.wheels[0].axis = axis;
	car.wheels[0].suspensionRestLength = suspensionRestLength;
	car.wheels[0].radius = wheel_radius;
	car.wheels[0].width = wheel_width;
	car.wheels[0].front = true;
	car.wheels[0].drive = true;
	car.wheels[0].brake = false;
	car.wheels[0].steering = true;

	// FRONT-RIGHT ------------------------
	car.wheels[1].connection.Set(-half_width + 0.3f * wheel_width, connection_height, half_length - wheel_radius);
	car.wheels[1].direction = direction;
	car.wheels[1].axis = axis;
	car.wheels[1].suspensionRestLength = suspensionRestLength;
	car.wheels[1].radius = wheel_radius;
	car.wheels[1].width = wheel_width;
	car.wheels[1].front = true;
	car.wheels[1].drive = true;
	car.wheels[1].brake = false;
	car.wheels[1].steering = true;

	// REAR-LEFT ------------------------
	car.wheels[2].connection.Set(half_width - 0.3f * wheel_width, connection_height, -half_length + wheel_radius);
	car.wheels[2].direction = direction;
	car.wheels[2].axis = axis;
	car.wheels[2].suspensionRestLength = suspensionRestLength;
	car.wheels[2].radius = wheel_radius;
	car.wheels[2].width = wheel_width;
	car.wheels[2].front = false;
	car.wheels[2].drive = false;
	car.wheels[2].brake = true;
	car.wheels[2].steering = false;

	// REAR-RIGHT ------------------------
	car.wheels[3].connection.Set(-half_width + 0.3f * wheel_width, connection_height, -half_length + wheel_radius);
	car.wheels[3].direction = direction;
	car.wheels[3].axis = axis;
	car.wheels[3].suspensionRestLength = suspensionRestLength;
	car.wheels[3].radius = wheel_radius;
	car.wheels[3].width = wheel_width;
	car.wheels[3].front = false;
	car.wheels[3].drive = false;
	car.wheels[3].brake = true;
	car.wheels[3].steering = false;

	vehicle = App->physics->AddVehicle(car);
	position = { 0, 0, -4 };
	vehicle->SetPos(position.x, position.y, position.z);
	
	Sphere ballon;
	ballon.radius = 1.0f;
	ballon.SetPos(0, 16, 10);

	sphere = App->physics->AddBody(ballon);
	float x, y, z;
	vehicle->GetPos(x, y, z);
	float i, j, k;
	sphere->GetPos(i, j, k);
//	App->physics->AddConstraintP2P(*(PhysBody3D*)(vehicle->GetBody()), *sphere, vec3(0,0,0), vec3(0,1,0));
	//App->audio->PlayFx(0);
	music_index[0] = App->audio->LoadFx("Music/WAV/VICTORY.wav");
	music_index[1] = App->audio->LoadFx("Music/WAV/GAMEOVER.wav");
	return true;
}

// Unload assets
bool ModulePlayer::CleanUp()
{
	LOG("Unloading player");

	return true;
}

// Update: draw background
update_status ModulePlayer::Update(float dt)
{
	if (cantJump) {
		timerJump += 1 * dt;
		if (timerJump >= 6.0f) {
			cantJump = false;
			timerJump = 0.0f;
			vehicle->info.color = White;
		}
	}

	turn = acceleration = brake = 0.0f;

	if (App->input->GetKey(SDL_SCANCODE_UP) == KEY_REPEAT && App->scene_intro->winCondition == 0)
	{
		acceleration = MAX_ACCELERATION;	
	}

	if (App->input->GetKey(SDL_SCANCODE_LEFT) == KEY_REPEAT)
	{
		if (turn < TURN_DEGREES)
			turn += TURN_DEGREES;
	}

	if (App->input->GetKey(SDL_SCANCODE_RIGHT) == KEY_REPEAT)
	{
		if(turn > -TURN_DEGREES)
			turn -= TURN_DEGREES;
	}

	if (App->input->GetKey(SDL_SCANCODE_DOWN) == KEY_REPEAT && App->scene_intro->winCondition == 0)
	{
		acceleration = -MAX_ACCELERATION;
	}

	if (App->input->GetKey(SDL_SCANCODE_SPACE) == KEY_DOWN && !cantJump)
	{
		vehicle->Push(0, 5000.0f, 0);
		cantJump = true;
		vehicle->info.color = Red;
	}

	if (App->input->GetKey(SDL_SCANCODE_A) == KEY_REPEAT)
	{
		brake = BRAKE_POWER;
	}
	
	if (App->input->GetKey(SDL_SCANCODE_Z) == KEY_REPEAT)
	{
		vehicle->body->setLinearVelocity(btVector3(0, 0, 0));
		vehicle->body->setAngularVelocity(btVector3(0, 0, 0));
		vehicle->SetTransform(IdentityMatrix.M);
		vehicle->SetPos(0, 0, -4);
		finished = false;
		cantJump = false;
		timerJump = 0.0f;
		vehicle->info.color = White;
		App->scene_intro->loopsCount = 0;
		App->scene_intro->minutes = 1;
		App->scene_intro->seconds = 60;
		App->scene_intro->endTime = false;
		App->scene_intro->winCondition = 0;
		App->scene_intro->checkpoints_index = 0;
	}

	if (App->input->GetKey(SDL_SCANCODE_R) == KEY_REPEAT && App->scene_intro->winCondition == 0)
	{
			vehicle->body->setLinearVelocity(btVector3(0, 0, 0));
			vehicle->body->setAngularVelocity(btVector3(0, 0, 0));
			vehicle->SetTransform(IdentityMatrix.M);
		
		switch (App->scene_intro->checkpoints_index)
		{
		case 0:
			vehicle->SetPos(0, 0, -4);
			break;
		case 1: case 6: case 11:
			vehicle->SetPos(60, 12, 65);
			break;
		case 2: case 7: case 12:
			vehicle->SetPos(60, 12, 300);
			break;
		case 3: case 8: case 13:
			vehicle->SetTransform(IdentityMatrix.rotate(180, vec3(0, 1, 0)).M);
			IdentityMatrix.rotate(180, vec3(0, 1, 0));
			vehicle->SetPos(277, 12, 267);
			break;
		case 4: case 9: case 14:
			vehicle->SetTransform(IdentityMatrix.rotate(180, vec3(0, 1, 0)).M);
			IdentityMatrix.rotate(180, vec3(0, 1, 0));
			vehicle->SetPos(277, 12, 70);
			break;
		case 5: case 10: case 15:
			vehicle->SetTransform(IdentityMatrix.rotate(-90, vec3(0, 1, 0)).M);
			IdentityMatrix.rotate(90, vec3(0, 1, 0));
			vehicle->SetPos(338, 12, -55);		
			break;
		}
	}

	if (vehicle->GetKmh() > 80)
		acceleration = 0;

	if (vehicle->GetKmh() > 0)
		acceleration -= 100;

	if (speedupZ)
	{
		vehicle->Push(0, 0, 300.0f);
		speedupZ = false;
	}
	else if (speedupZnegative)
	{
		vehicle->Push(0, 0, -300.0f);
		speedupZnegative = false;
	}
	else if (speedupX)
	{
		vehicle->Push(150.0f, 0, 0);
		speedupX = false;
	}
	else if (speedupXnegative)
	{
		vehicle->Push(-300.0f, 0, 0);
		speedupXnegative = false;
	}

	if ((App->scene_intro->winCondition == 1 && !finished) || App->input->GetKey(SDL_SCANCODE_L) == KEY_DOWN)
	{
		App->audio->PlayFx(music_index[0]);
		vehicle->body->setLinearVelocity(btVector3(0, 0, 0));
		vehicle->body->setAngularVelocity(btVector3(0, 0, 0));
		App->scene_intro->winCondition = 1;
		vehicle->SetPos(170, 40, 184);
		finished = true;
	}
	else if ((App->scene_intro->winCondition == 2 && !finished) || App->input->GetKey(SDL_SCANCODE_K) == KEY_DOWN)
	{
		App->audio->PlayFx(music_index[1]);
		vehicle->body->setLinearVelocity(btVector3(0, 0, 0));
		vehicle->body->setAngularVelocity(btVector3(0, 0, 0));
		App->scene_intro->winCondition = 2;
		vehicle->SetPos(200, 200, 200);
		finished = true;
	}

	vehicle->ApplyEngineForce(acceleration);

	vehicle->Turn(turn);
	vehicle->Brake(brake);
	vehicle->Render();

	vehicle->GetPos(position.x, position.y, position.z);
	position;
	char title[80];
	p2SString win = "...";
	if (App->scene_intro->winCondition == 1)
		win = "VICTORY!!!";
	else if (App->scene_intro->winCondition == 2)
		win = "Maybe next time...";
	sprintf_s(title, "%.1f Km/h, checkpoint: %i loops: %i, time: %i:%.1f, Win: %s", vehicle->GetKmh(), App->scene_intro->checkpoints_index, App->scene_intro->loopsCount, App->scene_intro->minutes, App->scene_intro->seconds, win.GetString());
	App->window->SetTitle(title);

	return UPDATE_CONTINUE;
}