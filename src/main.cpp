#pragma once
#define _USING_V141_
#define GLEW_STATIC
#include "Gl/glew.h"
#include <windows.h>  // for MS Windows
#include <cstdlib> // needs to be included before glut to fix gluts stupid global namespace declaration of 'exit'
#include <glut.h>  // GLUT, include glu.h and gl.h
#include <string>
#include <sstream>
#include "GravityGame.h"
#include <iostream>
#include <absl\strings\str_cat.h>
using namespace std;

inline string intToString(int num)
{
	return absl::StrCat(num);
}

int main(int argc, char** argv)
{

	try
	{
		glutInit(&argc, argv);
		GravityGame g;
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

