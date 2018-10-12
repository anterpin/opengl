#pragma once
#include <GL\glew.h>
#include "Texture2D.h"

class Material {
#define COLOR_TEX	0
#define NORMAL_TEX	1
private:
	std::vector<Texture2D> textures;

public:
	void init(std::string fileName) {
		textures.resize(2);

		if (!textures[COLOR_TEX].loadFile(fileName)) {
			textures[COLOR_TEX].loadFile("C:/Users/Alex/Desktop/character\\animation\\thinMatrix\\blank.png");
			textures[COLOR_TEX].noFilter();
		}
		else {
			textures[COLOR_TEX].bilinearFilter();
		}
		
	}
	~Material() {}
	inline void bindAndActiveAll(){
		for (int i = 0; i < textures.size();i++) {
			textures[i].bindAndActive(i);
		}
	}
	inline Texture* getColorTexture(){
		return &textures[COLOR_TEX];
	}
	inline Texture* getNormalTexture() {
		return &textures[NORMAL_TEX];
	}


};
