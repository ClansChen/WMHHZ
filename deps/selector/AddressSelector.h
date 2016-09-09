#pragma once
#include <windows.h>
#include <cstdint>

template <std::uintptr_t... __Addresses>
struct AddressesList {};

class AddressSelectorBase
{
public:
	enum class GameVersion
	{
		GV_UNINITIALIZED,
		GV_10,
		GV_11,
		GV_STEAM,
		GV_UNK,
	};

protected:
	static GameVersion ms_gv;
};

template <typename __List10, typename __List11, typename __ListSTEAM>
class AddressSelector;

template <std::uintptr_t... __EP10, std::uintptr_t... __EP11, std::uintptr_t... __EPSTEAM>
class AddressSelector<AddressesList<__EP10...>, AddressesList<__EP11...>, AddressesList<__EPSTEAM...> > :public AddressSelectorBase
{
private:
	static bool ListContainsAddress(std::uintptr_t address, AddressesList<>)
	{
		return false;
	}

	template <std::uintptr_t __First, std::uintptr_t...__Rest>
	static bool ListContainsAddress(std::uintptr_t address, AddressesList<__First, __Rest...>)
	{
		return ((address == __First) || (ListContainsAddress(address, AddressesList<__Rest...>())));
	}

	static void Init()
	{
		std::uintptr_t base = (std::uintptr_t)GetModuleHandleW(NULL);
		IMAGE_DOS_HEADER *dos = (IMAGE_DOS_HEADER *)(base);
		IMAGE_NT_HEADERS *nt = (IMAGE_NT_HEADERS *)(base + dos->e_lfanew);

		DWORD ep = nt->OptionalHeader.AddressOfEntryPoint + 0x400000;

		if (ListContainsAddress(ep, AddressesList<__EP10...>()))
		{
			ms_gv = GameVersion::GV_10;
		}
		else if (ListContainsAddress(ep, AddressesList<__EP11...>()))
		{
			ms_gv = GameVersion::GV_11;
		}
		else if (ListContainsAddress(ep, AddressesList<__EPSTEAM...>()))
		{
			ms_gv = GameVersion::GV_STEAM;
		}
		else
		{
			ms_gv = GameVersion::GV_UNK;
		}
	}

public:
	template <std::uintptr_t __Addr10, std::uintptr_t __Addr11, std::uintptr_t __AddrSteam, typename __DestType = void>
	static __DestType *SelectAddress()
	{
		if (ms_gv == GameVersion::GV_UNINITIALIZED)
		{
			Init();
		}

		if (ms_gv == GameVersion::GV_10)
		{
			return (reinterpret_cast<__DestType *>(__Addr10));
		}
		if (ms_gv == GameVersion::GV_11)
		{
			return (reinterpret_cast<__DestType *>(__Addr11));
		}
		else if (ms_gv == GameVersion::GV_STEAM)
		{
			return (reinterpret_cast<__DestType *>(__AddrSteam));
		}
		else
		{
			return nullptr;
		}
	}
};

typedef AddressSelector<AddressesList<0x5C1E70>, AddressesList<0x5C2130>, AddressesList<0x9912ED, 0x5C6FD0> > AddressSelectorLC;
typedef AddressSelector<AddressesList<0x667BF0>, AddressesList<0x667C40>, AddressesList<0xA402ED, 0x666BA0> > AddressSelectorVC;
typedef AddressSelector<AddressesList<0x82457C, 0x824570>, AddressesList<0x8252FC>, AddressesList<0x858EA8> > AddressSelectorSAUS;