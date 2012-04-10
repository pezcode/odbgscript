#pragma once

#include <windows.h>
#include "ollydbg201.h"
#include "OllyMenu.h"
#include <string>
#include <vector>

class MRU
{
public:

	MRU(unsigned int max_size);

	unsigned int size() const;

	bool load();
	bool save() const;
	void clear();

	std::wstring get(int i) const;
	bool add(const std::wstring& file);
	bool remove(const std::wstring& file);

	OllyMenu build_menu(MENUFUNC* handler) const;

private:

	unsigned int max_size;
	std::vector<std::wstring> items;
};
