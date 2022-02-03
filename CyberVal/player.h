#pragma once
#define FINLINE __forceinline

#include <cstdint>


class Enemy
{
public:
	FINLINE bool RecentlyRendered() {		
		float fLastSubmitTime = read<float>(mesh_ptr + Offsets::last_render_time);
		float fLastRenderTimeOnScreen = read<float>(mesh_ptr + Offsets::last_submit_time);
		const float fVisionTick = 0.06f;
		return fLastRenderTimeOnScreen + fVisionTick >= fLastSubmitTime;
	}

	FINLINE bool IsVisible()
	{
		if (!pointer::game_instance || !pointer::uworld || !pointer::ulevel || !pointer::local_player_pawn || !pointer::local_player || !pointer::local_player_controller)
			return false;

		if (!RecentlyRendered() || !IsValidPlayer())
			return false;
		
		Vector3 temp = Vector3(0, 0, 0);
		return LineOfSight(pointer::local_player_controller, actor_ptr, &temp, false);	
		//return RecentlyRendered();
	}

	FINLINE bool IsBot()
	{
		if (bone_count == 103)
			return true;

		return false;
	}

	FINLINE string GetCurrentWeapon()
	{
		auto Inventory = read<intptr_t>(actor_ptr + Offsets::Inventory);
		intptr_t CurrentEquip = read<intptr_t>(Inventory + Offsets::CurrentWeapon);
		int weaponObj = read<int>(CurrentEquip + Offsets::ObjID);
		return GetWeaponName(weaponObj);
	}

	FINLINE string GetCharacter()
	{
		int charObj = read<int>(actor_ptr + Offsets::ObjID);
		auto characterName = GetCharacterName(charObj);
		return characterName;
	}

	FINLINE bool IsDormant()
	{
		bool dormant = read<bool>(actor_ptr + Offsets::Dormant);
		if (!dormant)
		{
			return false;
		}
		return true;
	}

	FINLINE float GetHealth()
	{
		return read<float>(damage_handler_ptr + Offsets::Health);
	}

	FINLINE bool IsAlive()
	{
		return (read<float>(damage_handler_ptr + Offsets::Health) > 0 && read<float>(damage_handler_ptr + Offsets::Health) < 200);
	}

	FINLINE Vector3 GetPosition()
	{
		return read<Vector3>(root_component_ptr + Offsets::RelativeLocation);
	}

	FINLINE Vector3 GetWorldToScreenPosition()
	{
		Vector3 x = read<Vector3>(root_component_ptr + Offsets::RelativeLocation);
		return WorldToScreen(x);
	}

	FINLINE bool IsValidPlayer()
	{
		uintptr_t unique_id = read<uintptr_t>(actor_ptr + Offsets::UniqueID);
		if (unique_id != 18743553)
			return false;

		return true;
	}

	FINLINE bool IsMale()
	{
		if (bone_count == 102) return true;
		return false;
	}

	FINLINE bool IsFemale()
	{
		if (bone_count == 99) return true;
		return false;
	}

	uintptr_t actor_ptr;
	uintptr_t damage_handler_ptr;
	uintptr_t player_state_ptr;
	uintptr_t root_component_ptr;
	uintptr_t mesh_ptr;

	int bone_count;
	int team_id;
	int object_id;
	int actor_type;
};

inline bool operator==(const Enemy& a, const Enemy& b) // Enemy-Operator
{
	if (a.actor_ptr == b.actor_ptr)
	{
		if (a.mesh_ptr == b.mesh_ptr)
			return true;
	}
	return false;
}