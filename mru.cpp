#include "mru.h"

#include "version.h"
#include <shlwapi.h>
#include <algorithm>
#include <sstream>

MRU::MRU(unsigned int max_size) : max_size(max_size)
{
	items.reserve(max_size);
	menu_strings.reserve(max_size);
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

std::vector<t_menu> MRU::build_menu(MENUFUNC* handler) const
{
	menu_strings.clear();
	std::vector<t_menu> menu;

	for(size_t i = 0; i < items.size(); i++)
	{
		std::wstring file = items[i];

		if(!file.empty() && PathFileExists(file.c_str()))
		{
			menu_strings.push_back(file);

			t_menu item =
			{
				const_cast<wchar_t*>(menu_strings.rbegin()->c_str()), 
				0,
				K_NONE,
				handler,
				NULL,
				i
			};

			menu.push_back(item);
		}
	}

	return menu;
}

//NOTE : OllyDBG Main Menu is Static... so we need to store MRU for next OllyDbg start

/*
void mruAddFile(std::wstring filePath)
{
	wchar_t buf[4096] = {0};
	wchar_t key[] = L"NRU ";

	for(int i = 1; i <= 9; i++)
	{
		memset(&buf, 0, sizeof(buf));

		key[3] = '0' + i;
		Getfromini(NULL, PLUGIN_NAME, key, L"%s", buf);

		if (wcscmp(buf, filePath.c_str()) == 0) //Move File to first MRU
		{
			key[3] = '1';
			Getfromini(NULL, PLUGIN_NAME, key, L"%s", buf);
			Writetoini(NULL, PLUGIN_NAME, key, L"%s", filePath.c_str());
			key[3] = '0' + i;
			Writetoini(NULL, PLUGIN_NAME, key, L"%s", buf);
			return;
		}
	}

	for(int i = 9; i > 0; i--)
	{
		memset(&buf, 0, sizeof(buf));

		//Add File then Move others
		key[3] = '0' + i;
		Getfromini(NULL, PLUGIN_NAME, key, L"%s", buf);
		if (wcslen(buf) > 0)
		{
			key[3] = '0' + i + 1;
			Writetoini(NULL, PLUGIN_NAME, key, L"%s", buf);
		}
	}

	key[3] = '1';
	Writetoini(NULL, PLUGIN_NAME, key, L"%s", filePath.c_str());
}
*/
//ON MAIN MENU, ITS CALLED ONCE
/*
int  mruGetMenu(char* buf) {

	char buf2[4096] = {0};
	char key[5];
	char key2[5];
	int p=0;
	int c,v;
	string path;

	strcpy(key,"NRU ");
	strcpy(key2,"MRU ");

 	for(int n=1; n<=5; n++) {
		key[3]=n+0x30; //ASCII n
		key2[3]=key[3];

		ZeroMemory(&buf2, sizeof(buf2));
		Getfromini(NULL,PLUGIN_NAME, key, L"%s", buf2)
		Pluginwritestringtoini(0,key2, buf2);
		if (strlen(buf2)) {
			if (PathFileExists(buf2)) {
				buf[p]=0x32;   p++;
				buf[p]=key[3]; p++;
				buf[p]=0x20;   p++;

				path=buf2;
				c=path.rfind('\\') + 1;

				while ( (v = strchr(&buf2[c],',')-&buf2[c]) > 0) {
					buf2[c+v]='.';
				}
				while ( (v = strchr(&buf2[c],'{')-&buf2[c]) > 0) {
					buf2[c+v]='[';
				}
				while ( (v = strchr(&buf2[c],'}')-&buf2[c]) > 0) {
					buf2[c+v]=']';
				}

				strcpy(&buf[p],&buf2[c]); p+=strlen(&buf2[c]);
				buf[p]=',';p++;
			}
		}
	}
	if (p>0) buf[--p]=0;

	return p;
}
*/
/*
//ON DISASM WINDOW, ITS CALLED ON CONTEXT MENU
int mruGetCurrentMenu(wchar_t* buf)
{

	wchar_t buf2[4096] = {0};
	wchar_t key[5];
	int p = 0;
	int c, v;
	wstring path;

	wcscpy_s(key, L"NRU ");

	for(int n = 1; n <= 9; n++)
	{
		key[3] = n + 0x30; //ASCII n

		ZeroMemory(&buf2, sizeof(buf2));
		Getfromini(NULL, PLUGIN_NAME, key, L"%s", buf2);
		if (wcslen(buf2))
		{
			if (PathFileExists(buf2))
			{
				buf[p] = 0x32;
				p++;
				buf[p] = key[3];
				p++;
				buf[p] = 0x20;
				p++;

				path = buf2;
				c = path.rfind('\\') + 1;

				while ( (v = wcschr(&buf2[c], ',') - &buf2[c]) > 0)
				{
					buf2[c + v] = '.';
				}
				while ( (v = wcschr(&buf2[c], '{') - &buf2[c]) > 0)
				{
					buf2[c + v] = '[';
				}
				while ( (v = wcschr(&buf2[c], '}') - &buf2[c]) > 0)
				{
					buf2[c + v] = ']';
				}

				wcscpy(&buf[p], &buf2[c]);
				p += wcslen(&buf2[c]);
				buf[p] = ',';
				p++;
			}
		}
	}
	if (p > 0) buf[--p] = 0;

	return p;
}

vector<t_menu> mruBuildMenu()
{
	vector<t_menu> menu;

	wchar_t buf[4096] = {0};
	wchar_t key[] = L"NRU ";

	int p = 0;
	int c, v;

	wstring path;

	for(int i = 1; i <= 9; i++)
	{
		memset(&buf, 0, sizeof(buf));

		key[3] = '0' + i;
		Getfromini(NULL, PLUGIN_NAME, key, L"%s", buf);

		if (wcslen(buf) > 0)
		{
			if (PathFileExists(buf))
			{
				t_menu item =
				{
					0, 
					0,
					K_NONE,
					NULL,
					NULL,
					i
				};

				menu.push_back(item);
			}
		}
	}

	return menu;
}

int mruGetCurrentMenu(HMENU mmru, int cmdIndex)
{

	wchar_t buf2[4096] = {0};
	wchar_t key[5];
	int c, v;
	wstring path;

	wcscpy_s(key, L"NRU ");

	for(int n = 1; n <= 9; n++)
	{
		key[3] = n + 0x30; //ASCII n

		ZeroMemory(&buf2, sizeof(buf2));
		Getfromini(NULL, PLUGIN_NAME, key, L"%s", buf2);
		if (wcslen(buf2))
		{
			if (PathFileExists(buf2))
			{
				path = buf2;
				c = path.rfind('\\') + 1;
				AppendMenu(mmru, MF_STRING, cmdIndex + n, &buf2[c]);
			}
		}
	}

	return 1;
}


int mruCmdMenu(HMENU mmru, int cmdIndex)
{

	AppendMenu(mmru, MF_STRING, cmdIndex + 1, L"ESP¶¨ÂÉ");
	return 1;
}

//MessageBox(hwmain,buf,"",MB_OK|MB_ICONEXCLAMATION|MB_TOPMOST);
*/
