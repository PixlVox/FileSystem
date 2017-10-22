#include "filesystem.h"
#include <sstream>

FileSystem::FileSystem() {

	//Resets the info used/Creates a newly created instance of them
	this->resetInfo();

}

FileSystem::~FileSystem() {



}

void FileSystem::resetMemBlock(){

	//Resets the memBlock
	this->mBD.reset();

	//Resets the TreeStructure
	this->tree.resetTree();
	this->tree.setNrOfSubs(0);

	//Resets the info used
	this->resetInfo();

}

void FileSystem::resetInfo() {

	//Reset Arrays
	this->emptyIndex[0] = 0;
	this->isFolder[0] = 1;

	for (int i = 1; i < NROFBLOCKS; i++) {

		this->emptyIndex[i] = 1;
		this->isFolder[i] = 0;

	}

	//Writes in the root information
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

std::string FileSystem::listDir(){

	std::string helpStr = "";;
	helpStr += "Type      Name      Size\n";

	if (this->tree.getCurrentSubs() != nullptr) {

		std::string blockStr = "";
		
		for (int i = 0; i < tree.getNrOfCurrentSubs(); i++) {

			blockStr = mBD.readBlock(this->tree.getCurrentSubs()[i]).toString();
			if (this->isFolder[this->tree.getCurrentSubs()[i]] == 1) {
				helpStr += splitBlockStrFolder(blockStr);
			}
			else if (this->isFolder[this->tree.getCurrentSubs()[i]] == 0) {
				helpStr += splitBlockStrFile(blockStr);
			}

		}

	}

	return helpStr;

}

std::string FileSystem::splitBlockStrFolder(std::string blockStr)
{
	std::string blockInfo = "";
	std::istringstream f(blockStr);
	std::string s = "";
	for (int i = 0; i < SPLITARRSIZE; i++) {

		getline(f, s, '|');
		blockInfo += (s + "      ");

	}

	blockInfo += "\n";

	return blockInfo;
}

std::string FileSystem::splitBlockStrFile(std::string blockStr)
{

	std::string blockInfo = "";
	std::istringstream f(blockStr);
	std::string s = "";
	for (int i = 0; i < 3; i++) {

		getline(f, s, '|');

		if (i < 2) {
			blockInfo += (s + "      ");
		}
		else if (i = 2) {
			blockInfo += std::to_string(s.size());
		}

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

int FileSystem::getNrOfSubs(int blockId) {

	int nrOfSubs = -1;
	std::string temp = this->mBD.readBlock(blockId).toString();
	std::string tempSeparated = "";
	int checkString = 0;

	for (int i = 0; i < temp.length() && checkString < 3; i++) {

		if (checkString == 2 && temp.at(i) != '|') {

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
		for (int i = 0; i < temp.length() && checkString < 4; i++) {

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
		for (int j = 0; j < temp.length() && !stopSearchInner; j++) {

			if (temp.at(j) == '|') {

				stopSearchInner = true;

			}

			tempIndex++;

		}

		//Saves filePath
		while (temp.at(tempIndex) != '|') {

			path += temp.at(tempIndex);
			tempIndex++;

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

int FileSystem::searchForBlockId(std::string filePath, int isFolder) const{

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

		if (tempPath == filePath && this->isFolder[subs[i]] == 1 && isFolder == 1) {

			index = subs[i];
			found = true;

		}
		else if (tempPath == filePath && this->isFolder[subs[i]] == 0 && isFolder == 0) {

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

int FileSystem::createFile(std::string fileName, std::string fileContent){

	int error = -1;

	//Finds empty block
	int blockId = this->findEmptyBlock();

	if (blockId != -1) {

		//Search the current subs for a file with the same name
		error = this->findExistingSub(fileName, false);

		if (error == -1) {

			//Create string with the releveant info
			std::string info = "File|" + fileName + '|' + fileContent + '|';

			//Save the data in the correct block
			this->writeToBlock(blockId, info);

			//Updates arrays
			this->isFolder[blockId] = 0;
			this->emptyIndex[blockId] = 0;

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

	return 0;

}

int FileSystem::createFolder(std::string folderName){
	
	int error = -1;

	//Finds empty block
	int blockId = this->findEmptyBlock();
	
	if (blockId != -1) {

		//Search the current subs for a folder with the same name
		error = this->findExistingSub(folderName, true);
		
		if (error == -1) {

			//Create string with the releveant info
			std::string info = "Dir|" + folderName + "|0||";

			//Save the data in the correct block
			this->writeToBlock(blockId, info);

			//Updates arrays
			this->isFolder[blockId] = 1;
			this->emptyIndex[blockId] = 0;

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

int FileSystem::remove(std::string filePath, int isFolder){

	int removed = -1; // -1 = succsess, else failed
	int blockId = searchForBlockId(filePath, isFolder);	//Looks for blockId by searching for the filePath
	
	if (blockId != -1) {

		//Change value of emptyIndex array
		this->emptyIndex[blockId] = 1;

		//Remove the sub in currentDirectory that stores the blockId
		this->tree.removeSub(blockId);

	}
	else {

		//If searched for file = 1, folder = 2
		removed = 1 + isFolder;

	}

	return removed;

}

std::string FileSystem::getFileContentFromBlock(std::string blockStr)
{
	std::string fileContent = "";
	std::istringstream f(blockStr);
	std::string s = "";

	for (int i = 0; i < 3; i++) {

		getline(f, s, '|');

		if (i == 2) {

			fileContent += (s + "      ");

		}
	}

	fileContent += "\n";

	return fileContent;

}

std::string FileSystem::getContentOfFile(std::string filePath)
{
	std::string content = "File not found.\n";
	std::string blockStr = "";

	int blockId = searchForFilePath(filePath, false);

	if (blockId != -1) {

		blockStr = mBD.readBlock(blockId).toString();
		content = getFileContentFromBlock(blockStr);
	
	}

	return content;
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

std::string FileSystem::getWorkingDir() {

	std::string wD = "";
	int arrSize = 5;
	int* index = new int[arrSize];
	int blockId = 0;
	int counter = 0;

	//Add blockId for the working dir in an array
	blockId = this->tree.getPreviousBlockId(counter);
	while (blockId != -1) {

		//Expand intArr if needed
		if (counter == arrSize) {

			arrSize += 5;
			int* temp = new int[arrSize];

			for (int i = 0; i < counter; i++) {

				temp[i] = index[i];

			}

			delete[] index;
			index = temp;

		}

		//Add blockId to arr
		index[counter] = blockId;
		counter++;
		blockId = this->tree.getPreviousBlockId(counter);

	}

	//Put together the workdingDirectory string
	for (int i = (counter - 1); i >= 0; i--) {

		wD += this->getFilePath(index[i]) + '/';

	}

	return wD;

}

std::string FileSystem::getFilePath(int blockId) {

	std::string temp = this->mBD.readBlock(blockId).toString();
	std::string filePath = "";
	int startIndex = 0;

	int checkString = 0;
	for (int i = 0; i < temp.length() && checkString < 2; i++) {

		if (checkString == 1 && temp.at(i) != '|') {

			filePath += temp.at(i);

		}

		if (temp.at(i) == '|') {

			checkString++;

		}

	}

	return filePath;

}

int FileSystem::createImage(std::string filePath) {

	int result = -1;
	std::ofstream file;
	file.open(filePath + ".txt");

	//Save content to file
	//First Row EmptyINdex
	//Second Row isFolder
	//250 Rows of Blocks 0 - 249

	file.close();

	return result;

}

int FileSystem::copyFile(std::string copyPath, std::string newPath) {

	int error = -1;

	//Search for the oldFile in currentDir

	//If found seach for emptyBlockId

	//If found copy over copyPath Content into new Path

	//CreateFile function call with the newPath file

}