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

	start_pos.x = node.child("start_pos").attribute("x").as_float();
	start_pos.y = node.child("start_pos").attribute("y").as_float();
	start_pos.z = node.child("start_pos").attribute("z").as_float();

	checkpoint1_pos.x = node.child("checkpoint1").attribute("x").as_float();
	checkpoint1_pos.y = node.child("checkpoint1").attribute("y").as_float();
	checkpoint1_pos.z = node.child("checkpoint1").attribute("z").as_float();

	checkpoint2_pos.x = node.child("checkpoint2").attribute("x").as_float();
	checkpoint2_pos.y = node.child("checkpoint2").attribute("y").as_float();
	checkpoint2_pos.z = node.child("checkpoint2").attribute("z").as_float();

	checkpoint3_pos.x = node.child("checkpoint3").attribute("x").as_float();
	checkpoint3_pos.y = node.child("checkpoint3").attribute("y").as_float();
	checkpoint3_pos.z = node.child("checkpoint3").attribute("z").as_float();

	checkpoint4_pos.x = node.child("checkpoint4").attribute("x").as_float();
	checkpoint4_pos.y = node.child("checkpoint4").attribute("y").as_float();
	checkpoint4_pos.z = node.child("checkpoint4").attribute("z").as_float();

	checkpoint5_pos.x = node.child("checkpoint5").attribute("x").as_float();
	checkpoint5_pos.y = node.child("checkpoint5").attribute("y").as_float();
	checkpoint5_pos.z = node.child("checkpoint5").attribute("z").as_float();

	win_pos.x = node.child("win_pos").attribute("x").as_float();
	win_pos.y = node.child("win_pos").attribute("y").as_float();
	win_pos.z = node.child("win_pos").attribute("z").as_float();

	lose_pos.x = node.child("lose_pos").attribute("x").as_float();
	lose_pos.y = node.child("lose_pos").attribute("y").as_float();
	lose_pos.z = node.child("lose_pos").attribute("z").as_float();

	FX1_path = node.child("FX1").attribute("path").as_string();
	FX2_path = node.child("FX2").attribute("path").as_string();

	seconds_to_jump = node.child("seconds_to_jump").attribute("value").as_float();
	jump_force = node.child("jump_force").attribute("value").as_float();
	
	speed_positive_x = node.child("speed_up").attribute("positive_x").as_float();
	speed_negative_x = node.child("speed_up").attribute("negative_x").as_float();
	speed_positive_z = node.child("speed_up").attribute("positive_z").as_float();
	speed_negative_z = node.child("speed_up").attribute("negative_z").as_float();

	max_speed = node.child("max_speed").attribute("value").as_float();
	max_speed = node.child("friction").attribute("value").as_float();

	return ret;
}

ModulePlayer::~ModulePlayer()
{}

