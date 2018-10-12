#pragma once
#include "Texture2D.h"
#include "VBO.h"
#include "Node.h"
#include "Quad.h"
#include "Display.h"

struct Box : public Node {
	Texture2D image;
	Quad* quad;
	bool invertedTex ;
	float trasparency = 1.0f;
public:
	Box(bool invertedTex=false):invertedTex(invertedTex) {
		this->quad = Quad::getQuad();
	};
	~Box() {};
	static Box* getFastQuad() {
		static Box* box = nullptr;
		if (box == nullptr) {
			box = new Box();
			float w, h;
			Display::getWindow()->getSize(w, h);
			float aspect = (w / h);
			glm::vec2 q = glm::vec2(1.0f, aspect)*0.23f;
			box->setPosition(0.23f, 0.8f);
			box->setScale(q);
			box->setTrasparency(1.0f);
		}
		return box;
	}
	static Box* getFastQuad2() {
		static Box* box = nullptr;
		if (box == nullptr) {
			box = new Box();
			float w, h;
			Display::getWindow()->getSize(w, h);
			float aspect = (w / h);
			glm::vec2 q = glm::vec2(1.0f, aspect)*0.23f;
			box->setPosition(0.78f, 0.8f);
			box->setScale(q);
			box->setTrasparency(1.0f);
		}
		return box;
	}
	static Box* getFastQuadCenter() {
		static Box* box = nullptr;
		if (box == nullptr) {
			box = new Box();
			float w, h;
			Display::getWindow()->getSize(w, h);
			float aspect = (w / h);
			glm::vec2 q = glm::vec2(1.0f, aspect)*0.23f;
			box->setPosition(0.5f, 0.8f);
			box->setScale(q);
			box->setTrasparency(1.0f);
		}
		return box;
	}

	static Box* getFastBigQuad() {
		static Box* box = nullptr;
		if (box == nullptr) {
			box = new Box();
			float w, h;
			Display::getWindow()->getSize(w, h);
			float aspect = (w / h);
			glm::vec2 q = glm::vec2(1.0f, aspect)*0.5f;
			box->setPosition(0.5f, 0.5f);
			box->setScale(q);
			box->setTrasparency(1.0f);
		}
		return box;
	}
	static Box* getFastBox() {
		static Box* box = nullptr;
		if (box == nullptr) {
			box = new Box();
			box->setPosition(0.23f,0.8f);
			box->setScale(0.3f, 0.3f);
			box->setTrasparency(1.0f);
		}
		return box;
	}
	static Box* getFastBox2() {
		static Box* box = nullptr;
		if (box == nullptr) {
			box = new Box();
			box->setPosition(0.83f, 0.8f);
			box->setScale(0.3f, 0.3f);
			box->setTrasparency(1.0f);
		}
		return box;
	}
	static Box* getScreenBox() {
		static Box* box = nullptr;
		if (box == nullptr) {
			box = new Box(true);
			box->setPosition(0.5, 0.5);
			box->setScale(1.0f, 1.0f);
			box->setTrasparency(1.0f);
		}
		return box;
	}
	inline void setTrasparency(float t) { trasparency = t; }
	inline void setPosition(float x, float y) {
		getLocalTrasform().setPosition(x * 2 - 1, 1 - y * 2, 0);
	}
	inline void setScale(float x,float y) {
		getLocalTrasform().setScale(x,y,0);
	}
	inline void setScale(const glm::vec2& v) {
		getLocalTrasform().setScale(v.x,v.y,0);
	}
	inline float getTrasparency() { return trasparency; }
	inline glm::mat4 getTrasformationMatrix() {
		return getLocalTrasform().trasformationMatrix();
	}
	inline Texture* getTexture() { return &image; }
	inline bool getInvertedTex() {
		return invertedTex;
	}
	inline void draw() {
		quad->draw();
	}
	inline void bind() {
		quad->bindMesh();
	}
	inline void unbind() {
		quad->unbindMesh();
	}



};
