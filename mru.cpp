#include "mru.h"

#include "version.h"
#include <shlwapi.h>
#include <algorithm>
#include <sstream>

MRU::MRU(unsigned int max_size) : max_size(max_size)
{
	items.reserve(max_size);
}

unsigned int MRU::size() const
{
	return items.size();
}

bool MRU::load()
{
	wchar_t buf[MAX_PATH];

	clear();

	for(size_t i = 0; i < max_size; i++)
	{
		std::wostringstream key;
		key << "MRU" << i + 1;
		buf[0] = L'\0';
		Getfromini(NULL, PLUGIN_NAME, const_cast<wchar_t*>(key.str().c_str()), L"%260s", buf);
		buf[_countof(buf) - 1] = L'\0';
		if(wcslen(buf) > 0 && PathFileExists(buf))
		{
			items.push_back(buf);
		}
	}	

	return true;
}

bool MRU::save() const
{
	for(size_t i = 0; i < max_size; i++)
	{
		std::wostringstream key;
		key << "MRU" << i + 1;

		std::wstring val = get(i);
		Writetoini(NULL, PLUGIN_NAME, const_cast<wchar_t*>(key.str().c_str()), L"%s", val.c_str());
	}

	return true;
}

void MRU::clear()
{
	items.clear();
}

std::wstring MRU::get(int i) const
{
	std::wstring val;

	if(i >= 0 && i < items.size())
	{
		val = items[i];
	}

	return val;
}

bool MRU::add(const std::wstring& file)
{
	std::vector<std::wstring>::iterator it = std::find(items.begin(), items.end(), file);
	if(it != items.end())
	{
		if(it != items.begin())
		{
			std::swap(items.front(), *it);
		}
	}
	else
	{
		items.insert(items.begin(), file);
		if(items.size() >= max_size)
		{
			items.resize(max_size);
		}
	}

	return true;
}

bool MRU::remove(const std::wstring& file)
{
	std::vector<std::wstring>::iterator it = find(items.begin(), items.end(), file);
	if(it != items.end())
	{
		items.erase(it);
		return true;
	}

	return false;
}

OllyMenu MRU::build_menu(MENUFUNC* handler) const
{
	OllyMenu menu;

	for(size_t i = 0; i < items.size(); i++)
	{
		std::wstring file = items[i];

		if(!file.empty() && PathFileExists(file.c_str()))
		{
			menu.add(file, L"", K_NONE, handler, i);
		}
	}

	return menu;
}
