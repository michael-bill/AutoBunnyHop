#include <iostream>
#include <Windows.h>
#include <TlHelp32.h>

uintptr_t GetModuleBaseAddress(DWORD procId, const wchar_t* modName);
bool Flags();
const DWORD dwForceJump = 0x51F8EF4;
const DWORD dwLocalPlayer = 0xD3ABEC;
const DWORD m_fFlags = 0x104;
HANDLE processHandle;
DWORD baseAdress;
int localPlayer;
int flags;
int sizeOfInt;
const int FOUR = 4;
const int FIVE = 5;

int main()
{
	sizeOfInt = sizeof(int);
	HWND window = FindWindowA(0, "Counter-Strike: Global Offensive");
	if (window != 0) {
		std::cout << "Window found!" << '\n';
	}
	else {
		std::cout << "Window not found!" << '\n';
		return 0;
	}
	DWORD processId;
	GetWindowThreadProcessId(window, &processId);
	baseAdress = GetModuleBaseAddress(processId, L"client.dll");
	if (baseAdress > 0) {
		std::cout << "Base adress found!" << '\n';
	}
	else {
		std::cout << "Base adress not found!" << '\n';
		return 0;
	}
	processHandle = OpenProcess(PROCESS_ALL_ACCESS, false, processId);
	if (processHandle) {
		std::cout << "The process is open!" << "\n\n";
		std::cout << "BunnyHop is working..." << "\n";

		while (true) {
			if (GetAsyncKeyState(VK_SPACE) != 0) {
				if (Flags()) {
					WriteProcessMemory(processHandle, (LPVOID)(baseAdress + dwForceJump), &FOUR, 1, NULL);
				}
				else {
					WriteProcessMemory(processHandle, (LPVOID)(baseAdress + dwForceJump), &FIVE, 1, NULL);
				}
			}
		}
	}
	else {
		std::cout << "The process is not open!" << '\n';
		return 0;
	}
	return 0;
}

bool Flags() {
	ReadProcessMemory(processHandle, (LPCVOID)(baseAdress + dwLocalPlayer), &localPlayer, sizeOfInt, NULL);
	ReadProcessMemory(processHandle, (LPCVOID)(localPlayer + m_fFlags), &flags, sizeOfInt, NULL);
	if (flags == 256) return true;
	else return false;
}

uintptr_t GetModuleBaseAddress(DWORD procId, const wchar_t* modName)
{
	uintptr_t modBaseAddr = 0;
	HANDLE hSnap = CreateToolhelp32Snapshot(TH32CS_SNAPMODULE | TH32CS_SNAPMODULE32, procId);
	if (hSnap != INVALID_HANDLE_VALUE)
	{
		MODULEENTRY32 modEntry;
		modEntry.dwSize = sizeof(modEntry);
		if (Module32First(hSnap, &modEntry))
		{
			do
			{
				if (!_wcsicmp(modEntry.szModule, modName))
				{
					modBaseAddr = (uintptr_t)modEntry.modBaseAddr;
					break;
				}
			} while (Module32Next(hSnap, &modEntry));
		}
	}
	CloseHandle(hSnap);
	return modBaseAddr;
}