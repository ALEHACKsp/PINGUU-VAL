#pragma once

static Vector3 pRadar;

void DrawRadar(Vector3 EntityPos, DWORD EntityColor)
{
	if (!pointer::game_instance || !pointer::uworld || !pointer::ulevel || !pointer::local_player_pawn || !pointer::local_player || entityList.empty())
		return;

	auto radar_posX = pRadar.x + 135;
	auto radar_posY = pRadar.y + 135;

	if (pointer::local_player_pawn == 0) return;

	Vector3 LocalPos = read<Vector3>(pointer::local_root_comp + Offsets::RelativeLocation);
	auto Radar2D = WorldRadar(LocalPos, EntityPos, GetCameraCache().Rotation.y, radar_posX, radar_posY, 135.f);

	DrawCircleFilled(Radar2D.x, Radar2D.y, 4, 10, EntityColor, draw.GetDevice());
}

void EspLoop()
{
	if (!pointer::game_instance || !pointer::uworld || !pointer::ulevel || !pointer::local_player_pawn || !pointer::local_player || entityList.empty())
		return;

	pRadar.x = Width - 450;
	pRadar.y = ScreenCenterY - ScreenCenterY + 300;

	if (menu.items[9].value == 1)
	{
		DrawRect(pRadar.x, pRadar.y, 270, 270, TBlack2, draw.GetDevice());
		DrawBorder(pRadar.x, pRadar.y, 270, 270, 1, SilverWhite, draw.GetDevice());

		auto radar_posX = pRadar.x + 135;//155
		auto radar_posY = pRadar.y + 135;//165
		// alt & �st
		DrawLine(radar_posX, radar_posY, radar_posX, radar_posY + 135, SilverWhite, 1);
		DrawLine(radar_posX, radar_posY, radar_posX, radar_posY - 135, SilverWhite, 1);

		// sol & sa�
		DrawLine(radar_posX, radar_posY, radar_posX + 135, radar_posY, SilverWhite, 1);
		DrawLine(radar_posX, radar_posY, radar_posX - 135, radar_posY, SilverWhite, 1);

		DrawCircleFilled(radar_posX - 0.5f, radar_posY - 0.5f, 3, 50, Yellow, draw.GetDevice());
	}

	for (int i = 0; i < entityList.size(); i++)
	{
		auto entity = entityList[i];

		if (!entity.mesh_ptr || !entity.actor_ptr)
			continue;

		Vector3 entity_root_pos_out = entity.GetWorldToScreenPosition();

		Vector3 local_position = read<Vector3>(pointer::local_root_comp + Offsets::RelativeLocation);
		float distance = local_position.Distance(entity.GetPosition());

		char dist_[64];
		sprintf(dist_, xorstr_("[%.fm]"), (distance / 100));

		if (distance <= 1.f)
			continue;

		if ((entity.actor_type == Player || entity.actor_type == Bot) && entity.IsAlive() && entity.IsDormant() && (entity.IsMale() || entity.IsFemale() || entity.IsBot()))
		{
			if (entity.actor_type == Player && !entity.player_state_ptr)
			{
				auto it = std::find(entityList.begin(), entityList.end(), entity);
				if (it != entityList.end())
				{
					entityList.erase(it);
				}
				continue;
			}		

			Vector3 Head = GetBoneWithRotation(entity.mesh_ptr, 8);
			Vector3 Foot = GetBoneWithRotation(entity.mesh_ptr, 0);

			if (Head.z <= Foot.z)
			{
				// Delete from list
				auto it = std::find(entityList.begin(), entityList.end(), entity);
				if (it != entityList.end())
				{
					entityList.erase(it);
				}
				continue;
			}

			Vector3 vHead = WorldToScreen(Head);
			Vector3 vFoot = WorldToScreen(Foot);

			DWORD visible_color = Grey;

			if (menu.items[1].value > 0) // Box ESP 
			{
				Vector3 Box = vHead - vFoot;

				if (Box.y < 0)
					Box.y *= -1;

				float BoxHeight = Box.y + 5;
				float BoxWidth = Box.y / 2.4f;

				//box3d(Foot, 39, 186, visible_color);

				DrawESPBox(vHead, vFoot, visible_color, 10);
			}

			if (menu.items[9].value == 1) // Radar
			{
				DrawRadar(entity.GetPosition(), visible_color);
			}

			if (menu.items[2].value > 0) // Snaplines
			{
				if (menu.items[2].value == 1) // Top
					DrawLine(ScreenCenterX, ScreenCenterY - ScreenCenterY, vFoot.x, vFoot.y, visible_color, 2.f);
				else if (menu.items[2].value == 2) // Middle
					DrawLine(ScreenCenterX, ScreenCenterY, vFoot.x, vFoot.y, visible_color, 2.f);
				else if (menu.items[2].value == 3) // Bottom
					DrawLine(ScreenCenterX, ScreenCenterY + ScreenCenterY, vFoot.x, vFoot.y, visible_color, 2.f);
			}

			if (menu.items[5].value == 1) // Skeleton
			{
				if (menu.items[0].value != 2)
					DrawSkeleton(entity.mesh_ptr, true, entity.IsDormant() ? ASilverWhite : (entity.IsVisible() ? SilverWhite : SilverWhite));
				else
					DrawSkeletonBot(entity.mesh_ptr, entity.IsDormant() ? ASilverWhite : (entity.IsVisible() ? SilverWhite : SilverWhite));
			}

			if (menu.items[3].value == 1) // Head ESP
			{
				DrawCircleFilled(vHead.x, vHead.y, (vHead.y - vFoot.y) / 12, 100, entity.IsDormant() ? ACyan : (entity.IsVisible() ? Cyan : Cyan), draw.GetDevice());
				DrawCircleFilled(vHead.x, vHead.y, (vHead.y - vFoot.y) / 20, 100, entity.IsDormant() ? ACyan2 : (entity.IsVisible() ? Cyan2 : Cyan2), draw.GetDevice());
			}

			if (menu.items[6].value == 1) // Character ESP
			{
				char tttt[64];
				sprintf(tttt, xorstr_("%s [%s]"), entity.GetCharacter(), entity.GetCurrentWeapon());

				DrawString(vFoot.x - (strlen(tttt) * 7 / 2) + (Rect.w / 2) - 1, vFoot.y + 5, entity.IsDormant() ? ASilverWhite : (entity.IsVisible() ? SilverWhite : SilverWhite), tttt);
				DrawString(vFoot.x - (strlen(dist_) * 7 / 2) + (Rect.w / 2) - 1, vFoot.y + 17, entity.IsDormant() ? ASilverWhite : (entity.IsVisible() ? SilverWhite : SilverWhite), dist_);
			}

			if (menu.items[4].value == 1) // Healthbar
			{
				const int middle = vFoot.y - vHead.y;
				const auto width = middle / 4.8;
				const auto health_calc = std::clamp(static_cast<int>(entity.GetHealth()) * middle / 100, 0, middle);
				const auto health_color = color().hsv_to_rgb(int(entity.GetHealth()) + 25, 1, 1, 255);
			//	DrawString(vFoot.x - (strlen(string(std::to_string((int)entity.GetHealth()) + xorstr_(" HP")).c_str()) * 7 / 2) + 5, vFoot.y + 5, entity.IsDormant() ? D3DCOLOR_RGBA(236, 236, 236, 255) : D3DCOLOR_RGBA(health_color.r, health_color.g, health_color.b, 255), string(std::to_string((int)entity.GetHealth()) + " HP").c_str());
				DrawString(vHead.x + width + 8, vHead.y - 1, entity.IsDormant() ? D3DCOLOR_RGBA(236, 236, 236, 255) : D3DCOLOR_RGBA(health_color.r, health_color.g, health_color.b, 255), string(std::to_string((int)entity.GetHealth()) + " HP").c_str());
				DrawBox(vHead.x - width, vHead.y, 4, std::clamp(100 * middle / 100, 0, middle) + 5, D3DCOLOR_RGBA(120, 120, 120, 250), D3DCOLOR_RGBA(120, 120, 120, 250), draw.GetDevice());
				DrawRect(vHead.x - width, vHead.y + middle - health_calc, 4, health_calc + 5, D3DCOLOR_RGBA(health_color.r, health_color.g, health_color.b, 255), draw.GetDevice());
			}
		}

		if (entity.actor_type == Abs) // Ability
		{
			if (menu.items[7].value == 1)
			{
				Vector3 vScreen = entity.GetWorldToScreenPosition();

				DrawRect(vScreen.x - (strlen("a") * 7 / 2) + (Rect.w / 2) - 1, vScreen.y, 5, 5, Cyan, draw.GetDevice());
				DrawString(vScreen.x - (strlen("Player") * 7 / 2) + (Rect.w / 2) - 1, vScreen.y + 7, entity.IsDormant() ? ASilverWhite : (entity.IsVisible() ? SilverWhite : SilverWhite), "Player");
				DrawString(vScreen.x - (strlen(dist_) * 7 / 2) + (Rect.w / 2) - 1, vScreen.y + 19, entity.IsDormant() ? ASilverWhite : (entity.IsVisible() ? SilverWhite : SilverWhite), dist_);
			}
		}

		if (entity.actor_type == sSpike) // Spike ESP
		{
			if (menu.items[8].value == 1)
			{
				float timer = read<float>(entity.actor_ptr + Offsets::SpikeTimer);
				if (timer < 0.2)
				{
					auto it = std::find(entityList.begin(), entityList.end(), entity);
					if (it != entityList.end())
					{
						entityList.erase(it);
					}
					continue;
				}

				char c_timer[64];

				Vector3 vScreen = entity.GetWorldToScreenPosition();

				float dis = (distance / 100);

				sprintf(c_timer, xorstr_("Spike [%.2fs]\n[%.fm]"), timer, dis);
				DrawString(vScreen.x - (strlen(c_timer) * 7 / 2), vScreen.y - 5, (timer > 7 ? Yellow : Red), c_timer);
				DrawLine(ScreenCenterX, ScreenCenterY + ScreenCenterY, vScreen.x, vScreen.y + 20, (dis > 35.5 ? Green : Grey), 1.f);
			}
		}
		
		if (entity.actor_type == DropedSpike)
		{
			if (menu.items[7].value == 1)
			{ // Spike wird bei aufheben meistens nicht gelöscht
				char text[256];

				Vector3 vScreen = entity.GetWorldToScreenPosition();

				sprintf(text, xorstr_("Dropped Spike [%.fm]"), (distance / 100));
				DrawString(vScreen.x - (strlen(text) * 7 / 2), vScreen.y - 5, White, text);
			}
		}

	/*	if (entity.actor_type == All)
		{
			int UniqueID = read<int>(entity.actor_ptr + Offsets::UniqueID);

			char all_txt[64];
			sprintf(all_txt, xorstr_("UniqueID: %d\nObjID: %d"), UniqueID, entity.object_id);

			Vector3 vScreen = entity.GetWorldToScreenPosition();
			DrawString(vScreen.x - (strlen(all_txt) * 7 / 2), vScreen.y - 5, Yellow, all_txt);
		} */
	}

	if (menu.items[14].value == 1) // Show FOV
	{
		Circle(ScreenCenterX, ScreenCenterY, (int)AimFOV, 50, SilverWhite);
	}
}

