#include "filesystem.h"

FileSystem::FileSystem() {

	this->emptyIndex[0] = 0;
	this->isFolder[0] = 1;

	for (int i = 1; i < (NROFBLOCKS - 1); i++) {

		this->emptyIndex[i] = 1;
		this->isFolder[i] = 0;

	}

}

FileSystem::~FileSystem() {



}

void FileSystem::resetMemBlock(){

	this->mBD.reset();

}

int FileSystem::changeFolder(std::string filePath) {

	return this->tree.changeDir(filePath);

}


/* Please insert your code */