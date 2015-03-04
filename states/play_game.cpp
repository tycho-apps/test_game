//////////////////////////////////////////////////////////////////////////////
// Tycho Game Library
// Copyright (C) 2008 Martin Slater
// Created : Sunday, 18 May 2008 7:46:10 PM
//////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////
// INCLUDES
//////////////////////////////////////////////////////////////////////////////
#include "play_game.h"
#include "input/interface.h"
#include "graphics/forward_decls.h"
#include "graphics/viewport.h"
#include "graphics/camera.h"
#include "graphics/shader/parameter_buffer.h"
#include "graphics/shader/parameter_buffer_layout.h"
#include "graphics/shader/parameter_buffer_view.h"
#include "graphics/shader/parameter_object_lookup_table.h"
#include "graphics/shader/shader.h"
#include "graphics/vertex_buffer.h"
#include "graphics/vertex_format.h"
#include "graphics/shapes.h"
#include "graphics/mesh.h"
#include "graphics/camera.h"
#include "graphics/texture.h"
#include "graphics/texture_utils.h"
#include "ui/panel.h"
#include "ui/image.h"
#include "ui/button.h"
#include "ui/layout.h"
#include "engine/globals.h"
#include "core/clock.h"
#include "core/console.h"
#include "core/colour/constants.h"
#include "core/globals.h"
#include "io/types.h"
#include "io/stream.h"
#include "io/interface.h"
#include "math/vector2.h"
#include "../test_game.h"

using namespace tycho::engine;
using namespace tycho::graphics;

//////////////////////////////////////////////////////////////////////////////
// CLASS
//////////////////////////////////////////////////////////////////////////////
namespace test_game
{

	namespace detail
	{
		enum game_input_actions
		{
			game_show_pause_menu, ///< bring up the pause menu
		};
		
		static input::action game_actions[] = {
			{ "game.show_pause_menu",  detail::game_show_pause_menu, input::event_type_key },
			{ 0, 0 }
		};	
	}
		
	play_game::play_game(input::interface* input, io::interface* io, game* g) :
		game_state(input, io, g),
		m_num_players(1),
		m_paused(false)
	{
		m_update_state.arena = 0;
		m_update_state.tick = 0;
		m_update_state.secs = 0;			
	}
	
	void play_game::setup_game(int num_players)
	{
		m_num_players = num_players;
	}
	
	void play_game::enter()
	{	
		graphics::renderer_ptr renderer = m_game->get_renderer();
		m_paused = false;
		// create an arena 
		m_arena = m_update_state.arena = new entity::arena(renderer);
				
		// create a global shader parameter buffer, this handles all the marshaling of parameters
		// from user code to the shader system
		m_param_table = shader::parameter_object_lookup_table_ptr(new shader::parameter_object_lookup_table);

		// create the floor, a box..
		tycho::engine::entity::mesh_ptr mesh = entity::mesh_ptr(new entity::mesh(renderer, "box01"));
		mesh->set_mesh(graphics::create_box(renderer, math::vector3f(10000, 10, 10000), shape_create_normals));
		mesh->set_shader(renderer->create_system_shader(system_shader_single_clr)).
			set<math::vector4f>("obj_clr", core::to_vector4<float>(core::colours::mediumpurple)).
			set<math::matrix4x4f>("world_matrix", math::matrix4x4f::create_identity().set_translation(math::vector3f(0,-5,0)));
		m_arena->add_entity(mesh);
	
		// setup players				
		graphics::viewport player_viewports[2];		
		if(m_num_players == 2) 
		{
			player_viewports[0] = graphics::viewport(math::vector2f(0,0), math::vector2f(1,0.5f), 0.0f, 1.0f);
			player_viewports[1] = graphics::viewport(math::vector2f(0,0.5f), math::vector2f(1,0.5f), 0.0f, 1.0f);
		}
		else
		{
			player_viewports[0] = graphics::viewport(math::vector2f(0,0), math::vector2f(1,1), 0.0f, 1.0f);	
		}

		// get controllers 		
		const input::device_description* controllers[8] = { 0 };
		m_input->enumerate_controllers(controllers, 8);

		for(int i = 0; i < m_num_players; ++i)
		{
			// create and configure a player 
			m_players[i] = test_game::player_ptr(new test_game::player(m_input, m_arena, "player01"));
			graphics::camera_ptr player_camera = graphics::camera_ptr(new graphics::camera());
			player_camera->set_parameter_object_lookup_table(m_param_table);
			player_camera->set_render_target(m_game->get_main_window());
			player_camera->set_viewport(player_viewports[i]);
			m_players[i]->set_camera(player_camera);
			m_arena->add_entity(m_players[i]);		
			int use_controller = i;
			// get controllers 
			if(controllers[use_controller])
			{			
				m_input->bind_device(i, controllers[use_controller]->id);
				m_players[i]->bind_input(i);
				m_input->push_action_group(i, "game", detail::game_actions, this);		
			}
		}			
		make_pause_ui();
	}
	
