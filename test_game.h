//////////////////////////////////////////////////////////////////////////////
// Tycho Game Library
// Copyright (C) 2008 Martin Slater
// Created : Monday, 31 March 2008 9:42:03 PM
//////////////////////////////////////////////////////////////////////////////
#if _MSC_VER > 1000
#pragma once
#endif  // _MSC_VER

#ifndef __TEST_GAME_H_32A1DFE2_06A5_49CA_B465_6734B7D64FF3_
#define __TEST_GAME_H_32A1DFE2_06A5_49CA_B465_6734B7D64FF3_

//////////////////////////////////////////////////////////////////////////////
// INCLUDES
//////////////////////////////////////////////////////////////////////////////
#include "test_game_abi.h"
#include "input/interface.h"
#include "graphics/renderer.h"
#include "graphics/shader/forward_decls.h"
#include "application.h"
#include "states/game_state.h"
#include "states/main_menu.h"
#include "states/play_game.h"
#include "io/interface.h"
#include "input/interface.h"
#include <vector>

//////////////////////////////////////////////////////////////////////////////
// CLASS
//////////////////////////////////////////////////////////////////////////////

namespace test_game
{
    class game : public application
    {
	public:    
		game();
		
		/// called at startup to initialise the game
		virtual void initialise_game();    

		/// called when user has requested termination
		virtual void shutdown_game();    
			    
		/// per frame update
		virtual int update_frame();		
		
		void start_game(int num_players);
		void quit_game();
		void show_main_menu();
		void show_pause_menu();		
		
		tycho::graphics::renderer_ptr  get_renderer() 
			{  return m_renderer; }
			
		tycho::graphics::window_ptr get_main_window()
			{ return m_main_window; }
		
	private:
		/// per platform game init
		void platform_initialise();
		
		/// per platform game shutdown
		void platform_shutdown();
		
		/// initialise the game world
		void create_main_menu();

		void set_next_state(game_state*);
		
    private:
        tycho::input::interface                 m_input;
        tycho::io::interface                    m_io;
		tycho::graphics::renderer_ptr   m_renderer;
		tycho::graphics::window_ptr		m_main_window;
		game_state* m_cur_state;
		game_state* m_next_state;
		main_menu* m_main_menu_state;
		play_game* m_play_game_state;
		bool m_quit;
    };
    
} // end namespace

#endif // __TEST_GAME_H_32A1DFE2_06A5_49CA_B465_6734B7D64FF3_
