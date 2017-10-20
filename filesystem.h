#ifndef FILESYSTEM_H
#define FILESYSTEM_H

#include "memblockdevice.h"
#include "TreeStructure.h"

static int const NROFBLOCKS = 250;

class FileSystem
{

private:
    
	MemBlockDevice mBD;	//Blocks are saved containing: Type|Name|Content(ex if DIR: 5,3,6)
	TreeStructure tree;

	int emptyIndex[NROFBLOCKS];  // 0 = Used, 1 = Empty
	int isFolder[NROFBLOCKS];	//0 = File, 1 = Folder

	int remove(std::string filePath, bool isFolder);

public:

    FileSystem();
    ~FileSystem();

    void resetMemBlock();
	int searchForFilePath(std::string filePath, bool isFolder);

    /* These API functions need to be implemented
	   You are free to specify parameter lists and return values
    */

    /* This function creates a file in the filesystem */
    // createFile(...)

    /* Creates a folder in the filesystem */
    // createFolderi(...);

    /* Removes a file in the filesystem */
	int removeFile(std::string filePath);

    /* Removes a folder in the filesystem */
	int removeFolder(std::string filePath);

    /* Function will move the current location to a specified location in the filesystem */
	int changeDir(std::string filePath);

    /* This function will get all the files and folders in the specified folder */
    // listDir(...);

    /* Add your own member-functions if needed */

};

#endif // FILESYSTEM_H
