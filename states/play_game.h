//////////////////////////////////////////////////////////////////////////////
// Tycho Game Library
// Copyright (C) 2008 Martin Slater
// Created : Sunday, 18 May 2008 7:46:09 PM
//////////////////////////////////////////////////////////////////////////////
#if _MSC_VER > 1000
#pragma once
#endif  // _MSC_VER

#ifndef __PLAY_GAME_H_6635C8C3_DADA_4E8F_A13A_FC691D5ED379_
#define __PLAY_GAME_H_6635C8C3_DADA_4E8F_A13A_FC691D5ED379_

//////////////////////////////////////////////////////////////////////////////
// INCLUDES
//////////////////////////////////////////////////////////////////////////////
#include "../test_game_abi.h"
#include "engine/entity/arena.h"
#include "engine/entity/entity.h"
#include "engine/entity/update_state.h"
#include "engine/entity/mesh.h"
#include "engine/entity/camera.h"
#include "engine/entity/forward_decls.h"
#include "graphics/renderer.h"
#include "graphics/shader/forward_decls.h"
#include "ui/forward_decls.h"
#include "input/forward_decls.h"
#include "../player.h"
#include "game_state.h"

//////////////////////////////////////////////////////////////////////////////
// CLASS
//////////////////////////////////////////////////////////////////////////////

namespace test_game
{
    class play_game : 
		public game_state,
		public input::input_handler
    {
    public:
        play_game(input::interface* input, io::interface* io, game* g);
		
		void setup_game(int num_players);
		
		virtual void enter();
		virtual void leave();
		virtual void update();
		virtual void render();
		    		
    private:
		static const int MaxPlayers = 2;

		virtual bool handle_key(int /*action_id*/, input::key_type /*key*/, input::key_state /*state*/);
		void show_pause_menu();
		void make_pause_ui();
		void resume_game();
		void quit_to_main_menu();
		void quit_game();

		tycho::engine::entity::arena*			m_arena;		
		tycho::engine::entity::update_state		m_update_state;
		int m_num_players;
		test_game::player_ptr m_players[MaxPlayers];
		tycho::graphics::shader::parameter_object_lookup_table_ptr m_param_table;
		tycho::core::float64 m_last_update_time;
		tycho::ui::panel_ptr m_pause_ui;
		tycho::ui::panel_ptr m_hud;
		bool m_paused;
    };
    
} // end namespace

#endif // __PLAY_GAME_H_6635C8C3_DADA_4E8F_A13A_FC691D5ED379_
