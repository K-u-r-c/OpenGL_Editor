#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <iostream>
#include <filesystem>

#include "Filesystem.h"
#include "Utility/Camera.h"
#include "Utility/FramesPerSecondCounter.h"

struct MouseState {
  glm::vec2 pos = glm::vec2(0);
  bool pressedRight = false;
} mouseState;

CameraPositioner_FirstPerson positioner(glm::vec3(-5.0f, 5.0f, -5.0f), glm::vec3(0), glm::vec3(0.0f, 1.0f, 0.0f));
Camera camera(positioner);

#ifdef DEBUG
FramesPerSecondCounter fpsCounter(0.5f);
#endif

const unsigned int SCR_WIDTH  = 1920;
const unsigned int SCR_HEIGHT = 1080;
float RATIO = static_cast<float>(SCR_WIDTH) / static_cast<float>(SCR_HEIGHT);

float deltaTime = 0.0f;
float lastFrame = 0.0f;

glm::vec3 lightPos(0.0f, 2.0f, 0.0f);

glm::mat4 projection = glm::perspective(glm::radians(camera.getFOV()), RATIO, camera.getZNear(), camera.getZFar());

int main(int, char **argv) {
#pragma region Setup
  glfwInit();
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

  GLFWwindow *window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "3D-Engine", nullptr, nullptr);
  if (window == nullptr) {
#ifdef DEBUG
    std::cout << "Failed to create GLFW window" << std::endl;
#endif
    glfwTerminate();
    return -1;
  }
  glfwMakeContextCurrent(window);
  glfwSwapInterval(0);

  if (!gladLoadGLLoader((GLADloadproc) glfwGetProcAddress)) {
#ifdef DEBUG
    std::cout << "Failed to initialize GLAD" << std::endl;
#endif
    return -1;
  }

#ifdef DEBUG
  std::cout << "OpenGL Version: " << glGetString(GL_VERSION) << std::endl;
#endif

  glViewport(0, 0, SCR_WIDTH, SCR_HEIGHT);

  glfwSetFramebufferSizeCallback(window, [](auto* window, int x, int y) {
    glViewport(0, 0, x, y);

    RATIO = static_cast<float>(x) / static_cast<float>(y);
    projection = glm::perspective(glm::radians(camera.getFOV()), RATIO, camera.getZNear(), camera.getZFar());
  });

  glfwSetCursorPosCallback(window, [](auto* window, double x, double y) {
    int width, height;
    glfwGetFramebufferSize(window, &width, &height);
    mouseState.pos.x = static_cast<float>(x / width);
    mouseState.pos.y = static_cast<float>(y / height);
  });

  glfwSetMouseButtonCallback(window, [](auto* window, int button, int action, int mods) {
    if (button == GLFW_MOUSE_BUTTON_RIGHT && action != GLFW_RELEASE) {
      mouseState.pressedRight = action == GLFW_PRESS;
      glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    } else if (button == GLFW_MOUSE_BUTTON_RIGHT) {
      mouseState.pressedRight = false;
      glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
    }
  });

  glfwSetKeyCallback(window, [](auto* window, int key, int scancode, int action, int mods) {
    const bool pressed = action == GLFW_PRESS || action == GLFW_REPEAT;
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) glfwSetWindowShouldClose(window, GLFW_TRUE);
    if (key == GLFW_KEY_W) positioner.movement.forward = pressed;
    if (key == GLFW_KEY_S) positioner.movement.backward = pressed;
    if (key == GLFW_KEY_A) positioner.movement.left = pressed;
    if (key == GLFW_KEY_D) positioner.movement.right = pressed;
    if (key == GLFW_KEY_SPACE) positioner.movement.up = pressed;
    if (key == GLFW_KEY_LEFT_CONTROL) positioner.movement.down = pressed;
  });

