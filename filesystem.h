#ifndef FILESYSTEM_H
#define FILESYSTEM_H

#include "memblockdevice.h"
#include "TreeStructure.h"

static int const NROFBLOCKS = 250;

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

	int findEmptyBlock();
	int findExistingSub(std::string filePath, bool isFolder);

	int searchForFilePath(std::string filePath, bool isFolder);
	int searchForBlockId(std::string filePath) const;

	int create(std::string name, bool isFolder);
	/* Removes a file in the filesystem */
	int removeFile(int blockId);
    /* Removes a folder in the filesystem */
	int removeFolder(int blockId);


public:

    FileSystem();
    ~FileSystem();
	
    void resetMemBlock();
	int remove(std::string filePath);

    /* These API functions need to be implemented
	   You are free to specify parameter lists and return values
    */

    /* This function creates a file in the filesystem */
	int createFile(std::string fileName);

    /* Creates a folder in the filesystem */
	int createFolder(std::string folderName);

    /* Function will move the current location to a specified location in the filesystem */
	int changeDir(std::string filePath);

    /* This function will get all the files and folders in the specified folder */
    // listDir(...);

};

#endif // FILESYSTEM_H
