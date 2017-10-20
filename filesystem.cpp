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

int FileSystem::searchForFilePath(std::string filePath, bool isFolder) {

	int index = -1;
	int stringCheck = 0;
	std::string temp = "";
	std::string tempPath = "";
	const int* subs = this->tree.getCurrentSubs();
	bool found = false;

	for (int i = 0; i < this->tree.getNrOfCurrentSubs() && !found; i++) {

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

			if(this->isFolder[subs[i]] == 1 && isFolder){
			
				index = subs[i];
				found = true;

			}
			else if (this->isFolder[subs[i]] == 0 && !isFolder) {

				index = subs[i];
				found = true;

			}

		}

		//Reset variables
		temp = "";
		stringCheck = 0;
		tempPath = "";
		found = false;

	}

	//Returns the index of the folder in MemBlockDevice, if -1 it was not found
	return index;

}

int FileSystem::changeDir(std::string filePath) {

	int result = -1;

	if (filePath == "..") {

		result = this->tree.goToPreviousDir();

	}
	else {

		result = this->searchForFilePath(filePath, true);

		if (result != -1) {

			this->tree.goToNextDir(result);

		}

	}
	return this->tree.changeDir(filePath);

}

/* Please insert your code */