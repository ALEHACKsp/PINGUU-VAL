#include <cstdint>

#if defined(__GNUC__)
typedef          long long ll;
typedef unsigned long long ull;
#define __int64 long long
#define __int32 int
#define __int16 short
#define __int8  char
#define MAKELL(num) num ## LL
#define FMT_64 "ll"
#elif defined(_MSC_VER)
typedef          __int64 ll;
typedef unsigned __int64 ull;
#define MAKELL(num) num ## i64
#define FMT_64 "I64"
#elif defined (__BORLANDC__)
typedef          __int64 ll;
typedef unsigned __int64 ull;
#define MAKELL(num) num ## i64
#define FMT_64 "L"
#else
#error "unknown compiler"
#endif
typedef unsigned int uint;
typedef unsigned char uchar;
typedef unsigned short ushort;
//typedef unsigned long ulong;

typedef          char   int8;
typedef   signed char   sint8;
typedef unsigned char   uint8;
typedef          short  int16;
typedef   signed short  sint16;
typedef unsigned short  uint16;
typedef          int    int32;
typedef   signed int    sint32;
typedef unsigned int    uint32;
typedef ll              int64;
typedef ll              sint64;
typedef ull             uint64;

// Partially defined types:
#define _BYTE  uint8
#define _WORD  uint16
#define _DWORD uint32
#define _QWORD uint64
#if !defined(_MSC_VER)
#define _LONGLONG __int128
#endif

#ifndef _WINDOWS_
typedef int8 BYTE;
typedef int16 WORD;
typedef int32 DWORD;
typedef int32 LONG;
#endif
typedef int64 QWORD;
#ifndef __cplusplus
typedef int bool;       // we want to use bool in our C programs
#endif

						// Some convenience macros to make partial accesses nicer
						// first unsigned macros:
#define LOBYTE(x)   (*((_BYTE*)&(x)))   // low byte
#define LOWORD(x)   (*((_WORD*)&(x)))   // low word
#define LODWORD(x)  (*((_DWORD*)&(x)))  // low dword
#define HIBYTE(x)   (*((_BYTE*)&(x)+1))
#define HIWORD(x)   (*((_WORD*)&(x)+1))
#define HIDWORD(x)  (*((_DWORD*)&(x)+1))
#define BYTEn(x, n)   (*((_BYTE*)&(x)+n))
#define WORDn(x, n)   (*((_WORD*)&(x)+n))
#define BYTE1(x)   BYTEn(x,  1)         // byte 1 (counting from 0)
#define BYTE2(x)   BYTEn(x,  2)
#define BYTE3(x)   BYTEn(x,  3)
#define BYTE4(x)   BYTEn(x,  4)
#define BYTE5(x)   BYTEn(x,  5)
#define BYTE6(x)   BYTEn(x,  6)
#define BYTE7(x)   BYTEn(x,  7)
#define BYTE8(x)   BYTEn(x,  8)
#define BYTE9(x)   BYTEn(x,  9)
#define BYTE10(x)  BYTEn(x, 10)
#define BYTE11(x)  BYTEn(x, 11)
#define BYTE12(x)  BYTEn(x, 12)
#define BYTE13(x)  BYTEn(x, 13)
#define BYTE14(x)  BYTEn(x, 14)
#define BYTE15(x)  BYTEn(x, 15)
#define WORD1(x)   WORDn(x,  1)
#define WORD2(x)   WORDn(x,  2)         // third word of the object, unsigned
#define WORD3(x)   WORDn(x,  3)
#define WORD4(x)   WORDn(x,  4)
#define WORD5(x)   WORDn(x,  5)
#define WORD6(x)   WORDn(x,  6)
#define WORD7(x)   WORDn(x,  7)

						// now signed macros (the same but with sign extension)
