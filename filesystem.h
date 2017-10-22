#ifndef FILESYSTEM_H
#define FILESYSTEM_H

#include "memblockdevice.h"
#include "TreeStructure.h"
#include <fstream>

//Constants
static int const NROFBLOCKS = 250;
static const int SPLITARRSIZE = 3;

class FileSystem
{

private:
    
	MemBlockDevice mBD;	//Blocks are saved containing: Type|Name|Content|
	TreeStructure tree;

	int emptyIndex[NROFBLOCKS];  // 0 = Used, 1 = Empty
	int isFolder[NROFBLOCKS];	//0 = File, 1 = Folder

	//Private functions
	int* getSubs(int blockId);
	int getNrOfSubs(int blockId);
	void writeToBlock(int blockId, std::string content);

	std::string getFileContentFromBlock(std::string blockStr);
	std::string splitBlockStrFolder(std::string blockStr);
	std::string splitBlockStrFile(std::string blockStr);

	int findEmptyBlock();
	int findExistingSub(std::string filePath, bool isFolder);

	int searchForFilePath(std::string filePath, bool isFolder);
	int searchForBlockId(std::string filePath, int isFolder) const;
	std::string getFilePath(int blockId);

	int create(std::string name, bool isFolder);

	void resetInfo();

public:

    FileSystem();
    ~FileSystem();
	
    void resetMemBlock();
	int remove(std::string filePath, int isFolder);

	std::string getContentOfFile(std::string filePath);

    /* These API functions need to be implemented
	   You are free to specify parameter lists and return values
    */

    /* This function creates a file in the filesystem */
	int createFile(std::string fileName, std::string fileContent);

    /* Creates a folder in the filesystem */
	int createFolder(std::string folderName);

    /* Function will move the current location to a specified location in the filesystem */
	int changeDir(std::string filePath);

    /* This function will get all the files and folders in the specified folder */
    std::string listDir();

	//Returns the working directory path
	std::string getWorkingDir();

	int createImage(std::string filePath);

};

#endif // FILESYSTEM_H
