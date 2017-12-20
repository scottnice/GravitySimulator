#pragma once
#include "Shape.h"
#include "Vector2D.h"
#include "Vector3d.h"
#include <glut.h>
#include <string>
#include <array>
#include <atomic>

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

public:

	Vector3d centerPoint;
	float rotatex;
	float rotatey;
	float rotatez;
	// used to initialize the opengl screen and set the width, height and screen title, also binds member function resize to
	// friend function reshape that is passed to glutReshapeFunc so the window can be resized whenever a resize event occurs
	void initializeScreen(std::string title = "test", const int w = Screen::DEFAULT_WIDTH, const int h = Screen::DEFAULT_HEIGHT)
	{
		magnitude = calculate_zoom(step);
		glutInitWindowSize(w, h);   // Set the window's initial width & height
		glutInitWindowPosition(0, 0); // Position the window's initial top-left corner
		screenId = glutCreateWindow(title.c_str()); // Create a window with the given title
		glShadeModel(GL_SMOOTH);
		glutReshapeFunc(resize);


		glClearDepth(1.0f);
		glEnable(GL_DEPTH_TEST);
		glDepthFunc(GL_LEQUAL);
	}

	// used to initialize the open gl viewport window
	static inline void resize(GLsizei width, GLsizei height)
	{
		// Compute aspect ratio of the new window
	   if (height == 0) height = 1;                // To prevent divide by 0
	   GLfloat aspect = (GLfloat)width / (GLfloat)height;
 
	   // Set the viewport to cover the new window
	   glViewport(0, 0, width, height);
 
	   // Set the aspect ratio of the clipping area to match the viewport
	   glMatrixMode(GL_PROJECTION);  // To operate on the Projection matrix
	   glLoadIdentity();             // Reset the projection matrix
	   if (width >= height) {
		 // aspect >= 1, set the height from -1 to 1, with larger width
		  //gluOrtho2D(-1.0 * aspect, 1.0 * aspect, -1.0, 1.0);
		   gluPerspective(45, aspect, 0.01, -1000.0);
	   } else {
		  // aspect < 1, set the width to -1 to 1, with larger height
		 //gluOrtho2D(-1.0, 1.0, -1.0 / aspect, 1.0 / aspect);
		   gluPerspective(45, aspect, 0.01, -1000.0);
	   }
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
		glMatrixMode(GL_MODELVIEW);      // To operate on Model-View matrix
		glLoadIdentity();                // Reset the model-view matrix
		//glTranslatef((0-centerPoint.x)*magnitude, (0-centerPoint.y)*magnitude, 0.0f); 

		glRotatef(rotatex, 1, 0, 0);
		glRotatef(rotatey, 0, 1, 0);
		glRotatef(rotatez, 0, 0, 1);
		glTranslatef((0 - centerPoint.x), (0 - centerPoint.y), (0 - centerPoint.z) - magnitude );
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

	void render(const Shape& s, const Vector3d& location, std::array<float, 3> color)
	{
		//glBegin(GL_POLYGON);
		// Draw Red
		glPushMatrix();
		glColor3f(color[0], color[1], color[2]);
		GLUquadric* quad = gluNewQuadric();

		glTranslatef(location.x, location.y, location.z);
		gluSphere(quad, s.getRadius(), 20, 20);

		gluDeleteQuadric(quad);
		glPopMatrix();
		/**for (const auto& vertex : v)
		{
			float x = (vertex.x + location.x);//*magnitude;
			float y = (vertex.y + location.y);//*magnitude;
			glVertex3f(x, y, 0.0f);
			//glVertex2f((vertex.x + location.x)*magnitude, (vertex.y + location.y)*magnitude);
		}**/

		//glEnd();
		
		glBegin(GL_POINTS);
		glColor3f(color[0], color[1], color[2]);
		glVertex3f(location.x, location.y, location.z);
		glEnd();
		
		
		
	}

	void refresh()
	{
		glClearColor(0.0f, 0.0f, 0.0f, 0.0f); // Set background color to black and opaque
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

