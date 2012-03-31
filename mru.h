#pragma once

#include <windows.h>
#include "ollydbg201.h"
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

	std::vector<t_menu> build_menu(MENUFUNC* handler) const;

private:

	unsigned int max_size;
	std::vector<std::wstring> items;
	mutable std::vector<std::wstring> menu_strings;
};
