//////////////////////////////////////////////////////////////////////////////
// Tycho Game Library
// Copyright (C) 2008 Martin Slater
// Created : Monday, 21 April 2008 1:14:23 AM
//////////////////////////////////////////////////////////////////////////////
#if _MSC_VER > 1000
#pragma once
#endif  // _MSC_VER

#ifndef __PLAYER_H_03F32363_78B2_4BEE_B39B_1F654AD7163C_
#define __PLAYER_H_03F32363_78B2_4BEE_B39B_1F654AD7163C_

//////////////////////////////////////////////////////////////////////////////
// INCLUDES
//////////////////////////////////////////////////////////////////////////////
#include "test_game_abi.h"
#include "input/types.h"
#include "engine/entity/camera.h"
#include "graphics/forward_decls.h"
#include "input/forward_decls.h"
#include "core/intrusive_ptr.h"
#include "math/matrix4x4.h"

//////////////////////////////////////////////////////////////////////////////
// CLASS
//////////////////////////////////////////////////////////////////////////////

namespace test_game
{
	TYCHO_DECLARE_INTRUSIVE_PTR(TEST_GAME_ABI, player);

    class TEST_GAME_ABI player : 
		public tycho::engine::entity::entity_base,
		public tycho::input::input_handler
    {
    public:
		/// constructor
        player(tycho::input::interface* ii, tycho::engine::entity::arena* arena, const char *);
		
		/// destructor
		~player();

		/// \name player functions
		//@{
		/// set the camera the player renders to.
		void set_camera(tycho::graphics::camera_ptr);

		/// \returns the camera to render this player with 
		tycho::graphics::camera_ptr get_camera();
		
		/// bind player input to the specified gamepad_index index
		void bind_input(int gamepad_index);
		
		/// unbind all player input
		void unbind_input();
		//@}
		
		/// \name entity interface
		//@{
		void update(tycho::engine::entity::update_state&);
		void render();
		//@}
		
		/// \name input_handler interface
		//@{
		virtual bool handle_axis(int action_id, const float value) override;
		virtual bool handle_key(int action_id, tycho::input::key_type type, tycho::input::key_state state) override;
		//@}
		
    private:		
		TYCHO_INTRUSIVE_PTR_FRIEND_DECLS(TEST_GAME_ABI, player)
		int m_input_group;		
		tycho::graphics::camera_ptr m_camera;
		tycho::math::matrix4x4f m_world_mat;
		tycho::math::matrix4x4f m_view_mat;
		tycho::math::matrix4x4f m_proj_mat;
		tycho::engine::	entity::mesh_ptr m_mesh;
		float	m_move_speed;
		float   m_strafe_speed;
        tycho::input::interface* m_ii;
    };

    TYCHO_INTRUSIVE_PTR_FUNC_DECLS(TEST_GAME_ABI, player)
    
} // end namespace

#endif // __PLAYER_H_03F32363_78B2_4BEE_B39B_1F654AD7163C_
