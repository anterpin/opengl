#pragma once


#include "ObjImporter.h"
#include "MeshData.h"
#include "Animation.h"
#include "Simplify.h"
#include "ColladaLoader.h"
#include <assimp\scene.h>
#include <assimp\cimport.h>
#include <assimp\Importer.hpp>
#include <assimp\cimport.h>
#include <assimp\postprocess.h>
#include <sstream>
#include "Mesh.h"
#include "Material.h"




namespace Importer {
	void loadFromFile(const std::string& str, MeshData& model,Animation& a);

}

