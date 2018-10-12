#include "MeshImporter.h"

void collapse(RawModel* in, RawModel* out) {

	Simplify s;
	s.Init(in);
	s.simplify_mesh(in->indices.size()*0.01f);
	s.Return(out);
}
void toRaw(aiMesh* mesh, RawModel* model,unsigned int& prevVertSize,unsigned int& prevIndSize) {
	unsigned int startI = prevIndSize;
	unsigned int startV = prevVertSize;
	prevVertSize += mesh->mNumVertices;
	prevIndSize += mesh->mNumFaces;
	model->vertices.resize(prevVertSize);
	model->uvs.resize(prevVertSize);
	model->normals.resize(prevVertSize);
	model->indices.resize(prevIndSize);
	
	for (unsigned int i = 0; i < mesh->mNumFaces; i++){
		const aiFace& face = mesh->mFaces[i];
		glm::uvec3 f;
		f.x = face.mIndices[0] + startV;
		f.y = face.mIndices[1] + startV;
		f.z = face.mIndices[2] + startV;
		model->indices[i+startI] = f;
	}

	for (int i = 0; i < mesh->mNumVertices; i++) {
		aiVector3D pos = mesh->mVertices[i];
		aiVector3D uv = mesh->mTextureCoords[0][i];
		aiVector3D normal = mesh->HasNormals() ? mesh->mNormals[i] : aiVector3D(1.0f, 1.0f, 1.0f);

		glm::vec3 p; glm::vec2 u; glm::vec3 n;
		p.x = pos.x; p.y = pos.y; p.z = pos.z;
		u.x = uv.x; u.y = uv.y;
		n.x = normal.x; n.y = normal.y; n.z = normal.z;
		model->vertices[i+startV] = p;
		model->uvs[i+startV] = u;
		model->normals[i+startV] = n;

	}
}
void appendRaw(RawModel* toAppend, RawModel* model, unsigned int& prevVertSize, unsigned int& prevIndSize) {
	unsigned int startI = prevIndSize;
	unsigned int startV = prevVertSize;
	prevVertSize += toAppend->vertices.size();
	prevIndSize += toAppend->indices.size();
	model->vertices.resize(prevVertSize);
	model->uvs.resize(prevVertSize);
	model->normals.resize(prevVertSize);
	model->indices.resize(prevIndSize);
	for (unsigned int i = 0; i < toAppend->indices.size(); i++) {
		const glm::uvec3& face = toAppend->indices[i];
		glm::uvec3 f;
		f.x = face.x + startV;
		f.y = face.y + startV;
		f.z = face.z + startV;
		model->indices[i + startI] = f;
	}
	for (int i = 0; i < toAppend->vertices.size(); i++) {
		glm::vec3 pos = toAppend->vertices[i];

		glm::vec3 p;
		p.x = pos.x; p.y = pos.y; p.z = pos.z;
		model->vertices[i + startV] = p;

	}
}
void Importer::loadFromFile(const std::string& path,MeshData& model,Animation& anim) {
	unsigned int prevIndSize = 0;
	unsigned int prevVertSize = 0;
	

		
		//Assimp::Importer importer;
		std::stringstream ss;
		ss << path <<".obj";
		//std::vector<RawModel> v;
		Collada c;
		//c.load("C:/Users/Alex/Desktop/character\\animation\\Wolf Rigged and Game Ready\\Wolf_dae.dae", model,anim);
		
		bool d = c.load("C:/Users/Alex/Desktop/character\\animation\\thinMatrix\\model.dae", model,anim);

		//std::cout << d << std::endl;
		//c.load("C:/Users/Alex/Desktop/character\\animation\\Black Dragon NEW\\Dragon 2.5_dae.dae", model);
		//bool b = c.load("C:/Users/Alex/Desktop/character\\Character Running.dae", model);
		//bool b = c.load("C:/Users/Alex/Desktop/character\\animation\\spider\\Only_Spider_with_Animations_Export.dae", model);
		//model = v[1];
		//model.root.printIds();
		
		return;
		//const aiScene* scene=aiImportFile(ss.str().c_str(), aiProcessPreset_TargetRealtime_MaxQuality | aiProcess_FlipUVs);
		//const aiScene* scene = importer.ReadFile(ss.str().c_str(), aiProcess_Triangulate | aiProcess_FlipUVs);
		//aiMesh* mesh = scene->mMeshes[0];
		//toRaw(mesh, model,prevVertSize,prevIndSize);
		model.meshes.resize(1);
		ObjImporter::loadObj(ss.str().c_str(), model.meshes[0]);
		return;
		RawModel in;
		collapse(&model.meshes[0], &in);
		//models[0] = in;
		//models[0].indicesSize[0] = in.indices.size();
}
