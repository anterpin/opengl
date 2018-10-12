#pragma once
#include <string>
#include <vector>
#include <map>
#include <fstream>
#include <sstream>
#include <iostream>
#include <algorithm>
#include "ImporterUtils.h"

struct Tag {
	//tag
	std::map<std::string, std::string> attributes;
	std::string name;

	//inside
	std::vector<Tag*> children;
	std::string data;
	Tag* parent;

	bool selfClose = false;

	bool isLeaf() {
		return children.size() == 0;
	}
	
	Tag* cdAttr(std::string str, const std::string& attr, const std::string& value) {
		for (Tag* t : children) {
			std::string val = t->getValue(attr);
			if (str == t->name && val == value) {
				return t;
			}
		}
		return nullptr;
	}
	Tag* cd(std::string str) {
		std::vector<std::string>& vector = ImporterUtils::splitString(str, '/');

		return cdPath(vector);
	}
	Tag* cdPath(const std::vector<std::string>& vector, unsigned int level = 0) {

		Tag* c = cdSingle(vector[level]);

		if (!c)
			return nullptr;

		if (vector.size() - 1 == level)
			return c;

		return c->cdPath(vector, level + 1);
	}
	void printNameChilden() {
		for (Tag* tag : children) {
			std::cout << tag->name << std::endl;
		}
	}
	Tag* cdSingle(const std::string& name) {

		for (Tag* n : children) {

			if (n->name == name) {
				return n;
			}
		}

		return nullptr;
	}
	Tag* getParent() {
		return parent;
	}
	bool isSetAttr(const std::string& str) {
		std::map<std::string, std::string>::iterator it = attributes.find(str);

		return it != attributes.end();
	}
	void printAttr() {

		for (std::pair<std::string, std::string> s : attributes) {
			std::cout << s.first << " = " << s.second << std::endl;
		}
	}
	std::string getValue(const std::string& str) {
		std::map<std::string, std::string>::iterator it = attributes.find(str);

		if (it == attributes.end())
			return "";

		return it->second;
	}
};
class XMLImporter {
	Tag root;
public:
	~XMLImporter() {
		for (Tag* t : root.children) {
			delete t;
		}
	}
private:
	std::string readName(std::string::iterator first, std::string::iterator last) {

		std::string name;
		for (std::string::iterator i = first + 1; i < last; i++) {

			char c = *i;
			if (c == ' ' || c == '>')
				break;
			name += c;
		}
		return name;
	}
	void readData(std::string::iterator first, std::string::iterator last, std::string& data) {
		for (std::string::iterator i = first; i < last; i++) {
			char c = *i;
			if (c == '<')
				break;
			data += c;
		}
	}

	Tag* readTag(std::string::iterator first, std::string::iterator last, std::string &n) {
		//std::string allTag;
		//readData(first, last, allTag);
		//std::cout << allTag << std::endl;
		std::string name = readName(first, last);
		if (name.size() == 0) {
			n = "";
		}
		if (name.front() == '/') {
			n = name.substr(1, name.size() - 1);
			return nullptr;
		}

		Tag* t = new Tag();


		if (*(last - 1) == '/') {
			t->selfClose = true;
		}
		std::string::iterator endAttr = first + name.size();
		if (name.front() == '?') {//it is comment
			t->selfClose = true;
			name = name.substr(1, name.size() - 1);
		}
		t->name = name;


		while (true) {
			std::string::iterator equal = std::find(endAttr, last, '=');
			if (equal == last) {
				//std::cout << t->attributes.size()<<std::endl;
				return t;
			}
			std::string attr = "", value = "";
			readData(endAttr + 2, equal, attr);


			std::string::iterator secondVirg = std::find(equal + 2, last, '\"');
			readData(equal + 2, secondVirg, value);
			//std::cout << attr << " " << value << std::endl;

			t->attributes[attr] = value;
			endAttr = secondVirg;
		}


		return t;

	}
	void printWithLevel(int level, const std::string& s, bool v = true) {
		std::stringstream ss;
		if (!v) {
			std::cout << s;
			return;
		}
		for (int i = 0; i < level; i++) {
			ss << "  ";
		}
		std::cout << std::endl << ss.str() << s;

	}
	void readContent(std::string::iterator& first, std::string::iterator last, Tag* parent) {
		static int level = 0;

		std::string::iterator openTagR, openTagL;
		std::string name;

		while (true) {
			openTagR = std::find(first, last, '<');
			if (openTagR == last) {
				break;
			}
			openTagL = std::find(openTagR + 1, last, '>');
			if (openTagL == last) {
				break;
			}


			Tag* t = readTag(openTagR, openTagL, name);//openedNewTag

			if (!t) {
				if (parent->name == name)
					break;
				else {
					std::cerr << "Problem with closing tags " << name << std::endl;
					first = last;
				}
				break;
			}



			parent->children.push_back(t);
			t->parent = parent;
			first = openTagL + 1;
			//printWithLevel(level, t->name);
			if (t->selfClose) {
				continue;
			}



			level++;
			readContent(first, last, t);
			level--;

			//printWithLevel(level,'/'+t->name, !t->isLeaf());

		}
		if (parent->isLeaf()) {
			readData(first, last, parent->data);
			parent->data += ' ';
		}
		first = openTagL + 1;


	}
public:
	void load(const std::string& path) {

		std::ifstream file;
		file.open(path.c_str(), std::ios::binary);
		if (!file.is_open()) {
			std::cerr << "Could not open file " << path << std::endl;
			return;
		}
		std::string str((std::istreambuf_iterator<char>(file)),
			std::istreambuf_iterator<char>());

		readContent(str.begin(), str.end(), &root);

		file.close();
	}
protected:
	inline Tag* getRoot() { return &root; }

};