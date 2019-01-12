#include "MenuItem.h"



MenuItem::MenuItem(int index, QString title)
{
	this->index = index;
	this->func_index = 0;
	this->max = 0;
	this->min = 0;
	this->para_index = 0;
	this->prev_index = 0;
	this->step = 0;
	this->sub_index[0] = this->sub_index[1]
		= this->sub_index[2] = this->sub_index[3]
		= this->sub_index[4] = 0;
	this->title = title;
	this->type = MENUTYPE_MENU;
}


MenuItem::~MenuItem()
{
}

MenuItem MenuItem::operator=(const MenuItem & source)
{
	MenuItem target(source.index, source.title);
	target.max = source.max;
	target.min = source.min;
	target.prev_index = source.prev_index;
	target.step = source.step;
	for(int i = 0;i<5;i++)
		target.sub_index[i] = source.sub_index[i];
	target.type = source.type;
	return target;
}

QString MenuItem::ExportItemStr()
{
	QString Str;
	Str += "{\"";
	Str += this->title;
	Str += "\",";
	Str += QString::number(this->prev_index);
	Str += ",";
	switch (this->type)
	{
	case MENUTYPE_MENU:
		Str += "MENUTYPE_MENU";
		for (int i = 0; i < 5; i++)
		{
			Str += ",";
			Str += QString::number(this->sub_index[i]);
		}
		break;
	case MENUTYPE_FUNC:
		Str += "MENUTYPE_FUNC,";
		Str += QString::number(this->func_index);
		Str += ",0,0,0,0";
		break;
	case MENUTYPE_INT:
		Str += "MENUTYPE_INT,";
		Str += QString::number(this->para_index);
		Str += ",";
		Str += QString::number(this->min);
		Str += ",";
		Str += QString::number(this->max);
		Str += ",";
		Str += QString::number(this->step);
		Str += ",0";
		break;
	case MENUTYPE_BOOL:
		Str += "MENUTYPE_BOOL,";
		Str += QString::number(this->para_index);
		Str += ",0,0,0,0";
		break;
	default:
		throw("Error:Unexpected menutype");
		break;
	}
	Str += "}\n";
	return Str;
}
