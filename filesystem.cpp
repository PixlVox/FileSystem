#include "filesystem.h"
#include <sstream>

FileSystem::FileSystem() {

	this->emptyIndex[0] = 0;
	this->isFolder[0] = 1;

	for (int i = 1; i < (NROFBLOCKS - 1); i++) {

		this->emptyIndex[i] = 1;
		this->isFolder[i] = 0;

	}

	//Creates root dir in memBlock using the structure
	//Type|FilePath|NrOfSubs|Subs(Separated by ,)|
	this->writeToBlock(0, "Dir|Root|0||");

}

FileSystem::~FileSystem() {



}

void FileSystem::resetMemBlock(){

	this->mBD.reset();
	this->writeToBlock(0, "Dir|Root|0||");

}

void FileSystem::writeToBlock(int blockId, std::string content) {
	
	//Takes the input for the block and fills the rest with zeros
	std::string temp = content;
	int startIndex = content.length();

	for (int i = startIndex; i < 512; i++) {

		temp += '0';

	}

	this->mBD.writeBlock(blockId, temp);

}

std::string FileSystem::listDir()
{
	int* subs = getSubs(tree.getCurrentBlockId());
	std::string blockStrArr[SPLITARRSIZE];
	std::string blockStr = "";
	std::string helpStr;
	helpStr += "Type      Name      Size\n";
	
	for (int i = 0; i < tree.getNrOfCurrentSubs(); i++) {
		blockStr = mBD.readBlock(subs[i]).toString();
		helpStr += splitBlockStr(blockStr, blockStrArr);
	}


	return helpStr;
}

std::string FileSystem::splitBlockStr(std::string blockStr, std::string blockStrArr[])
{
	std::string blockInfo;
	std::string strings[SPLITARRSIZE];
	std::istringstream f("Dir|Folder1|2|6,3|");
	std::string s;
	for (int i = 0; i < SPLITARRSIZE; i++) {
		getline(f, s, '|');
		blockInfo += (s+"      ");
	}
	blockInfo += "\n";

	return blockInfo;
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
		for (int j = 0; j < temp.length() && stringCheck < 2; j++) {

			if (stringCheck == 1 && temp.at(j) != '|') {

				tempPath += temp.at(j);

			}
			else if (temp.at(j) == '|') {

				stringCheck++;

			}

		}

		if (tempPath == filePath) {

			//Crash
			if(this->isFolder[subs[i]] == 1 && isFolder){

				index = subs[i];
				found = true;

			}
			else if (this->isFolder[subs[index]] == 0 && !isFolder) {

				index = subs[index];
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

int FileSystem::findEmptyBlock() {

	bool stopSearch = false;
	int blockId = -1;

	for (int i = 0; i < NROFBLOCKS && !stopSearch; i++) {

		if (this->emptyIndex[i] == 1) {

			blockId = i;
			stopSearch = true;

		}

	}

	return blockId;

}

int FileSystem::findExistingSub(std::string filePath, bool isFolder) {

	int error = -1;

	std::string path = "";
	std::string temp = "";
	int tempIndex = 0;
	bool stopSearchOuter = false;
	bool stopSearchInner = false;
	const int* subs = this->tree.getCurrentSubs();

	//Checks every subs filePath and compares with the new one & type
	for (int i = 0; i < this->tree.getNrOfCurrentSubs() && !stopSearchOuter; i++) {

		//Saves the subs content from block in a string
		temp = this->mBD.readBlock(subs[i]).toString();

		//Finds start of filePath
		for (int j = 0; j < temp.length() && !stopSearchInner; i++) {

			if (temp.at(j) == '|') {

				stopSearchInner = true;

			}

			tempIndex++;

		}

		//Saves filePath
		while (temp.at(tempIndex) != '|') {

			path += temp.at(tempIndex);

		}

		//Compares
		if (filePath == path && this->isFolder[subs[i]] == 1 && isFolder) {

			stopSearchOuter = true;

		}
		else if (filePath == path && this->isFolder[subs[i]] == 0 && !isFolder) {

			stopSearchOuter = true;

		}
		else {

			//Resets the variables
			temp = "";
			path = "";
			stopSearchInner = false;
			tempIndex = 0;

		}

	}

	//Returns error msg
	if (stopSearchOuter) {

		error = 2;

	}

	return error;

}

int FileSystem::searchForBlockId(std::string filePath) const{

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

int FileSystem::create(std::string name, bool isFolder){

	return 0;

}

int FileSystem::createFile(std::string fileName){

	return 0;

}

int FileSystem::createFolder(std::string folderName){
	
	int error = -1;

	//Finds empty block
	int blockId = this->findEmptyBlock();
	
	if (blockId != -1) {

		//Search the current subs for a folder with the same name
		error = this->findExistingSub(folderName, true);

		if (error = -1) {

			//Create string with the releveant info
			std::string info = "Dir|" + folderName + "|0||";

			//Save the data in the correct block
			this->writeToBlock(blockId, info);

			//Updates arrays
			this->isFolder[blockId] = 1;
			this->isFolder[blockId] = 0;

			//Adds sub to current node in tree
			this->tree.setNewSub(blockId);

			//Update the blocks data for the current folder
			std::string updatedData = "";
			std::string oldData = this->mBD.readBlock(this->tree.getCurrentBlockId()).toString();

			//Copies the data that has not changed (Type & Name)
			int checkString = 0;
			for (int i = 0; i < oldData.length() && checkString < 2; i++) {

				if (oldData.at(i) == '|') {

					checkString++;

				}

				updatedData += oldData.at(i);

			}

			//Adds the updated data (Number of subs and the subs)
			updatedData += std::to_string(this->tree.getCurrentBlockId()) + '|';

			const int* currentSubs = this->tree.getCurrentSubs();

			for (int i = 0; i < this->tree.getNrOfCurrentSubs(); i++) {

				updatedData += std::to_string(currentSubs[i]) + ',';

			}

			updatedData += '|';

			//Writes the new data to the correct block
			this->writeToBlock(this->tree.getCurrentBlockId(), updatedData);

		}

	}
	else {

		error = 1;

	}

	return error;

}

int FileSystem::remove(std::string filePath){

	int removed = 0; // 0 = failed, 1 = succsess

	int blockId = searchForBlockId(filePath);	//Fins block index of filepath
							
	//isFolder value, 0 = File, 1 = Folder
	if (this->isFolder[blockId] = 0) {

		removed = removeFile(blockId);
		
	}
	else {

		removed = removeFolder(blockId);
		
	}

	return removed;

}

int FileSystem::removeFile(int blockId){

	int removed = 0; // 0 = failed, 1 = succsess
	
	//if file or folder is found, set its block to empty.
	if (blockId != -1) {
		emptyIndex[blockId] = 1;
		removed = 1;
	}

	return removed;

}

int FileSystem::removeFolder(int blockId){

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