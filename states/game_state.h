//////////////////////////////////////////////////////////////////////////////
// Tycho Game Library
// Copyright (C) 2008 Martin Slater
// Created : Sunday, 18 May 2008 7:45:42 PM
//////////////////////////////////////////////////////////////////////////////
#if _MSC_VER > 1000
#pragma once
#endif  // _MSC_VER

#ifndef __GAME_STATE_H_97BA00EF_FDEA_4E1F_A81A_5CB111D8BFD1_
#define __GAME_STATE_H_97BA00EF_FDEA_4E1F_A81A_5CB111D8BFD1_

//////////////////////////////////////////////////////////////////////////////
// INCLUDES
//////////////////////////////////////////////////////////////////////////////
#include "../test_game_abi.h"
#include "graphics/forward_decls.h"
#include "io/forward_decls.h"
#include "input/forward_decls.h"

//////////////////////////////////////////////////////////////////////////////
// CLASS
//////////////////////////////////////////////////////////////////////////////

namespace test_game
{
	class game;
	
    class game_state
    {
    public:
		game_state(tycho::input::interface* input, tycho::io::interface* io, game* g) : 
            m_game(g),
            m_input(input),
            m_io(io) 
        {}
        
        virtual ~game_state() {}
		virtual void enter() = 0;
		virtual void leave() = 0;
		virtual void update() = 0;
		virtual void render() = 0;
	protected:
		game* m_game;
        tycho::input::interface* m_input;
        tycho::io::interface* m_io;
    };
    
} // end namespace

#endif // __GAME_STATE_H_97BA00EF_FDEA_4E1F_A81A_5CB111D8BFD1_
