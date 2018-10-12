#pragma once
#include <string>
#include "Texture2D.h"
#include <vector>
#include "GuiChar.h"
#include <map>
#include <iostream>
#include <fstream>
#include <sstream>
#include <algorithm>
#include <iterator>
#include "Display.h"

#define LINE_HEIGHT  0.03f
#define SPACE_ASCII 32


struct MetaFile {
	const int PAD_TOP = 0;
	const int PAD_LEFT = 1;
	const int PAD_BOTTOM = 2;
	const int PAD_RIGHT = 3;

	const int DESIRED_PADDING =8;

	const std::string SPLIITER=std::string(" ");
	const std::string NUMBER_SEPARATOR = std::string(",");

	float aspectRatio;

	float verticalPerPixelSize;
	float horizontalPerPixelSize;
	float spaceWidth;

	int scaleH;
	int scaleW;

	std::vector<int> padding;
	int paddingWidth;
	int paddingHeight;
};

const static std::string defaultFontPath = "C:/Users/Alex/Desktop/character/font/CambriaMath.fnt";
struct Font{
private:
	MetaFile info;

public:

	std::map<wchar_t,GuiChar> list;
	std::vector<Texture2D> fontAtlas;

public:
	static Font* getDefaultFont() {
		static Font* instance = nullptr;
		if (instance == nullptr) {
			instance = new Font();
			instance->loadFont(defaultFontPath);
		}
		return instance;
	}
	Font() {
		float w, h;Display::getWindow()->getSize(w, h);
		
		info.aspectRatio = w / h;
	}
	inline MetaFile getMeta() { return info; }
	void print() {
		for (std::pair<wchar_t, GuiChar> ca : list) {
			GuiChar& c = ca.second;
			std::wcout << ca.first;
			printf(" x:%f y:%f w:%f h:%f xO:%f y:%f xA:%f p:%d \n", c.x, c.y, c.w, c.h, c.xOffset, c.yOffset, c.xAdvance, c.page);
		}
		
	}
	void loadFont(const std::string& fileName) {
		
		std::string line;
		std::ifstream file(fileName.c_str());

		if (!file.is_open()){
			std::cout << "could not open file source : " << fileName << std::endl;
		}	
		while (getline(file, line)){
			std::istringstream iss(line);
			std::vector<std::string> tokens{ std::istream_iterator<std::string>{iss},std::istream_iterator<std::string>{} };

			if (tokens[0] == "common") {
				parseCommon(&tokens);
			}
			else if (tokens[0] == "info") {
				parseInfo(&tokens);
			}
			else if (tokens[0] == "chars") {
				parseChars(&tokens);
			}
			else if (tokens[0] == "page") {
				parsePage(&tokens,fileName);
			}
			else if (tokens[0] == "char") {
				parseChar(&tokens);
			}
		}
		file.close();
			
	}
private:
	//lineHeight=56 base=39 scaleW=512 scaleH=512 pages=3 packed=0
	const std::vector<std::string> explode(const std::string& s, const char& c)
	{
		std::string buff{ "" };
		std::vector<std::string> v;

		for (auto n : s)
		{
			if (n != c) buff += n; else
				if (n == c && buff != "") { v.push_back(buff); buff = ""; }
		}
		if (buff != "") v.push_back(buff);

		return v;
	}
	void parseCommon(std::vector<std::string>* tokens) {
		for (int i = 1; i < tokens->size(); i++) {
			std::string& str = tokens->at(i);
			size_t delimiter = str.find_first_of("=");
			std::string v = str.substr(0,delimiter);
			std::string val = str.substr(delimiter + 1, str.size() - delimiter - 1);
			if (v == "lineHeight") {
				int lineHeightPixels = std::stoi(val)-info.paddingHeight;
				info.verticalPerPixelSize = LINE_HEIGHT / (float)lineHeightPixels;
				info.horizontalPerPixelSize = info.verticalPerPixelSize / info.aspectRatio;
			}
			else if (v == "scaleW") {
				info.scaleW = std::stoi(val);
			}
			else if (v == "scaleH") {
				info.scaleH = std::stoi(val);
			}
			else if (v == "pages") {
				fontAtlas.resize(std::stoi(val));
			}
		}
	}
	void parsePage(std::vector<std::string>* tokens,std::string path) {
		unsigned int id = 0;
		for (int i = 1; i < tokens->size(); i++) {
			std::string& str = tokens->at(i);
			size_t delimiter = str.find_first_of("=");
			std::string v = str.substr(0, delimiter);
			std::string val = str.substr(delimiter + 1, str.size() - delimiter - 1);

			
			if (v == "id") {
				id = std::stoi(val);

			}
			else if (v == "file") {
				std::string file = val.substr(1, str.size() -2);
				file.pop_back();
				fontAtlas[id].loadFile(path.substr(0, path.find_last_of("/")+1)+file);	
				fontAtlas[id].bilinearFilter();
			}
		}
	}
	void parseInfo(std::vector<std::string>* tokens) {
		for (int i = 1; i < tokens->size(); i++) {
			std::string& str = tokens->at(i);
			size_t delimiter = str.find_first_of("=");
			std::string v = str.substr(0, delimiter);
			std::string val = str.substr(delimiter + 1, str.size() - delimiter - 1);


			if (v == "padding") {
				const std::vector<std::string> buf=explode(val, this->info.NUMBER_SEPARATOR[0]);
				this->info.padding.resize(buf.size());
				for (int s = 0; s < buf.size();s++) {
					this->info.padding[s] =std::stoi(buf[s]);
				}
				info.paddingWidth = info.padding[info.PAD_LEFT] + info.padding[info.PAD_RIGHT];
				info.paddingHeight = info.padding[info.PAD_TOP] + info.padding[info.PAD_BOTTOM];

			}
		}

	}
	void parseChars(std::vector<std::string>* tokens) {
		
	}
	void parseChar(std::vector<std::string>* tokens) {
		//char id=0       x=0    y=0    width=0    height=0    xoffset=-1   yoffset=0    xadvance=0    page=0    chnl=0 

		GuiChar character;
		wchar_t c = 0;

		int width, height;

		for (int i = 1; i < tokens->size(); i++) {
			std::string& str = tokens->at(i);
			size_t delimiter = str.find_first_of("=");
			std::string v = str.substr(0, delimiter);
			std::string val = str.substr(delimiter + 1, str.size() - delimiter - 1);

			
			if (v == "id") {
				c = (wchar_t)std::stoi(val);
			}
			else if (v == "x") {
				int n = std::stoi(val);
				character.x = ((float) n + (info.padding[info.PAD_LEFT] - info.DESIRED_PADDING)) /(float) info.scaleW;
			}
			else if (v == "y") {
				int n = std::stoi(val);
				character.y = ((float) n + (info.padding[info.PAD_TOP] - info.DESIRED_PADDING))/(float)info.scaleH;
			}
			else if (v == "width") {
				width = std::stoi(val)-(info.paddingWidth - (2 * info.DESIRED_PADDING));;
				character.w = (float)width / (float)info.scaleW;
			}
			else if (v == "height") {
				height = std::stoi(val)- ((info.paddingHeight)-(2 * info.DESIRED_PADDING));;
				character.h = (float)height / (float)info.scaleH;
			}
			else if (v == "xoffset") {
				int n = std::stoi(val);
				character.xOffset = (n+info.padding[info.PAD_LEFT]-info.DESIRED_PADDING)*info.horizontalPerPixelSize;
			}
			else if (v == "yoffset") {
				int n = std::stoi(val);
				character.yOffset = (n + info.padding[info.PAD_TOP] - info.DESIRED_PADDING)*info.verticalPerPixelSize;
			}
			else if (v=="xadvance") {
				int n = std::stoi(val);
				character.xAdvance = (n-info.paddingWidth)*info.horizontalPerPixelSize;
			}
			else if (v=="page") {
				int n = std::stoi(val);
				character.page = n;
			}
		}
		if (c == SPACE_ASCII) {
			info.spaceWidth = (character.xAdvance - info.paddingWidth) * info.horizontalPerPixelSize;
		}

		character.sizeX = width * info.horizontalPerPixelSize;
		character.sizeY = height * info.verticalPerPixelSize;

		list.emplace(c, character);
	}
};