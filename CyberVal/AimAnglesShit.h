#pragma once

#include <cstdio>

int findEnemyID = -1;
float AimFOV = 30.f;
float Smooth = 1.f;
DWORD Aimkey = NULL;

Vector3 SmoothAim(Vector3 Camera_rotation, Vector3 Target, float SmoothFactor)
{
	Vector3 diff = Target - Camera_rotation;
	normalize(diff);
	return Camera_rotation + diff / SmoothFactor;
}


void RCS(Vector3 Target, Vector3 Camera_rotation, float SmoothFactor)
{

	Vector3 ConvertRotation = Camera_rotation;
	normalize(ConvertRotation);

	auto ControlRotation = read<Vector3>(pointer::local_player_controller + Offsets::ControlRotation);
	Vector3 DeltaRotation = ConvertRotation - ControlRotation;
	normalize(DeltaRotation);

	ConvertRotation = Target - (DeltaRotation * SmoothFactor);
	normalize(ConvertRotation);

	Vector3 Smoothed = SmoothAim(Camera_rotation, ConvertRotation, SmoothFactor);
	Smoothed -= (DeltaRotation / SmoothFactor);
	Clamp(Smoothed);

	*(D3DXVECTOR3*)(pointer::local_player_controller + Offsets::ControlRotation) = D3DXVECTOR3(Smoothed.x, Smoothed.y, 0);
	

	return;
}

void AimbotHandling()
{
	if (!pointer::game_instance || !pointer::uworld || !pointer::ulevel || !pointer::local_player_pawn || !pointer::local_player || entityList.empty())
		return;

	if (findEnemyID == -1)
		return;

	auto enemy = entityList[findEnemyID];

	if (!enemy.mesh_ptr || !enemy.actor_ptr)
	{
		auto it = std::find(entityList.begin(), entityList.end(), enemy);
		if (it != entityList.end())
		{
			entityList.erase(it);
		}
		return;
	}

	if (!enemy.IsAlive() || !enemy.IsVisible())
		return;

	int bone = (menu.items[12].value == 0 ? 8 : menu.items[12].value == 1 ? 7 : 5);
	Vector3 aim_bone = GetBoneWithRotation(enemy.mesh_ptr, bone);
	Vector3 vHead = WorldToScreen(aim_bone);

	if (vHead.x == 0.f || vHead.y == 0.f || aim_bone.x == 0.f || aim_bone.y == 0.f || aim_bone.z == 0.f)
	{
		auto it = std::find(entityList.begin(), entityList.end(), enemy);
		if (it != entityList.end())
		{
			entityList.erase(it);
		}
		return;
	}

	if (!InsideCircle(ScreenCenterX, ScreenCenterY, AimFOV, vHead.x, vHead.y))
		return;

	Vector3 AimAngles = CalcAngle(GetCameraCache().Position, aim_bone);
	Clamp(AimAngles);

	if (enemy.IsDormant())
	{
		string wep = pointer::current_weapon;
		if (wep != "Knife" && wep != "Sheriff" && wep != "Operator" && wep != "Guardian" && wep != "Marshall")
		{
			RCS(AimAngles, GetCameraCache().Rotation, Smooth + 0.1f);
		}
		else
		{			
			Vector3 Smoothed = SmoothAim(read<Vector3>(pointer::local_player_controller + Offsets::ControlRotation), AimAngles, Smooth + 0.1f);
			Clamp(Smoothed);

			*(D3DXVECTOR3*)(pointer::local_player_controller + Offsets::ControlRotation) = D3DXVECTOR3(Smoothed.x, Smoothed.y, 0);
		}		
	}	
}

float getCurrentEnemyCrosshairDistance()
{
	if (findEnemyID == -1 || entityList.empty())
		return 9999.0;

	auto entity = entityList[findEnemyID];

	if (!entity.IsValidPlayer() || !entity.IsAlive())
		return 9999.0;

	int bone = (menu.items[12].value == 0 ? 8 : menu.items[12].value == 1 ? 7 : 5);
	Vector3 head_position = GetBoneWithRotation(entity.mesh_ptr, bone);
	Vector3 head_at_screen = WorldToScreen(head_position);

	return GetCrossDistance(head_at_screen.x, head_at_screen.y, (Width / 2), (Height / 2));
}

void findNearestEnemy()
{
	if (!pointer::game_instance || !pointer::uworld || !pointer::ulevel || !pointer::local_player_pawn || !pointer::local_player || entityList.empty())
		return;

	if (entityList.empty())
	{
		if (findEnemyID != -1)
			findEnemyID = -1;

		return;
	}

	for (int i = 0; i < entityList.size(); i++)
	{
		auto entity = entityList[i];

		if (!entity.actor_ptr || !entity.mesh_ptr || entity.actor_ptr == pointer::local_player_pawn || (!entity.IsBot() ? entity.team_id == pointer::local_team_id : entity.team_id == 2141421))
		{
			auto it = std::find(entityList.begin(), entityList.end(), entity);
			if (it != entityList.end())
			{
				entityList.erase(it);
			}
			continue;
		}

		if (!entity.actor_ptr || !entity.mesh_ptr || !entity.IsValidPlayer())
			continue;

		int bone = (menu.items[12].value == 0 ? 8 : menu.items[12].value == 1 ? 7 : 5);
		Vector3 head_position = GetBoneWithRotation(entity.mesh_ptr, bone);
		Vector3 head_at_screen = WorldToScreen(head_position);

		float distance_crosshair = GetCrossDistance(head_at_screen.x, head_at_screen.y, (Width / 2), (Height / 2));

		if (distance_crosshair < getCurrentEnemyCrosshairDistance())
		{
			findEnemyID = i;
		}
	}
}