	void play_game::leave()
	{
		m_input->pop_action_group(0, "game", detail::game_actions);
		core::safe_delete(m_arena);
		for(int i = 0; i < MaxPlayers; ++i)
		{
			if(m_players[i])
			{
				m_players[i]->unbind_input();
				m_players[i] = player_ptr();
			}
		}
		m_pause_ui = ui::panel_ptr();
	}
	
	void play_game::update()
	{
		// update the arena
		++m_update_state.tick;
		m_update_state.secs = core::clock::get_time();
		if(m_last_update_time == 0.0f)
			m_last_update_time = m_update_state.secs;
		m_update_state.secs_elapsed = static_cast<core::float32>(m_update_state.secs - m_last_update_time);
		m_arena->update(m_update_state);	
		m_last_update_time = m_update_state.secs;
	}
	
	void play_game::render()
	{			
		using namespace tycho::math;
		using namespace tycho::graphics;
		renderer_ptr renderer = m_game->get_renderer();
		
		if(m_paused)
		{
			/// \todo crazy amount of code to render a widget, factor out and simplify
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
			m_pause_ui->render(ri);
			ri.matrix_stack.pop();
			TYCHO_ASSERT(ri.matrix_stack.empty());		
		}
		else
		{	
			// render all player views
			if(m_num_players)
			{
				renderer->set_render_target(m_players[0]->get_camera()->get_render_target());
				for(int i = 0; i < m_num_players; ++i)
				{
					renderer->set_viewport(m_players[i]->get_camera()->get_viewport());
					shader::scoped_parameter_object spo(m_param_table, "camera", m_players[i]->get_camera()->get_parameter_buffer_view());				
					m_arena->render(renderer, m_players[i]->get_camera());
					ui::widget::render_info ri;
					ri.renderer = renderer;
					ri.camera = m_players[i]->get_camera();
					ri.matrix_stack.push(matrix4x4f::create_identity());
					m_hud->render(ri);
				}
			}
		}
	}

	bool play_game::handle_key(int action_id, input::key_type /*key*/, input::key_state /*state*/)
	{
		switch(action_id)
		{
			case detail::game_show_pause_menu : show_pause_menu(); return true;
		}
		return false;
	}

	void play_game::show_pause_menu()	
	{
		m_paused = true;
		m_pause_ui->give_focus();
		m_pause_ui->highlight_widget(0);
	}

	void play_game::make_pause_ui()
	{
		using namespace engine;
		using namespace math;

		graphics::renderer_ptr renderer = m_game->get_renderer();
		
		// create our ui 
		m_pause_ui = ui::panel_ptr(new ui::panel(m_input, "pause_menu"));
		tycho::io::stream_ptr s = m_io->open_stream("/game/ui/pause_menu/layout.ui", tycho::io::open_flag_read);
		TYCHO_ASSERT(!s->fail());
		std::vector<ui::widget_ptr> widgets;
		ui::load_layout(m_input, m_io, renderer, s, widgets);
		TYCHO_ASSERT(widgets.size() == 1);
		m_pause_ui = boost::static_pointer_cast<ui::panel>(widgets[0]);
		TYCHO_ASSERT(m_pause_ui);
		m_pause_ui->find_widget_t<ui::button>("resume")->bind_onclick_event(std::bind(&play_game::resume_game, this));
		m_pause_ui->find_widget_t<ui::button>("quit_to_main_menu")->bind_onclick_event(std::bind(&play_game::quit_to_main_menu, this));
		m_pause_ui->find_widget_t<ui::button>("quit_game")->bind_onclick_event(std::bind(&play_game::quit_game, this));		
		// give the widget focus
		m_pause_ui->give_focus();			
		m_pause_ui->highlight_widget(0);
		
		s = m_io->open_stream("/game/ui/hud/layout.ui", io::open_flag_read);
		ui::load_layout(m_input, m_io, renderer, s, widgets);
		TYCHO_ASSERT(widgets.size() == 1);
		m_hud = boost::static_pointer_cast<ui::panel>(widgets[0]);		
	}
	
	void play_game::resume_game()
	{
		m_pause_ui->remove_focus();
		m_paused = false;
	}

	void play_game::quit_to_main_menu()
	{
		m_pause_ui->remove_focus();
		m_game->show_main_menu();
	}

	void play_game::quit_game()
	{
		m_pause_ui->remove_focus();
		m_game->quit_game();
	}	
	
} // end namespace
