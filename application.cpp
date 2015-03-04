//////////////////////////////////////////////////////////////////////////////
// Tycho Game Library
// Copyright (C) 2008 Martin Slater
// Created : Sunday, 30 March 2008 10:04:22 PM
//////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////
// INCLUDES
//////////////////////////////////////////////////////////////////////////////
#include "application.h"
#include "core/globals.h"
#include "engine/engine.h"
#include "core/core.h"
#include "core/debug/debuggee.h"
#include "core/program_config.h"
#include "core/console.h"


extern int platform_run();
extern void platform_cleanup();

//////////////////////////////////////////////////////////////////////////////
// CLASS
//////////////////////////////////////////////////////////////////////////////
namespace tc = tycho::core;
namespace te = tycho::engine;

namespace test_game
{

	/// constructor
	application::application()
	{	
	}
	
	/// destructor
	application::~application()
	{	
	}
	
	/// called the very first thing after main
	int application::initialise()
	{
		// initialise subsystems
		tycho::core::initialise();
		tycho::engine::initialise();
		tc::console::initialise("test_game");

		typedef tc::program_config pc;
		// get config options
		bool show_help = false;
		static pc::option_pair options[] = {
			pc::opt("help", 'h', "display this help text", "0", &show_help),
			pc::null_opt()
		};						
		tc::globals::get_config()->process_option_group("game", "Game options", options);				
		
		if(show_help)
		{
			tc::globals::get_config()->print_help();
			tc::console::write_ln("\npress any key to exit...");
			tc::console::wait_until_keypress();
			return false;
		}	
		return true;
	}
	
	/// called last thing before main is exited
	int application::shutdown()
	{
		// ordered shutdown
		te::shutdown();
		tc::console::shutdown();
		tc::shutdown();
		platform_cleanup();	
		return 0;
	}
	
	/// main run loop, this doesn't terminate until the user quits
	int application::run()
	{				
		initialise_game();
		bool running = true;
		while(running)
		{
			// let the underlying platform have an update, i.e. process messages on windows
			if(!platform_run())
			{
				// platform wants us to quit
				break;
			}
			
			// let debugger process any messages			
			tycho::core::globals::get_debuggee()->run();

			// run any queued completion handlers
			
			// let network dispatch any updates
			
			// dispatch queued events
			

			// let game update 
			if(!update_frame())			
			{
				// game wants us to quit
				break;
			}
			
		}		
		shutdown_game();			
		return 0;
	}

	/// called at startup to initialise the game
	void application::initialise_game()
	{
		// process configuration options
		
		// create the renderer
// 		tycho::graphics::renderer *m_renderer;
// 		m_renderer = object::create_object("tycho.engine.graphics.pc.renderer", "test_game.renderer");
// 		m_renderer = tycho::graphics::renderer::create("global.renderer");
		
		// load the main menu UI
		
		// set currently handler to the main menu
		
		// boom, we're up and running
		
	}
		

} // end namespace
