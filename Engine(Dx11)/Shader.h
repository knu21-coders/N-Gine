#pragma once
#include "framework.h"

enum ShaderType {
	Vertex, Pixel, Domain, Hull
};
class Shader {

	Shader(const ShaderType& shader_variant, const WCHAR* unicode_name) {
		switch (shader_variant) {
		case Vertex:
			
			break;
		}
	}
	~Shader() {};
private:
	const WCHAR* name, entry_point, 
};