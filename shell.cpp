#include <iostream>
#include <sstream>
#include "filesystem.h"

const int MAXCOMMANDS = 8;
const int NUMAVAILABLECOMMANDS = 15;

std::string availableCommands[NUMAVAILABLECOMMANDS] = {
    "quit","format","ls","create","cat","createImage","restoreImage",
    "rm","cp","append","mv","mkdir","cd","pwd","help"
};

/* Takes usercommand from input and returns number of commands, commands are stored in strArr[] */
int parseCommandString(const std::string &userCommand, std::string strArr[]);
int findCommand(std::string &command);
bool quit();
std::string help();

/* More functions ... */
std::string getFilePath(std::string userCommand);
int searchForInvalidChar(std::string filePath);
void changeFolder(FileSystem& fS, std::string userCommand);
void createFolder(FileSystem& fS, std::string userCommand);
void createFile(FileSystem& fS, std::string userCommand);
void printFileContent(FileSystem& fS, std::string userCommand);
void printWorkingDir(FileSystem& fS);
void listDirectory(FileSystem& fS);
void remove(FileSystem& fS, std::string userCommand);
void createImage(FileSystem& fS, std::string userCommand);
void copyFile(FileSystem& fS, std::string userCommand);
void restoreImage(FileSystem& fS, std::string userCommand);


int main(void) {

	//Check for memory leaks
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);

	FileSystem fS;

	std::string userCommand, commandArr[MAXCOMMANDS];
	std::string user = "user@Hokage";    // Change this if you want another user to be displayed
	std::string currentDir = "/";    // current directory, used for output

    bool bRun = true;

    do {
        std::cout << user << ":" << currentDir << "$ ";
        getline(std::cin, userCommand);

        int nrOfCommands = parseCommandString(userCommand, commandArr);
        if (nrOfCommands > 0) {

            int cIndex = findCommand(commandArr[0]);
            switch(cIndex) {

			case 0: //quit
				bRun = quit();                
                break;
            case 1: // format
				fS.resetMemBlock();
                break;
            case 2: // ls
				listDirectory(fS);
                break;
            case 3: // create
				createFile(fS, userCommand);
                break;
            case 4: // cat
				printFileContent(fS, userCommand);
                break;
            case 5: // createImage
				createImage(fS, userCommand);
                break;
            case 6: // restoreImage
				restoreImage(fS, userCommand);
                break;
            case 7: // rm
				remove(fS, userCommand);
                break;
            case 8: // cp
				copyFile(fS, userCommand);
                break;
            case 9: // append
                break;
            case 10: // mv
                break;
            case 11: // mkdir
				createFolder(fS, userCommand);
                break;
            case 12: // cd
				changeFolder(fS, userCommand);
				break;
            case 13: // pwd
				printWorkingDir(fS);
                break;
            case 14: // help
                std::cout << help() << std::endl;
                break;
            default:
                std::cout << "Unknown command: " << commandArr[0] << std::endl;
            }
        }
    } while (bRun == true);

    return 0;
}

int parseCommandString(const std::string &userCommand, std::string strArr[]) {
    std::stringstream ssin(userCommand);
    int counter = 0;
    while (ssin.good() && counter < MAXCOMMANDS) {
        ssin >> strArr[counter];
        counter++;
    }
    if (strArr[0] == "") {
        counter = 0;
    }
    return counter;
}
int findCommand(std::string &command) {
    int index = -1;
    for (int i = 0; i < NUMAVAILABLECOMMANDS && index == -1; ++i) {
        if (command == availableCommands[i]) {
            index = i;
        }
    }
    return index;
}

bool quit() {
	std::cout << "Exiting\n";
	return false;
}

std::string help() {
    std::string helpStr;
    helpStr += "OSD Disk Tool .oO Help Screen Oo.\n";
    helpStr += "-----------------------------------------------------------------------------------\n" ;
    helpStr += "* quit:                             Quit OSD Disk Tool\n";
    helpStr += "* format;                           Formats disk\n";
    helpStr += "* ls     <path>:                    Lists contents of <path>.\n";
    helpStr += "* create <path>:                    Creates a file and stores contents in <path>\n";
    helpStr += "* cat    <path>:                    Dumps contents of <file>.\n";
    helpStr += "* createImage  <real-file>:         Saves disk to <real-file>\n";
    helpStr += "* restoreImage <real-file>:         Reads <real-file> onto disk\n";
    helpStr += "* rm     <file>:                    Removes <file>\n";
    helpStr += "* cp     <source> <destination>:    Copy <source> to <destination>\n";
    helpStr += "* append <source> <destination>:    Appends contents of <source> to <destination>\n";
    helpStr += "* mv     <old-file> <new-file>:     Renames <old-file> to <new-file>\n";
    helpStr += "* mkdir  <directory>:               Creates a new directory called <directory>\n";
    helpStr += "* cd     <directory>:               Changes current working directory to <directory>\n";
    helpStr += "* pwd:                              Get current working directory\n";
    helpStr += "* help:                             Prints this help screen\n";
    return helpStr;
}

/* Insert code for your shell functions and call them from the switch-case */
std::string getFilePath(std::string userCommand) {

	std::string filePath = "";
	bool stopSearch = false;
	int startIndex = 0;

	//Find end of function call, untill the filePath input starts
	for (int i = 0; i < userCommand.length() && !stopSearch; i++) {

		if (userCommand.at(i) == ' ') {

			stopSearch = true;

		}

		startIndex++;

	}

	//Save the filePath input in a separate string
	for (int i = startIndex; i < userCommand.length(); i++) {

		filePath += userCommand.at(i);

	}

	return filePath;

}

