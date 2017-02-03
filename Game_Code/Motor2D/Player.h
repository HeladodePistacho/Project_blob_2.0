#ifndef _PLAYER_
#define _PLAYER_

#include "j1Module.h"
#include "p2List.h"
#include "Animation.h"
#include "j1Timer.h"

struct PhysBody;
struct SDL_Texture;
enum BODY_TYPE;

///Player Bullet class ---------------------------
class Bullet
{
public:
	
	Bullet(int x, int y, uint bullet_size, uint scale = 1);
	~Bullet();

private:

	PhysBody*	body;
	Animation	anim;
	uint		scale;
	j1Timer		live_time;

public:

	PhysBody*			GetBody()const;
	const SDL_Rect&		GetCurrentAnimationRect();
	uint				GetScale()const;
	uint				GetLiveTime()const;

}; //_BULLET_
/// ----------------------------------------------

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

	//Player State ------------------------------
	bool				alive = true;
	bool				in_air = false;
	j1Timer				regen_timer;
	uint				regen_rate = 250;
	j1Timer				bleed_timer;
	uint				bleed_rate = 250;

	//Player PhysBodys --------------------------
	PhysBody*			body = nullptr;
	PhysBody*			mouth = nullptr;
	//Player sizes data -------------------------
	uint				level = 4;
	uint				evolve_size = 10;
	uint				base_width = 18;
	uint				base_height = 15;
	//Player velocities data --------------------
	float				horizontal_acceleration = 3.0f;
	float				horizontal_speed_limit = 25.0f;
	float				vertical_acceleration = 3.5f;

	//Bullets data ------------------------------
	p2List<Bullet*>		bullets_list;
	uint				bullets = 16;
	uint				bullets_to_evolve = 3;
	uint				bullet_size = 3;
	float				bullet_force = 0.8f;
	uint				bullet_active_delay = 850;

	//PLayer animations -------------------------
	SDL_Texture*		blob_spritesheet = nullptr;
	Animation*			current_animation = nullptr;
	Animation			idle;
	Animation			run_right;
	Animation			run_left;
	Animation			jump_end;
	Animation			dodge;
	Animation			die;

public:

	//Bullets Functionality -----------
	Bullet*		ShootBullet();
	void		PickBullet(Bullet* bullet);
	void		DeleteBullet(Bullet* bullet_body);
	Bullet*		FindBullet(PhysBody* bullet)const;

	//Level Functionality -------------
	bool		CheckLevel();

	//Handle Data Functionality -------
	bool		HandleInput();
	void		HandleVelocity();

	//Player state loop Functionality -
	void		Die();
	void		Respawn();
	void		Regenerate();
	void		Bleed();
	void		Impact();

	//Get Methods ---------------------
	float		GetVerticalAcceleration()const;

	//Set Methods ---------------------
	void		SetPosition(int x = 0, int y = 0);
};

#endif // _PLAYER_
