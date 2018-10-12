#pragma once
#include "Texture.h"
#include <vector>
#include "ShaderProgram.h"

class Compute {
protected:
	std::vector<Texture*> maps;
	ShaderProgram* shader;

	
	void init(Texture* t,int N, bool linearfilter = false) {
		t->generate();
		t->bind();
		if (linearfilter) {
			t->bilinearFilter();
		}
		else {
			t->noFilter();
		}
		
		t->storage2D(N,N);

		maps.push_back(t);
	}
	void initLog(Texture* t, int N, bool linearfilter = false) {
		t->generate();
		t->bind();
		if (linearfilter) {
			t->bilinearFilter();
		}
		else {
			t->noFilter();
		}
		int log_2_N = (int)(std::log(N) / std::log(2));
		t->storage2D(log_2_N, N);

		maps.push_back(t);
	}

	inline void setShader(ShaderProgram* shader) {
		this->shader = shader;
	}
public:
	virtual ~Compute() {
		if (!shader) {
			delete shader;
		}
		
	}
};