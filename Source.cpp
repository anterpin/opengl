#include <iostream>
#include "Engine.h"
#include "Render.h"
#include "Terrain.h"
#include "Water.h"
#include "Box.h"
#include "MeshImporter.h"
#include "FrameBuffer.h"
#include "FrameBufferMultisample.h"
#include "AnimatedObject.h"
#include "CameraBlock.h"
#include "SkyDome.h"
#include "AnimatedModel.h"

#define WIN32_LEAN_AND_MEAN

std::vector<GameObject*> objects;
Model* createModel(char* name) {

	MeshData model;
	Animation* a = new Animation();
	std::vector<Mesh*> meshes;
	std::vector<Material*> materials;
	Importer::loadFromFile(name, model, *a);

	if (model.meshes.size() == 0) {
		meshes.resize(1);
		materials.resize(1);
		meshes[0] = new Mesh();
		materials[0] = new Material();
		delete a;
		return new Model(&materials,&meshes);
	}

	meshes.resize(model.meshes.size());
	materials.resize(model.meshes.size());


	for (int i = 0; i < model.meshes.size(); i++) {

		meshes[i] = new Mesh();
		materials[i] = new Material();

		meshes[i]->loadToVao(model.meshes[i], model.hasAnimation);
		meshes[i]->boundingBox = model.meshes[i].box;
		meshes[i]->boundingBox.initForRender();


		materials[i]->init("/path/to/image/texture");

	}

	if (model.hasAnimation) {
		return new AnimatedModel(&materials, &meshes,a,model.root);
	}
	else {
		delete a;
		return new Model(&materials, &meshes);
	}

}
void addModel(char* path) {
	Model* m = nullptr;
	//m = new Model(path);

	m = createModel(path);
	GameObject* g = nullptr;
	if (m->isAnimated()) {
		g = new AnimatedObject(m);
	}
	else {
		g = new GameObject(m);
	}
	g->getTrasform().setScale(50);
	g->getTrasform().setPosition(0,300, 0);
	//g->getTrasform().increaseRotation(-90.0f, 0.0f, 0.0f);

	objects.push_back(g);
}
void clearModels() {
	for (GameObject* g : objects) {
		Model* m = g->getModel();
		delete m;
	}
}
int main(int argc,char** argv) {
	
	Engine* engine = Engine::getEngine();
	engine->createWindow();
	Camera::getInstance()->setPosition(0, 200, 0);
	engine->init();
	addModel("/path/to/obj/model");

	Terrain terrain;
	Water water;
	terrain.setShift(-150);
	//water.setShift(300);
	terrain.Init("terrain_settings.txt");
	water.Init("oceanwater_settings.txt");
	
	
	FrameBuffer refraction;
	FrameBuffer reflection;
	FrameBufferMultisample frame;
	FrameBuffer f;
	water.setReflRefract(reflection.getColorTexture(), refraction.getColorTexture());
	CameraBlock* block = CameraBlock::getInstance();
	while (!engine->getCloseRequest()) {
		
		block->update();//inside camera move
		terrain.updateQuadTree();
		water.updateQuadTree();
		//objects[0]->getTrasform().increaseRotation(0.0f, 0.3f, 0.0f);
		//objects[0]->getTrasform().increaseRotation(0.6f, 0.3f, 0.9f);
		objects[0]->update();
		//refraction.renderRefraction(&objects,&terrain,water.getShift());
		reflection.renderReflection(&objects, &terrain, water.getShift());
		
		frame.bind();
	//	f.bind();
		Render::prepare();
		Render::RenderScene(&objects, &terrain);
		Render::render(&water);

		frame.unbind();
		//f.unbind();
		frame.resolveToNormalFrameBuffer(&f);

		//GuiRenderer::render(Box::getFastQuad(), water.getConfig()->getDXDisplacement());
	//	GuiRenderer::render(Box::getFastQuadCenter(), water.getConfig()->getDYDisplacement());
		
		//f.renderColorTexture(Box::getScreenBox());

		GuiRenderer::render(Box::getScreenBox(), f.getColorTexture());
		//GuiRenderer::render(label);
		//GuiRenderer::render(Box::getFastQuad2(), water.getConfig()->getLightMap());
		//GuiRenderer::render(Box::getFastQuad(), water.getConfig()->getTotalDisplacement());

		//refraction.renderColorTexture(Box::getFastBox());
		//reflection.renderColorTexture(Box::getFastBox2());
	}
		
	clearModels();
	delete CameraBlock::getInstance();

	delete engine;

	return 0;
}
