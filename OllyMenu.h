#pragma once

#include <vector>
#include <string>
#include "ollydbg201.h"

class OllyMenu
{
public:

	OllyMenu();
	~OllyMenu();
	OllyMenu(const t_menu* menu);
	OllyMenu(const OllyMenu& menu);

	OllyMenu& operator=(OllyMenu menu);

	void clear();

	std::vector<t_menu> build() const;

	void add(const std::wstring& text, const std::wstring& help, int shortcut, MENUFUNC* callback, int index = 0);
	void add(const std::wstring& text, const std::wstring& help, int shortcut, const OllyMenu& menu);
	void add(const OllyMenu& menu);

	static const t_menu empty;
	static const t_menu divider;

private:

	static bool is_empty(const t_menu& item);
	static size_t menu_len(const t_menu* menu);

	void add(const t_menu& item);
	void add(const t_menu* menu, size_t count);
	void add(const t_menu* menu);

	static void free(const t_menu& item);
	static void free(const t_menu* menu, size_t count);
	static void free(const t_menu* menu);

	static t_menu dup(const t_menu& item);
	static t_menu* dup(const t_menu* menu, size_t count);
	static t_menu* dup(const t_menu* menu);

	std::vector<t_menu> items;
};
