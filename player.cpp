//////////////////////////////////////////////////////////////////////////////
// Tycho Game Library
// Copyright (C) 2008 Martin Slater
// Created : Monday, 21 April 2008 1:14:24 AM
//////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////
// INCLUDES
//////////////////////////////////////////////////////////////////////////////
#include "player.h"
#include "input/interface.h"
#include "graphics/camera.h"
#include "graphics/shapes.h"
#include "engine/entity/mesh.h"
#include "engine/entity/arena.h"
#include "engine/entity/update_state.h"
#include "core/debug/utilities.h"
#include "core/colour/constants.h"
#include "math/constants.h"

//////////////////////////////////////////////////////////////////////////////
// CLASS
//////////////////////////////////////////////////////////////////////////////
using namespace tycho;
using namespace tycho::engine;

namespace test_game
{
	TYCHO_INTRUSIVE_PTR_FUNC_IMPL(TEST_GAME_ABI, player)

	namespace detail
	{
		enum player_input_actions
		{
			player_turn_y, ///< turn about the y axis
			player_turn_x, ///< turn about the x axis
			player_move,   ///< move forward and back
			player_strafe, ///< strafe left and right
			player_fire	   ///< boom.
		};
		
		static tycho::input::action player_actions[] = {
			{ "player.turn_y",  detail::player_turn_y,	input::event_type_axis },
			{ "player.turn_x",  detail::player_turn_x,	input::event_type_axis },
			{ "player.move",	detail::player_move,	input::event_type_axis },
			{ "player.strafe",  detail::player_strafe,	input::event_type_axis },
			{ "player.fire",	detail::player_fire,	input::event_type_key  },	
			{ 0, 0 }
		};	
	}

	player::player(tycho::input::interface* ii, entity::arena* arena, const char* name) :
		entity::entity_base(name),
		m_input_group(-1),
        m_world_mat(math::matrix4x4f::create_identity()),
		m_move_speed(0),
		m_strafe_speed(0),
        m_ii(ii)
	{
		// create the player mesh (a box...with a single clr...woo)
		m_mesh = entity::mesh_ptr(new entity::mesh(arena->get_renderer(), "player01_mesh"));
		m_mesh->set_mesh(graphics::create_box(arena->get_renderer(), math::vector3f(1000, 1500, 1000), graphics::shape_create_normals));
		graphics::shader::shader_base_ptr shader = arena->get_renderer()->create_system_shader(graphics::system_shader_single_clr);
		m_mesh->set_shader(shader).set<math::vector4f>("obj_clr", core::to_vector4<float>(core::colours::aquamarine));		
		arena->add_entity(m_mesh);			
	}
	
	player::~player()
	{
        m_ii = nullptr;
	}

	/// set the viewport the player renders to.
	void player::set_camera(graphics::camera_ptr c)
	{
		m_camera = c;
	}
	

	/// \returns the camera to render this player with 
	tycho::graphics::camera_ptr player::get_camera()
	{
		return m_camera;
	}	

		
	void player::bind_input(int input_group) 
	{
		m_input_group = input_group;
		if(m_input_group >= 0)
			m_ii->push_action_group(input_group, "player", detail::player_actions, this);
	}
	
	void player::unbind_input() 
	{
		if(m_input_group >= 0)
			m_ii->pop_action_group(m_input_group, "player", detail::player_actions);
	}

	void player::update(entity::update_state &state)
	{
		if(m_camera)
		{
			math::vector3f eye(0, 1000, -3000);
			math::vector3f at(0, 0, 0);
			math::vector3f dir(at - eye);
			math::vector3f right(1,0,0);
			math::vector3f up(dir.cross(right));
			math::matrix4x4f view_mat(math::make_look_at(eye, at, up));
			math::matrix4x4f proj_mat(math::make_perspective(10.0f, 10.0f, 10.0f, 50000.0f));	
			math::matrix4x4f mat = view_mat * proj_mat;	

			math::vector3f trans = m_world_mat.get_translation();
			trans.z(trans.z() + m_move_speed * state.secs_elapsed);
			trans.x(trans.x() + m_strafe_speed * state.secs_elapsed);
			trans.y(750.0f);
			m_world_mat.set_translation(trans);
			m_mesh->set_world_mat(m_world_mat);
		
			// set this player up for rendering
 			m_camera->set_matrix(graphics::camera::matrix_type_view, view_mat);
 			m_camera->set_matrix(graphics::camera::matrix_type_proj, proj_mat);
 			m_camera->set_matrix(graphics::camera::matrix_type_view_proj, mat);			
 			
 			// 
		}
	}
	
	void player::render()
	{
	}
	
	bool player::handle_axis(int action_id, const float value)
	{
		const float DefaultMoveSpeed = 4000.0f; // centimeters per second			
		switch(action_id)
		{
			case detail::player_move : {
				m_move_speed = DefaultMoveSpeed * value;
			} break;

			case detail::player_strafe : {
				m_strafe_speed = DefaultMoveSpeed * value;
			} break;
		}
		
		return true;
	}

	bool player::handle_key(int /*action_id*/, input::key_type /*type*/, input::key_state /*state*/)
	{
		return true;
	}


} // end namespace
