
#include <fstream>
#include <sstream>
#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "gl_program.hpp"
#include "logging.hpp"

GLProgram::GLProgram(std::string vertexShaderPath,
                     std::string fragmentShaderPath) {
  myVertexShaderPath = vertexShaderPath;
  myFragmentShaderPath = fragmentShaderPath;
}

GLProgram::~GLProgram() {
}

bool GLProgram::compile() {
  {
    Logger::debug("compiling shader %s\n", myVertexShaderPath.c_str());
    std::stringstream vertexShaderStream;
    std::string vertexShaderSource;
    char vertexShaderBuf[1024];
    std::ifstream vertexShaderFile;
    vertexShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
    vertexShaderFile.open(myVertexShaderPath);
    vertexShaderStream << vertexShaderFile.rdbuf();
    myVertexShaderID = glCreateShader(GL_VERTEX_SHADER);
    vertexShaderSource = vertexShaderStream.str();
    const char *shaderSource = vertexShaderSource.c_str();
    Logger::debug("%s\n", shaderSource);
    glShaderSource(myVertexShaderID, 1, &shaderSource, NULL);
    glCompileShader(myVertexShaderID);

    int rc;
    glGetShaderiv(myVertexShaderID, GL_COMPILE_STATUS, &rc);
    if (!rc) {
      char infoLog[512];
      glGetShaderInfoLog(myVertexShaderID, sizeof(infoLog), NULL, infoLog);
      Logger::error("glCompileShader(vertexShader): %s\n", infoLog);
      return false;
    }
    vertexShaderFile.close();
  }
  {
    Logger::debug("compiling shader %s\n", myFragmentShaderPath.c_str());
    std::stringstream fragmentShaderStream;
    std::string fragmentShaderSource;
    char fragmentShaderBuf[1024];
    std::ifstream fragmentShaderFile;
    fragmentShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
    fragmentShaderFile.open(myFragmentShaderPath);
    fragmentShaderStream << fragmentShaderFile.rdbuf();
    myFragmentShaderID = glCreateShader(GL_FRAGMENT_SHADER);
    fragmentShaderSource = fragmentShaderStream.str();
    const char *shaderSource = fragmentShaderSource.c_str();
    Logger::debug("%s\n", shaderSource);
    glShaderSource(myFragmentShaderID, 1, &shaderSource, NULL);
    glCompileShader(myFragmentShaderID);

    int rc;
    glGetShaderiv(myFragmentShaderID, GL_COMPILE_STATUS, &rc);
    if (!rc) {
      char infoLog[512];
      glGetShaderInfoLog(myFragmentShaderID, sizeof(infoLog), NULL, infoLog);
      Logger::error("glCompileShader(fragmentShader): %s\n", infoLog);
      return false;
    }
    fragmentShaderFile.close();
  }

  return true;
}

bool GLProgram::link() {
  if (myVertexShaderID == 0 || myFragmentShaderID == 0) {
    Logger::error("GLProgram::link: shaderID not initialized\n");
    return false;
  }
  myID = glCreateProgram();
  glAttachShader(myID, myVertexShaderID);
  glAttachShader(myID, myFragmentShaderID);
  glLinkProgram(myID);

  int rc;
  glGetProgramiv(myID, GL_LINK_STATUS, &rc);
  if (!rc) {
    char infoLog[512];
    glGetProgramInfoLog(myID, 512, NULL, infoLog);
    Logger::error("glLinkProgram: %s\n", infoLog);
    return false;
  }

  glDeleteShader(myVertexShaderID);
  myVertexShaderID = 0;
  glDeleteShader(myFragmentShaderID);
  myFragmentShaderID = 0;

  return true;
}

void GLProgram::use() {
  glUseProgram(myID);
}