#pragma endregion  // Setup

  mfsys::Filesystem filesystem((std::filesystem::path) argv[0]);
  Shader myShader = filesystem.createShader("assets/shaders/Shader.vert", "assets/shaders/Shader.frag");
  Shader lightShader = filesystem.createShader("assets/shaders/lightCube.vert", "assets/shaders/lightCube.frag");
  Shader gridShader = filesystem.createShader("assets/shaders/grid.vert", "assets/shaders/grid.frag");

  glEnable(GL_DEPTH_TEST);
  glEnable(GL_MULTISAMPLE);
  glfwWindowHint(GLFW_SAMPLES, 4);
  glEnable(GL_BLEND);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

  float vertices[] = {
    // positions          // normals           // texture coords
    -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f,  0.0f,
    0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f,  0.0f,
    0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f,  1.0f,
    0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f,  1.0f,
    -0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f,  1.0f,
    -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f,  0.0f,

    -0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  0.0f,  0.0f,
    0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  1.0f,  0.0f,
    0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  1.0f,  1.0f,
    0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  1.0f,  1.0f,
    -0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  0.0f,  1.0f,
    -0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  0.0f,  0.0f,

    -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  1.0f,  0.0f,
    -0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  1.0f,  1.0f,
    -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  0.0f,  1.0f,
    -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  0.0f,  1.0f,
    -0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  0.0f,  0.0f,
    -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  1.0f,  0.0f,

    0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f,  0.0f,
    0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  1.0f,  1.0f,
    0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  0.0f,  1.0f,
    0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  0.0f,  1.0f,
    0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  0.0f,  0.0f,
    0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f,  0.0f,

    -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.0f,  1.0f,
    0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  1.0f,  1.0f,
    0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  1.0f,  0.0f,
    0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  1.0f,  0.0f,
    -0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  0.0f,  0.0f,
    -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.0f,  1.0f,

    -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f,  1.0f,
    0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  1.0f,  1.0f,
    0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f,  0.0f,
    0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f,  0.0f,
    -0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  0.0f,  0.0f,
    -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f,  1.0f
  };

  unsigned int VBO, cubeVAO;
  glGenVertexArrays(1, &cubeVAO);
  glGenBuffers(1, &VBO);

  glBindBuffer(GL_ARRAY_BUFFER, VBO);
  glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

  glBindVertexArray(cubeVAO);
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), nullptr);
  glEnableVertexAttribArray(0);
  glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
  glEnableVertexAttribArray(1);
  glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
  glEnableVertexAttribArray(2);

  // second, configure the light's VAO (VBO stays the same; the vertices are the same for the light object which is also a 3D cube)
  unsigned int lightCubeVAO;
  glGenVertexArrays(1, &lightCubeVAO);
  glBindVertexArray(lightCubeVAO);

  glBindBuffer(GL_ARRAY_BUFFER, VBO);
  // note that we update the lamp's position attribute's stride to reflect the updated buffer data
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), nullptr);
  glEnableVertexAttribArray(0);

  // load textures (we now use a utility function to keep the code more organized)
  // -----------------------------------------------------------------------------
  unsigned int diffuseMap = filesystem.loadTexture("assets/textures/container2.png");
  unsigned int specularMap = filesystem.loadTexture("assets/textures/container2_specular.png");

  myShader.use();
  myShader.setInt("material.diffuse", 0);
  myShader.setInt("material.specular", 1);

  positioner.setZNear(0.01f);
  positioner.setZFar(100.0f);

  // uncomment this call to draw in wireframe polygons.
  // glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

  while (!glfwWindowShouldClose(window)) {
#ifdef DEBUG
    fpsCounter.tick(deltaTime);
#endif

    auto currentFrame = static_cast<float>(glfwGetTime());
    deltaTime = currentFrame - lastFrame;
    lastFrame = currentFrame;

    positioner.movement.fastSpeed = (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) != GLFW_RELEASE);
    positioner.update(deltaTime, mouseState.pos, mouseState.pressedRight);

    glClearColor(0.15f, 0.15f, 0.15f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // Make light position run in circle
    lightPos.x = sin(glfwGetTime()) * 2.0f;
    lightPos.z = cos(glfwGetTime()) * 2.0f;

    myShader.use();
    myShader.setFloat("material.shininess", 32.0f);
    myShader.setVec3("light.position", lightPos);
    myShader.setVec3("light.ambient", 0.1f, 0.1f, 0.1f);
    myShader.setVec3("light.diffuse", 0.5f, 0.5f, 0.5f);
    myShader.setVec3("light.specular", 1.0f, 1.0f, 1.0f);
    myShader.setVec3("viewPos", camera.getPosition());

    glm::mat4 view = camera.getViewMatrix();
    myShader.setMat4("projection", projection);
    myShader.setMat4("view", view);
    glm::mat4 model = glm::mat4(1.0f);
    model = glm::translate(model, glm::vec3(0.0f, 0.5f, 0.0f));
    myShader.setMat4("model", model);

    // bind textures on corresponding texture units
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, diffuseMap);
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, specularMap);

    glBindVertexArray(cubeVAO);
    glDrawArrays(GL_TRIANGLES, 0, 36);

    lightShader.use();
    lightShader.setMat4("projection", projection);
    lightShader.setMat4("view", view);
    model = glm::mat4(1.0f);
    model = glm::translate(model, lightPos);
    model = glm::scale(model, glm::vec3(0.2f));
    lightShader.setMat4("model", model);

    glBindVertexArray(lightCubeVAO);
    glDrawArrays(GL_TRIANGLES, 0, 36);

    // Drawing grid
    gridShader.use();
    gridShader.setMat4("proj", projection);
    gridShader.setMat4("view", view);
    gridShader.setVec3("cameraPos", camera.getPosition());
    gridShader.setFloat("gridSize", camera.getZFar());
    gridShader.setFloat("gridCellSize", camera.getZFar() / camera.getZFar() / 2.0f);
    glDrawArraysInstancedBaseInstance(GL_TRIANGLES, 0, 6, 1, 0);

    glfwSwapBuffers(window);
    glfwPollEvents();
  }

  glfwTerminate();

  return 0;
}