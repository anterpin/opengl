#include "Model.h"

Model::~Model(){
	remove();
}
void Model::remove() {
	for (Mesh* mesh : meshes) {
		if (mesh)
			delete mesh;
	}
	for (Material* material : materials) {
		if (material)
			delete material;
	}	
}
