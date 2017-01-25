#include "Player.h"

#include "p2Log.h"

#include "j1App.h"
#include "j1Physics.h"
#include "j1Scene.h"
#include "j1Render.h"
#include "j1Textures.h"
#include "j1Input.h"

//Constructors ----------------------------------
j1Player::j1Player()
{
	name.create("player");
}

//Destructors -----------------------------------
j1Player::~j1Player()
{
}

//Game Loop -------------------------------------
bool j1Player::Awake(pugi::xml_node & config)
{
	return true;
}

bool j1Player::Start()
{

	//Generate inital blob body
	body = App->physics->CreateRectangle(50, 200, base_width * level, base_height * level, collision_type::PLAYER, BODY_TYPE::player);
	body->FixedRotation(true);
	body->listener = this;
	CheckLevel();

	//Set bullet size
	bullet_size = level * 4;

	//Load blob animations
	blob_spritesheet = App->tex->Load("textures/Blob_sprites.png");

	//IDLE
	idle.PushBack({ 0,4,19,16 });
	idle.PushBack({ 21,4,18,16 });
	idle.SetSpeed(350);

	//RUN LEFT
	run_left.PushBack({ 0,46,20,16 });
	run_left.PushBack({ 21,46,20,16 });
	run_left.SetSpeed(500);

	//RUN RIGHT
	run_right.PushBack({ 0,25,20,16 });
	run_right.PushBack({ 21,25,20,16 });
	run_right.SetSpeed(500);

	//JUMP
	jump.PushBack({ 44,68,16,15 });
	jump.PushBack({ 65,65,16,18 });
	jump.PushBack({ 86,65,16,15 });
	jump.PushBack({ 107,65,16,11 });
	jump.SetSpeed(300);

	//DODGE
	dodge.PushBack({ 0,67,20,16 });
	dodge.PushBack({ 21,67,20,16 });
	dodge.SetSpeed(350);

	//DIE
	die.PushBack({ 0,89,19,15 });
	die.PushBack({ 21,89,19,15 });
	die.PushBack({ 42,89,19,15 });
	die.PushBack({ 63,89,19,15 });
	die.PushBack({ 84,89,19,15 });
	die.PushBack({ 105,89,19,15 });
	die.PushBack({ 126,89,19,15 });
	die.PushBack({ 147,89,19,15 });
	die.PushBack({ 168,89,19,15 });
	die.SetSpeed(250);
	die.SetLoop(false);

	//Set initial animation
	current_animation = &idle;
	

	return true;
}

bool j1Player::Update(float dt)
{
	if (alive) {
		//Check all the action to set the current animation
		if (!body->IsInContact())
		{
			current_animation = &jump;
		}
		else if (!HandleInput())
		{
			HandleVelocity();
		}
	}
	else if (current_animation->IsEnd())Respawn();

	//Draw player current sprite
	int x, y;
	body->GetPosition(x, y);
	App->render->Blit(blob_spritesheet, x, y, &current_animation->GetCurrentFrame(),level);
	
	return true;
}

bool j1Player::Load(pugi::xml_node &load_node)
{

	return true;
}

bool j1Player::Save(pugi::xml_node &save_node)const
{

	return true;
}

bool j1Player::CleanUp()
{
	LOG("Freeing Player...");

	bool ret = true;
	p2List_item<PhysBody*>* item = bullets_list.end;
	p2List_item<PhysBody*>* item_prev = nullptr;

	if (item != nullptr)item_prev = item->prev;
	while (item) {

		//Delete all item data
		bullets_list.del(item);

		item = item_prev;
		if (item_prev != nullptr)item_prev = item_prev->prev;

	}

	return ret;
}

void j1Player::OnCollision(PhysBody * bodyA, PhysBody * bodyB)
{
	if (bodyA->collide_type == bullet && bodyB->collide_type == player)
	{
		//PickBullet(bodyA);
	}
}

//Functionality -------------------
PhysBody * j1Player::ShootBullet() 
{
	//Calculate the bullet origin
	int x, y;
	body->GetPosition(x, y);
	x += body->width;
	y += body->width;

	//Build the bullet
	PhysBody* bullet = App->physics->CreateCircle(x, y, bullet_size, collision_type::BULLET, BODY_TYPE::bullet);

	//Set as bullet for tunneling
	bullet->body->SetBullet(true);

	//Calculate vector for bullet direction
	float x_vec = (App->input->GetMouseX() - x) * 0.08f;
	float y_vec = (App->input->GetMouseY() - y) * 0.08f;

	//Apply force to th bullet with the calculated vector
	bullet->body->ApplyForce(b2Vec2(x_vec, y_vec), b2Vec2(0, 0), false);
	bullet->listener = this;

	//Add bullet to the list
	bullets_list.add(bullet);

	//Update current bullets
	bullets--;

	return bullet;
}

void j1Player::PickBullet(PhysBody* bullet)
{
	//Add bullet count to the player
	bullets++;
	//Delet body of bullet
	App->physics->DeleteBody(bullet);
	//Delet bullet from bullets list
	bullets_list.del(bullets_list.At(bullets_list.find(bullet)));
}

bool j1Player::CheckLevel()
{
	//Calculate current level
	uint current_level = floor(bullets/bullets_to_evolve);
	
	//Check level
	if (current_level == 0)
	{
		Die();
		return true;
	}
	if (current_level == level)return false;

	//Calculate player location
	int x, y;
	body->GetPosition(x, y);
	x += (base_width * level) * 0.5;
	y += (base_height * level) * 0.5;

	//Delete player body
	App->physics->DeleteBody(body);
	
	//Generate new body with new lvl scale
	body = App->physics->CreateRectangle(x, y, base_width * current_level, base_height * current_level, collision_type::PLAYER, BODY_TYPE::player);
	body->FixedRotation(true);
	//Update level
	level = current_level;
	
	//Update bullet size
	bullet_size = level * 3;

	return false;
}

bool j1Player::HandleInput()
{
	bool ret = false;
	if (App->input->GetKey(SDL_SCANCODE_W) == KEY_DOWN)
	{
		body->body->PlusLinearVelocity(b2Vec2(0, -vertical_acceleration));
		ret = !ret;
	}
	else if (App->input->GetKey(SDL_SCANCODE_A) != KEY_IDLE)
	{
		body->body->PlusLinearVelocity(b2Vec2(-horizontal_acceleration * App->GetDT(), 0.0f));
		current_animation = &run_left;
		ret = !ret;
	}
	else if (App->input->GetKey(SDL_SCANCODE_S) == KEY_REPEAT)
	{
		current_animation = &dodge;
		ret = !ret;
	}
	else if (App->input->GetKey(SDL_SCANCODE_D) != KEY_IDLE)
	{
		body->body->PlusLinearVelocity(b2Vec2(horizontal_acceleration * App->GetDT(), 0.0f));
		current_animation = &run_right;
		ret = !ret;
	}

	if (App->input->GetMouseButtonDown(1) == KEY_DOWN)
	{
		ShootBullet();
		ret = CheckLevel();
	}

	return ret;
}

void j1Player::HandleVelocity()
{
	//Get player current velocity
	b2Vec2 vel = body->body->GetLinearVelocity();
	//Set player animation
	if (vel.x < -0.5f)current_animation = &run_left;
	else if (vel.x > 0.5f)current_animation = &run_right;
	else current_animation = &idle;
}

void j1Player::Die()
{
	LOG("Player dead :(");
	alive = false;
	current_animation = &die;
	current_animation->Reset();
}

void j1Player::Respawn()
{
	LOG("Player Respawned!");
	alive = true;
	bullets = 16;
	CheckLevel();
}
