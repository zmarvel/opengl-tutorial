

#include <memory>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "logging.h"

void framebuffer_size_cb(GLFWwindow *window, int w, int h);
void error_cb(int err, const char *description);
void key_cb(GLFWwindow *window, int key, int scancode, int action, int mods);

int main(int argc, char *argv[]) {
  int rc = 0;
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

  while (!glfwWindowShouldClose(window)) {
    glClear(GL_COLOR_BUFFER_BIT);
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
