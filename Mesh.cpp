#include "Mesh.h"

  



Mesh::Mesh(){}
void Mesh::draw() {
	glDrawElements(GL_TRIANGLES, size, GL_UNSIGNED_INT, 0);	
}	
void Mesh::loadToVao(RawModel& m,bool anim) {

	genVao();
	bindVao();

	storeIndices(m.indices);

	size = m.indices.size()*3;

	storeDataInAttribList(0,m.vertices);
	storeDataInAttribList(1,m.uvs);
	storeDataInAttribList(2,m.normals);

	if (anim) {
		storeDataInAttribList(3, m.jointId);
		storeDataInAttribList(4, m.weight);
	}
	
	unbindVao();

	m.free();

}