#define SLOBYTE(x)   (*((int8*)&(x)))
#define SLOWORD(x)   (*((int16*)&(x)))
#define SLODWORD(x)  (*((int32*)&(x)))
#define SHIBYTE(x)   (*((int8*)&(x)+1))
#define SHIWORD(x)   (*((int16*)&(x)+1))
#define SHIDWORD(x)  (*((int32*)&(x)+1))
#define SBYTEn(x, n)   (*((int8*)&(x)+n))
#define SWORDn(x, n)   (*((int16*)&(x)+n))
#define SBYTE1(x)   SBYTEn(x,  1)
#define SBYTE2(x)   SBYTEn(x,  2)
#define SBYTE3(x)   SBYTEn(x,  3)
#define SBYTE4(x)   SBYTEn(x,  4)
#define SBYTE5(x)   SBYTEn(x,  5)
#define SBYTE6(x)   SBYTEn(x,  6)
#define SBYTE7(x)   SBYTEn(x,  7)
#define SBYTE8(x)   SBYTEn(x,  8)
#define SBYTE9(x)   SBYTEn(x,  9)
#define SBYTE10(x)  SBYTEn(x, 10)
#define SBYTE11(x)  SBYTEn(x, 11)
#define SBYTE12(x)  SBYTEn(x, 12)
#define SBYTE13(x)  SBYTEn(x, 13)
#define SBYTE14(x)  SBYTEn(x, 14)
#define SBYTE15(x)  SBYTEn(x, 15)
#define SWORD1(x)   SWORDn(x,  1)
#define SWORD2(x)   SWORDn(x,  2)
#define SWORD3(x)   SWORDn(x,  3)
#define SWORD4(x)   SWORDn(x,  4)
#define SWORD5(x)   SWORDn(x,  5)
#define SWORD6(x)   SWORDn(x,  6)
#define SWORD7(x)   SWORDn(x,  7)


						// Helper functions to represent some assembly instructions.

#ifdef __cplusplus

						// Fill memory block with an integer value
inline void memset32(void* ptr, uint32 value, int count)
{
	uint32* p = (uint32*)ptr;
	for (int i = 0; i < count; i++)
		*p++ = value;
}

// Generate a reference to pair of operands
template<class T>  int16 __PAIR__(int8  high, T low) { return (((int16)high) << sizeof(high) * 8) | uint8(low); }
template<class T>  int32 __PAIR__(int16 high, T low) { return (((int32)high) << sizeof(high) * 8) | uint16(low); }
template<class T>  int64 __PAIR__(int32 high, T low) { return (((int64)high) << sizeof(high) * 8) | uint32(low); }
template<class T> uint16 __PAIR__(uint8  high, T low) { return (((uint16)high) << sizeof(high) * 8) | uint8(low); }
template<class T> uint32 __PAIR__(uint16 high, T low) { return (((uint32)high) << sizeof(high) * 8) | uint16(low); }
template<class T> uint64 __PAIR__(uint32 high, T low) { return (((uint64)high) << sizeof(high) * 8) | uint32(low); }

// rotate left
template<class T> T __ROL__(T value, uint count)
{
	const uint nbits = sizeof(T) * 8;
	count %= nbits;

	T high = value >> (nbits - count);
	value <<= count;
	value |= high;
	return value;
}

// rotate right
template<class T> T __ROR__(T value, uint count)
{
	const uint nbits = sizeof(T) * 8;
	count %= nbits;

	T low = value << (nbits - count);
	value >>= count;
	value |= low;
	return value;
}

// carry flag of left shift
template<class T> int8 __MKCSHL__(T value, uint count)
{
	const uint nbits = sizeof(T) * 8;
	count %= nbits;

	return (value >> (nbits - count)) & 1;
}

// carry flag of right shift
template<class T> int8 __MKCSHR__(T value, uint count)
{
	return (value >> (count - 1)) & 1;
}

// sign flag
template<class T> int8 __SETS__(T x)
{
	if (sizeof(T) == 1)
		return int8(x) < 0;
	if (sizeof(T) == 2)
		return int16(x) < 0;
	if (sizeof(T) == 4)
		return int32(x) < 0;
	return int64(x) < 0;
}

// overflow flag of subtraction (x-y)
template<class T, class U> int8 __OFSUB__(T x, U y)
{
	if (sizeof(T) < sizeof(U))
	{
		U x2 = x;
		int8 sx = __SETS__(x2);
		return (sx ^ __SETS__(y)) & (sx ^ __SETS__(x2 - y));
	}
	else
	{
		T y2 = y;
		int8 sx = __SETS__(x);
		return (sx ^ __SETS__(y2)) & (sx ^ __SETS__(x - y2));
	}
}