// Load assets
bool ModulePlayer::Start()
{
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
	position = start_pos;
	vehicle->SetPos(position.x, position.y, position.z);
	
	// Load FX
	FX_index[0] = App->audio->LoadFx(FX1_path.GetString());
	FX_index[1] = App->audio->LoadFx(FX2_path.GetString());

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
	// F1: debug draw
	if (App->input->GetKey(SDL_SCANCODE_F1) == KEY_DOWN)
		App->physics->SetDebugDraw();

	// Vehicle jump
	if (cantJump) {
		timerJump += 1.0f * dt;
		if (timerJump >= seconds_to_jump) {
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
		vehicle->Push(0.0f, jump_force, 0.0f);
		cantJump = true;
		vehicle->info.color = IndianRed;
	}
	if (App->input->GetKey(SDL_SCANCODE_A) == KEY_REPEAT)
	{
		brake = BRAKE_POWER;
	}
	
	// Z: restart game (spawn at first checkpoint)
	if (App->input->GetKey(SDL_SCANCODE_Z) == KEY_DOWN)
	{
		vehicle->body->setLinearVelocity(btVector3(0, 0, 0));
		vehicle->body->setAngularVelocity(btVector3(0, 0, 0));

		IdentityMatrix = IM;
		vehicle->SetTransform(IdentityMatrix.M);

		finished = false;
		cantJump = false;
		timerJump = 0.0f;
		vehicle->info.color = White;
		App->scene_intro->loopsCount = 0;
		App->scene_intro->minutes = 1;
		App->scene_intro->seconds = 60.0f;
		App->scene_intro->endTime = false;
		App->scene_intro->winCondition = 0;
		App->scene_intro->checkpoints_index = 0;
	}

	// R: spawn at last checkpoint
	if (App->input->GetKey(SDL_SCANCODE_R) == KEY_DOWN && App->scene_intro->winCondition == 0)
	{
		vehicle->body->setLinearVelocity(btVector3(0, 0, 0));
		vehicle->body->setAngularVelocity(btVector3(0, 0, 0));

		switch (App->scene_intro->checkpoints_index)
		{
		case 0:
			vehicle->SetTransform(IdentityMatrix.M);
			vehicle->SetPos(start_pos.x, start_pos.y, start_pos.z);
			break;
		case 1:
			vehicle->SetTransform(IdentityMatrix.M);
			vehicle->SetPos(checkpoint1_pos.x, checkpoint1_pos.y, checkpoint1_pos.z);
			break;
		case 2:
			vehicle->SetTransform(IdentityMatrix.M);
			vehicle->SetPos(checkpoint2_pos.x, checkpoint2_pos.y, checkpoint2_pos.z);
			break;
		case 3:
			vehicle->SetTransform(IdentityMatrix.rotate(180.0f, vec3(0.0f, 1.0f, 0.0f)).M);
			IdentityMatrix.rotate(180.0f, vec3(0.0f, 1.0f, 0.0f));
			vehicle->SetPos(checkpoint3_pos.x, checkpoint3_pos.y, checkpoint3_pos.z);
			break;
		case 4:
			vehicle->SetTransform(IdentityMatrix.rotate(180.0f, vec3(0.0f, 1.0f, 0.0f)).M);
			IdentityMatrix.rotate(180.0f, vec3(0.0f, 1.0f, 0.0f));
			vehicle->SetPos(checkpoint4_pos.x, checkpoint4_pos.y, checkpoint4_pos.z);
			break;
		case 5:
			vehicle->SetTransform(IdentityMatrix.rotate(-90.0f, vec3(0.0f, 1.0f, 0.0f)).M);
			IdentityMatrix.rotate(90.0f, vec3(0.0f, 1.0f, 0.0f));
			vehicle->SetPos(checkpoint5_pos.x, checkpoint5_pos.y, checkpoint5_pos.z);
			break;
		}
	}

	// Simulate friction with the ground
	if (vehicle->GetKmh() > max_speed)
		acceleration = 0.0f;

	if (vehicle->GetKmh() > 0.0f)
		acceleration -= friction;

	// Speed-ups
	if (speedupZ)
	{
		vehicle->Push(0.0f, 0.0f, speed_positive_z);
		speedupZ = false;
	}
	else if (speedupZnegative)
	{
		vehicle->Push(0.0f, 0.0f, speed_negative_z);
		speedupZnegative = false;
	}
	else if (speedupX)
	{
		vehicle->Push(speed_positive_x, 0.0f, 0.0f);
		speedupX = false;
	}
	else if (speedupXnegative)
	{
		vehicle->Push(speed_negative_x, 0.0f, 0.0f);
		speedupXnegative = false;
	}

	// Win and lose conditions
	// L: win
	if ((App->scene_intro->winCondition == 1 && !finished) || App->input->GetKey(SDL_SCANCODE_L) == KEY_DOWN)
	{
		App->audio->PlayFx(FX_index[0]);
		vehicle->body->setLinearVelocity(btVector3(0, 0, 0));
		vehicle->body->setAngularVelocity(btVector3(0, 0, 0));
		App->scene_intro->winCondition = 1;
		IdentityMatrix = IM;
		vehicle->SetTransform(IdentityMatrix.M);
		vehicle->SetPos(win_pos.x, win_pos.y, win_pos.z);
		finished = true;
	}

	// K: lose
	else if ((App->scene_intro->winCondition == 2 && !finished) || App->input->GetKey(SDL_SCANCODE_K) == KEY_DOWN)
	{
		App->audio->PlayFx(FX_index[1]);
		vehicle->body->setLinearVelocity(btVector3(0, 0, 0));
		vehicle->body->setAngularVelocity(btVector3(0, 0, 0));
		App->scene_intro->winCondition = 2;
		IdentityMatrix = IM;
		vehicle->SetTransform(IdentityMatrix.M);
		vehicle->SetPos(lose_pos.x, lose_pos.y, lose_pos.z);
		finished = true;
	}

	// Update vehicle parameters
	vehicle->ApplyEngineForce(acceleration);
	vehicle->Turn(turn);
	vehicle->Brake(brake);
	vehicle->Render();

	vehicle->GetPos(position.x, position.y, position.z);

	// Set window title info
	p2SString song = "Persona 5";
	if (App->scene_intro->indexMusic == 0)
		song = "Persona 5";
	else if (App->scene_intro->indexMusic == 1)
		song = "Ghostbusters";
	else if (App->scene_intro->indexMusic == 2)
		song = "Let It Go";
	else if (App->scene_intro->indexMusic == 3)
		song = "Pokemon";

	char title[80];
	p2SString win = "...";
	if (App->scene_intro->winCondition == 1)
		win = "VICTORY!!!";
	else if (App->scene_intro->winCondition == 2)
		win = "LOSE!!!";
	sprintf_s(title, "%.1f Km/h, Checkpoint: %i, Time: %i:%.1f, Win: %s, Song: %s", vehicle->GetKmh(), App->scene_intro->checkpoints_index, App->scene_intro->minutes, App->scene_intro->seconds, win.GetString(), song.GetString());
	App->window->SetTitle(title);

	return UPDATE_CONTINUE;
}