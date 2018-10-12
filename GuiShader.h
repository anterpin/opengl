#pragma once

#define NUM_ATLASES 3

#include "ShaderProgram.h"
#include <sstream>


class GuiShader :public ShaderProgram {
	Univ3 color;
	Unim4 trasform;

	Unii atlases[NUM_ATLASES];

	Unif width;
	Unif edge;

	//effects
	Unif borderWidth;
	Unif borderEdge;
	Univ2 offset;
	Univ3 outLineColor;
	

public:
	inline static GuiShader* getInstance() {
		static GuiShader* instance = nullptr;
		if (!instance) {
			instance = new GuiShader();
		}
		return instance;
	}
	GuiShader() : ShaderProgram() { init(); }
	~GuiShader() { }
	void bindAttributes()override {
		bindAttrib(0, "position");
		bindAttrib(1, "textCoord");
		bindAttrib(2, "page");
	}
	void getAllUniformLocations()override {

		add(color, "color");
		add(trasform, "trasform");
		add(edge, "edge");
		add(width, "width");
		add(borderWidth, "borderWidth");
		add(borderEdge, "borderEdge");
		add(offset, "offset");
		add(outLineColor, "outLineColor");
		add(color, "color");
		for (int i = 0; i < NUM_ATLASES; i++) {
			std::stringstream ss;
			ss << "atlas[" << i << "]";
			add(atlases[i], ss.str());
		}
		
	}
	void bindShaders()override {
		addVertexShader("gui_VS.glsl");
		addFragmentShader("gui_FS.glsl");
	}
	void loadEffects(const vec2& off, const vec3& col, float w, float e) {

		//load("borderWidth", w);
		//load("borderEdge", e);
		//load("offset", off);
		//load("outLineColor", col);

		borderWidth.useLocation(w);
		borderEdge.useLocation(e);
		offset.useLocation(off);
		outLineColor.useLocation(col);

	}
	void loadVar(const vec3& col,const mat4& trasf) {
		color.useLocation(col);
		//load("color", col);
		//load("trasform", trasf);
		trasform.useLocation(trasf);
		
		if (trasf[0][0]>10) {
			//load("width", 0.51f);
			//load("edge", 0.02f);
			width.useLocation(0.51f);
			edge.useLocation(0.02f);
		}
		else if(trasf[0][0]<1){
			//load("width", 0.46f);
			//load("edge", 0.19f);
			width.useLocation(0.46f);
			edge.useLocation(0.19f);
		}
		else {
			//load("width", (trasf[0][0] - 1)*0.0055555f + 0.46f);
			//load("edge", (trasf[0][0] - 1)*-0.0188888f + 0.19f);
			width.useLocation((trasf[0][0] -1)*0.0055555f+0.46f);
			edge.useLocation((trasf[0][0] - 1)*-0.0188888f + 0.19f);
		}

		for (int i = 0; i < 3; i++) {
			//std::stringstream ss;
			//ss << "atlas[" << i << "]";
			//load(ss.str(), i);
			atlases[i].useLocation(i);
		}
	};

};