// overflow flag of addition (x+y)
template<class T, class U> int8 __OFADD__(T x, U y)
{
	if (sizeof(T) < sizeof(U))
	{
		U x2 = x;
		int8 sx = __SETS__(x2);
		return ((1 ^ sx) ^ __SETS__(y)) & (sx ^ __SETS__(x2 + y));
	}
	else
	{
		T y2 = y;
		int8 sx = __SETS__(x);
		return ((1 ^ sx) ^ __SETS__(y2)) & (sx ^ __SETS__(x + y2));
	}
}

// carry flag of subtraction (x-y)
template<class T, class U> int8 __CFSUB__(T x, U y)
{
	int size = sizeof(T) > sizeof(U) ? sizeof(T) : sizeof(U);
	if (size == 1)
		return uint8(x) < uint8(y);
	if (size == 2)
		return uint16(x) < uint16(y);
	if (size == 4)
		return uint32(x) < uint32(y);
	return uint64(x) < uint64(y);
}

// carry flag of addition (x+y)
template<class T, class U> int8 __CFADD__(T x, U y)
{
	int size = sizeof(T) > sizeof(U) ? sizeof(T) : sizeof(U);
	if (size == 1)
		return uint8(x) > uint8(x + y);
	if (size == 2)
		return uint16(x) > uint16(x + y);
	if (size == 4)
		return uint32(x) > uint32(x + y);
	return uint64(x) > uint64(x + y);
}

#else
						// The following definition is not quite correct because it always returns
						// uint64. The above C++ functions are good, though.
#define __PAIR__(high, low) (((uint64)(high)<<sizeof(high)*8) | low)
						// For C, we just provide macros, they are not quite correct.
#define __ROL__(x, y) __rotl__(x, y)      // Rotate left
#define __ROR__(x, y) __rotr__(x, y)      // Rotate right
#define __CFSHL__(x, y) invalid_operation // Generate carry flag for (x<<y)
#define __CFSHR__(x, y) invalid_operation // Generate carry flag for (x>>y)
#define __CFADD__(x, y) invalid_operation // Generate carry flag for (x+y)
#define __CFSUB__(x, y) invalid_operation // Generate carry flag for (x-y)
#define __OFADD__(x, y) invalid_operation // Generate overflow flag for (x+y)
#define __OFSUB__(x, y) invalid_operation // Generate overflow flag for (x-y)
#endif

						// No definition for rcl/rcr because the carry flag is unknown
#define __RCL__(x, y)    invalid_operation // Rotate left thru carry
#define __RCR__(x, y)    invalid_operation // Rotate right thru carry
#define __MKCRCL__(x, y) invalid_operation // Generate carry flag for a RCL
#define __MKCRCR__(x, y) invalid_operation // Generate carry flag for a RCR
#define __SETP__(x, y)   invalid_operation // Generate parity flag for (x-y)

						// In the decompilation listing there are some objects declarared as _UNKNOWN
						// because we could not determine their types. Since the C compiler does not
						// accept void item declarations, we replace them by anything of our choice,
						// for example a char:

#define _UNKNOWN char
struct uint128_t {
	uint64_t low;
	uint64_t high;
};
static uint8_t rol1(uint8_t x, unsigned int count) {
	count %= 8;
	return (x << count) | (x >> (8 - count));
}
static uint16_t __ROL2__(uint16_t x, unsigned int count) {
	count %= 16;
	return (x << count) | (x >> (16 - count));
}
static uint32_t __ROL4__(uint32_t x, unsigned int count) {
	count %= 32;
	return (x << count) | (x >> (32 - count));
}
static uint64_t rol8(uint64_t x, unsigned int count) {
	count %= 64;
	return (x << count) | (x >> (64 - count));
}
static uint8_t ror1(uint8_t x, unsigned int count) {
	count %= 8;
	return (x << (8 - count)) | (x >> count);
}
static uint16_t __ROR2__(uint16_t x, unsigned int count) {
	count %= 16;
	return (x << (16 - count)) | (x >> count);
}
static uint32_t __ROR4__(uint32_t x, unsigned int count) {
	count %= 32;
	return (x << (32 - count)) | (x >> count);
}
static uint64_t ror8(uint64_t x, unsigned int count) {
	count %= 64;
	return (x << (64 - count)) | (x >> count);
}
#ifdef _MSC_VER
#define snprintf _snprintf
#define vsnprintf _vsnprintf
#endif


