//////////////////////////////////////////////////////////////////////////////
// Tycho Game Library
// Copyright (C) 2008 Martin Slater
// Created : Sunday, 18 May 2008 7:45:51 PM
//////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////
// INCLUDES
//////////////////////////////////////////////////////////////////////////////
#include "main_menu.h"
#include "ui/panel.h"
#include "ui/image.h"
#include "ui/button.h"
#include "ui/label.h"
#include "ui/layout.h"
#include "graphics/renderer.h"
#include "graphics/camera.h"
#include "graphics/texture.h"
#include "graphics/texture_utils.h"
#include "graphics/shader/parameter_object_lookup_table.h"
#include "core/console.h"
#include "core/globals.h"
#include "io/types.h"
#include "io/stream.h"
#include "io/interface.h"
#include "../test_game.h"

//////////////////////////////////////////////////////////////////////////////
// CLASS
//////////////////////////////////////////////////////////////////////////////
using namespace tycho::engine;
using namespace tycho::graphics;

namespace test_game
{

	main_menu::main_menu(input::interface* input, io::interface* io, game* g) :
		game_state(input, io, g)
	{
		m_param_table = shader::parameter_object_lookup_table_ptr(new shader::parameter_object_lookup_table);
	}
	
	void main_menu::enter()
	{
		using namespace engine;
		using namespace math;

		graphics::renderer_ptr renderer = m_game->get_renderer();
		tycho::io::stream_ptr s = m_io->open_stream("/game/ui/main_menu/layout.ui", tycho::io::open_flag_read);
		TYCHO_ASSERT(s);
		std::vector<ui::widget_ptr> widgets;
		ui::load_layout(m_input, m_io, renderer, s, widgets);
		TYCHO_ASSERT(widgets.size() == 1);
		m_ui_panel = boost::static_pointer_cast<ui::panel>(widgets[0]);
		TYCHO_ASSERT(m_ui_panel);
		if(m_ui_panel->find_widget_t<ui::button>("startsp"))
			m_ui_panel->find_widget_t<ui::button>("startsp")->bind_onclick_event(std::bind(&main_menu::start_sp_game, this));
		if(m_ui_panel->find_widget_t<ui::button>("startmp"))
			m_ui_panel->find_widget_t<ui::button>("startmp")->bind_onclick_event(std::bind(&main_menu::start_mp_game, this));
		if(m_ui_panel->find_widget_t<ui::button>("quit"))
			m_ui_panel->find_widget_t<ui::button>("quit")->bind_onclick_event(std::bind(&main_menu::quit_game, this));
		// give the widget focus
		m_ui_panel->give_focus();			
		m_ui_panel->highlight_widget(0);
	}
	
	void main_menu::leave()
	{
		m_ui_panel->remove_focus();
		m_ui_panel = ui::panel_ptr();
	}
	
	void main_menu::update()
	{
	
	}
	
	void main_menu::render()
	{
		using namespace tycho::math;
		using namespace tycho::graphics;
		graphics::renderer_ptr renderer = m_game->get_renderer();
		graphics::render_target_ptr main_window = m_game->get_main_window();
		
		graphics::camera_ptr camera = graphics::camera_ptr(new graphics::camera());
		matrix4x4f view_mat = matrix4x4f::create_identity();
		matrix4x4f proj_mat(math::make_perspective(1.0f, 1.0f, 1.0f, 50000.0f));	
		matrix4x4f mat = view_mat * proj_mat;	
		camera->set_matrix(graphics::camera::matrix_type_view, view_mat);
		camera->set_matrix(graphics::camera::matrix_type_proj, proj_mat);
		camera->set_matrix(graphics::camera::matrix_type_view_proj, mat);			
		camera->set_viewport(viewport(vector2f(0,0), vector2f(1,1), 0, 1));
		camera->set_parameter_object_lookup_table(m_param_table);
		camera->set_render_target(main_window);
		renderer->set_render_target(camera->get_render_target());
		renderer->set_viewport(camera->get_viewport());
		ui::widget::render_info ri;
		ri.renderer = renderer;
		ri.camera = camera;
		ri.matrix_stack.push(matrix4x4f::create_identity());
		m_ui_panel->render(ri);
		ri.matrix_stack.pop();
		TYCHO_ASSERT(ri.matrix_stack.empty());		
	}
	
	void main_menu::start_sp_game()
	{
		core::console::write_ln("starting single player");
		m_game->start_game(1);
	}

	void main_menu::start_mp_game()
	{
		core::console::write_ln("starting multi player");		
		m_game->start_game(2);
	}

	void main_menu::quit_game()
	{
		core::console::write_ln("quitting");		
		m_game->quit_game();
	}


} // end namespace
