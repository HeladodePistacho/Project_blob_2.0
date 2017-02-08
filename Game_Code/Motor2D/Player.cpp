#define _CRT_SECURE_NO_WARNINGS
#include "Player.h"

#include "p2Log.h"

#include "j1App.h"
#include "j1Physics.h"
#include "j1Scene.h"
#include "j1Render.h"
#include "j1Textures.h"
#include "j1Input.h"
#include "j1Scene.h"

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
	mouth = App->physics->CreateRectangleSensor(50, 200,body->GetWidth(), body->GetHeight(), collision_type::PLAYER_MOUTH, BODY_TYPE::player_mouth);
	mouth->FixedRotation(true);

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
	jump_end.PushBack({ 0,4,19,16 });
	jump_end.PushBack({ 49,66,20,16 });
	jump_end.PushBack({ 70,66,20,16 });
	jump_end.PushBack({ 91,66,20,16 });
	jump_end.PushBack({ 70,66,20,16 });
	jump_end.PushBack({ 49,66,20,16 });
	jump_end.PushBack({ 0,4,19,16 });
	jump_end.SetSpeed(100);
	jump_end.SetLoop(false);

	//DODGE
	dodge.PushBack({ 0,68,20,15 });
	dodge.PushBack({ 21,68,20,15 });
	dodge.SetSpeed(350);

	//DIE
	die.PushBack({ 0,88,19,16 });
	die.PushBack({ 21,88,19,16 });
	die.PushBack({ 42,88,19,16 });
	die.PushBack({ 63,88,19,16 });
	die.PushBack({ 84,88,19,16 });
	die.PushBack({ 105,88,19,16 });
	die.PushBack({ 126,88,19,16 });
	die.PushBack({ 147,88,19,16 });
	die.PushBack({ 168,88,19,16 });
	die.SetSpeed(250);
	die.SetLoop(false);

	//Set initial animation
	current_animation = &idle;
	
	//Start player timers
	regen_timer.Start();
	bleed_timer.Start();

	return true;
}

bool j1Player::Update(float dt)
{
	//Get Player data -------------------------------------
	int x, y;
	body->GetPosition(x, y);

	//Player actions --------------------------------------
	if (alive) {
		//Check all the action to set the current animation
		if (!body->IsInContact())in_air = true;
		if (current_animation->IsEnd() && !HandleInput()) HandleVelocity();
	}
	else if (current_animation->IsEnd())
	{
		((j1Scene*)App->current_scene)->Reset();
	}
	// ----------------------------------------------------

	//Player Blits ----------------------------------------
	//Draw player current sprite
	App->render->Blit(blob_spritesheet, x - (level * 0.5f), y, &current_animation->GetCurrentFrame(),level);
	
	//Draw player bullets
	p2List_item<Bullet*>* item = bullets_list.start;
	while (item)
	{
		//Calculate bullet texture coordinates
		int x, y;
		item->data->GetBody()->GetPosition(x, y);
		x -= (int)floor(item->data->GetBody()->GetWidth()  * 0.25f);
		y -= (int)floor(item->data->GetBody()->GetHeight() * 0.25f);

		//Blit bullet 
		App->render->Blit(blob_spritesheet, x, y, &item->data->GetCurrentAnimationRect(), item->data->GetScale());

		item = item->next;
	}
	// ----------------------------------------------------


	//Update mouth coordinates at the center of player body
	b2Vec2 world_player_center = body->body->GetWorldCenter();
	mouth->body->SetTransform(world_player_center,0);


	return true;
}

bool j1Player::Load(pugi::xml_node& load_node)
{
	//Focus Completed scenes name node
	pugi::xml_node name_node = load_node.child("scene_name").first_child();
	//Focus Completed scenes blob node
	pugi::xml_node blob_node = load_node.child("scene_blobs").first_child();

	while (name_node && blob_node)
	{
		//Add names to player 
		p2SString* name = new p2SString(name_node.value());
		completed_names.PushBack(name);
		//Add blobs to player
		
		SDL_Rect* rect = new SDL_Rect();

		rect->x = blob_node.attribute("x").as_int();
		rect->y = blob_node.attribute("y").as_int();
		rect->w = blob_node.attribute("w").as_int();
		rect->h = blob_node.attribute("h").as_int();
		completed_blobs.PushBack(rect);

		name_node = name_node.next_sibling();
		blob_node = blob_node.next_sibling();
	}
	return true;
}