int searchForInvalidChar(std::string filePath) {

	int error = -1;
	bool stopSearch = false;

	if (filePath.length() == 0) {

		error = 4;

	}
	else{
	
		for (int i = 0; i < filePath.length() && !stopSearch; i++) {

			if (filePath.at(i) == '|') {

				error = 3;
				stopSearch = true;

			}

		}
	
	}

	return error;

}

void remove(FileSystem& fS, std::string userCommand) {

	int removed = -1;

	//Separates the filePath
	std::string filePath = getFilePath(userCommand);
	
	//Check for invalid chars
	removed = searchForInvalidChar(filePath);

	if (removed == -1) {

		//Gets input if it should search for a file or folder
		int isFolder = -1;		//0 = File, 1 = Folder
		std::cout << "Search for folder: 1 = Yes, 0 = No: ";
		std::cin >> isFolder;
		std::cin.ignore();

		//Calls the remove function in the filesystem
		removed = fS.remove(filePath, isFolder);

	}

	switch (removed) {

	case 1:
		std::cout << "The file " << filePath << " could not be found!\n";
		break;
	case 2:
		std::cout << "The folder " << filePath << " could not be found!\n";
		break;
	case 3:
		std::cout << "The filepath contains invalid chars!\n";
		break;
	case 4:
		std::cout << "The filepath needs a name!\n";
		break;
	default:
		break;
	}

}

void listDirectory(FileSystem& fS) {
	std::cout << fS.listDir();
}

void printFileContent(FileSystem& fS, std::string userCommand) {
	std::string filePath = getFilePath(userCommand);
	std::cout << fS.getContentOfFile(filePath);
}

void changeFolder(FileSystem& fS, std::string userCommand) {

	std::string filePath = getFilePath(userCommand);
	int result = fS.changeDir(filePath);

	switch (result) {

	case 1:
		std::cout << "There is no root directory!\n";
		break;
	case 2:
		std::cout << "You are in the root directory!\n";
		break;
	case 3:
		std::cout << "There is no directory with the name " << filePath << " in the current directory!\n";
		break;
	default:
		break;

	}

}

void createFolder(FileSystem& fS, std::string userCommand) {

	int result = -1;

	//Separates the filePath
	std::string filePath = getFilePath(userCommand);

	//Checks filePath for invalis char |
	result = searchForInvalidChar(filePath);

	//Tries to create folder
	if (result == -1) {

		result = fS.createFolder(filePath);

	}

	switch (result) {

	case 1:
		std::cout << "There are no empty blocks left!\n";
		break;
	case 2:
		std::cout << "A folder with the same name already exists in the current folder!\n";
		break;
	case 3:
		std::cout << "The folder has invalid characters in its name!\n";
	case 4:
		std::cout << "The folder needs a name!\n";
		break;
	default:
		break;

	}

}

void createFile(FileSystem& fS, std::string userCommand) {

	int result = -1;
	std::string fileContent = "";

	//Separates the filePath
	std::string filePath = getFilePath(userCommand);

	//Checks filePath for invalis char |
	result = searchForInvalidChar(filePath);

	if (result == -1) {

		//Get file input
		std::cout << "Enter text: ";
		std::getline(std::cin, fileContent);

		result = searchForInvalidChar(fileContent);

		//Tries to create file
		if (result == -1) {

			result = fS.createFile(filePath, fileContent);

		}
		else {

			//Increse the result index to make a unique error msg
			result+= 2;	//Error 5

		}

	}

	switch (result) {

	case 1:
		std::cout << "There are no empty blocks left!\n";
		break;
	case 2:
		std::cout << "A file with the same name already exists in the current folder!\n";
		break;
	case 3:
		std::cout << "The file has invalid characters in its name!\n";
		break;
	case 4:
		std::cout << "The file needs a name!\n";
		break;
	case 5:
		std::cout << "The file has invalid characters in its content!\n";
		break;
	default:
		break;

	}

}

void printWorkingDir(FileSystem& fS) {

	std::string wDir = "";
	wDir = fS.getWorkingDir();

	std::cout << wDir << std::endl;

}

void createImage(FileSystem& fS, std::string userCommand) {

	int result = -1;
	std::string filePath = "";
	filePath = getFilePath(userCommand);

	fS.createImage(filePath);

	if (result != -1) {

		switch (result) {

		case 1:
			break;
		default:
			break;
		}

	}

}

void copyFile(FileSystem& fS, std::string userCommand) {

	int result = -1;

	//Get Both FilePaths from userCommand (Assuming that the files does not contain space)
	std::string cpFile = "";
	std::string newFile = "";
	int counter = 0;
	int checkString = 0;
	while (counter != userCommand.length()) {

		if (checkString > 0) {

			if (checkString == 1 && userCommand.at(counter) != ' ') {

				cpFile += userCommand.at(counter);

			}
			else if (checkString == 2 && userCommand.at(counter) != ' ') {

				newFile += userCommand.at(counter);

			}

		}

		if (userCommand.at(counter) == ' ') {

			checkString++;

		}

		counter++;

	}

	result = fS.copyFile(cpFile, newFile);
	
	/*if (result != -1) {

		switch (result) {

		case 1:
			break;
		default:
			break;
		}
			
	}*/

}

void restoreImage(FileSystem & fS, std::string userCommand)
{
	std::string filePath = getFilePath(userCommand);

	int result = fS.readImage(filePath);

	if (result == -1) {
		std::cout << "Image not found.\n";
	}
	else {
		std::cout << "Succsesfully restored image.\n";
	}


}

