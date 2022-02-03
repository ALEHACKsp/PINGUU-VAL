#pragma once

#include <cstdint>

namespace pointer
{
	static intptr_t uworld = NULL;
	static intptr_t game_instance = NULL;
	static intptr_t ulevel = NULL;
	static intptr_t local_player_array = NULL;
	static intptr_t local_player = NULL;
	static intptr_t local_player_controller = NULL;
	static intptr_t local_player_pawn = NULL;
	static intptr_t local_damage_handler = NULL;
	static intptr_t local_player_state = NULL;
	static intptr_t local_team_component = NULL;
	static intptr_t camera_cache = NULL;
	static intptr_t local_root_comp = NULL;
	static int local_team_id = -1;
	static string current_weapon = "Unknown";
	static intptr_t old_player_pawn = NULL;
}