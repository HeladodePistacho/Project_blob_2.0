#ifndef _PLAYER_
#define _PLAYER_

#include "j1Module.h"
#include "p2List.h"
#include "Animation.h"

struct PhysBody;
struct SDL_Texture;

enum PLAYER_INPUT
{
	INVALID_KEY,
	W_KEY,
	A_KEY,
	S_KEY,
	D_KEY
};

class j1Player :public j1Module
{
public:

	j1Player();
	~j1Player();

	// Called before render is available
	bool Awake(pugi::xml_node& config);

	bool Start();

	bool Update(float dt);

	bool Load(pugi::xml_node&);

	bool Save(pugi::xml_node&)const;

	bool CleanUp();

	void OnCollision(PhysBody* bodyA, PhysBody* bodyB);

private:

	//Player Body
	PhysBody*			body = nullptr;
	uint				level = 4;
	uint				evolve_size = 10;
	uint				base_width = 18;
	uint				base_height = 16;
	float				horizontal_force = 5.0f;
	float				vertical_force = 18.0f;

	//Bullets data
	p2List<PhysBody*>	bullets_list;
	uint				bullets = 16;
	uint				bullets_to_evolve = 3;
	uint				bullet_size = 0;

	//PLayer animations
	SDL_Texture*		blob_spritesheet = nullptr;
	Animation*			current_animation = nullptr;
	Animation			idle;
	Animation			run_right;
	Animation			run_left;
	Animation			jump;

public:

	//Functionality -------------------
	PhysBody*	ShootBullet();
	void		PickBullet(PhysBody* bullet);
	
	void		CheckLevel();

	void		HandleInput(PLAYER_INPUT input);
};

#endif // _PLAYER_
