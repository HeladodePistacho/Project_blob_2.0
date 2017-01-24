#ifndef __ANIMATION_H__
#define __ANIMATION_H__

#include "SDL/include/SDL_rect.h"
#include "p2DynArray.h"
#include "j1Timer.h"

class Animation
{
private:

	j1Timer					frame_timer;
	float					current_frame = 0;
	int						frames_num = -1;
	p2DynArray<SDL_Rect>	frames;
	uint					speed = 1000;

public:

	void SetSpeed(uint new_speed)
	{
		speed = new_speed;
	}

	void PushBack(const SDL_Rect& rect)
	{
		frames.PushBack(rect);
		frames_num++;
	}

	const SDL_Rect& GetCurrentFrame()
	{
		current_frame = (int)floor(frame_timer.Read() / speed);
		if (current_frame > frames_num)
		{
			current_frame = 0;
			frame_timer.Start();
		}

		return frames[(int)current_frame];
	}

	void Reset()
	{
		current_frame = 0;
	}

	void Set_frame(uint position)
	{
		if (position > frames_num)return;

		current_frame = position;
	}

};

#endif