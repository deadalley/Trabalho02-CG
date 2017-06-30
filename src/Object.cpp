#include "Object.h"

Object::Object(Point3D pos, float scale, ObjectType type, std::string lab) {
  this->position = pos;
  //std::cout << pos.x << std::endl;

  this->scale = scale;
  //std::cout << this->scale << std::endl;

  this->type = type;

  this->selected = false;

  this->rotation.x = 0;
  this->rotation.y = 0;
  this->rotation.z = 0;

  // label = new Label(pos, lab);
  //std::cout << label->str << std::endl;
}

void Object::select(bool s) {
  this->selected = s;
}

bool Object::isSelected() {
  return selected;
}

void Object::setPosition(Point3D pos){
  this->position = pos;
}
