#include "VException.h"

VException::VException(int line, const char* file) :
	line(line), file(file)
{
}

const char* VException::what() const
{
	std::ostringstream oss;
	oss << GetType() << std::endl
		<< GetOriginString();
	whatBuffer = oss.str();
	return whatBuffer.c_str();
}

const char* VException::GetType() const
{
	return "VExeption";
}

int VException::GetLine() const
{
	return line;
}

const std::string& VException::GetFile() const
{
	return file;
}

std::string VException::GetOriginString() const
{
	std::ostringstream oss;
	oss << "[File] " << file << std::endl
		<< "[Line] " << line;
	return oss.str();
}
