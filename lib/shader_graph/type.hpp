#pragma once
#include <string>
enum class Type
{
	Float,
	Float2,
	Float3,
	Float4,
	Int,
	Void,
};

inline std::string to_string(Type type)
{
	switch (type)
	{
		case Type::Float:
			return "float";
		case Type::Float2:
			return "vec2";
		case Type::Float3:
			return "vec3";
		case Type::Float4:
			return "vec4";
		case Type::Int:
			return "int";
		case Type::Void:
			return "";
	}
	return "unknown";
}