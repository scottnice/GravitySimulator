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
class SpaceObject;
namespace GameLib
{

class Screen
{
private:
	const float MAX_MAG = 0.05f;
	const float MIN_MAG = 0.00000001f;
	const float RESCALE_POINT = 0.01f;
	const float RESCALE_ZOOM_FACTOR = (MAX_MAG - MIN_MAG) / 100.0f;
	atomic<int> step = -50;
	float magnitude;
	static int DEFAULT_WIDTH;
	static int DEFAULT_HEIGHT;
	int screenId;
	std::unique_ptr<ShaderProgram> program;
	unsigned int VBO;
	glm::mat4 view{ 1.0f };
	static glm::mat4 projection;
	glm::vec3 front{ 0.0f, 0.0f, -1.0f };
	glm::vec3 up{ 0.0f, 1.0f, 0.0f };
	glm::vec3 position;
	std::array<string, 20> lightPositionStrings;
public:

	Vector3d centerPoint;
	float rotatex{2.0f};
	float rotatey{ 2.0f };
	float rotatez{ 0.0f };
	// used to initialize the opengl screen and set the width, height and screen title, also binds member function resize to
	// friend function reshape that is passed to glutReshapeFunc so the window can be resized whenever a resize event occurs
	void initializeScreen(std::string title = "test", const int w = Screen::DEFAULT_WIDTH, const int h = Screen::DEFAULT_HEIGHT)
	{
		magnitude = calculate_zoom(step);
		glutInitWindowSize(w, h);   // Set the window's initial width & height
		glutInitWindowPosition(0, 0); // Position the window's initial top-left corner
		screenId = glutCreateWindow(title.c_str()); // Create a window with the given title
		glutReshapeFunc(resize);

		glewInit();
		glEnable(GL_DEPTH_TEST);

		auto& vertexShader = make_vertex_shader(AMBIENT_LIGHTING_VERTEX_SHADER);
		auto& fragmentShader = make_fragment_shader(POINT_LIGHT_FRAGMENT_SHADER);
		program = std::make_unique<ShaderProgram>(vertexShader, fragmentShader);

		glGenBuffers(1, &VBO);
		glBindBuffer(GL_ARRAY_BUFFER, VBO);
		initializeShaderContext(Shape{ 0,0 });
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

	void setTranslationX(float x)
	{
		centerPoint.x += (screen_width() / 2.0 - x) * magnitude;
	}

	void setTranslationY(float y)
	{
		centerPoint.y += (screen_height() / 2.0 - y) * magnitude;
	}

	void centerOn(const Vector3d& center)
	{
		centerPoint = center;
		//view = glm::mat4(1.0f);
		position = glm::vec3{ center.x, center.y, center.z + magnitude };
		view = glm::lookAt(position, position + front, up);
		//view = glm::scale(view, glm::vec3(1/magnitude));
		view = glm::rotate(view, glm::degrees(45.0f), glm::vec3(rotatex, 0, rotatey));
	}

	Vector2D applyMagnitude(const Vector2D& point) const{
		return point/magnitude;
	}

	Vector2D getCenterPoint()
	{
		return Vector2D(screen_width() / 2.0F, screen_height() / 2.0F);
	}

	static float calculate_zoom(int step){
		return std::powf((1 / 1.1f), step);
	}

	void zoom_out()
	{
		--step;
		magnitude = calculate_zoom(step);
	}

	void zoom_in()
	{
		++step;
		magnitude = calculate_zoom(step);
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

