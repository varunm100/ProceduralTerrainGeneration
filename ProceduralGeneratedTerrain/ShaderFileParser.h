#pragma once
#pragma once
#include <fstream>
#include <sstream>
#include <iostream>
using namespace std;
class ShaderFileParser {
public:
	static void getShaderSrc(const char* filepath, string& vs, string& fs) {
		ifstream stream(filepath);

		enum class ShaderType {
			NONE = -1, VERTEX = 0, FRAGMENT = 1
		};

		string line;
		stringstream ss[2];
		ShaderType type = ShaderType::NONE;
		while (getline(stream, line)) {
			if (line.find("#SHADER") != string::npos) {
				if (line.find("VERTEX") != string::npos) {
					type = ShaderType::VERTEX;
				}
				else if (line.find("FRAGMENT") != string::npos) {
					type = ShaderType::FRAGMENT;
				}
			}
			else {
				if (type == ShaderType::VERTEX) {
					ss[0] << line << '\n';
				}
				else if (type == ShaderType::FRAGMENT) {
					ss[1] << line << '\n';
				}
			}
		}
		vs = ss[0].str();
		fs = ss[1].str();
	}
};