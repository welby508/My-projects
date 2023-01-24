#pragma once
#include "ofxAssimpModelLoader.h"
#ifndef UTILS_H
#define UTILS_H

const string PATH_ASSETS = "..\\data\\Assets\\";
const string PATH_COLLECTION = "..\\data\\Collection\\";
const string PATH_FONTS = "..\\data\\Fonts\\";
const string PATH_SHADERS = "..\\data\\Shaders\\";

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
	curves,
	surfaces,
	debug
};

enum camState
{
	multi,
	freecam
};

struct paramCoords {
	int x;
	int y;
	int w;
	int h;
};

enum class ShaderType { none, lambert, gouraud, phong };

enum class objectType3D { none, box, cylinder, sphere, cone };

struct object3D
{
	objectType3D type;
	int model_type;
	ofColor specular;
	ofFloatColor diffuse;
	ofFloatColor ambient;
	int shininess;
	float radius;
	float height;
	ofVec3f vector_position;
	ofVec3f vector_proportion;
	ofVec3f vector_rotation;

};

//Source module 6: Exemple 3 (IFT3100H21_Convolution)
enum class ConvolutionKernel
{
	identity,
	emboss,
	sharpen,
	edge_detect,
	blur
};

// kernel de convolution (3x3) : identité
const std::array<float, 9> convolution_kernel_identity
{
  0.0f,  0.0f,  0.0f,
  0.0f,  1.0f,  0.0f,
  0.0f,  0.0f,  0.0f
};

// kernel de convolution (3x3) : aiguiser
const std::array<float, 9> convolution_kernel_sharpen
{
  0.0f, -1.0f,  0.0f,
 -1.0f,  5.0f, -1.0f,
  0.0f, -1.0f,  0.0f
};

// kernel de convolution (3x3) : détection de bordure
const std::array<float, 9> convolution_kernel_edge_detect
{
  0.0f,  1.0f,  0.0f,
  1.0f, -4.0f,  1.0f,
  0.0f,  1.0f,  0.0f
};

// kernel de convolution (3x3) : bosseler
const std::array<float, 9> convolution_kernel_emboss
{
 -2.0f, -1.0f,  0.0f,
 -1.0f,  1.0f,  1.0f,
  0.0f,  1.0f,  2.0f
};

// kernel de convolution (3x3) : flou
const std::array<float, 9> convolution_kernel_blur
{
  1.0f / 9.0f,  1.0f / 9.0f,  1.0f / 9.0f,
  1.0f / 9.0f,  1.0f / 9.0f,  1.0f / 9.0f,
  1.0f / 9.0f,  1.0f / 9.0f,  1.0f / 9.0f
};

#endif /* UTILS_H */