//#define Lala(s) LI_FN(OutputDebugStringA)(##s);



//===========================================================================
typedef LONG(WINAPI* RealNtFlushInstructionCache)(HANDLE ProcessHandle, PVOID BaseAddress, ULONG NumberOfBytesToFlush);

RealNtFlushInstructionCache NewNtFlushInstructionCache = NULL;
//===========================================================================
typedef struct _PEB_LDR_DATA {
	UINT8 _PADDING_[12];
	LIST_ENTRY InLoadOrderModuleList;
	LIST_ENTRY InMemoryOrderModuleList;
	LIST_ENTRY InInitializationOrderModuleList;
} PEB_LDR_DATA, * PPEB_LDR_DATA;

typedef struct _PEB {
#ifdef _WIN64
	UINT8 _PADDING_[24];
#else
	UINT8 _PADDING_[12];
#endif
	PEB_LDR_DATA* Ldr;
} PEB, * PPEB;

typedef struct _LDR_DATA_TABLE_ENTRY {
	LIST_ENTRY InLoadOrderLinks;
	LIST_ENTRY InMemoryOrderLinks;
	LIST_ENTRY InInitializationOrderLinks;
	VOID* DllBase;
} LDR_DATA_TABLE_ENTRY, * PLDR_DATA_TABLE_ENTRY;

typedef struct _UNLINKED_MODULE
{
	HMODULE hModule;
	PLIST_ENTRY RealInLoadOrderLinks;
	PLIST_ENTRY RealInMemoryOrderLinks;
	PLIST_ENTRY RealInInitializationOrderLinks;
	PLDR_DATA_TABLE_ENTRY Entry;
} UNLINKED_MODULE;

#define UNLINK(x)					\
	(x).Flink->Blink = (x).Blink;	\
	(x).Blink->Flink = (x).Flink;

#define RELINK(x, real)			\
	(x).Flink->Blink = (real);	\
	(x).Blink->Flink = (real);	\
	(real)->Blink = (x).Blink;	\
	(real)->Flink = (x).Flink;

std::vector<UNLINKED_MODULE> UnlinkedModules;

struct FindModuleHandle
{
	HMODULE m_hModule;
	FindModuleHandle(HMODULE hModule) : m_hModule(hModule)
	{
	}
	bool operator() (UNLINKED_MODULE const& Module) const
	{
		return (Module.hModule == m_hModule);
	}
};

void RelinkModuleToPEB(HMODULE hModule)
{
	std::vector<UNLINKED_MODULE>::iterator it = std::find_if(UnlinkedModules.begin(), UnlinkedModules.end(), FindModuleHandle(hModule));

	if (it == UnlinkedModules.end())
	{
		//DBGOUT(TEXT("Module Not Unlinked Yet!"));
		return;
	}

	RELINK((*it).Entry->InLoadOrderLinks, (*it).RealInLoadOrderLinks);
	RELINK((*it).Entry->InInitializationOrderLinks, (*it).RealInInitializationOrderLinks);
	RELINK((*it).Entry->InMemoryOrderLinks, (*it).RealInMemoryOrderLinks);
	UnlinkedModules.erase(it);
}

