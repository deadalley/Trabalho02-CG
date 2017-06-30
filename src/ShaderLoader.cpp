#include "ShaderLoader.h"

ShaderLoader::ShaderLoader() {
  mProgramID = NULL;
}

void ShaderLoader::freeProgram() {
  glDeleteProgram(mProgramID);
}

bool ShaderLoader::bind() {
  glUseProgram(mProgramID);

  GLenum err = glGetError();
  if (err != GL_NO_ERROR) {
    cout << "Error binding shader. " << gluErrorString(err) << endl;
    return false;
  }

  return true;
}

void ShaderLoader::unbind() {
  glUseProgram(NULL);
}

GLuint ShaderLoader::getProgramID() {
  return mProgramID;
}

GLuint ShaderLoader::loadShaderFromFile(string path, GLenum type) {
  GLuint shaderID = 0;
  string shaderString;
  ifstream sourceFile(path.c_str());

  int success;
	char infoLog[512];

  if (sourceFile) {
    shaderString.assign(istreambuf_iterator<char>(sourceFile), istreambuf_iterator<char>());

    shaderID = glCreateShader(type);
    const GLchar *shaderSource = shaderString.c_str();
    glShaderSource(shaderID, 1, (const GLchar**)&shaderSource, NULL);
    glCompileShader(shaderID);

    glGetShaderiv(shaderID, GL_COMPILE_STATUS, &success);
  	if (!success) {
  		glGetShaderInfoLog(shaderID, 512, NULL, infoLog);
  		cout << "ERROR: " << infoLog << endl;
      glDeleteShader(shaderID);
      shaderID = 0;
  	}
  }

  else {
    cout << "ERROR: Cannot open file " << path.c_str() << endl;
  }

  return shaderID;
}

bool ShaderLoader::loadProgram() {
  int success;
	char infoLog[512];
  mProgramID = glCreateProgram();

  // Vertex shader
  cout << "Loading vertex shader" << endl;
  GLuint vertexShader = loadShaderFromFile(vsPath, GL_VERTEX_SHADER);
  if (vertexShader == 0) {
    glDeleteProgram(mProgramID);
    mProgramID = 0;
    return false;
  }

  glAttachShader(mProgramID, vertexShader);

  // Fragment shader
  cout << "Loading fragment shader" << endl;
  GLuint fragmentShader = loadShaderFromFile(fsPath, GL_FRAGMENT_SHADER);
  if (fragmentShader == 0) {
    glDeleteProgram(mProgramID);
    mProgramID = 0;
    return false;
  }

  glAttachShader(mProgramID, fragmentShader);

  glLinkProgram(mProgramID);

  glGetProgramiv(mProgramID, GL_LINK_STATUS, &success);
	if(!success) {
	    glGetProgramInfoLog(mProgramID, 512, NULL, infoLog);
			cout << "ERROR: " << infoLog << endl;
      glDeleteShader(vertexShader);
      glDeleteShader(fragmentShader);
	}
  
  return true;
}
