#pragma once
#include <DirectXMath.h>

struct Vertex
{
	Vertex() {}
	Vertex(float x, float y, float z)
		: Position(x, y, z) {}

	DirectX::XMFLOAT3 Position;
};

struct Color
{
	float r, g, b, a;
	Color(){};
	Color(float r, float g, float b, float a = 1.0f) :r(r), g(g), b(b), a(a) {};
};

namespace Colours
{
	const Color Blue(0, 0, 1);
	const Color Red(1, 0, 0);
	const Color Green(0, 1, 0);
	const Color White(1, 1, 1);
	const Color Grey(0.5, 0.5, 0.5);

	const Color Yellow(0.9, 0.9, 0.1); //yellow??
}


struct Vertex_COLOR
{
	Vertex_COLOR() {}
	Vertex_COLOR(float x, float y, float z, const Color& color)
		: pos(x, y, z), color(color) {}

	DirectX::XMFLOAT3 pos;
	Color color;
};
