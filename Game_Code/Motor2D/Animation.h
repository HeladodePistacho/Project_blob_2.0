#ifndef __ANIMATION_H__
#define __ANIMATION_H__

#include "SDL/include/SDL_rect.h"
#include "p2DynArray.h"
#include "j1Timer.h"
#include <math.h>

class Animation
{
private:

	j1Timer					frame_timer;
	float					current_frame = 0;
	int						frames_num = -1;
	p2DynArray<SDL_Rect>	frames;
	uint					speed = 1000;
	bool					loop = true;
	
public:

	//Set functions
	void SetLoop(bool it_loop)
	{
		loop = it_loop;
	}

	void SetSpeed(uint new_speed)
	{
		speed = new_speed;
	}

	void PushBack(const SDL_Rect& rect)
	{
		frames.PushBack(rect);
		frames_num++;
	}

	void Set_frame(uint position)
	{
		if (position > frames_num)return;

		current_frame = position;
	}

	//Get functions
	bool IsEnd()const
	{
		return (current_frame == -1);
	}

	//Loop
	const SDL_Rect& GetCurrentFrame()
	{
		if (current_frame == -1)return {0,0,0,0};

		current_frame = (int)floor(frame_timer.Read() / speed);
		if (current_frame > frames_num)
		{
			if (loop)
			{
				current_frame = 0;
				frame_timer.Start();
			}
			else
			{
				current_frame = -1;
				return{ 0,0,0,0 };
			}
		}

		return frames[(int)current_frame];
		
	}

	void Reset()
	{
		current_frame = 0;
		frame_timer.Start();
	}



};

#endif