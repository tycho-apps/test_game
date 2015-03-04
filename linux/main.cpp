//////////////////////////////////////////////////////////////////////////////
// Tycho Game Library
// Copyright (C) 2008 Martin Slater
// Created : Sunday, 30 March 2008 1:03:31 PM
//////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////
// INCLUDES
//////////////////////////////////////////////////////////////////////////////
#include "../test_game_abi.h"
#include "../test_game.h"
#include "engine/engine.h"
#include "core/core.h"
//#include <SDL/SDL.h>

test_game::game  g_game;
//static bool		 g_quitting = false;

bool platform_run()
{
    return true;
}

void platform_cleanup()
{
  
}

int main(int argc, char *argv[])
{
	// initialise basic tycho subsystems
	tycho::core::initialise();

	// initialise tycho engine subsystems
	tycho::engine::initialise();
	
#if 0
	// create screen
    if(SDL_Init(SDL_INIT_VIDEO) < 0) 
	{
        fprintf(stderr, "Unable to init SDL: %s\n", SDL_GetError());
        exit(1);
    }
	SDL_Surface *screen;
    screen = SDL_SetVideoMode(640, 480, 16, SDL_SWSURFACE);
    if(screen == NULL) 
	{
        fprintf(stderr, "Unable to set 640x480 video: %s\n", SDL_GetError());
        exit(1);
    }

#endif // 0
    
	// enter main game initialisation, this will not exit until game quit time
	g_game.run();

	// all done
	int res = g_game.shutdown();
#if 0
	SDL_Quit();
#endif // 0
	return res;
}	 
