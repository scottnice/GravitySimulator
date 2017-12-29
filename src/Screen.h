#pragma once
#define GLEW_STATIC
#include "Shape.h"
#include "Vector2D.h"
#include "Vector3d.h"
#include <string>
#include <array>
#include <atomic>
#include "Shader.h"
#include <glut.h>
#include <memory>
#include "glm/glm/glm.hpp"
#include "glm/glm/gtc/matrix_transform.hpp"
#include "glm/glm/gtc/type_ptr.hpp"
#include "Camera.h"
#include "Utility.h"
class SpaceObject;
namespace GameLib
{

class Screen
{
private:
	static int DEFAULT_WIDTH;
	static int DEFAULT_HEIGHT;
	int screenId;
	std::unique_ptr<ShaderProgram> program;
	unsigned int VBO;
	static glm::mat4 projection;
	glm::vec3 front{ 0.0f, 0.0f, -1.0f };
	glm::vec3 up{ 0.0f, 1.0f, 0.0f };
	static const size_t MAX_LIGHT_POSITIONS = 100;
	std::array<string, MAX_LIGHT_POSITIONS> lightPositionStrings;
public:
	float rotation = 0.0f;
	std::unique_ptr<Camera> camera;
	Vector3d centerPoint;
	// used to initialize the opengl screen and set the width, height and screen title, also binds member function resize to
	// friend function reshape that is passed to glutReshapeFunc so the window can be resized whenever a resize event occurs
	void initializeScreen(std::string title = "test", const int w = Screen::DEFAULT_WIDTH, const int h = Screen::DEFAULT_HEIGHT)
	{
		glutInitWindowSize(w, h);   // Set the window's initial width & height
		glutInitWindowPosition(0, 0); // Position the window's initial top-left corner
		screenId = glutCreateWindow(title.c_str()); // Create a window with the given title
		glutReshapeFunc(resize);

		glewInit();
		glEnable(GL_DEPTH_TEST);
		glDepthFunc(GL_LESS);

		auto& vertexShader = make_vertex_shader(util::loadFile("../src/shaders/point_light.vs"));
		auto& fragmentShader = make_fragment_shader(util::loadFile("../src/shaders/point_light.fs"));
		program = std::make_unique<ShaderProgram>(vertexShader, fragmentShader);

		glGenBuffers(1, &VBO);
		glBindBuffer(GL_ARRAY_BUFFER, VBO);
		initializeShaderContext(Shape{ 0,0 });
		camera = make_unique<Camera>(glm::vec3{0,0,3000});
	}

	// used to initialize the open gl viewport window
	inline static void resize(GLsizei width, GLsizei height)
	{
		// Compute aspect ratio of the new window
	   if (height == 0) height = 1;                // To prevent divide by 0
	   GLfloat aspect = (GLfloat)width / (GLfloat)height;
 
	   // Set the viewport to cover the new window
	   glViewport(0, 0, width, height);
 
	   projection = glm::perspective(glm::radians(45.0f), aspect, 0.1f, 1000000.0f);
	}

	int screen_width()
	{return glutGet(GLUT_WINDOW_WIDTH);}
	int screen_height()
	{return glutGet(GLUT_WINDOW_HEIGHT);}

	Vector2D getCenterPoint()
	{
		return Vector2D(screen_width() / 2.0F, screen_height() / 2.0F);
	}

	static float calculate_zoom(int step){
		return std::powf((1 / 1.1f), step);
	}

	Screen(const int w = Screen::DEFAULT_WIDTH, const int h = Screen::DEFAULT_HEIGHT, std::string title = "");
	
	~Screen();

	void initializeShaderContext(const Shape & s);

	void render(SpaceObject& spaceObject);

	void refresh()
	{
		glClearColor(0.0f, 0.0f, 0.0f, 1.0f); // Set background color to black and opaque
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);         // Clear the color buffer (background)
	}

	void update()
	{
		glFlush();  // Render now
	}
	int getScreenId(){
		return screenId;
	}
};
}

