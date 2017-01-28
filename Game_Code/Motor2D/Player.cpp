#include "Player.h"

#include "p2Log.h"

#include "j1App.h"
#include "j1Physics.h"
#include "j1Scene.h"
#include "j1Render.h"
#include "j1Textures.h"
#include "j1Input.h"

///BULLET CLASS ----------------------------------
//Constructor
Bullet::Bullet(int x, int y, uint bullet_size, uint scale): scale(scale)
{
	anim.PushBack({ 114,2,7,7 });
	anim.PushBack({ 124,2,7,8 });
	anim.PushBack({ 114,2,7,7 });
	anim.PushBack({ 145,2,8,7 });
	anim.SetSpeed(250);

	body = App->physics->CreateCircle(x, y, bullet_size* scale, collision_type::BULLET, BODY_TYPE::bullet, 1);
	body->body->SetBullet(true);
	body->listener = App->player;
	body->body->SetFixedRotation(true);

	live_time.Start();
}
//Destructor
Bullet::~Bullet()
{
}

PhysBody * Bullet::GetBody() const
{
	return body;
}

const SDL_Rect & Bullet::GetCurrentAnimationRect()
{
	return anim.GetCurrentFrame();
}

uint Bullet::GetScale() const
{
	return scale;
}
uint Bullet::GetLiveTime() const
{
	return live_time.Read();
}
/// ----------------------------------------------


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
	//Generate initial blob mouth
	mouth = App->physics->CreateRectangle(50, 220,25,25, collision_type::PLAYER_MOUTH, BODY_TYPE::player_mouth);
	mouth->FixedRotation(true);
	App->physics->CreateRevoluteJoint(body,mouth);
	
	b2WeldJointDef weld_joint;
	weld_joint.bodyA = body->body;
	weld_joint.bodyB = mouth->body;
	//Update level from number of bullets
	CheckLevel();

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
	int x, y;
	body->GetPosition(x, y);

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
	App->render->Blit(blob_spritesheet, x, y, &current_animation->GetCurrentFrame(),level);
	
	//Draw player bullets
	p2List_item<Bullet*>* item = bullets_list.start;
	while (item)
	{
		//Calculate bullet texture coordinates
		int x, y;
		item->data->GetBody()->GetPosition(x, y);
		x -= floor(item->data->GetBody()->GetWidth()  * 0.25f);
		y -= floor(item->data->GetBody()->GetHeight() * 0.25f);

		//Blit bullet 
		App->render->Blit(blob_spritesheet, x, y, &item->data->GetCurrentAnimationRect(), item->data->GetScale());

		item = item->next;
	}

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
	p2List_item<Bullet*>* item = bullets_list.end;
	p2List_item<Bullet*>* item_prev = nullptr;

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
		p2List_item<Bullet*>* item = bullets_list.start;
		while (item)
		{
			if (item->data->GetBody() == bodyA && item->data->GetLiveTime() > bullet_active_delay)
			{
				PickBullet(item->data);
				break;
			}
			item = item->next;
		}
	}

}

//Functionality -------------------
Bullet * j1Player::ShootBullet() 
{
	//Calculate the bullet origin
	int x, y;
	body->GetPosition(x, y);
	x += body->width;
	y += body->width;

	//Build the bullet
	Bullet* new_bullet = new Bullet(x, y, bullet_size, level);
	bullets_list.add(new_bullet);

	//Calculate vector for bullet direction
	//Build vector from mouse coordinates
	float x_vec = (App->input->GetMouseX() - x);
	float y_vec = (App->input->GetMouseY() - y);
	b2Vec2 force_vector = { x_vec,y_vec };
	//Get vector length
	float length = force_vector.Length();
	//Normalize vector
	force_vector.Normalize();
	//Calculate new vector length in relation the bullet mass and player shoot force
	force_vector *= (length * new_bullet->GetBody()->body->GetMass()) * bullet_force;

	//Apply force to th bullet with the calculated vector
	new_bullet->GetBody()->body->ApplyForceToCenter(force_vector, false);

	//Update current bullets
	bullets--;

	return new_bullet;
}

void j1Player::PickBullet(Bullet* bullet)
{
	//Add bullet count to the player
	bullets++;
	
	//Delet body of bullet
	App->physics->DeleteBody(bullet->GetBody());
	//Delet bullet from bullets list
	bullets_list.del(bullets_list.At(bullets_list.find(bullet)));

	//Check player level
	CheckLevel();
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