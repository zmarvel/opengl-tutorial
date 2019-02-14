

#include <memory>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <png++/png.hpp>

#include "logging.hpp"
#include "gl_program.hpp"
#include "png_image.hpp"


void framebuffer_size_cb(GLFWwindow *window, int w, int h);
void error_cb(int err, const char *description);
void key_cb(GLFWwindow *window, int key, int scancode, int action, int mods);


int main(int argc, char *argv[]) {
  int rc = 0;

  float vertices[] = {
    0.5f, 0.5f, 0.0f,   1.0f, 0.0f, 0.0f,   1.0f, 1.0f, // pos, color, tex coords
    0.5f, -0.5f, 0.0f,  0.0f, 1.0f, 0.0f,   1.0f, 0.0f,
    -0.5f, -0.5f, 0.0f, 0.0f, 0.0f, 1.0f,   0.0f, 0.0f,
    -0.5f, 0.5f, 0.0f,  1.0f, 1.0f, 0.0f,   0.0f, 1.0f,
  };

  unsigned int indices[] = {
    0, 1, 3,
    1, 2, 3,
  };

  // Declare variables here, not because I like it, but because GCC issues an
  // error when gotos cross declarations---and I want to use gotos. <3
  unsigned int vao, vbo, ebo;
  unsigned int magnemite, clefairy;
  unsigned int transformLoc;

  Logger::setLogLevel(LogLevel::DEBUG);

  if (!glfwInit()) {
    Logger::error("glfwInit failed\n");
    return -1;
  }

  GLProgram shaderProgram{"src/vertex_shader.glsl", "src/fragment_shader.glsl"};
 
  


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
  glGenVertexArrays(1, &vao);
  glGenBuffers(1, &vbo);
  glGenBuffers(1, &ebo);
  glBindVertexArray(vao);

  glBindBuffer(GL_ARRAY_BUFFER, vbo);
  glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);


  // Configure the vertex attributes
  // Vertices
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8*sizeof(float), (void*)(0));
  glEnableVertexAttribArray(0);

  // Colors
  glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8*sizeof(float), (void*)(3*sizeof(float)));
  glEnableVertexAttribArray(1);

  // Texture coordinates
  glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8*sizeof(float), (void*)(6*sizeof(float)));
  glEnableVertexAttribArray(2);

  {
    PngImage image{"magnemite.png"};
    std::unique_ptr<unsigned char[]> rawImage = image.readRaw();
    uint32_t width = image.getWidth(), height = image.getHeight();

    glGenTextures(1, &magnemite);
    glBindTexture(GL_TEXTURE_2D, magnemite);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA,
                 GL_UNSIGNED_BYTE, rawImage.get());
    glGenerateMipmap(GL_TEXTURE_2D);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT); // s = x = col
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT); // t = y = row
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  } // make sure the decoded image is freed
  {
    PngImage image{"clefairy.png"};
    std::unique_ptr<unsigned char[]> rawImage = image.readRaw();
    uint32_t width = image.getWidth(), height = image.getHeight();

    glGenTextures(1, &clefairy);
    glBindTexture(GL_TEXTURE_2D, clefairy);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA,
                 GL_UNSIGNED_BYTE, rawImage.get());
    glGenerateMipmap(GL_TEXTURE_2D);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT); // s = x = col
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT); // t = y = row
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  }

  shaderProgram.use();

  transformLoc = glGetUniformLocation(shaderProgram.getID(), "transform");

  glUniform1i(glGetUniformLocation(shaderProgram.getID(), "ourTexture"), 0);

  // Clear VAO binding -- have to restore later to use it again
  glBindVertexArray(0);

  while (!glfwWindowShouldClose(window)) {
    glClear(GL_COLOR_BUFFER_BIT);

    shaderProgram.use();

    glActiveTexture(GL_TEXTURE0);
    //glBindTexture(GL_TEXTURE_2D, magnemite);
    glActiveTexture(GL_TEXTURE1);

    {
      glm::mat4 transf = glm::mat4(1.0f);
      transf = glm::translate(transf, glm::vec3(0.5f, -0.5f, 0.0f));
      transf = glm::rotate(transf, (float)glfwGetTime(),
                           glm::vec3(0.0, 0.0, 1.0));

      glUniformMatrix4fv(transformLoc, 1, GL_FALSE, glm::value_ptr(transf));
    }

    glBindVertexArray(vao);
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

    {
      glm::mat4 transf = glm::mat4(1.0f);
      transf = glm::translate(transf, glm::vec3(-0.5f, 0.5f, 0.0f));
      float scale = sin(glfwGetTime()) + 1.0f;
      transf = glm::scale(transf, glm::vec3(scale));

      glUniformMatrix4fv(transformLoc, 1, GL_FALSE, glm::value_ptr(transf));
    }

    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

    glfwSwapBuffers(window);
    glfwPollEvents();
  }



cleanup:
  glfwTerminate();
//done:
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
