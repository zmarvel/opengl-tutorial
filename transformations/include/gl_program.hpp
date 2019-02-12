
#ifndef GLPROGRAM_HPP
#define GLPROGRAM_HPP

#include <string>

class GLProgram {
  public:
    explicit GLProgram(std::string vertexShaderPath, std::string fragmentShaderPath);
    ~GLProgram();

    bool compile();
    bool link();
    void use(); 

    unsigned int getID() const {
      return myID;
    };

  private:
    unsigned int myVertexShaderID, myFragmentShaderID, myID;
    std::string myVertexShaderPath;
    std::string myFragmentShaderPath;
};

#endif // GLPROGRAM_HPP
