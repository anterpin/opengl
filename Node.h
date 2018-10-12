#pragma once
#include <vector>
#include "Trasform.h"


class Node {
private:
	Node* parent = nullptr;
	std::vector<Node*> children;
	Trasform localTrasform;
	Trasform worldTrasform;
public:
	void addChild(Node* child) {
		child->parent = this;
		children.push_back(child);
	}
	virtual ~Node() {
		for (Node* child : children) {
			delete child;
		}
	}
	virtual void update() {
		for (Node* child : children) {
			child->update();
		}
	}
	virtual void input() {
		for (Node* child : children) {
			child->input();
		}
	}
	virtual void render() {
		for (Node* child : children) {
			child->render();
		}
	}
	virtual void shutDown() {
		for (Node* child : children) {
			child->shutDown();
		}
	}
public:
	inline Node* getParent() { return parent; }
	inline std::vector<Node*>& getChildren(){return children; }
	inline Trasform& getLocalTrasform() {
		return localTrasform;
	}
	inline Trasform& getWorldTrasform() {
		return worldTrasform;
	}




};


