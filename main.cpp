#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>

void framebuffer_size_callback(GLFWwindow *window, int width, int height);
void processInput(GLFWwindow *window);

const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

const char *vertexShaderSource = "#version 330 core\n"
  "layout (location = 0) in vec3 aPos;\n"
  "void main()\n"
  "{\n"
  "   gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
  "}\0";


int main(){

  glfwInit();
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

  GLFWwindow *window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "LearnOpenGL", NULL, NULL);
  if(window == NULL){
    std::cout << "Failed to create GLFW window" << std::endl;
    glfwTerminate();//
    return -1;
  }

  glfwMakeContextCurrent(window);

  //Initialize GLAD
  if(!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
    std::cout << "Failed to initialize GLAD" << std::endl;
    return -1;
  }

  //tell GLFW to call this function every window resize
  glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);


  //Normalized Device Coordinates (NDC) space, because glViewport!
  float vertices[] = {
    -0.5f, -0.5f, 0.0f,
    0.5f, -0.5f, 0.0f,
    0.0f, 0.5f, 0.0f
  };

  //First we put the data to Graphics Card Memory (VBO)
  unsigned int VBO;
  glGenBuffers(1, &VBO);
  glBindBuffer(GL_ARRAY_BUFFER, VBO);
  glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

  //Second we create vertex shader and unique ID
  unsigned int vertexShader;
  vertexShader = glCreateShader(GL_VERTEX_SHADER);
  // attach shader source code to shader object
  glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
  // compile the shader
  glCompileShader(vertexShader);

  //Check if there is error in compiling the shader
  int success;
  char infoLog[512];
  glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);

  if(!success){
    glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
    std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILDED\n" << infoLog << std::endl;
  }
  
  // render loop
  while(!glfwWindowShouldClose(window)){

    // input
    processInput(window);

    // rendering commands here
    glClearColor(0.2f,0.3f,0.3f,1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    // check and call events and swap buffers
    glfwSwapBuffers(window);
    glfwPollEvents();
  }
  
  // clean/delete all of GLFW resources
  glfwTerminate();
  return 0;
}

//each time is called when resized the window
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
   // Size of the rendering window
   // (-0.5,0.5) would (as its final transformation) be mapped to (200,450)
    glViewport(0, 0, width, height);
}  

void processInput(GLFWwindow *window){
  if(glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
    glfwSetWindowShouldClose(window, true);
  }
}
