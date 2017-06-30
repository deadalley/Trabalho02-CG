#ifndef SHADER_LOADER_H
#define SHADER_LOADER_H

#include <iostream>
#include <string>
#include <fstream>

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
using namespace glm;
using namespace std;
#include <GL/glut.h>
#include <GL/freeglut.h>

class ShaderLoader {
protected:
  GLuint loadShaderFromFile(string path, GLenum type);
  GLuint mProgramID;

public:
  string vsPath, fsPath;
  ShaderLoader();

  bool loadProgram();
  void freeProgram();
  bool bind();
  void unbind();
  GLuint getProgramID();
};

#endif
