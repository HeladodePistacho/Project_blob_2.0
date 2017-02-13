#ifndef _SCENEITEM_H_
#define _SCENEITEM_H_

#include "p2Defs.h"
#include "SDL\include\SDL_rect.h"

struct PhysBody;

enum ITEM_TYPE
{
	//Balls
	BALL_GREEN,
	BALL_YELLOW,
	BALL_RED,
	BALL_ORANGE,
	BALL_BLUE,
	//Boxes
	BOX_BOOKS,
	BOX_XMAS,
	BOX_SNES,
	BOX_NUKE,
	BOX_LARGE_XMAS,
	//Tables
	TABLE_STANDAR,
	TABLE_SHORT,
	//Drawers
	DRAWER_LEFT,
	DRAWER_LEFT_SEG,
	DRAWER_RIGHT_SEG,
	DRAWER_CENTER,
	DRAWER_SHORT_CLOSE,
	DRAWER_LARGE_CLOSE,
	//Other items
	CHAIR,
	CAMPFIRE,
	TUPPER,
	BLUE_TANK_PERFIL,
	BLUE_TANK_FRONT,
	BALANCE,
	
};

class Item
{
public:

	Item(SDL_Rect new_tex, ITEM_TYPE TYPE, uint scale = 1);
	~Item();

private:

	PhysBody*		body;
	SDL_Rect		texture;
	int				tex_x_margin = 0;
	int				tex_y_margin = 0;
	uint			scale;
	ITEM_TYPE		item_type;

public:

	//Functionality --------------
	void			SetPosition(int x, int y);
	void			SetBody(PhysBody* new_body);
	void			SetTextMargins(int x, int y);

	int				GetTex_X_margin()const;
	int				GetTex_Y_margin()const;
	void			GetPosition(int& x, int& y)const;
	float			GetRotation()const;
	PhysBody*		Get_body() const;
	ITEM_TYPE		Get_type() const;
	SDL_Rect		Get_Texture() const;
	int				Get_Scale()const;
	
};

#endif // !_SCENEITEM_H_

