#pragma once
#include "Mesh.h"
#include "Material.h"
#include <iostream>

class Model
{
protected:
	std::vector<Material*> materials;
	std::vector<Mesh*> meshes;

public:
	Model(std::vector<Material*>* materials,std::vector<Mesh*>* meshes) {

		this->materials.resize(materials->size());
		this->meshes.resize(meshes->size());
		for (int i = 0; i < materials->size(); i++) {
			this->materials[i] = materials->at(i);
			this->meshes[i] = meshes->at(i);
		}
	};
	~Model();
	virtual bool isAnimated() {
		return false;
	}
	virtual void bind(unsigned int i) {
		meshes[i]->bindMesh(3);
	}
	void activeTex(unsigned int i) {
		materials[i]->bindAndActiveAll();
	}
	virtual void unbind(unsigned int i) {
		meshes[i]->unbindMesh(3);
	}

	__forceinline Material* getMaterial(unsigned int i=0) { return materials[i]; }
	__forceinline Mesh* getMesh(unsigned int i=0) { return meshes[i]; }
	__forceinline size_t getSize() { return meshes.size(); }
	void remove();

	bool operator==(Model& model) {
		return meshes[0]->getVaoId() == model.getMesh()->getVaoId();
	}
};

