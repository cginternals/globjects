#include <fstream>
#include <iostream>
#include <sstream>

#include <glow/internal/FileReader.h>

using namespace glow::internal;

bool FileReader::readFile(const std::string& filePath, std::string& content)
{
	// http://insanecoding.blogspot.de/2011/11/how-to-read-in-file-in-c.html

	std::ifstream in(filePath, std::ios::in | std::ios::binary);

	if (!in) return false;

	content = std::string((std::istreambuf_iterator<char>(in)), std::istreambuf_iterator<char>());

	return true;
}
