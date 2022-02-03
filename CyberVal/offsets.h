namespace Offsets
{
	uintptr_t oKey = 0x8FB6978; // 4.02
	uintptr_t oState = 0x8FB6940; // 4.02

	uintptr_t oLevel = 0x38;//ok
	uintptr_t oGameInstance = 0x1A8;//ok
	uintptr_t oLocalPlayers = 0x40;
	uintptr_t oPlayerController = 0x38;//ok
	uintptr_t oLocalPawn = 0x460;//ok
	uintptr_t RootComp = 0x210 + 0x20;//ok
	uintptr_t oCameraManager = 0x458 + 0x20;//ok
	uintptr_t oCameraCache = 0x1220 + 0x20;//ok
	uintptr_t ObjID = 0x18;
	uintptr_t Dormant = 0x120;
	uintptr_t IsDormant = 0x0D8;
	uintptr_t RelativeLocation = 0x164;//ok
	uintptr_t RelativeRotation = 0x170;//ok
	uintptr_t PlayerState = 0x3F0;//ok
	uintptr_t Ping = 0x3B8;
	uintptr_t TeamComponent = 0x5A0;//ok
	uintptr_t Team = 0xF8;//ok
	uintptr_t DamageHandler = 0x978 + 0x20;//ok
	uintptr_t Health = 0x1B0;
	uintptr_t Mesh = 0x410 + 0x20;//ok
	uintptr_t StaticMesh = 0x558; //0x518;
	uintptr_t StaticMesh_Cached = 0x568;//0x528;
	uintptr_t ComponentToWorld = 0x250;//ok
	uintptr_t UniqueID = 0x38;
	uintptr_t ControlRotation = 0x440;//ok
	uintptr_t SpikeTimer = 0x4D4;
	uintptr_t BoneCount = 0x560;
	uintptr_t last_render_time = 0x350;
	uintptr_t last_submit_time = 0x358;
	uintptr_t Inventory = 0x940;
	uintptr_t CurrentWeapon = 0x218;
};