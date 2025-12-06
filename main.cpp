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

const char *fragmentShaderSource = "#version 330 core\n"
  "out vec4 FragColor;\n"
  "void main()\n"
  "{\n"
  "   FragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f);\n"
  "}\n\0";


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
  

  //2. We create Vertex Shader and unique ID
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
  
  //3. Create Fragment shader
  unsigned int fragmentShader;
  fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
  glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
  glCompileShader(fragmentShader);

  // check for shader compile errors
  glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
  if (!success){
    glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
    std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
  }

  //4. Link both shader objects into a shader program.
  unsigned int shaderProgram;
  shaderProgram = glCreateProgram();
  glAttachShader(shaderProgram, vertexShader);
  glAttachShader(shaderProgram, fragmentShader);
  glLinkProgram(shaderProgram);

  glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);

  if(!success) {
    glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
    std::cout << "ERROR::SHADER::PROGRAM::COMPILATION_FAILDED\n" << infoLog << std::endl;
  }

  //5. Delete vertex and fragment shader, they are already linked.
  glDeleteShader(vertexShader);
  glDeleteShader(fragmentShader);

  //Normalized Device Coordinates (NDC) space, because glViewport!
  float vertices[] = {
    -0.5f, -0.5f, 0.0f,
    0.5f, -0.5f, 0.0f,
    0.0f, 0.5f, 0.0f
  };

  //1. We put the data to Graphics Card Memory (VBO)
  unsigned int VBO, VAO;
  glGenVertexArrays(1, &VAO);
  glGenBuffers(1, &VBO);
  glBindVertexArray(VAO);

  glBindBuffer(GL_ARRAY_BUFFER, VBO);
  glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);


  //6. Linking Vertex Attributes, layout (location = 0).
  // VERTEX 1            VERTEX 2         VERTEX 3
  // x   y   z           x   y   z        x   y   z
  // 0-4-8-12           12-16-20-24      24-28-32-36
  glVertexAttribPointer(0,3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
  glEnableVertexAttribArray(0);

  glBindBuffer(GL_ARRAY_BUFFER, 0);
  glBindVertexArray(0); 
  
  // render loop
  while(!glfwWindowShouldClose(window)){

    // input
    processInput(window);

    // rendering commands here
    glClearColor(0.2f,0.3f,0.3f,1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    // Every shader and rendering call after glUseProgram will now use
    // this program object (and thus the shaders).
    glUseProgram(shaderProgram);
    glBindVertexArray(VAO);
       glDrawArrays(GL_TRIANGLES, 0, 3);

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
