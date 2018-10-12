#pragma once

#include <string>
#include <vector>
#include <GLM\glm.hpp>

namespace ImporterUtils {
	static void printVec(const glm::vec3& v) {
		std::cout << v.x << " " << v.y << " " << v.z << std::endl;
	}
	static void printVec(const glm::vec2& v) {
		std::cout << v.x << " " << v.y << std::endl;
	}
	static void printVec(const glm::vec4& v) {
		std::cout << v.x << " " << v.y << " "<<v.z<<" "<<v.w<<std::endl;
	}
	static void printMat(const glm::mat4& m) {
		for (int i = 0; i < 4; i++) {
			for (int j = 0; j < 4; j++) {
				std::cout << m[i][j]<<"  ";
			}
			std::cout<<std::endl;
		}
	}
	static void printVec(const glm::quat& v) {
		std::cout << v.x << " " << v.y << " " << v.z << " " << v.w << std::endl;
	}
	static void printVec(const glm::uvec4& v) {
		std::cout << v.x << " " << v.y << " " << v.z << " " << v.w << std::endl;
	}
	static void splitString(std::string& str, char delimiter, std::vector<std::string>& vector) {
		std::string bufferString;
		for (std::string::iterator i = str.begin(); i < str.end(); i++) {
			char c = *i;
			if (c == delimiter) {
				if (bufferString.size() != 0) {
					vector.push_back(bufferString);
					bufferString = "";
				}
				continue;
			}
			bufferString += c;
		}
		if (bufferString.size() != 0) {
			vector.push_back(bufferString);
		}
	}
	static std::vector<std::string> splitString(std::string::iterator first, std::string::iterator last, char delimiter) {
		std::vector<std::string> vector;
		std::string bufferString;
		for (std::string::iterator i = first; i < last; i++) {
			char c = *i;
			if (c == delimiter) {
				if (bufferString.size() != 0) {
					vector.push_back(bufferString);
					bufferString = "";
				}
				continue;
			}
			bufferString += c;
		}
		if (bufferString.size() != 0) {
			vector.push_back(bufferString);
		}
		return vector;
	}
	static std::vector<std::string> splitString(std::string& str, char delimiter) {
		return splitString(str.begin(), str.end(), delimiter);
	}
	static void splitString(std::string& str, std::string* vector, char delimiter) {
		std::string bufferString;
		size_t pos = 0;
		for (std::string::iterator i = str.begin(); i < str.end(); i++) {
			char c = *i;
			if (c == delimiter) {
				if (bufferString.size() != 0) {
					vector[pos]=bufferString;
					bufferString = "";
					pos++;
				}
				continue;
			}
			bufferString += c;
		}

	}
	static void splitString(std::string& str, char delimiter,std::vector<glm::vec3>& vector,bool reverseCoord=false) {
		std::string bufferString;
		glm::vec3 buf;
		unsigned int pos = 0;
		for (std::string::iterator i = str.begin(); i < str.end(); i++) {
			char c = *i;
			if (c == delimiter) {
				if (bufferString.size() != 0) {
					int p = pos % 3;
					buf[p] = std::stof(bufferString);

					bufferString = "";

					if (p == 2) {
						
						
						if (reverseCoord) {
							float temp = buf.z;
							buf.z = buf.y;
							buf.y = temp;
						}
						
						vector[pos / 3] = buf;

					}
					pos++;
				}
				continue;
			}
			bufferString += c;
		}
		
	}
	static void splitString(std::string& str, char delimiter, std::vector<glm::uvec3>& vector) {
		std::string bufferString;
		glm::uvec3 buf;
		unsigned int pos = 0;
		for (std::string::iterator i = str.begin(); i < str.end(); i++) {
			char c = *i;
			if (c == delimiter) {
				if (bufferString.size() != 0) {
					int p = pos % 3;
					buf[p] = std::stoi(bufferString);

					bufferString = "";
					if (p == 2) {
						vector[pos / 3] = buf;
					}
					pos++;
				}
				continue;
			}
			bufferString += c;
		}


	}
	static void splitString(std::string& str, char delimiter, std::vector<glm::vec2>& vector) {
		std::string bufferString;
		glm::vec2 buf;
		unsigned int pos = 0;
		for (std::string::iterator i = str.begin(); i < str.end(); i++) {
			char c = *i;
			if (c == delimiter) {
				if (bufferString.size() != 0) {
					int p = pos % 2;
					buf[p] = std::stof(bufferString);

					bufferString = "";
					if (p == 1) {
						buf.y = 1 - buf.y;
						vector[pos / 2] = buf;
					}
					pos++;
				}
				continue;
			}
			bufferString += c;
		}


	}
	static void splitString(std::string& str, char delimiter, glm::vec3* vector, bool reverseCoord = false) {
		//glm::mat4 reverse = glm::rotate(glm::mat4(1.0f),glm::radians(-90.0f),glm::vec3(1,0,0));//-90 //rotate


		std::string bufferString;
		glm::vec3 buf;
		unsigned int pos = 0;
		for (std::string::iterator i = str.begin(); i < str.end(); i++) {
			char c = *i;
			if (c == delimiter) {
				if (bufferString.size() != 0) {
					int p = pos % 3;
					buf[p] = std::stof(bufferString);

					bufferString = "";
					if (p == 2) {
						if (reverseCoord) {
							/*ImporterUtils::printVec(buf);
							glm::vec4 v(buf, 0.0f);
							v = reverse* v;
							buf.x = v.x;
							buf.y = v.y;
							buf.z = v.z;
							ImporterUtils::printVec(buf);
							std::cout << std::endl;*/
							/*buf.x = -buf.x;
							buf.y = (buf.y)>0 ? 1 - buf.y : buf.y-1;
							buf.z = (buf.z)>0 ? 1 - buf.z : buf.z - 1;*/

							float temp = buf.z;
							buf.z = -buf.y;
							buf.y = temp;
							//buf.x = -buf.x;
							
						}
						vector[pos / 3] = buf;//glm::normalize(buf);
					}
					pos++;
				}
				continue;
			}
			bufferString += c;
		}

	}
	static void splitString(std::string& str, char delimiter, glm::uvec3* vector) {
		std::string bufferString;
		glm::uvec3 buf;
		unsigned int pos = 0;
		for (std::string::iterator i = str.begin(); i < str.end(); i++) {
			char c = *i;
			if (c == delimiter) {
				if (bufferString.size() != 0) {
					int p = pos % 3;
					buf[p] = std::stoi(bufferString);

					bufferString = "";
					if (p == 2) {
						vector[pos / 3] = buf;
						buf = glm::uvec3(0, 0, 0);
					}
					pos++;
				}
				continue;
			}
			bufferString += c;
		}


	}
	static void splitString(std::string& str, char delimiter, glm::vec2* vector,bool reverseY = true) {
		std::string bufferString;
		glm::vec2 buf;
		unsigned int pos = 0;
		for (std::string::iterator i = str.begin(); i < str.end(); i++) {
			char c = *i;
			if (c == delimiter) {
				if (bufferString.size() != 0) {
					int p = pos % 2;
					buf[p] = std::stof(bufferString);

					bufferString = "";
					if (p == 1) {
						if (reverseY) {
							//float temp = buf.y;
							buf.y = 1 - buf.y;
							//buf.x = x;
						}
							
						vector[pos / 2] = buf;
					}
					pos++;
				}
				continue;
			}
			bufferString += c;
		}

	}
	static void splitString(std::string& str, char delimiter, float* vector) {
		std::string bufferString;
		unsigned int pos = 0;
		for (std::string::iterator i = str.begin(); i < str.end(); i++) {
			char c = *i;
			if (c == delimiter) {
				if (bufferString.size() != 0) {
					vector[pos] = std::stof(bufferString);
					bufferString = "";
					pos++;
				}
				continue;
			}
			bufferString += c;
		}

	}
	static void splitString(std::string& str, char delimiter, unsigned int* vector) {
		std::string bufferString;
		unsigned int pos = 0;
		for (std::string::iterator i = str.begin(); i < str.end(); i++) {
			char c = *i;
			if (c == delimiter) {
				if (bufferString.size() != 0) {
					vector[pos] = std::stoi(bufferString);
					bufferString = "";
					pos++;
				}
				continue;
			}
			bufferString += c;
		}

	}
	static void splitString(std::string& str, char delimiter, std::vector<unsigned int>& vector) {
		std::string bufferString;
		for (std::string::iterator i = str.begin(); i < str.end(); i++) {
			char c = *i;
			if (c == delimiter) {
				if (bufferString.size() != 0) {
					vector.push_back(std::stoi(bufferString));
					bufferString = "";
				}
				continue;
			}
			bufferString += c;
		}

	}
	static void splitString(std::string& str, char delimiter, std::vector<float>& vector) {
		std::string bufferString;
		for (std::string::iterator i = str.begin(); i < str.end(); i++) {
			char c = *i;
			if (c == delimiter) {
				if (bufferString.size() != 0) {
					vector.push_back(std::stof(bufferString));
					bufferString = "";
				}
				continue;
			}
			bufferString += c;
		}

	}
	
}
