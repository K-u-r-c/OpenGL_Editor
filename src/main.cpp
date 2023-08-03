#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <iostream>
#include <filesystem>

#include "filesystem/public/filesystem.h"
#include "camera/public/camera.h"
#include "utility/public/frames_per_second_counter.h"

struct mouse_state {
  glm::vec2 pos = glm::vec2(0);
  bool pressed_right = false;
} mouse_state;

camera_positioner_first_person positioner(glm::vec3(-5.0f, 5.0f, -5.0f), glm::vec3(0), glm::vec3(0.0f, 1.0f, 0.0f));
camera camera(positioner);

frames_per_second_counter fps_counter(0.5);

GLsizei scr_width  = 1600;
GLsizei scr_height = 900;
float_t ratio = static_cast<float_t>(scr_width) / static_cast<float_t>(scr_height);

double delta_time = 0.0;
double last_frame = 0.0;

glm::vec3 light_pos(0.0f, 2.0f, 0.0f);

glm::mat4 projection = glm::perspective(glm::radians(camera.get_fov()), ratio, camera.get_z_near(), camera.get_z_far());

int main(int, char **argv) {
#pragma region Setup

#ifdef __APPLE__
  glfwInit();
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

  glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GLFW_TRUE);
#else
  glfwInit();
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
#endif

  GLFWwindow *window = glfwCreateWindow(scr_width, scr_height, "3D-Engine", nullptr, nullptr);
  if (window == nullptr) {
#ifdef DEBUG
    std::cout << "Failed to create GLFW window" << std::endl;
#endif
    glfwTerminate();
    return -1;
  }
  glfwMakeContextCurrent(window);
  glfwSwapInterval(0);

  if (!gladLoadGLLoader(reinterpret_cast<GLADloadproc>(glfwGetProcAddress))) {
#ifdef DEBUG
    std::cout << "Failed to initialize GLAD" << std::endl;
#endif
    return -1;
  }

#ifdef DEBUG
  std::cout << "OpenGL Version: " << glGetString(GL_VERSION) << std::endl;
