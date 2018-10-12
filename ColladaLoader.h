#pragma once
#include "XMLImporter.h"
#include "ModelType.h"
#include "MeshData.h"
#include "Vertex.h"
#include "Animation.h"

class Collada : public XMLImporter{
#define MAX_JOINTS 4
#define CCW true
	std::vector<Vertex*> vertices;
private:
	Joint* findJointInVector(const std::string& s, std::vector<Joint*>& vector) {

		for (std::vector<Joint*>::iterator i = vector.begin(); i < vector.end(); i++) {
			if ((*i)->name == s) {
				return *i;
			}
		}
		return nullptr;
	}
	bool loadNode(Joint* parent,Tag* parentTag
			,std::vector<Joint*>& vector,const glm::mat4& bindParentTrasform) {
		
		

		for (Tag* t : parentTag->children) {
			if (t->name == "node") {
				Joint* j = findJointInVector(t->getValue("id"), vector);
			
				if (!j) {
					std::cout << t->getValue("id") << std::endl;
					continue;
				}
				
				parent->addChild(j);
				glm::mat4 local(1.0f);
				Tag* matrix = t->cdSingle("matrix");
				// read matrix
				if (matrix) {
					bool row_major = false;
					float arr[16];
					ImporterUtils::splitString(matrix->data, ' ', arr);

					for (int i = 0; i < 4; i++) {
						for (int j = 0; j < 4; j++) {
							float& ref = row_major ? local[i][j] : local[j][i];

							ref = arr[i * 4 + j];
						}
					}

			
				}

				glm::mat4 bindTrasform = bindParentTrasform * local;
				j->inverseBindMat = glm::inverse(bindTrasform);

				loadNode(j,t, vector,bindTrasform);
			}
		}

	}
	bool loadHeriarchy(MeshData& model,std::vector<Joint*>& armature, const std::string& name,bool ccw=true) {
		
		Tag* scenes = getRoot()->cd("COLLADA/library_visual_scenes");
		if (!scenes) {
			return false;
		}
		Tag* scene = scenes->cdSingle("visual_scene");
		if (!scene) {
			return false;
		}
		Tag* nodeArmature = nullptr;
		for (Tag* node : scene->children) {
			if (!node) {
				continue;
			}
			nodeArmature = node;
			std::string s = nodeArmature->getValue("name");
			if (s == name) {
				break;
			}
			else {
				nodeArmature = nullptr;
			}
		}

		if (!nodeArmature) {
			return false;
		}

		glm::vec3 rot = ccw ? glm::vec3(-90, 0, 0) : glm::vec3(0, 0, 0);

		loadNode(&model.root, nodeArmature, armature, math::Rot(math::eulerToQuat(math::toRad(rot))));

		return true;
	}
	bool loadArmature(MeshData& model,Animation& a,bool ccw=true) {

		Tag* controllers = getRoot()->cd("COLLADA/library_controllers");
		if (!controllers) {
			return false;
		}

		Tag* controller = controllers->children[0];

		if (!controller) {
			return false;
		}

		Tag* skin = controller->cdSingle("skin");

		if (!skin) {
			return false;
		}
		std::string name = controller->getValue("name");
		std::string id = controller->getValue("id");
		std::vector<Joint*> armature;

		//joints
		{
			Tag* pos = skin->cdAttr("source", "id", id + "-joints");
			if (!pos) {
				return false;
			}
			pos = pos->cdSingle("Name_array");
			if (!pos) {
				return false;
			}
			size_t l = std::stoi(pos->getValue("count"));
			armature.resize(l);
			//split string
			{
				std::string bufferString;
				size_t iter = 0;
				for (std::string::iterator i = pos->data.begin(); i < pos->data.end(); i++) {
					char c = *i;
					if (c == ' ') {
						if (bufferString.size() != 0) {
							armature[iter] = new Joint(iter, bufferString);
							bufferString = "";
							iter++;
						}
						continue;
					}
					bufferString += c;
				}
			}
		}

		/*//bindPoses
		{
			bool row_major = false;
			Tag* pos = skin->cdAttr("source", "id", id + "-bind_poses");
			if (!pos) {
				return false;
			}

			pos = pos->cdSingle("float_array");
			if (!pos) {
				return false;
			}

			size_t l = std::stoi(pos->getValue("count"));
			float* arr = new float[l];

			ImporterUtils::splitString(pos->data, ' ', arr);
			glm::mat4 rot90 = model.root.inverseBindMat;
			for (int iter = 0; iter < armature.size(); iter++) {

				glm::mat4& m = armature[iter]->getInverseBindMatRef();

				for (int i = 0; i < 4; i++) {
					for (int j = 0; j < 4; j++) {
						float& ref = row_major ? m[i][j] : m[j][i];

						ref = arr[iter * 16 + i * 4 + j];
					}
				}

				m *= rot90;
			}


			delete[] arr;
		}
		*/
		if(!loadHeriarchy(model,armature, name,ccw))
			return false;
		
		return loadAnim(a, armature,&model.root);

	}
	bool loadJW(glm::vec4* tempW,glm::vec4* tempJ,const std::string& source) {

		Tag* controllers = getRoot()->cd("COLLADA/library_controllers");
		if (!controllers) {
			return false;
		}

		Tag* skin = nullptr;
		for (Tag* controller : controllers->children) {
			skin = controller->cdSingle("skin");
			if (!skin)
				continue;
			std::string s = skin->getValue("source");

			if (s == "#"+source) {
				break;
			}
			else {
				skin = nullptr;
			}
		}

		if (!skin) {
			return false;
		}
		std::string id = skin->getParent()->getValue("id");
		

		float* tempWeights = nullptr;
		unsigned int* vCount = nullptr;
		


		//weights
		{
			Tag* pos = skin->cdAttr("source", "id", id + "-weights");
			if (!pos) {
				return false;
			}
			pos = pos->cdSingle("float_array");
			if (!pos) {
				return false;
			}
			size_t l = std::stoi(pos->getValue("count"));
			tempWeights = new float[l];
			ImporterUtils::splitString(pos->data, ' ', tempWeights);

		}

		//align
		{
			Tag* vertex_weights = skin->cdSingle("vertex_weights");
						 

			int jointOff = 0, weightOff = 0, maxOff = 0;

			for (Tag* t : vertex_weights->children) {
				if (t->name == "input") {
					std::string semantic = t->getValue("semantic");
					int offset = std::stoi(t->getValue("offset"));
	
					if (offset > maxOff) {
						maxOff = offset;
					}
					if (semantic == "JOINT") {
						jointOff = offset;
					}
					else if (semantic == "WEIGHT") {
						weightOff = offset;
					}

				}
			}
		

			Tag* vcount = vertex_weights->cdSingle("vcount");
			Tag* v = vertex_weights->cdSingle("v");
			size_t size = std::stoi(vertex_weights->getValue("count"));

			vCount = new unsigned int[size];
			ImporterUtils::splitString(vcount->data, ' ', vCount);

			std::vector<unsigned int> v_array;
			ImporterUtils::splitString(v->data, ' ', v_array);


			unsigned int iterator = 0;
			for (unsigned int i = 0; i < size; i++) {

				unsigned int howMany = vCount[i];

				glm::uvec4 ids;
				glm::vec4 w;

				unsigned int times = MAX_JOINTS;
				bool hasToBeNormalize = true;
				if (howMany < MAX_JOINTS) {
					times = howMany;
					hasToBeNormalize = false;
				}

				for (int j = 0; j < times; j++) {
					ids[j] = v_array[iterator + j*(maxOff+1)+jointOff];
					unsigned int wId = v_array[iterator + j*(maxOff + 1) +weightOff];
					w[j] = tempWeights[wId];
				}
				
				if (hasToBeNormalize) {
					w = glm::normalize(w);
				}
				
				tempJ[i] = ids;
				tempW[i] = w;

				iterator += howMany*(maxOff + 1);
			}
			
		}


		delete[] vCount;
		delete[] tempWeights;
		return true;
		
	}
	Vertex* dealWithAlreadyProcessedVertex(unsigned int& ind, Vertex* previousVertex,
		int newTextureIndex, int newNormalIndex) {
		if (previousVertex->hasSameTextureAndNormal(newTextureIndex, newNormalIndex)) {
			ind = previousVertex->index;
			return previousVertex;
		}
		else {
			Vertex* anotherVertex = previousVertex->duplicate;
			if (anotherVertex) {
				return dealWithAlreadyProcessedVertex(ind,anotherVertex, newTextureIndex, newNormalIndex);
			}
			else {
				Vertex* duplicateVertex = new Vertex();
				duplicateVertex->index = vertices.size();
				duplicateVertex->position = previousVertex->position;
				duplicateVertex->uvIndex = newTextureIndex;
				duplicateVertex->normalIndex = newNormalIndex;
				duplicateVertex->initlialPosIndex = previousVertex->initlialPosIndex;
				vertices.push_back(duplicateVertex);
				
				previousVertex->duplicate = duplicateVertex;
				ind = duplicateVertex->index;
				return previousVertex->duplicate;
			}

		}
	}
	bool loadModel(Tag* geometry,RawModel& model,bool ccw=true) {
		std::string geometryId = geometry->getValue("id");
		Tag* meshData = geometry->cdSingle("mesh");
		glm::vec3* tempNorm = nullptr;
		glm::vec2* tempUvs = nullptr;
		glm::vec4* tempJoint = nullptr;
		glm::vec4* tempWeight = nullptr;
		vertices.clear();
		
		if (!meshData) {
			return false;
		}
		//positions
		{
			Tag* pos = meshData->cdAttr("source", "id", geometryId + "-positions");
			if (!pos) {
				return false;
			}
			pos = pos->cdSingle("float_array");
			if (!pos) {
				return false;
			}
			size_t s = std::stoi(pos->getValue("count"));
			
			vertices.resize(s / 3);
			//split with check box
			{
				std::string bufferString;
				glm::vec3 buf;
				unsigned int iter = 0;
				for (std::string::iterator i = pos->data.begin(); i < pos->data.end(); i++) {
					char c = *i;
					if (c == ' ') {
						if (bufferString.size() != 0) {
							int p = iter % 3;
							buf[p] = std::stof(bufferString);

							bufferString = "";

							if (p == 2) {
								if (ccw) {
									float temp = buf.z;
									buf.z = -buf.y;
									buf.y = temp;
								}
								model.box.limitsCheck(buf);
								Vertex* v = new Vertex();
								
								v->position = buf;
								v->index = iter / 3;
								v->initlialPosIndex = v->index;
								vertices[iter / 3] = v;
							}
							iter++;
						}
						continue;
					}
					bufferString += c;
				}

			}
		}
		//normals
		{
			Tag* pos = meshData->cdAttr("source", "id", geometryId + "-normals");
			if (!pos) {
				goto endnormals;
			}
			pos = pos->cdSingle("float_array");
			if (!pos) {
				goto endnormals;
			}
			size_t si = std::stoi(pos->getValue("count")) / 3;
			tempNorm = new glm::vec3[si];
			ImporterUtils::splitString(pos->data, ' ', tempNorm, ccw);
		}
	endnormals:
		//uvs
		{
			Tag* pos = meshData->cdAttr("source", "id", geometryId + "-map-0");
			if (!pos) {
				goto enduvs;
			}
			pos = pos->cdSingle("float_array");
			if (!pos) {
				goto enduvs;
			}
			tempUvs = new glm::vec2[std::stoi(pos->getValue("count")) / 2];
			ImporterUtils::splitString(pos->data, ' ', tempUvs);
		}	
	enduvs:	
		tempWeight = new glm::vec4[vertices.size()];
		tempJoint = new glm::vec4[vertices.size()];
		bool hasSkinData = loadJW(tempWeight,tempJoint, geometryId);
		Tag* polylist = meshData->cdSingle("polylist");
		size_t vCount = std::stoi(polylist->getValue("count"));
		model.indices.resize(vCount);

		int maxOff = 0, vertOff = 0, uvOff = 0, normOff = 0;

		for (Tag* t : polylist->children) {
			if (t->name == "input") {
				std::string semantic = t->getValue("semantic");
				int offset = std::stoi(t->getValue("offset"));
				if (offset > maxOff) {
					maxOff = offset;
				}
				if (semantic == "VERTEX") {
					vertOff = offset;
				}
				else if (semantic == "NORMAL") {
					normOff = offset;
				}
				else if (semantic == "TEXCOORD") {
					uvOff = offset;
				}
			} 
		}

		Tag* p = polylist->cdSingle("p");
		//loop faces
		{
			int typeCount = maxOff + 1;
			std::string bufString = "";
			std::vector<std::string> indexData;
			ImporterUtils::splitString(p->data, ' ', indexData);
			
			for (int i = 0; i<indexData.size() / typeCount; i++) {

				unsigned int positionIndex = std::stoi(indexData[i * typeCount]);
				unsigned int normalIndex = std::stoi(indexData[i * typeCount + normOff]);
				unsigned int texCoordIndex = std::stoi(indexData[i * typeCount + uvOff]);
				int rest = i % 3;
				if (!ccw) {
					switch (rest) {
					case 1: 
						rest = 2;
						break;
					case 2:
						rest = 1;
					}
				}
				Vertex* currentVertex = vertices[positionIndex];
				if (!currentVertex->isSet()) {
					currentVertex->uvIndex = texCoordIndex;
					currentVertex->normalIndex = normalIndex;
					model.indices[i / 3][rest] = positionIndex;	
				}
				else {
					dealWithAlreadyProcessedVertex(model.indices[i / 3][rest],currentVertex, texCoordIndex,normalIndex);
				}
				currentVertex->addNormal(tempNorm[normalIndex]);
				
			}
		}
		model.resizeAll(vertices.size());
		
		
		for (int i = 0; i < vertices.size(); i++) {
			Vertex* vert = vertices[i];

			model.vertices[i] = vert->position;
			model.uvs[i] = tempUvs[vert->uvIndex];
			//model.normals[i] = tempNorm[vert->normalIndex];
			model.normals[i] = glm::normalize(vert->normal);

			if (hasSkinData) {

				model.jointId[i] = tempJoint[vert->initlialPosIndex];
				model.weight[i] = tempWeight[vert->initlialPosIndex];
			}

			delete vert;
		}

		delete[] tempWeight;
		delete[] tempJoint;
		delete[] tempNorm;
		delete[] tempUvs;
	}

