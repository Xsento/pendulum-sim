#pragma once

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <random>
#include <ctime>

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>

// settings
constexpr int SCR_WIDTH = 800;
constexpr int SCR_HEIGHT = 600;

extern float deltaTime;

// callback for window resizing, also called once at the beginning to set the viewport
void framebuffer_size_callback(GLFWwindow* window, int width, int height);

// process keyboard input
void processInput(GLFWwindow *window);
