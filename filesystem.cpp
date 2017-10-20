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

int FileSystem::searchForFilePath(std::string filePath) {

	int index = -1;
	int stringCheck = 0;
	std::string temp = "";
	std::string tempPath = "";
	const int* subs = this->tree.getCurrentSubs();
	bool found = false;

	for (int i = 0; i < this->tree.getNrOfCurrentSubs() && !found; i++) {

		//Check if it is a folder
		if (isFolder[subs[i]] == 1) {

			//Read the content from block
			temp = this->mBD.readBlock(subs[i]).toString();

			//Handle the content from block to separate the name to compare with filePath
			for (int i = 0; i < temp.length && stringCheck < 2; i++) {

				if (stringCheck == 1 && temp.at[i] != '|') {

					tempPath += temp.at[i];

				}
				else if (temp.at[i] == '|') {

					stringCheck++;

				}

			}

			if (tempPath == filePath) {

				index = subs[i];
				found = true;

			}

			//Reset variables
			temp = "";
			stringCheck = 0;
			tempPath = "";
			found = false;

		}
		

	}

	//Returns the index of the folder in MemBlockDevice, if -1 it was not found
	return index;

}

int FileSystem::changeFolder(std::string filePath) {

	int result = -1;

	if (filePath == "..") {

		result = this->tree.goToPreviousDir();

	}
	else {

		result = this->searchForFilePath(filePath);

		if (result != -1) {



		}

	}
	return this->tree.changeDir(filePath);

}

/* Please insert your code */