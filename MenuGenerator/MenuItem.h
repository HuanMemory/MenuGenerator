#pragma once
#include <qstring.h>
#include <QJsonObject>
#include <QJsonArray>

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
	MenuItem(int index, QString title);
	MenuItem(QJsonObject json);
	~MenuItem();
	MenuItem operator=(const MenuItem& source);
	QString ExportItemStr();
	QJsonObject ExportItemJson();

	int index;
	MENUTYPE type;
	int sub_index[5];
	int func_index;
	int para_index;
	int min;
	int max;
	int step;
	QString title;
};