//////////////////////////////////////////////////////////////////////////////
// Tycho Game Library
// Copyright (C) 2008 Martin Slater
// Created : Wednesday, 2 April 2008 1:45:12 AM
//////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////
// INCLUDES
//////////////////////////////////////////////////////////////////////////////
#include "../test_game.h"
#include "engine/globals.h"
#include "graphics/format.h"
//#include "dx9/render_interface.h"
//#include "input/pc/xinput_driver.h"
#include "core/console.h"
#include "core/globals.h"
#include "core/platform.h"
#include "io/filesystem_device.h"
#include "io/interface.h"

//////////////////////////////////////////////////////////////////////////////
// CLASS
//////////////////////////////////////////////////////////////////////////////

using namespace tycho;
using namespace tycho::engine;
using namespace tycho::graphics;


namespace test_game
{

	void game::platform_initialise()
	{
		const char* app_name = "test_game";

		// configure filesystem
		std::string base_dir = core::current_working_directory();
		io::filesystem_device_ptr fs_device(new io::filesystem_device());
		m_io.add_device(fs_device);
		m_io.mount("/game/", fs_device->make_mount_point((base_dir + "assets\\").c_str()));
		m_io.mount("/app_data/", fs_device->make_mount_point(core::app_data_dir(app_name).c_str()));
		m_io.mount("/save_games/", fs_device->make_mount_point(core::save_game_dir(app_name).c_str()));
		m_io.mount("/temp/", fs_device->make_mount_point(core::temp_dir().c_str()));

		// create input driversl1
// 		input::interface *ii = globals::get_input_interface();
// 		ii->add_driver(new input::pc::keyboard_mouse_driver());
		
		// create the renderer and main game window
//		m_renderer = renderer_ptr(new graphics::renderer(new graphics::dx9::render_interface(g_hwnd)));	
		TYCHO_VERIFY(m_renderer->initialise(0));
//		m_main_window = m_renderer->create_window(800, 600, format::pixel_xrgb8888, format::depth_32bit, g_hwnd);
	}

	/// per platform game shutdown
	void game::platform_shutdown()
	{
	}

} // end namespace
