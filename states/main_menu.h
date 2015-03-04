//////////////////////////////////////////////////////////////////////////////
// Tycho Game Library
// Copyright (C) 2008 Martin Slater
// Created : Sunday, 18 May 2008 7:45:50 PM
//////////////////////////////////////////////////////////////////////////////
#if _MSC_VER > 1000
#pragma once
#endif  // _MSC_VER

#ifndef __MAIN_MENU_H_A8F8DC25_EDCD_46D2_AF99_6840D13A0000_
#define __MAIN_MENU_H_A8F8DC25_EDCD_46D2_AF99_6840D13A0000_

//////////////////////////////////////////////////////////////////////////////
// INCLUDES
//////////////////////////////////////////////////////////////////////////////
#include "../test_game_abi.h"
#include "graphics/shader/forward_decls.h"
#include "ui/forward_decls.h"
#include "input/forward_decls.h"
#include "game_state.h"

//////////////////////////////////////////////////////////////////////////////
// CLASS
//////////////////////////////////////////////////////////////////////////////

namespace test_game
{
	class game;
	
    class main_menu : public game_state
    {
    public:
		main_menu(input::interface* input, io::interface* io, game* g);
		
		virtual void enter();
		virtual void leave();
		virtual void update();
		virtual void render();
				
	private:
		void start_sp_game();
		void start_mp_game();
		void quit_game();
		
	private:
		ui::panel_ptr m_ui_panel;
		graphics::shader::parameter_object_lookup_table_ptr m_param_table;
    };
    
} // end namespace

#endif // __MAIN_MENU_H_A8F8DC25_EDCD_46D2_AF99_6840D13A0000_
