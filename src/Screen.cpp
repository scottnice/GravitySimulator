#include "Screen.h"
namespace GameLib
{

int Screen::DEFAULT_WIDTH;
int Screen::DEFAULT_HEIGHT;

Screen::Screen(const int w, const int h, std::string title)
{
	DEFAULT_WIDTH = glutGet(GLUT_SCREEN_WIDTH);
	DEFAULT_HEIGHT = glutGet(GLUT_SCREEN_HEIGHT);
}

Screen::~Screen()
{
}

}
