#ifndef FILE_H
#define FILE_H

#include <string>

class File {

private:

	int blockID;
	std::string name;
	std::string type;
	std::string content;

public:

	File();
	~File();
	std::string getName();
	std::string getType();
	std::string getContent();

};

#endif // !FILE_H
