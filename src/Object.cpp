#include "Object.h"

Object::Object(Point3D pos, float scale, ObjectType type, std::string lab) {
  this->position = pos;
  //std::cout << pos.x << std::endl;

  this->scale = scale;
  //std::cout << this->scale << std::endl;

  this->type = type;

  this->selected = false;

  label = new Label(pos, lab);
  //std::cout << label->str << std::endl;
}

void Object::select(bool s) {
  this->selected = s;
  if (s) {
    this->label->color.x = 0.0;
    this->label->color.y = 1.0;
    this->label->color.z = 0.0;
  }

  else {
    this->label->color.x = 1.0;
    this->label->color.y = 1.0;
    this->label->color.z = 1.0;
  }
}

bool Object::isSelected() {
  return selected;
}