#endif

  glfwGetFramebufferSize(window, &scr_width, &scr_height);
  glViewport(0, 0, scr_width, scr_height);

  glfwSetFramebufferSizeCallback(window, [](auto* window, int x, int y) {
    glViewport(0, 0, x, y);

    ratio = static_cast<float>(x) / static_cast<float>(y);
    projection = glm::perspective(glm::radians(camera.get_fov()), ratio, camera.get_z_near(), camera.get_z_far());
  });

  glfwSetCursorPosCallback(window, [](auto* window, double x, double y) {
    int width, height;
    glfwGetFramebufferSize(window, &width, &height);
    mouse_state.pos.x = static_cast<float>(x / width);
    mouse_state.pos.y = static_cast<float>(y / height);
  });

  glfwSetMouseButtonCallback(window, [](auto* window, int button, int action, int mods) {
    if (button == GLFW_MOUSE_BUTTON_RIGHT && action != GLFW_RELEASE) {
      mouse_state.pressed_right = action == GLFW_PRESS;
      glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    } else if (button == GLFW_MOUSE_BUTTON_RIGHT) {
      mouse_state.pressed_right = false;
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

  const mfsys::filesystem filesystem((std::filesystem::path) argv[0]);
#ifdef __APPLE__
  const shader my_shader = filesystem.create_shader("assets/shaders/shader410.vert", "assets/shaders/shader410.frag");
  const shader light_shader = filesystem.create_shader("assets/shaders/lightCube410.vert", "assets/shaders/lightCube410.frag");
  const shader grid_shader = filesystem.create_shader("assets/shaders/grid/grid410.vert", "assets/shaders/grid/grid410.frag");
#else
  const shader my_shader = filesystem.create_shader("assets/shaders/shader460.vert", "assets/shaders/shader460.frag");
  const shader light_shader = filesystem.create_shader("assets/shaders/lightCube460.vert", "assets/shaders/lightCube460.frag");
  const shader grid_shader = filesystem.create_shader("assets/shaders/grid/grid460.vert", "assets/shaders/grid/grid460.frag");
#endif

  glEnable(GL_DEPTH_TEST);
  glEnable(GL_MULTISAMPLE);
  glfwWindowHint(GLFW_SAMPLES, 4);
  glEnable(GL_BLEND);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

  constexpr float vertices[] = {
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

  unsigned int vbo, cube_vao;
  glGenVertexArrays(1, &cube_vao);
  glGenBuffers(1, &vbo);

  glBindBuffer(GL_ARRAY_BUFFER, vbo);
  glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

  glBindVertexArray(cube_vao);
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), nullptr);
  glEnableVertexAttribArray(0);
  glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), reinterpret_cast<void*>(3 * sizeof(float)));
  glEnableVertexAttribArray(1);
  glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), reinterpret_cast<void*>(6 * sizeof(float)));
  glEnableVertexAttribArray(2);

  // second, configure the light's VAO (VBO stays the same; the vertices are the same for the light object which is also a 3D cube)
  unsigned int light_cube_vao;
  glGenVertexArrays(1, &light_cube_vao);
  glBindVertexArray(light_cube_vao);

  glBindBuffer(GL_ARRAY_BUFFER, vbo);
  // note that we update the lamp's position attribute's stride to reflect the updated buffer data
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), nullptr);
  glEnableVertexAttribArray(0);

  // load textures (we now use a utility function to keep the code more organized)
  // -----------------------------------------------------------------------------
  const unsigned int diffuse_map = filesystem.load_texture("assets/textures/container2.png");
  const unsigned int specular_map = filesystem.load_texture("assets/textures/container2_specular.png");

  my_shader.use();
  my_shader.setInt("material.diffuse", 0);
  my_shader.setInt("material.specular", 1);

  positioner.set_z_near(0.01f);
  positioner.set_z_far(100.0f);

  // uncomment this call to draw in wire frame polygons.
  // glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

  while (!glfwWindowShouldClose(window)) {
    fps_counter.tick(delta_time);

    const auto current_frame = glfwGetTime();
    delta_time = current_frame - last_frame;
    last_frame = current_frame;

    positioner.movement.fast_speed = (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) != GLFW_RELEASE);
    positioner.update(delta_time, mouse_state.pos, mouse_state.pressed_right);

    glClearColor(0.15f, 0.15f, 0.15f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // Make light position run in circle
    light_pos.x = sinf(static_cast<float>(glfwGetTime())) * 2.0f;
    light_pos.z = cosf(static_cast<float>(glfwGetTime())) * 2.0f;

    my_shader.use();
    my_shader.setFloat("material.shininess", 32.0f);
    my_shader.setVec3("light.position", light_pos);
    my_shader.setVec3("light.ambient", 0.1f, 0.1f, 0.1f);
    my_shader.setVec3("light.diffuse", 0.5f, 0.5f, 0.5f);
    my_shader.setVec3("light.specular", 1.0f, 1.0f, 1.0f);
    my_shader.setVec3("viewPos", camera.get_position());

    glm::mat4 view = camera.get_view_matrix();
    my_shader.setMat4("projection", projection);
    my_shader.setMat4("view", view);
    glm::mat4 model = glm::mat4(1.0f);
    model = glm::translate(model, glm::vec3(0.0f, 0.5f, 0.0f));
    my_shader.setMat4("model", model);

    // bind textures on corresponding texture units
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, diffuse_map);
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, specular_map);

    glBindVertexArray(cube_vao);
    glDrawArrays(GL_TRIANGLES, 0, 36);

    light_shader.use();
    light_shader.setMat4("projection", projection);
    light_shader.setMat4("view", view);
    model = glm::mat4(1.0f);
    model = glm::translate(model, light_pos);
    model = glm::scale(model, glm::vec3(0.2f));
    light_shader.setMat4("model", model);

    glBindVertexArray(light_cube_vao);
    glDrawArrays(GL_TRIANGLES, 0, 36);

    // Drawing grid
    grid_shader.use();
    grid_shader.setMat4("proj", projection);
    grid_shader.setMat4("view", view);
    grid_shader.setVec3("cameraPos", camera.get_position());
    grid_shader.setFloat("gridSize", camera.get_z_far());
    grid_shader.setFloat("gridCellSize", 1 / 2.0f);
    glDrawArrays(GL_TRIANGLES, 0, 6);

    glfwSwapBuffers(window);
    glfwPollEvents();
  }

  glfwTerminate();

  return 0;
}