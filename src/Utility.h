#pragma once
#include <fstream>
#include <string>
#include "absl\strings\str_cat.h"
namespace util {
	using namespace std;
	inline string loadFile(const string& fileName) {
		ifstream file(fileName, ios::in | ios::ate);
		if (file.is_open())
		{
			size_t size = file.tellg();
			string contents( size, ' ' );
			file.seekg(0, ios::beg);
			file.read(&contents[0], size);
			file.close();
			return move(contents);
		}
		throw exception(absl::StrCat("Failed to load file: ", fileName).c_str());
	}
}
