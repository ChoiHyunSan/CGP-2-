#pragma once
#include "define.h"
#include <vector>
#include <string>
#include "d3dclass.h"
#include "struct.h"


using namespace std;
using std::vector;

#include <map>
using std::map;
using std::make_pair;
using std::string;
using std::wstring;

#define BOXSCALE 1
#define fDT TimeMgr::GetInst()->GetfDT()

struct Pos
{
	float x;
	float y;
	float z;

	Pos operator+(Pos _pos)
	{
		return Pos(x + _pos.x, y + _pos.y, z + _pos.z);
	}
	Pos():
		x(0.f), y(0.f), z(0.f)
	{

	}

	Pos(float _x, float _y, float _z):
		x(_x), y(_y), z(_z)
	{

	}
};

struct Vertex	//Overloaded Vertex Structure
{
	Vertex() {}
	Vertex(float x, float y, float z,
		float u, float v,
		float nx, float ny, float nz)
		: pos(x, y, z), texCoord(u, v), normal(nx, ny, nz) {}

	XMFLOAT3 pos;
	XMFLOAT2 texCoord;
	XMFLOAT3 normal;
};