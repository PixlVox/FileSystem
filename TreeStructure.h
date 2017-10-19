#ifndef TREESTRUCTURE_H
#define TREESTRUCTURE_H

#include <string>

class TreeStructure {

private:

	class Node {

	public:

		int blockId;
		Node* previousDir;
		int* sub;
		int nrOfSubs;

	public:

		Node(int blockId, Node* previousDir) {

			this->blockId = blockId;
			this->previousDir = previousDir;
			this->nrOfSubs = 0;
			this->sub = nullptr;

		}

		~Node() {

			//Alla noder raderas i TreeStructure Destruktorn

		}

	};

	
private:

	Node* currentDir;

public:

	TreeStructure();
	~TreeStructure();

	int changeDir(std::string dirName);

};

#endif // !TREESTRUCTURE_H
