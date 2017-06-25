#ifndef OBJECT_H
#define OBJECT_H

#include <stdio.h>
#include <iostream>
#include <string>
#include "Point3D.h"

enum ObjectType {
	Cube,
	LightSource,
	Teapot
};

class Object {
	class Label {
	public:
		std::string str;
		Point3D pos;
		Point3D color;

		Label(Point3D pos, std::string str) {
			this->str = str;

			this->pos.x = pos.x + 0.0;
			this->pos.y = pos.y + 10.0;
			this->pos.z = pos.z + 0.0;

			this->color.x = this->color.y = this->color.z = 1.0;
		}
	};

	bool selected;

public:
	Label *label;
	Point3D position;
	float scale;
	ObjectType type;

	Object(Point3D pos, float scale, ObjectType type, std::string lab);
	void select(bool s);
	bool isSelected();
};

#endif