bool j1Player::Save(pugi::xml_node& save_node)const
{
	//Append child to save completed scenes names
	pugi::xml_node names_node = save_node.append_child("scene_names");
	//Append child to save completed scenes blobs
	pugi::xml_node blobs_node = save_node.append_child("scene_blobs");

	uint scenes_num = completed_names.Count();
	for (uint k = 0; k < scenes_num; k++)
	{
		//Append Scene names
		names_node.append_child("name").append_attribute(completed_names[k]->GetString()).as_string();
		//Append Scene blobs rect
		pugi::xml_node current_blob = blobs_node.append_child("blob");
		current_blob.append_attribute("x").as_int(completed_blobs[k]->x);
		current_blob.append_attribute("y").as_int(completed_blobs[k]->y);
		current_blob.append_attribute("w").as_int(completed_blobs[k]->w);
		current_blob.append_attribute("h").as_int(completed_blobs[k]->h);
	}
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

	completed_blobs.Delete_All();
	completed_names.Delete_All();

	return ret;
}

void j1Player::OnCollision(PhysBody * bodyA, PhysBody * bodyB)
{

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
	CheckLevel();

	return new_bullet;
}

void j1Player::PickBullet(Bullet* bullet)
{
	if (bullet == nullptr || bullet->GetLiveTime() < bullet_active_delay)return;

	//Add bullet count to the player
	bullets++;

	//Delete bullet
	DeleteBullet(bullet);

	//Update player level
	CheckLevel();
}

void j1Player::DeleteBullet(Bullet* bullet)
{
	if (bullet == nullptr)return;

	//Delet bullet from bullets list
	bullets_list.del(bullets_list.At(bullets_list.find(bullet)));

	//Delet body of bullet
	App->physics->DeleteBody(bullet->GetBody());
}

Bullet * j1Player::FindBullet(PhysBody * bullet) const
{
	p2List_item<Bullet*>* item = bullets_list.start;
	while (item)
	{
		if (item->data->GetBody() == bullet)
		{
			return item->data;
		}
		item = item->next;
	}
	return nullptr;
}

bool j1Player::CheckLevel()
{
	//Calculate current level
	uint current_level = floor(bullets/bullets_to_evolve);
	
	//Check level
	if (current_level == level)return false;
	else if (current_level == 0)
	{
		Die();
		return true;
	}

	//Calculate player location
	int x, y;
	body->GetPosition(x, y);
	x += (int)((base_width * level)  * 0.5);
	y += (int)((base_height * level) * 0.5);
	//Get player velocity
	b2Vec2 player_velocity = body->body->GetLinearVelocity();
	
	//Delete player bodys
	App->physics->DeleteBody(body);
	App->physics->DeleteBody(mouth);

	//Generate new body with new lvl scale
	body = App->physics->CreateRectangle(x, y, base_width * current_level, base_height * current_level, collision_type::PLAYER, BODY_TYPE::player);
	body->FixedRotation(true);
	body->body->SetLinearVelocity(player_velocity);
	//Generate new mouth with new body size
	mouth = App->physics->CreateRectangleSensor(x, y, body->GetWidth(), body->GetHeight(), collision_type::PLAYER_MOUTH, BODY_TYPE::player_mouth);
	mouth->FixedRotation(true);

	//Update level
	level = current_level;

	return false;
}

void j1Player::AddSceneCompleted(j1Scene* completed_scene)
{
	/*completed_names.PushBack(&completed_scene->name);
	completed_blobs.PushBack(completed_scene->GetBlob()->GetType());
	App->SaveGame(App->GetSaveDir());*/
}

bool j1Player::HandleInput()
{
	bool ret = false;
	if (App->input->GetKey(SDL_SCANCODE_W) == KEY_DOWN && !in_air)
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
		ret = true;
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
	if (!alive)return;

	LOG("Player dead :(");
	alive = false;
	current_animation = &die;
	current_animation->Reset();
}

void j1Player::Respawn()
{
	//Delete all bullets free in scene
	p2List_item<Bullet*>* item = bullets_list.start;
	while (item)
	{
		App->physics->DeleteBody(item->data->GetBody());
		item = item->next;
	}
	bullets_list.clear();

	//Reset Player stats
	bullets = 16;
	CheckLevel();
	body->body->SetLinearVelocity(b2Vec2(0, 0));
	int x, y;
	((j1Scene*)App->current_scene)->GetPlayerSpawn(x, y);
	body->SetPosition(x, y);
	body->body->SetAwake(true);
	alive = true;
	current_animation = &idle;

	LOG("Player Respawned!");
}

void j1Player::Regenerate()
{
	if (regen_timer.Read() > regen_rate)
	{
		bullets++;
		CheckLevel();
		regen_timer.Start();
	}
}

void j1Player::Bleed()
{
	if (bleed_timer.Read() > bleed_rate)
	{
		bullets--;
		CheckLevel();
		bleed_timer.Start();
	}
}

void j1Player::Impact()
{
	if (!in_air)return;

	current_animation = &jump_end;
	current_animation->Reset();
	in_air = false;
}

float j1Player::GetVerticalAcceleration() const
{
	return vertical_acceleration;
}

void j1Player::SetPosition(int x, int y)
{
	body->SetPosition(x, y);
}
