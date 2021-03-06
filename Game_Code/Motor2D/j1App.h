#ifndef __j1APP_H__
#define __j1APP_H__

#include "p2List.h"
#include "j1Module.h"
#include "j1PerfTimer.h"
#include "j1Timer.h"
#include "PugiXml\src\pugixml.hpp"
struct SDL_Texture;

// Modules
class j1Window;
class j1Input;
class j1Render;
class j1Textures;
class j1Audio;
class j1FileSystem;
class j1Fonts;
class j1Gui;
class j1Console;
class j1Physics;
class j1Player;
class j1SceneManager;
class j1Scene;

//Game Scenes
class Hub;
class Tutorial_1;
class Level_1;


class j1App
{
public:

	// Constructor
	j1App(int argc, char* args[]);

	// Destructor
	virtual ~j1App();

	// Called before render is available
	bool Awake();

	// Called before the first frame
	bool Start();

	// Called each loop iteration
	bool Update();

	// Called before quitting
	bool CleanUp();

	// Exposing some properties for reading
	int GetArgc() const;
	const char* GetArgv(int index) const;
	const char* GetTitle() const;
	const char* GetOrganization() const;
	float GetDT() const;

	void LoadGame(const char* file);
	void SaveGame(const char* file) const;

	bool IsXMLdir(const char* dir)const;

private:

	// Load config file
	pugi::xml_node LoadConfig(pugi::xml_document&) const;

	// Call modules before each loop iteration
	void PrepareUpdate();

	// Call modules before each loop iteration
	void FinishUpdate();

	// Call modules before each loop iteration
	bool PreUpdate();

	// Call modules on each loop iteration
	bool DoUpdate();

	// Call modules after each loop iteration
	bool PostUpdate();

	// Load / Save
	bool LoadGameNow();
	bool SavegameNow() const;

public:

	// Modules
	j1Window*			win = NULL;
	j1Input*			input = NULL;
	j1Render*			render = NULL;
	j1Textures*			tex = NULL;
	j1Audio*			audio = NULL;
	j1Physics*			physics = NULL;
	j1FileSystem*		fs = NULL;
	j1Fonts*			font = NULL;
	j1Gui*				gui = NULL;
	j1Console*			console = NULL;
	j1Player*			player = NULL;
	j1SceneManager*		scene_manager = NULL;
	
	Hub*				hub = NULL;
	Tutorial_1*			tutorial_1 = NULL;
	Level_1*			level_1 = NULL;


	// Add a new module to handle
	void		AddModule(j1Module* module);
	//Get Module pointer by name
	j1Module*	GetModule(const p2SString* module_name)const;
	//Get number of modules
	uint		GetModulesNum()const;
	//Get module of position x in array
	j1Module*	GetModuleAt(uint index)const;

public:

	//Handle Console Input ----------------------
	void Console_Command_Input(Command* command, Cvar* cvar, p2SString* input);
	void Console_Cvar_Input(Cvar* cvar, Command* command_type, p2SString* input);

private:

	p2List<j1Module*>	modules;
	p2List<j1Module*>	scenes;

	int					argc;
	char**				args;

	p2SString			title;
	p2SString			organization;

	mutable bool		want_to_save = false;
	bool				want_to_load = false;
	mutable p2SString	game_save_dir;

	j1PerfTimer			ptimer;
	uint64				frame_count = 0;
	j1Timer				startup_time;
	j1Timer				frame_time;
	j1Timer				last_sec_frame_time;
	uint32				last_sec_frame_count = 0;
	uint32				prev_last_sec_frame_count = 0;
	float				dt = 0.0f;
	int					capped_ms = -1;

	bool				want_to_quit = false;

public:

	void SetQuit();

	const char*	GetSaveDir()const;
	const char*	GetLoadDir()const;

	SDL_Texture* Platforms_Spritesheet = nullptr;
	SDL_Texture* Items_Spritesheet = nullptr;
	SDL_Texture* WinBlobs_Spritesheet = nullptr;

	j1Module*	GetNextScene(j1Module* current_scene);
	j1Module*	current_scene;
};

extern j1App* App;

#endif