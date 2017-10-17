#ifndef TREESTRUCTURE_H
#define TREESTRUCTURE_H

#include <string>

class TreeStructure {

private:

	class Node {

	public:

		int blockId;
		Node* previousDir;
		Node* nextDir;

	public:

		Node(int blockId, Node* previousDir, Node* nextDir) {

			this->blockId = blockId;
			this->previousDir = previousDir;
			this->nextDir = nextDir;

		}

		~Node() {

		}

	};

private:

	Node* currentDir;

public:

	TreeStructure();
	~TreeStructure();

	void changeDir(std::string dirName);

};

#endif // !TREESTRUCTURE_H
