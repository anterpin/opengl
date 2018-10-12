#pragma once
#include "ShaderProgram.h"
#include "Texture2DMultisample.h"

class ImageShader : public ShaderProgram {
private:
	Unii image;
	Unim4 trasform;
	Unif trasparency;
	Unib linealizeDepth;
	Unib invertedTex;

	Unii imageMs;
	Unii msaa;

	Unib useMsaa;

public:
	ImageShader() :ShaderProgram() {
		init();
	}
	~ImageShader() {};

	inline static ImageShader* getInstance() {
		static ImageShader* instance = nullptr;
		if (instance == nullptr) {
			instance = new ImageShader();
		}
		return instance;
	}
	void bindShaders() {
		addVertexShader("image_VS.glsl");
		addFragmentShader("image_FS.glsl");
	}
	void getAllUniformLocations() {
		add(image, "image");
		add(trasform, "trasform");
		add(trasparency, "trasp");
		add(linealizeDepth, "linealizeDepth");
		add(invertedTex, "invertedTex");

		add(imageMs, "imageMs");
		add(msaa, "msaa");
		add(useMsaa, "useMsaa");
		
	}
	void loadVariable(const glm::mat4& tras, float t, bool inv, Texture* tex, bool linearDepth = false) {

		if (typeid(*tex).hash_code() == typeid(Texture2DMultisample).hash_code()) {
			imageMs.useLocation(0);
			msaa.useLocation(((Texture2DMultisample*)tex)->getMsaaSample());
			useMsaa.useLocation(true);
		}
		else {
			image.useLocation(0);
			useMsaa.useLocation(false);
		}
		invertedTex.useLocation(inv);
		trasform.useLocation(tras);
		trasparency.useLocation(t);
		linealizeDepth.useLocation(linearDepth);
		
	}
	void bindAttributes() {
		bindAttrib(0, "position");
		bindAttrib(1, "textCoords");
	}

};