/*HRESULT DrawIndexedHook(ID3D11DeviceContext* Context, UINT IndexCount, UINT StartIndexLocation, INT BaseVertexLocation) {
	static ID3D11RasterizerState* CustomState;
	ID3D11Device* Device;
	ID3D11Buffer* VertexBuffer, * ConstantBuffer;
	UINT VertexWidth, VertexBufferOffset, ConstantWidth, Stride;

	Context->GetDevice(&Device);
	if (!CustomState)
	{
		D3D11_RASTERIZER_DESC Desc;
		Desc.AntialiasedLineEnable = 0;
		Desc.CullMode = D3D11_CULL_MODE::D3D11_CULL_NONE;
		Desc.DepthBiasClamp = 0.f;
		Desc.ScissorEnable = FALSE;
		Desc.FillMode = D3D11_FILL_MODE::D3D11_FILL_SOLID;
		Desc.DepthBias = INT_MAX;
		Desc.DepthClipEnable = TRUE;
		Desc.FrontCounterClockwise = FALSE;
		Device->CreateRasterizerState(&Desc, &CustomState);
	}

	Context->IAGetVertexBuffers(0, 1, &VertexBuffer, &Stride, &VertexBufferOffset);
	if (VertexBuffer) {
		D3D11_BUFFER_DESC VertexDesc;
		VertexBuffer->GetDesc(&VertexDesc);
		VertexWidth = VertexDesc.ByteWidth;
		VertexBuffer->Release();
	}

	Context->PSGetConstantBuffers(0, 1, &ConstantBuffer);
	if (ConstantBuffer) {
		D3D11_BUFFER_DESC ConstantDesc;
		ConstantBuffer->GetDesc(&ConstantDesc);
		ConstantWidth = ConstantDesc.ByteWidth;
		ConstantBuffer->Release();
	}

	if (Stride == 12 && ConstantWidth == 256 && VertexWidth - 20000 <= 380000 && CustomState) {
		ID3D11RasterizerState* OldState;
		Context->RSGetState(&OldState);
		Context->RSSetState(CustomState);
		(decltype(&DrawIndexedHook)(DrawIndexedOriginal))(Context, IndexCount, StartIndexLocation, BaseVertexLocation);
		Context->RSSetState(OldState);
	}

	return decltype(&DrawIndexedHook)(DrawIndexedOriginal)(Context, IndexCount, StartIndexLocation, BaseVertexLocation);
}

HRESULT PresentHook(IDXGISwapChain* SwapChain, UINT SyncInterval, UINT Flags) {
	ID3D11Device* Device;
	ID3D11DeviceContext* Context;
	SwapChain->GetDevice(__uuidof(ID3D11Device), (void**)&Device);
	Device->GetImmediateContext(&Context);
	if (Context) {
		uintptr_t* ContextVirtualTable = *(uintptr_t**)(Context);
		uintptr_t DrawIndexedFunction = ContextVirtualTable[12];

		MH_Initialize();
		MH_CreateHook((void*)DrawIndexedFunction, DrawIndexedHook, (void**)&DrawIndexedOriginal);
		MH_EnableHook((void*)DrawIndexedFunction);

		InterlockedExchangePointer((volatile PVOID*)DiscordPresentDataPtr, DiscordPresent);
	}

	return decltype(&PresentHook)(DiscordPresent)(SwapChain, SyncInterval, Flags);
}*/