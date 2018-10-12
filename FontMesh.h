#pragma once
#include "Font.h"
#include "ModelType.h"
#include "VBO.h"
#include <GLM\glm.hpp>
#include "Node.h"


class FontMesh :public VBO,Node {
private:
	Font* font;

	glm::vec3 color;

	float lineMaxSize;

	//effect
	float borderWidth = 0;
	float borderEdge = 0.4f;
	glm::vec2 offset = glm::vec2(0, 0);
	glm::vec3 outLineColor = glm::vec3(0, 0, 0);


public:
	FontMesh(Font* font, const std::wstring& msg, const glm::vec2& translation, const glm::vec2& scale, float maxLineLength, const glm::vec3& color = glm::vec3(0, 0, 0))
		:color(color), font(font), lineMaxSize(maxLineLength) {
		getLocalTrasform().setPosition(translation.x * 2 - 1, 1 - translation.y * 2, 0);
		getLocalTrasform().setScale(scale.x, scale.y, 0);
		genVao();
		generateBufferData(font, msg);
	}
	inline glm::mat4 getTrasform() {
		return getLocalTrasform().trasformationMatrix();
	}
	inline Font* getFont() { return font; }
	inline void setFont(Font* f) { font = f; }
	inline glm::vec3 getColor() { return color; }
	inline glm::vec3 getOutLineColor() { return outLineColor; }
	inline glm::vec2 getOffset() { return offset; }
	inline float getBorderWidth() { return borderWidth; }
	inline float getBorderEdge() { return borderEdge; }
	inline unsigned int getVao() { return vaoId; }
	inline size_t getSize() { return size; }
	inline void setEffect(const glm::vec3& col,const glm::vec2& off,float w,float e) {
		borderWidth = w;
		borderEdge = e;
		outLineColor = col;
		offset = off;
	}
	inline void resetEffect() {
		borderWidth = 0;
		borderEdge = 0.4f;
		offset = glm::vec2(0, 0);
		outLineColor = glm::vec3(0, 0, 0);
	}
	inline void draw(){
		VBO::draw(GL_QUADS);
	}
	void changeText(std::wstring str) {
		removeVBOs();
		generateBufferData(font, str);
	}
private:
	double calcWorldMaxLength(Font* f, const std::wstring& str) {
		double max = 0;
		
		double length = 0;

		for (int i = 0; i < str.size(); i++) {
			std::map<wchar_t, GuiChar>::iterator& it = f->list.find(str[i]);
			if (it == f->list.end())
				continue;
			GuiChar& gui = it->second;
			if (it->first == SPACE_ASCII) {
				if (length > max) {
					max = length;
					length = 0;
				}
				continue;
			}
			length += gui.xAdvance * gui.size;
		}
	}
	void generateBufferData(Font* f, const std::wstring& str) {
		std::vector<glm::vec2> vertices(str.size() * 4);
		std::vector<glm::vec2> coords(str.size() * 4);
		std::vector<int> pages(str.size() * 4);
		float cursor = 0;
		float height = 0;
		size = str.size()*4;

	
		float cursorStateWord = cursor;
		int ptrStateWord = 0;
		int wordPerLine = 0;

		for (int i = 0; i < str.size();i++) {
			std::map<wchar_t, GuiChar>::iterator& it = f->list.find(str[i]);
			if (it == f->list.end())
				continue;

			GuiChar& gui = it->second;
			//p0---p3
			// | / |
			// p1--p2
			//test if letter to insert overtlap the line length
			if (cursor+ gui.xAdvance * gui.size > lineMaxSize) {
				
				height += LINE_HEIGHT*  gui.size;

				if (wordPerLine>0) {
					//changing prevpositions;
					for (int j = ptrStateWord; j < i; j++) {
						for (int v = 0; v < 4; v++) {
							vertices[j * 4 + v].x -= cursorStateWord;
							vertices[j * 4 + v].y -= LINE_HEIGHT*  gui.size;
						}
					}
					
					cursor -= cursorStateWord;
				}
				else if (it->first == SPACE_ASCII ) {
					wordPerLine = 0;
					cursor = 0;
					cursorStateWord = cursor;
					ptrStateWord = i + 1;
					continue;
				}
				else{
					cursor = 0;
				}
				cursorStateWord = cursor;
				ptrStateWord = i + 1;
				wordPerLine = 0;
			}

			if (it->first == SPACE_ASCII || it->first == 46 || it->first == 44 || it->first == 58 || it->first == 59) {//. , ; :
				wordPerLine ++;
				cursorStateWord = cursor + gui.xAdvance * gui.size;
				ptrStateWord = i + 1;
			}
		
			for (int x = 0; x < 2; x++) {
				for (int y = 0; y < 2; y++) {
					glm::vec2 vertex(cursor + gui.xOffset*gui.size + (gui.sizeX*gui.size)*x, -height - gui.yOffset*gui.size - (gui.sizeY*gui.size*(y^x)));//XOR
					

					vertices[i * 4 + x * 2 + y] = vertex;

					glm::vec2 uv(gui.x + (gui.w*x), (gui.y + (gui.h*(y^x))));

					coords[i * 4 + x * 2 + y] = uv;

					pages[i * 4 + x * 2 + y] = gui.page;

				}
			}

			cursor+= gui.xAdvance * gui.size;
		}
		
		bindVao();

		storeDataInAttribList(0, vertices);
		storeDataInAttribList(1, coords);
		storeDataInAttribList(2, pages);

		unbindVao();
	}

};