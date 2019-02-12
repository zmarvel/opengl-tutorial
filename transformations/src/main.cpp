

#include <memory>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <png++/png.hpp>

#include "logging.hpp"
#include "gl_program.hpp"


void framebuffer_size_cb(GLFWwindow *window, int w, int h);
void error_cb(int err, const char *description);
void key_cb(GLFWwindow *window, int key, int scancode, int action, int mods);


int main(int argc, char *argv[]) {
  int rc = 0;

  float vertices[] = {
    0.5f, 0.5f, 0.0f,
    0.5f, -0.5f, 0.0f,
    -0.5f, -0.5f, 0.0f,
    -0.5f, 0.5f, 0.0f,
  };

  unsigned int indices[] = {
    0, 1, 3,
    1, 2, 3,
  };

  Logger::setLogLevel(LogLevel::DEBUG);

  if (!glfwInit()) {
    Logger::error("glfwInit failed\n");
    return -1;
  }

  GLProgram shaderProgram{"src/vertex_shader.glsl", "src/fragment_shader.glsl"};
 
  //{
  //  png::image<png::rgb_pixel> image{"magnemite.png"};
  //  uint32_t imageRows = image.get_height();
  //  uint32_t imageCols = image.get_width();
  //  uint32_t imageSize = imageRows * imageCols;
  //  std::unique_ptr<unsigned char[]> rawImage{new unsigned char[imageSize*3]};
  //  for (uint32_t row = 0; row < imageRows; row++) {
  //    png::image<png::rgb_pixel>::row_access data = image.get_row(0);
  //    for (size_t i = 0; i < data.size(); i++) {
  //      png::rgb_pixel pix = data.at(i);
  //      rawImage[3*i+0] = pix.red;
  //      rawImage[3*i+1] = pix.green;
  //      rawImage[3*i+2] = pix.blue;
  //    }
  //  }

  //  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, imageCols, imageRows, 0, GL_RGB,
  //               GL_UNSIGNED_BYTE, rawImage.get());
  //  glGenerateMipmap(GL_TEXTURE_2D);


  //  unsigned int texture;
  //  glGenTextures(1, &texture);
  //  glBindTexture(GL_TEXTURE_2D, texture);

  //  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT); // s = x = col
  //  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT); // t = y = row
  //  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  //  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);



  //} // make sure the decoded image is freed
  





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

  if (!shaderProgram.compile()) {
    Logger::error("GLProgram::compile failed\n");
    goto cleanup;
  }
  else if (!shaderProgram.link()) {
    Logger::error("GLProgram::link failed\n");
    goto cleanup;
  }




  // Configure vertex array object
  // Allocate and copy vertex data to GPU
  unsigned int vao, vbo, ebo;
  glGenVertexArrays(1, &vao);
  glGenBuffers(1, &vbo);
  glGenBuffers(1, &ebo);
  glBindVertexArray(vao);

  glBindBuffer(GL_ARRAY_BUFFER, vbo);
  glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);


  // Configure the vertex attributes
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3*sizeof(float), (void*)0);
  glEnableVertexAttribArray(0);

  // Clear VAO binding -- have to restore later to use it again
  glBindVertexArray(0);

  while (!glfwWindowShouldClose(window)) {
    glClear(GL_COLOR_BUFFER_BIT);

    shaderProgram.use();
    glBindVertexArray(vao);
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
    //glDrawArrays(GL_TRIANGLES, 0, 3);

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
