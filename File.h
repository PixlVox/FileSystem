#ifndef FILE_H
#define FILE_H

#include <string>

class File {

private:

	int blockID;
	std::string name;
	std::string type;

public:

	File();
	~File();

};

#endif // !FILE_H