void UnlinkModuleFromPEB(HMODULE hModule)
{
	std::vector<UNLINKED_MODULE>::iterator it = std::find_if(UnlinkedModules.begin(), UnlinkedModules.end(), FindModuleHandle(hModule));
	if (it != UnlinkedModules.end())
	{
		//DBGOUT(TEXT("Module Already Unlinked!"));
		return;
	}

#ifdef _WIN64
	PPEB pPEB = (PPEB)__readgsqword(0x60);
#else
	PPEB pPEB = (PPEB)__readfsdword(0x30);
#endif

	PLIST_ENTRY CurrentEntry = pPEB->Ldr->InLoadOrderModuleList.Flink;
	PLDR_DATA_TABLE_ENTRY Current = NULL;

	while (CurrentEntry != &pPEB->Ldr->InLoadOrderModuleList && CurrentEntry != NULL)
	{
		Current = CONTAINING_RECORD(CurrentEntry, LDR_DATA_TABLE_ENTRY, InLoadOrderLinks);
		if (Current->DllBase == hModule)
		{
			UNLINKED_MODULE CurrentModule = { 0 };
			CurrentModule.hModule = hModule;
			CurrentModule.RealInLoadOrderLinks = Current->InLoadOrderLinks.Blink->Flink;
			CurrentModule.RealInInitializationOrderLinks = Current->InInitializationOrderLinks.Blink->Flink;
			CurrentModule.RealInMemoryOrderLinks = Current->InMemoryOrderLinks.Blink->Flink;
			CurrentModule.Entry = Current;
			UnlinkedModules.push_back(CurrentModule);

			UNLINK(Current->InLoadOrderLinks);
			UNLINK(Current->InInitializationOrderLinks);
			UNLINK(Current->InMemoryOrderLinks);

			break;
		}

		CurrentEntry = CurrentEntry->Flink;
	}
}

bool HideThread(HANDLE hThread)
{
	__try
	{
		typedef NTSTATUS(NTAPI* pNtSetInformationThread)
			(HANDLE, UINT, PVOID, ULONG);
		NTSTATUS Status;

		// Get NtSetInformationThread
		pNtSetInformationThread NtSIT = (pNtSetInformationThread)
			GetProcAddress(GetModuleHandle(TEXT("ntdll.dll")),
				"NtSetInformationThread");

		// Shouldn't fail
		if (NtSIT == NULL)
			return false;

		// Set the thread info
		if (hThread == NULL)
			Status = NtSIT(GetCurrentThread(),
				0x11, // HideThreadFromDebugger
				0, 0);
		else
			Status = NtSIT(hThread, 0x11, 0, 0);

		if (Status != 0x00000000)
			return false;
		else
			return true;
	}
	__except (1)
	{
		return false;
	}	
}

struct vertex
{
	FLOAT x, y, z, rhw;
	DWORD color;
};

