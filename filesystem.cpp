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
		for (int i = 0; i < temp.length() && stringCheck < 2; i++) {

			if (stringCheck == 1 && temp.at(i) != '|') {

				tempPath += temp.at(i);

			}
			else if (temp.at(i) == '|') {

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

<<<<<<< HEAD
=======

>>>>>>> a3db7faf95dac6817ffd9abe336219e5b8212c97
int FileSystem::getNrOfSubs(int blockId) {

	int nrOfSubs = -1;
	std::string temp = this->mBD.readBlock(blockId).toString();
	std::string tempSeparated = "";
	int checkString = 0;

	for (int i = 0; i < temp.length() < 3; i++) {

		if (checkString == 2) {

			tempSeparated += temp.at(i);

		}

		if (temp.at(i) == '|') {

			checkString++;

		}

	}

	nrOfSubs = stoi(tempSeparated);

	return nrOfSubs;

}

int* FileSystem::getSubs(int blockId) {

	int* subs = nullptr;
	int subsCounter = 0;

	//Checks the amount of subs
	int nrOfSubs = this->getNrOfSubs(blockId);

	if (nrOfSubs > 0) {

		std::string temp = this->mBD.readBlock(blockId).toString();
		std::string tempSeparated = "";

		subs = new int[nrOfSubs];

		int checkString = 0;
		for (int i = 0; i < temp.length() < 4; i++) {

			if (checkString == 3) {

				if (temp.at(i) == ',') {

					subs[subsCounter] = stoi(tempSeparated);
					subsCounter++;
					tempSeparated = "";

				}
				else {

					tempSeparated = temp.at(i);

				}

			}

			if (temp.at(i) == '|') {

				checkString++;

			}

		}

	}

	return subs;
}

<<<<<<< HEAD
}

=======
>>>>>>> a3db7faf95dac6817ffd9abe336219e5b8212c97
int FileSystem::searchForBlockId(std::string filePath) const
{
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
		for (int i = 0; i < temp.length() && stringCheck < 2; i++) {

			if (stringCheck == 1 && temp.at(i) != '|') {

				tempPath += temp.at(i);

			}
			else if (temp.at(i) == '|') {

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

	//Returns the index of the folder in MemBlockDevice, if -1 it was not found
	return index;
}

int FileSystem::getTypeFromBlockId(int BlockId) const
{
	int type = -1;
	if (BlockId != -1) {
		type = isFolder[BlockId];
	}
	return type;
}

int FileSystem::create(std::string name, bool isFolder)
{
	return 0;
}

int FileSystem::createFile(std::string fileName)
{
	return 0;
}

int FileSystem::createFolder(std::string folderName)
{
	return 0;
}

int FileSystem::remove(std::string filePath)
{
	int removed = 0; // 0 = failed, 1 = succsess

	int blockId = searchForBlockId(filePath);
	int isFolder = getTypeFromBlockId(blockId); //0 = File, 1 = Folder

	if (isFolder = 0) {
		removed = removeFile(blockId);
		
	}
	else if (isFolder = 1) {
		removed = removeFolder(blockId);
		
	}

	return removed;
}

int FileSystem::removeFile(int blockId)
{
	int removed = 0; // 0 = failed, 1 = succsess
	
	//if file or folder is found, set its block to empty.
	if (blockId != -1) {
		emptyIndex[blockId] = 1;
		removed = 1;
	}

	return removed;
}

int FileSystem::removeFolder(int blockId)
{
	int removed = 0; // 0 = failed, 1 = succsess
	
	//if file or folder is found, set its block to empty.
	if (blockId != -1) {
		emptyIndex[blockId] = 1;
		removed = 1;
	}

	return removed;

}

int FileSystem::changeDir(std::string filePath) {

	int result = -1;

	if (filePath == "..") {

		result = this->tree.goToPreviousDir();

	}
	else {

		result = this->searchForFilePath(filePath, true);

		if (result != -1) {

			this->tree.goToNextDir(result, this->getNrOfSubs(result), this->getSubs(result));
			result = -1;

		}
		else {

			result = 3;

		}

	}

	return result;

}

/* Please insert your code */