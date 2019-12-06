#pragma once

typedef struct
{
	GLfloat XYZW[4];

} Vertex;

const Vertex Vertices_RTriangle[] =
{
	{ 0.0f, 0.5f, 0.0f, 1.0f },
	{ 0.0f, 0.0f, 0.0f, 1.0f },
	{ 0.5f, 0.0f, 0.0f, 1.0f }
};

const GLubyte Indices_RTriangle[] =
{
	0,1,2
};

const Vertex Vertices_Square[] =
{
	{ 0.0f, 0.0f, 0.0f, 1.0f }, //0
	{ 0.5f, 0.0f, 0.0f, 1.0f },  //1 
	{ 0.0f, 0.5f, 0.0f, 1.0f },  //2
	{ 0.5f, 0.5f, 0.0f, 1.0f } //3

};

const GLubyte Indices_Square[] =
{
	0,1,2,
	2,1,3

};

const Vertex Vertices_Parallelogram[] =
{
	{ 0.0f, 0.0f, 0.0f, 1.0f }, //0
	{ 0.75f, 0.0f, 0.0f, 1.0f },  //1 
	{ 0.25f, 0.25f, 0.0f, 1.0f },  //2
	{ 1.0f, 0.25f, 0.0f, 1.0f }, //3

};

const GLubyte Indices_Parallelogram[] =
{
	0,1,2,
	2,1,3

};