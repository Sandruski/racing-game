
#pragma once
#include "glmath.h"
#include "Color.h"

enum PrimitiveTypes
{
	Primitive_Point,
	Primitive_Line,
	Primitive_Plane,
	Primitive_Cube,
	Primitive_Sphere,
	Primitive_Cylinder,
	Primitive_Cone
};

class Primitive
{
public:

	Primitive();

	virtual void	Render() const;
	virtual void	InnerRender() const;
	void			SetPos(float x, float y, float z);
	vec3			GetPos() const;
	void			SetRotation(float angle, const vec3 &u);
	void			Scale(float x, float y, float z);
	vec3			GetScale() const;
	PrimitiveTypes	GetType() const;

public:
	
	Color color;
	mat4x4 transform;
	bool axis,wire;

protected:
	PrimitiveTypes type;
};

// ============================================
class Cube : public Primitive
{
public :
	Cube();
	Cube(float sizeX, float sizeY, float sizeZ);
	void InnerRender() const;
	vec3 GetSize() const;
public:
	vec3 size;
};

// ============================================
class Sphere : public Primitive
{
public:
	Sphere();
	Sphere(float radius);
	void InnerRender() const;
public:
	float radius;
};

// ============================================
class Cylinder : public Primitive
{
public:
	Cylinder();
	Cylinder(float radius, float height);
	void InnerRender() const;
public:
	float radius;
	float height;
};

// ============================================
class Cone : public Primitive
{
public:
	Cone();
	Cone(float radius, float height);
	void InnerRender() const;
public:
	float radius;
	float height;
};

// ============================================
class Line : public Primitive
{
public:
	Line();
	Line(float x, float y, float z);
	void InnerRender() const;
public:
	vec3 origin;
	vec3 destination;
};

// ============================================
class LinesPlane : public Primitive
{
public:
	LinesPlane();
	LinesPlane(float x, float y, float z, float d);
	void InnerRender() const;
public:
	vec3 normal;
	float constant;
};

class PolygonPlane : public Primitive
{
public:
	PolygonPlane();
	PolygonPlane(float x, float y, float z, float d);
	void InnerRender() const;
	void SetVertexes(int a, int b, int c, int d, int ha = 0, int hb = 0, int hc = 0, int hd = 0);

public:
	vec3 normal;
	float constant;

private:
	int a = 1, b = 1, c = 1, d = 1, ha = 0, hb = 0, hc = 0, hd = 0;
};