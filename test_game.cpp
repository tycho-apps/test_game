//////////////////////////////////////////////////////////////////////////////
// Tycho Game Library
// Copyright (C) 2008 Martin Slater
// Created : Monday, 31 March 2008 9:42:03 PM
//////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////
// INCLUDES
//////////////////////////////////////////////////////////////////////////////
#include "test_game.h"
#include "graphics/window.h"
#include "input/types.h"
#include "core/debug/assert.h"
#include "core/memory.h"
#include "core/console.h"
#include "core/globals.h"
#include "io/interface.h"
#include "engine/globals.h"
#include "math/matrix4x4.h"
#include "math/constants.h"

// game states
#include "states/main_menu.h"
#include "states/play_game.h"


//TEMP
#include "graphics/text/font_manager.h"

//////////////////////////////////////////////////////////////////////////////
// CLASS
//////////////////////////////////////////////////////////////////////////////
using namespace tycho;
using namespace tycho::engine;
using namespace tycho::engine::entity;
using namespace tycho::graphics;
using namespace tycho::graphics::shader;

namespace test_game
{	
	
	 
	game::game() :
		m_cur_state(0),
		m_next_state(0),
		m_main_menu_state(new main_menu(&m_input, &m_io, this)),
		m_play_game_state(new play_game(&m_input, &m_io, this)),
		m_quit(false)
	{
	}

	void game::set_next_state(game_state* s)
	{
		TYCHO_ASSERT(s);
		TYCHO_ASSERT(!m_next_state);
		m_next_state = s;
	}
			
	/// called at startup to initialise the game
	void game::initialise_game()
	{		
		// create the renderer
		platform_initialise();	
		TYCHO_ASSERT(m_renderer);
				
		// setup default key bindings for player 0
		for(size_t i = 0; i < m_input.get_devices().size(); ++i)
		{
			const input::device_description& desc = m_input.get_devices()[i];
			core::console::write_ln("Found Input Device : %s(%d)", desc.name, desc.index);
		}
		// get controllers 
		const input::device_description* controllers[8] = { 0 };
		m_input.enumerate_controllers(controllers, 8);

		// player input bindings
		{
			static input::binding bindings[] = {
				{ "player.turn_y", input::make_axis_input(input::axis_rthumb_y) },
				{ "player.turn_x", input::make_axis_input(input::axis_rthumb_x) },
				{ "player.move", input::make_axis_input(input::axis_lthumb_y) },
				{ "player.strafe", input::make_axis_input(input::axis_lthumb_x) },
				{ "player.fire", input::make_keyboard_input(input::key_button_a, input::key_state_down) },
				{ 0 }
				};
			m_input.register_bindings("player", bindings);								
		}
		
		// ui::panel input bindings
		{
			static input::binding bindings[] = {
				{ "ui.panel.highlight_next", input::make_keyboard_input(input::key_button_dpad_down, input::key_state_down) },
				{ "ui.panel.highlight_prev", input::make_keyboard_input(input::key_button_dpad_up, input::key_state_down) },
				{ "ui.panel.select", input::make_keyboard_input(input::key_button_a, input::key_state_down) },
				{ 0 }
				};
			m_input.register_bindings("ui.panel", bindings);								
		}
		
		// game input bindings
		{		
			static input::binding bindings[] = {
				{ "game.show_pause_menu", input::make_keyboard_input(input::key_button_start, input::key_state_down) },
				{ 0 }
				};
			m_input.register_bindings("game", bindings);								
		}

		if(controllers[0])
			m_input.bind_device(0, controllers[0]->id);
		
		//TEMP 
		show_main_menu();
		//start_game(1);
	}
		    
	/// called when user has requested termination
	void game::shutdown_game()
	{		
		if(m_cur_state)
			m_cur_state->leave();
		core::safe_delete(m_main_menu_state);			
		core::safe_delete(m_play_game_state);
		m_cur_state = m_next_state = 0;
		m_main_window = window_ptr();
		m_renderer->destroy();
		m_renderer.reset();		
	}
		    
	/// per frame update
	int game::update_frame()
	{
		using namespace math;
				
		if(m_quit)
		{
			return 0;
		}
			
		// switch states before doing anything else
		if(m_next_state)
		{			
			// sync rendering so all outstanding buffers are flushed so all resources used by the renderer are freed
			m_renderer->sync();
			if(m_cur_state)
				m_cur_state->leave();
			m_cur_state = m_next_state;
			m_cur_state->enter();
			m_next_state = 0;
		}
		
		// process user input first
		m_input.update();

		//TEMP
#if 0		
		text::font_manager& fm = m_renderer->get_font_manager();
		graphics::text::font_ptr f = fm.open_font("/game/ui/fonts/arial.ttf");			
		graphics::text::font_ptr f2 = fm.open_font("/game/ui/fonts/maturasc.ttf");			
		TYCHO_ASSERT(f);
		graphics::text::font_manager::string_handle h5 = fm.build_string(f2, " Hello World!", -1);
		graphics::text::font_manager::string_handle h0 = fm.build_string(f, "abcdefghijklmnopqrsuvwxyz", -1);
		graphics::text::font_manager::string_handle h1 = fm.build_string(f, "ABCDEFGHIJKLMNOPQRSTUVWXYZ", -1);
		graphics::text::font_manager::string_handle h2 = fm.build_string(f2, "abcdefghijklmnopqrsuvwxyz", -1);
		graphics::text::font_manager::string_handle h3 = fm.build_string(f2, "ABCDEFGHIJKLMNOPQRSTUVWXYZ", -1);
		graphics::text::font_manager::string_handle h4 = fm.build_string(f, "abcdefghijklmnopqrsuvwxyz", -1);
		fm.release(h0);
		fm.release(h1);
		fm.release(h2);
		fm.release(h3);
		fm.release(h4);
		fm.release(h5);	
#endif
				
		m_cur_state->update();
		m_cur_state->render();
		m_renderer->present(m_main_window);
		m_renderer->flip();
		
		return 1;
	}
	

	void game::start_game(int num_players)
	{
		m_play_game_state->setup_game(num_players);
		set_next_state(m_play_game_state);
	}
	
	void game::quit_game()
	{
		m_quit = true;
	}
	
	void game::show_main_menu()
	{
		set_next_state(m_main_menu_state);
	}
	
	void game::show_pause_menu()
	{
	}

} // end namespace
