#pragma once
#include <qstring.h>
enum MENUTYPE
{
	MENUTYPE_MENU = 0,
	MENUTYPE_FUNC,
	MENUTYPE_INT,
	MENUTYPE_BOOL
};

class MenuItem
{
public:
	MenuItem(int index,QString title);
	~MenuItem();
	MenuItem operator=(const MenuItem& source);
	QString ExportItemStr();

	int index;
	int prev_index;
	MENUTYPE type;
	int sub_index[5];
	int func_index;
	int para_index;
	int min;
	int max;
	int step;
	bool used;
	QString title;

};