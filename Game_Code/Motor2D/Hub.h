#ifndef _HUB_
#define _HUB_

#include "j1Module.h"
#include "p2DynArray.h"
struct UI_Button;
struct UI_Image;
struct UI_String;
struct _TTF_Font;
struct Mini_Blob;

class Hub : public j1Module
{
public:
	
	Hub();
	~Hub();

	// Called before render is available
	bool Awake(pugi::xml_node& config);

	bool Start();

	bool Update(float dt);

	bool CleanUp();

	void GUI_Input(UI_Element* target, GUI_INPUT input);

	//Active/Deasctive module
	void Activate();
	void Desactivate();

private:

	UI_Element*		ui = nullptr;
	UI_Button*		start_button = nullptr;
	UI_Button*		exit_button = nullptr;

	p2DynArray<UI_Image*>	completed_blob;
	p2DynArray<UI_String*>	completed_name;

	_TTF_Font*	levels_font = nullptr;

public:

	//Functionality -------------------
	void	AddBlob(Mini_Blob* new_blob);
	void	AddSceneName(char* name);

};
#endif // !_HUB_
