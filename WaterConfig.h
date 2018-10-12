#pragma once

#include <Windows.h>
#include <string>
#include <sstream>
#include <iostream>
#include <cmath>
#include "Texture2D.h"
#include "NormalMapCompute.h"
#include "FFTmapsh.h"


#define NUM_LOD_RANGE 8
#define ROOT_NODES 8

struct WaterConfig {

private:

	int tessellationFactor;
	float tessellationSlope;
	float tessellationShift;

	float scaleY;
	float scaleXZ;
	Texture2D heightMap;
	Texture2D normalMap;

	Texture* reflection;
	Texture* refraction;

	FFTOcean ocean;

	float shift = 0;


	std::vector<unsigned int> lod_range = std::vector<unsigned int>(NUM_LOD_RANGE);

public:
	WaterConfig() {
		scaleY = 0;
		scaleXZ = 0;
	}
	~WaterConfig() {}
	void loadFile(std::string path) {

		FILE* file = fopen(path.c_str(), "r");

		if (!file) {
			std::cout << "could not open file source : " << path << std::endl;
			setDefaults();
			return;
		}

		while (true) {
			char lineHeader[128];
			int res = fscanf(file, "%s", lineHeader);
			if (res == EOF)
				break; // EOF = End Of File. Quit the loop.
			if (strcmp(lineHeader, "scaleY") == 0) {
				fscanf(file, " %f\n", &scaleY);
			}
			else if (strcmp(lineHeader, "tessellationFactor") == 0) {
				fscanf(file, " %d\n", &tessellationFactor);
			}
			else if (strcmp(lineHeader, "tessellationSlope") == 0) {
				fscanf(file, " %f\n", &tessellationSlope);
			}
			else if (strcmp(lineHeader, "tessellationShift") == 0) {
				fscanf(file, " %f\n", &tessellationShift);
			}
			else if (strcmp(lineHeader, "#noise") == 0) {
				Texture2D noise[4];

				for (int i = 0; i < 4; i++) {
					char line[128];
					int res = fscanf(file, "%s", line);
					if (res == EOF)
						break;
					std::stringstream ss;
					ss << "noise_" << i ;
					if (strcmp(line, ss.str().c_str()) == 0) {
						char path[128];
						fscanf(file, " %s\n", &path);

						noise[i].loadFile(std::string(path));
						noise[i].noFilter();
					}
				}
				ocean.init(noise);

			}
			/*else if (strcmp(lineHeader, "heightmap") == 0) {
				char path[128];
				fscanf(file, " %s\n", &path);
				heightMap.loadFile(std::string(path));
				normalMap.setResolution(heightMap.getWidth(), heightMap.getHeight());
				NormalMapCompute renderer(&normalMap);
				renderer.setStrength(8);
				renderer.compute(&heightMap);

			}*/
			else if (strcmp(lineHeader, "scaleXZ") == 0) {
				fscanf(file, " %f\n", &scaleXZ);
			}
	
			else if (strcmp(lineHeader, "#lod_range") == 0) {
				for (int i = 0; i < NUM_LOD_RANGE; i++) {
					char line[128];
					int res = fscanf(file, "%s", line);
					if (res == EOF)
						break;
					std::stringstream ss;
					ss << "lod" << i + 1 << "_range";
					if (strcmp(line, ss.str().c_str()) == 0) {
						unsigned int value = 0;
						fscanf(file, " %d\n", &value);

						this->lod_range[i] = value;
					}
				}

			}
		}

		fclose(file);

	}
	inline int getTessellationFactor() { return tessellationFactor; }
	inline float getTessellationSlope() { return tessellationSlope; }
	inline float getTessellationShift() { return tessellationShift; }
	inline float getShift() {
		return shift;
	}
	inline void setShift(float shift) {
		this->shift = shift;
	}
	inline float getScaleY() { return scaleY; }
	int setMorphingArea(unsigned int lod) {
		return (int)((scaleXZ / ROOT_NODES) / pow(2, lod));
	}
	void setDefaults() {
		tessellationFactor = 420;
		tessellationSlope = 2.2f;
		tessellationShift = 0.178f;

		scaleY = 500.0f;
		scaleXZ = 6000.0f;
		lod_range[0] = 450;
		lod_range[1] = 304;
		lod_range[2] = 240;
		lod_range[3] = 190;
		lod_range[4] = 120;
		lod_range[5] = 45;
		lod_range[5] = 24;
		lod_range[7] = 9;

	}
	inline void updateSurface() {
		ocean.update();
	}
	inline float getScaleXZ() { return scaleXZ; }
	inline const std::vector<unsigned int>& getLodRange() {return lod_range;}
	inline Texture* getHeightMap() { return &heightMap; }
	inline Texture* getNormalMap() { return &normalMap; }
	inline Texture* getHK0() { return ocean.get_hk0(); }
	inline Texture* getHKMinus0() { return ocean.get_hkminus0(); }
	inline Texture* getDY() { return ocean.getDYComponent(); }
	inline Texture* getDX() { return ocean.getDXComponent(); }
	inline Texture* getDZ() { return ocean.getDZComponent(); }
	inline Texture* getTwiddleInd() { return ocean.getTwiddleIndices(); }
	inline Texture* getReflection() { return reflection; }
	inline Texture* getRefraction() { return refraction; }
	inline Texture* getLightMap() { return ocean.getNormalMap(); }
	inline Texture* getPingPong() { return ocean.getPingPong(); }
	inline Texture* getDXDisplacement() { return ocean.getDXDisplacement(); }
	inline Texture* getDYDisplacement() { return ocean.getDYDisplacement(); }
	inline Texture* getDZDisplacement() { return ocean.getDZDisplacement(); }
	inline Texture* getTotalDisplacement() { return ocean.getTotalDisplacement(); }
	inline void setReflection(Texture* t) { reflection = t; }
	inline void setRefraction(Texture* t) { refraction = t; }
};

