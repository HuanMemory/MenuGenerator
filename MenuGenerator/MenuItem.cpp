#include "MenuItem.h"



MenuItem::MenuItem(int index)
{
	this->index = index;
}


MenuItem::~MenuItem()
{
}

MenuItem MenuItem::operator=(const MenuItem & source)
{
	MenuItem target(source.index);
	target.max = source.max;
	target.min = source.min;
	target.prev_index = source.prev_index;
	target.step = source.step;
	for(int i = 0;i<5;i++)
		target.sub_index[i] = source.sub_index[i];
	target.title = source.title;
	target.type = source.type;
	target.used = source.used;
	return target;
}

QString MenuItem::ExportStr()
{
	QString Str;

	return Str;
}
