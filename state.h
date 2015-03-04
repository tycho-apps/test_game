//////////////////////////////////////////////////////////////////////////////
// Tycho Game Library
// Copyright (C) 2008 Martin Slater
// Created : Saturday, 17 May 2008 5:05:38 PM
//////////////////////////////////////////////////////////////////////////////
#if _MSC_VER > 1000
#pragma once
#endif  // _MSC_VER

#ifndef __STATE_H_D53AB9F7_DA69_4F8C_B180_64B9E62C3799_
#define __STATE_H_D53AB9F7_DA69_4F8C_B180_64B9E62C3799_

//////////////////////////////////////////////////////////////////////////////
// INCLUDES
//////////////////////////////////////////////////////////////////////////////
#include "test_game_abi.h"

//////////////////////////////////////////////////////////////////////////////
// CLASS
//////////////////////////////////////////////////////////////////////////////

namespace test_game
{

	class game;
	
	/// game state, this is pushed on to a stack of states maintained by the game
    class state
    {
    public:
// 		virtual ~state(game* g) : m_game(g) {}
// 		virtual void enter() = 0;
// 		virtual void leave() = 0;
		
	protected:
		game* m_game;    
    };
    
} // end namespace

#endif // __STATE_H_D53AB9F7_DA69_4F8C_B180_64B9E62C3799_