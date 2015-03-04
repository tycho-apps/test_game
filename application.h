//////////////////////////////////////////////////////////////////////////////
// Tycho Game Library
// Copyright (C) 2008 Martin Slater
// Created : Sunday, 30 March 2008 10:04:22 PM
//////////////////////////////////////////////////////////////////////////////
#if _MSC_VER > 1000
#pragma once
#endif  // _MSC_VER

#ifndef __APPLICATION_H_2823AC11_A074_4FBA_A497_7E28BD2092F7_
#define __APPLICATION_H_2823AC11_A074_4FBA_A497_7E28BD2092F7_

//////////////////////////////////////////////////////////////////////////////
// INCLUDES
//////////////////////////////////////////////////////////////////////////////
#include "test_game_abi.h"

//////////////////////////////////////////////////////////////////////////////
// CLASS
//////////////////////////////////////////////////////////////////////////////

namespace test_game
{
	/// main application object
    class application
    {
    public:
		/// constructor
		application();
		
		/// destructor
		~application();
		
		/// called the very first thing after main
		int initialise();
		
		/// called last thing before main is exited
		int shutdown();
		
		/// main run loop, this doesn't terminate until the user quits
		int run();

    private:		
		/// called at startup to initialise the game
		virtual void initialise_game() = 0;    

		/// called when user has requested termination
		virtual void shutdown_game() = 0;    

		/// per frame update
		virtual int update_frame() = 0;		
		
    };
    
} // end namespace

#endif // __APPLICATION_H_2823AC11_A074_4FBA_A497_7E28BD2092F7_