std::string GetWeaponName(int id)
{
	switch (id)
	{
	default: return std::to_string(id);
		break;
	case 12956131:  return "Knife";
		break;
	case 12820166:  return "Classic";
		break;
	case 12919640:  return "Shorty";
		break;
	case 12913101:  return "Sheriff";
		break;
	case 12896266:  return "Frenzy";
		break;
	case 12908345:  return "Ghost";
		break;
	case 12948574:  return "Stinger";
		break;
	case 12941076:  return "Spectre";
		break;
	case 12890155:  return "Bucky";
		break;
	case 12886546:  return "Judge";
		break;
	case 12873042:  return "Bulldog";
		break;
	case 12930576:  return "Guardian";
		break;
	case 12877198:  return "Phantom";
		break;
	case 12865969:  return "Vandal";
		break;
	case 12937485:  return "Marshall";
		break;
	case 12921596:  return "AWP";
		break;
	case 12861987:  return "Aries";
		break;
	case 12858759:  return "Odin";
		break;
	case 12770448:  return "Botgun"; //Out
		break;
	case 12707056: return "Spike"; //Out
		break;
		// Abilities (All Out)
	case 12771357: return "Dark Cover";
		break;
	case 12771685: return "Paranoia";
		break;
	case 12771536: return "Shrouded Step";
		break;
	case 12771834: return "Omen Ult";
		break;
	case 12764147: return "Curveball";
		break;
	case 12764365: return "Blaze";
		break;
	case 12764098: return "Hot Hands";
		break;
	case 12764562: return "Phoenix Ult";
		break;
	case 12752353: return "Boom Bot";
		break;
	case 12752622: return "Blast Pack";
		break;
	case 12752169: return "Paint Shells";
		break;
	case 12752791: return "Raze Ult";
		break;
	case 12770575: return "Leer";
		break;
	case 12771075: return "Reyna Ult";
		break;
	case 12769866: return "Barrier Orb";
		break;
	case 12769711: return "Slow Orb";
		break;
	case 12770111: return "Healing Orb";
		break;
	case 12770222: return "Sage Revive";
		break;

	case 12757823: return "Regrowth";
		break;
	case 12758255: return "Trailblazer";
		break;
	case 12757947: return "Guiding Light";
		break;
	case 12758663: return "Skye Ult";
		break;

	case 12760700: return "Owl Drone";
		break;
	case 12760276: return "Shock Bolt";
		break;
	case 12760761: return "Recon Bolt";
		break;
	case 12761025: return "Sova Ult";
		break;
	case 12763307: return "Snake Bite";
		break;
	case 12762876: return "Toxic Cloud";
		break;
	case 12763077: return "Toxic Screen";
		break;
	case 12763459: return "Viper Ult";
		break;

	case 12768838: return "Fakeout";
		break;
	case 12769283: return "Blindside";
		break;
	case 12768989: return "Gatecrash";
		break;
	case 12769387: return "Yuro Ult";
		break;

	case 12765901: return "Astra Form";
		break;
	case 12765093: return "Nebula";
		break;
	case 12764973: return "Gravity Well";
		break;
	case 12765290: return "Nova Pulse";
		break;


	case 12751597: return "Aftershock";
		break;
	case 12751852: return "Flashpoint";
		break;
	case 12751740: return "Fault Line";
		break;
	case 12751955: return "Breach Ult";
		break;

	case 12766368: return "Stim Beacon";
		break;
	case 12766500: return "Incendiary";
		break;
	case 12766167: return "Sky Smoke";
		break;
	case 12766636: return "Brim Ult";
		break;

	case 12754209: return "Trademark";
		break;
	case 12755121: return "Headhunter";
		break;
	case 12754665: return "Redezvous";
		break;
	case 12755577: return "Chamber Ult";
		break;

	case 12759140: return "Trapwire";
		break;
	case 12759035: return "Cyber Cage";
		break;
	case 12759520: return "Spycam";
		break;
	case 12759528: return "Spycam (Active)";
		break;
	case 12759947: return "Cypher Ult";
		break;

	case 12772269: return "Cloudburst";
		break;
	case 22772496: return "Updraft";
		break;
	case 12772453: return "Tailwind";
		break;
	case 12772540: return "Jett Ult";
		break;

	case 12757135: return "Fragment";
		break;
	case 12756739: return "Flashdrive";
		break;
	case 12756887: return "Zeropoint";
		break;
	case 12757281: return "KayO Ult";
		break;

	case 12761548: return "Nanoswarm";
		break;
	case 12762160: return "Alarmbot";
		break;
	case 12761753: return "Turret";
		break;
	case 12762428: return "KJ Ult";
		break;

	case 12767046: return "Fast Lane";
		break;
	case 12767357: return "Relay Bolt";
		break;
	case 12767202: return "High Gear";
		break;
	case 12767678: return "Neon Ult";
		break;

	case 12787079: return "Defuser";
		break;
	case 12773152: return "Orb";
		break;

	case 0: return "AFK";
		break;
	}
}

std::string GetCharacterName(int id)
{

	switch (id)
	{
	case 12814502: return "ASTRA"; // ok
		break;
	case 12821874: return "JETT"; // ok
		break;
	case 12803805: return "CHAMBER"; // ok
		break;
	case 12811102: return "KILLJOY"; // ok
		break;
	case 12801807: return "RAZE"; // ok
		break;
	case 12820224: return "REYNA"; // ok
		break;
	case 12819357: return "SAGE"; // ok
		break;
	case 12807475: return "SKYE"; // ok
		break;
	case 12816690: return "NEON"; // ok
		break;
	case 12812417: return "VIPER"; // ok
		break;
	case 12801214: return "BREACH"; // ok
		break;
	case 12815819: return "BRIMSTONE"; // ok
		break;
	case 12808684: return "CYPHER"; // ok
		break;
	case 12821015: return "OMEN"; // ok
		break;
	case 12813505: return "PHOENIX"; // ok
		break;
	case 12809896: return "SOVA"; // ok 
		break;
	case 12818495: return "YORU"; // ok
		break;
	case 12806381: return "KAY/O"; // ok
		break;
	case 12820177: return  "BOT"; // ok
		break;
	default:
		return std::to_string(id);
		break;
	}
}

int roundto127(int a)
{
	if (a < -127)
	{
		return -127;
	}
	else if (a > 127)
	{
		return 127;
	}
	else
	{
		return (int)a;
	}
}