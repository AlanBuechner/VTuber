#pragma once
#include <exception>
#include <string>
#include <iostream>
#include <sstream>


class VException : public std::exception
{
public:
	VException(int line, const char* file);
	const char* what() const override;
	virtual const char* GetType() const;
	int GetLine() const;
	const std::string& GetFile() const;
	std::string GetOriginString() const;
private:
	int line;
	std::string file;
protected:
	mutable std::string whatBuffer;
};
