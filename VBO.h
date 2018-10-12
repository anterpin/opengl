#pragma once
#include <vector>
#include <GLM\glm.hpp>
#include <GL\glew.h>


class VBO {
protected:
	unsigned int vaoId=0;
	size_t size=0;
	std::vector<unsigned int> vbos;
protected:
	virtual ~VBO() {
		free();
	}
	template<typename T>
	void storeDataInAttribList(unsigned int num,const std::vector<T>& vector) {
		unsigned int vboId; 
		glGenBuffers(1, &vboId);
		vbos.push_back(vboId);

		glBindBuffer(GL_ARRAY_BUFFER, vboId);

		glBufferData(GL_ARRAY_BUFFER, vector.size() * sizeof(T), vector.data(), GL_STATIC_DRAW);


		glVertexAttribPointer(num, sizeof(T) / sizeof(float), GL_FLOAT,false, 0, 0);//put into vao

		glBindBuffer(GL_ARRAY_BUFFER, 0);
	}
	void storeDataInAttribList(unsigned int num, const std::vector<glm::uvec4>& vector) {
		unsigned int vboId;
		glGenBuffers(1, &vboId);
		vbos.push_back(vboId);

		glBindBuffer(GL_ARRAY_BUFFER, vboId);

		glBufferData(GL_ARRAY_BUFFER, vector.size() * sizeof(glm::uvec4), vector.data(), GL_STATIC_DRAW);


		glVertexAttribIPointer(num, sizeof(glm::uvec4) / sizeof(unsigned int), GL_UNSIGNED_INT, false, 0);//put into vao

		glBindBuffer(GL_ARRAY_BUFFER, 0);
	}
	void storeDataInAttribList(unsigned int num, const std::vector<int>& vector) {
		unsigned int vboId;
		glGenBuffers(1, &vboId);
		vbos.push_back(vboId);

		glBindBuffer(GL_ARRAY_BUFFER, vboId);

		glBufferData(GL_ARRAY_BUFFER, vector.size() * sizeof(int), vector.data(), GL_STATIC_DRAW);


		glVertexAttribIPointer(num, 1, GL_INT, false, 0);//put into vao

		glBindBuffer(GL_ARRAY_BUFFER, 0);
	}
	template<typename T>
	void storeIndices(const std::vector<T>& arr) {
		unsigned int vboId;
		
		glGenBuffers(1, &vboId);
		vbos.push_back(vboId);

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vboId);

		glBufferData(GL_ELEMENT_ARRAY_BUFFER, arr.size() * sizeof(T), arr.data(), GL_STATIC_DRAW);

	}
	virtual void free() {
		glDeleteVertexArrays(1, &vaoId);//delete vao
		vaoId = 0;

		removeVBOs();

	}
	virtual void removeVBOs() {
		for (unsigned int& vbo : vbos) {//delete vbos
			glDeleteBuffers(1, &vbo);
		}
		vbos.clear();
	}
	virtual void genVao() {
		glGenVertexArrays(1, &vaoId);
	}
	virtual void bindVao() {
		glBindVertexArray(vaoId);
	}
	virtual void unbindVao() {
		glBindVertexArray(0);
	}
public:
	inline void draw(int primitive = GL_TRIANGLES)const {
		glDrawArrays(primitive, 0,(GLsizei)size);
	}
	inline unsigned int getVaoId()const {
		return vaoId;
	}
	inline size_t getSize()const { return size; }
	virtual void bindMesh(int bindes=3)const {
		glBindVertexArray(vaoId);
		for (int i = 0; i < bindes; i++) {
			glEnableVertexAttribArray(i);
		}
	}
	virtual void unbindMesh(int bindes = 3)const {
		for (int i = 0; i < bindes; i++) {
			glDisableVertexAttribArray(i);
		}
		glBindVertexArray(0);
	}
};
