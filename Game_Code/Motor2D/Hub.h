#ifndef _HUB_
#define _HUB_

#include "j1Scene.h"
#include "p2DynArray.h"
struct UI_Button;
struct UI_Image;
struct UI_String;
struct _TTF_Font;

class Hub : public j1Scene
{
public:
	
	Hub();
	~Hub();

	// Called before render is available
	bool Awake(pugi::xml_node& config);

	bool Start();

	bool Update(float dt);

	bool Load(pugi::xml_node& load_node);

	bool Save(pugi::xml_node& save_node)const;

	bool CleanUp();

private:

	UI_Button*	start_button;
	UI_Button*	quit_button;

	p2DynArray<UI_Image*>	completed_blob;
	p2DynArray<UI_String*>	completed_name;

	_TTF_Font*	levels_font = nullptr;

public:

	//Functionality -------------------
	void	AddBlob(Mini_Blob* new_blob);
	void	AddSceneName(char* name);

};
#endif // !_HUB_
