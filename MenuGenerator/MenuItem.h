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
	MenuItem(int index);
	~MenuItem();
	MenuItem operator=(const MenuItem& source);
	QString ExportStr();

	int index;
	int prev_index;
	int sub_index[5];
	MENUTYPE type;
	int min;
	int max;
	int step;
	bool used;
	QString title;


};