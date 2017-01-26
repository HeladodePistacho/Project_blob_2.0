#include "Platform.h"

Platform::Platform(SDL_Rect texture, PLATFORM_TYPE TYPE): Item(texture,PLATFORM),platform_type(TYPE)
{
}

Platform::~Platform()
{
}
