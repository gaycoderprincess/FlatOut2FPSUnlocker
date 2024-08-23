#include <windows.h>
#include "toml++/toml.hpp"
#include "nya_commonhooklib.h"

auto FMODChannels1_call = (void(__stdcall*)(int))0x619F15;
void __stdcall FMODChannels1(int a1) {
	FMODChannels1_call(1024);
}

auto FMODChannels2_call = (void(__stdcall*)(int, int, int))0x61A3A9;
void __stdcall FMODChannels2(int a1, int a2, int a3) {
	FMODChannels2_call(a1, 1024, a3);
}

auto ProcessRaceAudioHooked_call = (void(__stdcall*)(void*, int, void*, int))0x4721C0;
void __stdcall ProcessRaceAudioHooked(void* a1, int a2, void* a3, int a4) {
	// don't process audio on repeat frames
	if (a4 <= 0) return;

	ProcessRaceAudioHooked_call(a1, a2, a3, a4);
}

uintptr_t DisableVSyncASM_jmp = 0x5A5D77;
void __attribute__((naked)) DisableVSyncASM() {
	__asm__ (
		"push edi\n\t"
		"mov edi, 0x80000000\n\t" // D3DPRESENT_INTERVAL_IMMEDIATE
		"mov dword ptr [0x8DA444], edi\n\t"
		"pop edi\n\t"
		"jmp %0\n\t"
			:
			: "m" (DisableVSyncASM_jmp)
	);
}

BOOL WINAPI DllMain(HINSTANCE, DWORD fdwReason, LPVOID) {
	switch( fdwReason ) {
		case DLL_PROCESS_ATTACH: {
			if (NyaHookLib::GetEntryPoint() != 0x202638) {
				MessageBoxA(nullptr, "Unsupported game version! Make sure you're using DRM-free v1.2 (.exe size of 2990080 bytes)", "nya?!~", MB_ICONERROR);
				exit(0);
				return TRUE;
			}

			auto config = toml::parse_file("FlatOut2FPSUnlocker_gcp.toml");
			bool bRemoveVSync = config["main"]["remove_vsync"].value_or(false);

			NyaHookLib::PatchRelative(NyaHookLib::CALL, 0x42077B, &FMODChannels1);
			NyaHookLib::PatchRelative(NyaHookLib::CALL, 0x4207AA, &FMODChannels2);

			NyaHookLib::PatchRelative(NyaHookLib::CALL, 0x479400, &ProcessRaceAudioHooked);
			NyaHookLib::Patch<uint8_t>(0x45F8CF, 0xEB); // do repeat frames when faster than sim tick
			if (bRemoveVSync) {
				NyaHookLib::PatchRelative(NyaHookLib::JMP, 0x5A5D71, &DisableVSyncASM);
			}
		} break;
		default:
			break;
	}
	return TRUE;
}