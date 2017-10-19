#ifndef FILESYSTEM_H
#define FILESYSTEM_H

#include "memblockdevice.h"
#include "TreeStructure.h"

static int const NROFBLOCKS = 250;

class FileSystem
{

private:
    
	MemBlockDevice mBD;
	TreeStructure tree;

	int emptyIndex[NROFBLOCKS];
	int isFolder[NROFBLOCKS];

public:

    FileSystem();
    ~FileSystem();

    void resetMemBlock();

    /* These API functions need to be implemented
	   You are free to specify parameter lists and return values
    */

    /* This function creates a file in the filesystem */
    // createFile(...)

    /* Creates a folder in the filesystem */
    // createFolderi(...);

    /* Removes a file in the filesystem */
    // removeFile(...);

    /* Removes a folder in the filesystem */
    // removeFolder(...);

    /* Function will move the current location to a specified location in the filesystem */
	int changeFolder(std::string filePath);

    /* This function will get all the files and folders in the specified folder */
    // listDir(...);

    /* Add your own member-functions if needed */

};

#endif // FILESYSTEM_H
