#pragma once
#include "ofxAssimpModelLoader.h"
#ifndef UTILS_H
#define UTILS_H

const string PATH_ASSETS = "..\\data\\Assets\\";
const string PATH_COLLECTION = "..\\data\\Collection\\";
const string PATH_FONTS = "..\\data\\Fonts\\";


enum appStates
{
	state2D,
	state3D
};

enum rendererStates
{
	idle,
	cardPreview,
	collection,
	createcard,
	randomcard,
	hist,
	debug
};

enum camState
{
	multi,
	freecam
};

enum class VectorPrimitiveType { none, line, rectangle, parral, triangle, triangler, ellipse };

struct VectorPrimitive
{
	VectorPrimitiveType type;
	float               position1[2];
	float               position2[2];
	float               stroke_width;
	unsigned char       fill_color[4];
	unsigned char		stroke_color[4];
	bool				selected;
};

struct paramCoords {
	int x;
	int y;
	int w;
	int h;
};

enum class objectType3D { none, box, cylinder, sphere, cone, tetra, octa, dode,  model };

struct object3D
{
	objectType3D type;
	int model_type;
	unsigned char color[4];
	float radius;
	float height;
	ofVec3f vector_position;
	ofVec3f vector_proportion;
	ofVec3f vector_rotation;
	bool selected;

};
#endif /* UTILS_H */