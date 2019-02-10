

#include <memory>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "logging.h"


const char *vertexShaderSource =
"#version 330 core\n"
"layout (location = 0) in vec3 aPos;\n"
"void main() {\n"
"  gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
"}";

const char *fragmentShaderSource =
"#version 330 core\n"
"out vec4 FragColor;\n"
"void main() {\n"
"  FragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f);\n"
"}";




void framebuffer_size_cb(GLFWwindow *window, int w, int h);
void error_cb(int err, const char *description);
void key_cb(GLFWwindow *window, int key, int scancode, int action, int mods);

int main(int argc, char *argv[]) {
  int rc = 0;

  float vertices[] = {
    -0.5f, -0.5f, 0.0f,
    0.0f, 0.5f, 0.0f,
    0.5f, -0.5f, 0.0f,
  };

  Logger::setLogLevel(LogLevel::DEBUG);

  if (!glfwInit()) {
    Logger::error("glfwInit failed\n");
    return -1;
  }

  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

  GLFWwindow* window = glfwCreateWindow(640, 480, "Hello world", NULL, NULL);
  glfwMakeContextCurrent(window);
  if (!window) {
    Logger::error("glfwCreateWindow failed\n");
    goto cleanup;
  }

  glfwSetErrorCallback(error_cb);
  glfwSetFramebufferSizeCallback(window, framebuffer_size_cb);
  glfwSetKeyCallback(window, key_cb);

  if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
    Logger::error("gladLoadGL failed\n");
    goto cleanup;
  }

  glViewport(0, 0, 640, 480);
  glClearColor(0.39f, 0.13f, 0.06f, 1.0f);
  //
  // Compile vertex shader
  unsigned int vertexShader;
  vertexShader = glCreateShader(GL_VERTEX_SHADER);
  glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
  glCompileShader(vertexShader);

  int success;
  glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
  if (!success) {
    char infoLog[512];
    glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
    Logger::error("glCompileShader: %s\n", infoLog);
    goto cleanup;
  }

  // Compile fragment shader
  unsigned int fragmentShader;
  fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
  glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
  glCompileShader(fragmentShader);

  glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
  if (!success) {
    char infoLog[512];
    glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
    Logger::error("glCompileShader: %s\n", infoLog);
    goto cleanup;
  }

  // Link and use the shaders
  unsigned int shaderProgram;
  shaderProgram = glCreateProgram();
  glAttachShader(shaderProgram, vertexShader);
  glAttachShader(shaderProgram, fragmentShader);
  glLinkProgram(shaderProgram);

  glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
  if (!success) {
    char infoLog[512];
    glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
    Logger::error("glLinkProgram: %s\n", infoLog);
    goto cleanup;
  }

  // Clean up compiled shaders
  glDeleteShader(vertexShader);
  glDeleteShader(fragmentShader);



  // Configure vertex array object
  // Allocate and copy vertex data to GPU
  unsigned int vao;
  unsigned int vbo; // vertex buffer object
  glGenVertexArrays(1, &vao);
  glGenBuffers(1, &vbo);
  glBindVertexArray(vao);

  glBindBuffer(GL_ARRAY_BUFFER, vbo);
  glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

  // Configure the vertex attributes
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3*sizeof(float), (void*)0);
  glEnableVertexAttribArray(0);

  // Clear VAO binding -- have to restore later to use it again
  glBindVertexArray(0);










  while (!glfwWindowShouldClose(window)) {
    glClear(GL_COLOR_BUFFER_BIT);

    glUseProgram(shaderProgram);
    glBindVertexArray(vao);
    glDrawArrays(GL_TRIANGLES, 0, 3);

    glfwSwapBuffers(window);
    glfwPollEvents();
  }



cleanup:
  glfwTerminate();
done:
  return rc;
}


void framebuffer_size_cb(GLFWwindow *window, int w, int h) {
  glViewport(0, 0, w, h);
}

void error_cb(int err, const char *description) {
  Logger::error("GLFW %d: %s\n", err, description);
}

void key_cb(GLFWwindow *window, int key, int scancode, int action, int mods) {
  if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) {
    glfwSetWindowShouldClose(window, true);
  }
}
