#include "MenuItem.h"



MenuItem::MenuItem(int index, QString title)
{
	this->index = index;
	this->func_index = 0;
	this->max = 0;
	this->min = 0;
	this->para_index = 0;
	this->step = 0;
	this->sub_index[0] = this->sub_index[1]
		= this->sub_index[2] = this->sub_index[3]
		= this->sub_index[4] = 0;
	this->title = title;
	this->type = MENUTYPE_MENU;
}

MenuItem::MenuItem(QJsonObject json)
{
	QJsonArray sub_array = json.value("data").toArray();
	index = json.value("index").toInt();
	title = json.value("title").toString();
	switch (json.value("type").toInt())
	{
	case 0:
		type = MENUTYPE_MENU;
		sub_index[0] = sub_array[0].toInt();
		sub_index[1] = sub_array[1].toInt();
		sub_index[2] = sub_array[2].toInt();
		sub_index[3] = sub_array[3].toInt();
		sub_index[4] = sub_array[4].toInt();
		func_index = 0;
		max = 0;
		min = 0;
		para_index = 0;
		step = 0;
		break;
	case 1:
		type = MENUTYPE_FUNC;
		sub_index[0] = 0;
		sub_index[1] = 0;
		sub_index[2] = 0;
		sub_index[3] = 0;
		sub_index[4] = 0;
		func_index = sub_array[0].toInt();
		max = 0;
		min = 0;
		para_index = 0;
		step = 0;
		break;
	case 2:
		type = MENUTYPE_INT;
		sub_index[0] = 0;
		sub_index[1] = 0;
		sub_index[2] = 0;
		sub_index[3] = 0;
		sub_index[4] = 0;
		func_index = 0;
		para_index = sub_array[0].toInt();
		min = sub_array[1].toInt();
		max = sub_array[2].toInt();
		step = sub_array[3].toInt();
		break;
	case 3:
		type = MENUTYPE_BOOL;
		sub_index[0] = 0;
		sub_index[1] = 0;
		sub_index[2] = 0;
		sub_index[3] = 0;
		sub_index[4] = 0;
		func_index = 0;
		para_index = sub_array[0].toInt();
		min = 0;
		max = 0;
		step = 0;
		break;
	}
}

MenuItem::~MenuItem()
{
}

MenuItem MenuItem::operator=(const MenuItem & source)
{
	MenuItem target(source.index, source.title);
	target.max = source.max;
	target.min = source.min;
	target.step = source.step;
	for(int i = 0;i<5;i++)
		target.sub_index[i] = source.sub_index[i];
	target.type = source.type;
	return target;
}

QString MenuItem::ExportItemStr()
{
	QString Str;
	Str += "  { .Title = \"";
	Str += title;
	Str += "\", .Type = ";
	Str += QString::number(type);
	Str += ", .Submenu = {";
	switch (type)
	{
	case MENUTYPE_MENU:
		for (int j = 0; j < 5; j++)
		{
			Str += QString::number(sub_index[j]);
			if (j != 4)
				Str += ",";
		}
		break;
	case MENUTYPE_FUNC:
		Str += QString::number(func_index);
		Str += ",0,0,0,0";
		break;
	case MENUTYPE_BOOL:
		Str += QString::number(para_index);
		Str += ",0,0,0,0";
		break;
	case MENUTYPE_INT:
		Str += QString::number(para_index);
		Str += ",";
		Str += QString::number(min);
		Str += ",";
		Str += QString::number(max);
		Str += ",";
		Str += QString::number(step);
		Str += ",0";
		break;
	}
	Str += "} }";
	return Str;
}

QJsonObject MenuItem::ExportItemJson()
{
	QJsonObject json;
	QJsonArray sub_array;
	json.insert("title",title);
	json.insert("index", index);
	switch (type)
	{
	case MENUTYPE_MENU:
		json.insert("type", 0);
		for (int i = 0; i < 5; i++)
			sub_array.append(sub_index[i]);
		break;
	case MENUTYPE_FUNC:
		json.insert("type", 1);
		sub_array.append(func_index);
		for (int i = 1; i < 5; i++)
			sub_array.append(0);
		break;
	case MENUTYPE_INT:
		json.insert("type", 2);
		sub_array.append(para_index);
		sub_array.append(min);
		sub_array.append(max);
		sub_array.append(step);
		sub_array.append(0);
		break;
	case MENUTYPE_BOOL:
		json.insert("type", 3);
		sub_array.append(para_index);
		for (int i = 1; i < 5; i++)
			sub_array.append(0);
		break;
	}
	json.insert("data", sub_array);
	return json;
}
