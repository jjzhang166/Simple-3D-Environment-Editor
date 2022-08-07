#pragma once

#if DEBUG
	#include <iostream>
	#define LOG(x) std::cout << x << std::endl;
#else
	#define LOG(x)
#endif

#define WINDOW_WIDTH 900
#define WINDOW_HEIGHT 700

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "imgui/imgui.h"
#include "imgui/imgui_impl_glfw.h"
#include "imgui/imgui_impl_opengl3.h"
#include <string>
#include <sstream>
#include <cerrno>
#include <fstream>
#include <vector>

