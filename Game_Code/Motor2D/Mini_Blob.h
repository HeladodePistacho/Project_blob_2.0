#ifndef _MINI_BLOB_
#define _MINI_BLOB_

#include "p2Defs.h"

#include "Animation.h"
struct PhysBody;

enum BLOB_TYPE
{
	BLOB_GREEN,
	BLOB_YELLOW,
	BLOB_RED,
	BLOB_ORANGE,
	BLOB_BLUE
};

class Mini_Blob
{
public:

	Mini_Blob(BLOB_TYPE TYPE, uint scale = 1);
	~Mini_Blob();

private:

	BLOB_TYPE	type;
	PhysBody*	body;
	uint		scale;

	Animation*	current_animation;
	Animation	fear_anim;
	Animation	happy_anim;

public:

	//Functionality -------------------
	void		SetHappy();
	void		SetFear();
	void		SetPosition(int x, int y);
	void		SetBody(PhysBody* new_body);

	PhysBody*	GetBody()const;
	void		GetPosition(int& x, int& y);
	uint		GetScale()const;
	BLOB_TYPE*	GetType();
	Animation*	GetHappyAnim();
	Animation*	GetFearAnim();
	SDL_Rect	GetCurrentAnimRect();
	bool		IsHappy()const;

};
#endif // _MINI_BLOB_