	bool loadAnim(Animation& anim,std::vector<Joint*>& armature,Joint* rootJ) {
		Tag* root = getRoot();

		anim.jointAnimation.resize(armature.size());

		Tag* animations = root->cd("COLLADA/library_animations");

		if (!animations) {
			return false;
		}
		glm::mat4 CORRECTION = glm::rotate(glm::mat4(1.0f), glm::radians(-90.0f), glm::vec3(1, 0, 0));
		for (Tag* animat : animations->children) {
			if (animat->name != "animation") {
				continue;
			}
			Tag* channel = animat->cdSingle("channel");

			if (!channel) {
				return false;
			}
			std::string target = channel->getValue("target");
			size_t s = target.find('/');
			target = target.substr(0, s);
			Joint* j = findJointInVector(target, armature);

			if (!j) {
				return false;
			}
			std::string id = animat->getValue("id");
			std::vector<float> times;
			std::vector<float> poses;
			//times
			{
				Tag* source = animat->cdAttr("source","id",id + "-input");
				if (!source) {
					std::cout << "MERDA\n";
					return false;
				}
				Tag* float_array = source->cdSingle("float_array");
				if (!float_array) {
					return false;
				}

				size_t c = std::stoi(float_array->getValue("count"));
				
				//times.resize(c);

				ImporterUtils::splitString(float_array->data, ' ', times);
			}
			//poses
			{
				Tag* source = animat->cdAttr("source", "id", id + "-output");
				if (!source) {
					return false;
				}
				Tag* float_array = source->cdSingle("float_array");
				if (!float_array) {
					return false;
				}

				size_t c = std::stoi(float_array->getValue("count"));

				//poses.resize(c);

				ImporterUtils::splitString(float_array->data, ' ', poses);
		
			}
			
			std::vector<KeyFrame>& aKey = anim.jointAnimation[j->index].keyFrames;
			aKey.resize(times.size());
			
			for (int i = 0; i < times.size(); i++) {
				aKey[i].time = times[i];

				glm::mat4 mat;
				for (int m = 0; m < 4; m++) {
					for (int n = 0; n < 4; n++) {
						float& ref = true ? mat[n][m] : mat[m][n];
						ref = poses[i * 16 + 4 * m + n];
					}
				}
				if (rootJ->NameInChildren(j->name)) {
					mat = CORRECTION*mat;
				}
				aKey[i].trasform.setFromMatrix(mat);
				
			}
		}
		return true;
	}
public:
	bool load(const std::string& fileName, MeshData& model, Animation& a) {
		
		XMLImporter::load(fileName);
		bool ccw = true;
		Tag* root = getRoot();

		Tag* geometries = root->cd("COLLADA/library_geometries");
		
		if (!geometries) {
			return false;
		}
		model.meshes.resize(geometries->children.size());
		bool v = false;
		for (int i = 0; i < geometries->children.size();i++) {
			v = loadModel(geometries->children[i], model.meshes[i],ccw) || v;
		}

		bool hasArmature = loadArmature(model,a,ccw);
		model.hasAnimation = hasArmature;
		v = hasArmature || v;

		return v;
	}

};

