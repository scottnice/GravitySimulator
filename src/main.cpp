#pragma once
#define _USING_V110_SDK71_
#include <windows.h>  // for MS Windows
#include <glut.h>  // GLUT, include glu.h and gl.h
#include <string>
#include <sstream>
#include "GravityGame.h"
#include <iostream>
using namespace std;

inline string intToString(int num)
{
	std::stringstream ss;
	ss << num;
	return ss.str();
}

int main(int argc, char** argv)
{

	try
	{
		glutInit(&argc, argv);
		GravityGame g;
		g.init();
		g.play();
	}
	catch(exception & e)
	{
		ios_base::sync_with_stdio(false);
		cerr << e.what();
		system("pause");
	}
	return 1;
}

