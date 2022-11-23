#pragma once
#include "make_Shader.h"

//width = ¹Øº¯/2  height = ³ôÀÌ / 2
GLvoid makeCuboid(std::vector<GLfloat>& obj, const GLfloat& width, const GLfloat& hegiht)
{
	obj = {
		//¾Õ¸é
		-width, -hegiht, width,
		width, hegiht, width,
		-width, hegiht, width,

		-width, -hegiht, width,
		width, -hegiht, width,
		width, hegiht, width
			,
		//¿À¸¥¸é
		width, -hegiht, width,
		width, hegiht, -width,
		width, hegiht, width,

		width, -hegiht, width,
		width, -hegiht, -width,
		width, hegiht, -width
		,
		//µÞ¸é
		width, -hegiht, -width,
		-width, -hegiht, -width,
		-width, hegiht, -width,

		width, -hegiht, -width,
		-width, hegiht, -width,
		width, hegiht, -width
		,
		//¿Þ¸é
		-width, -hegiht, -width,
		-width, -hegiht, width,
		-width, hegiht, width,

		-width, -hegiht, -width,
		-width, hegiht, width,
		-width, hegiht, -width
		,
		//À­¸é
		width, hegiht, width,
		width, hegiht, -width,
		-width, hegiht, width,

		-width, hegiht, width,
		width, hegiht, -width,
		-width, hegiht, -width
		,
		//µÞ¸é
		width, -hegiht, width,
		-width, -hegiht, width,
		-width, -hegiht, -width,

		width, -hegiht, width,
		-width, -hegiht, -width,
		width, -hegiht, -width
	};
}

GLvoid makeCuboid(std::vector<GLfloat>& obj, const GLfloat& width,const GLfloat& length, const GLfloat& hegiht)
{
	obj = {
		//¾Õ¸é
		-width, -hegiht, length,
		width, hegiht, length,
		-width, hegiht, length,

		-width, -hegiht, length,
		width, -hegiht, length,
		width, hegiht, length
			,
		//¿À¸¥¸é
		width, -hegiht, length,
		width, hegiht, -length,
		width, hegiht, length,

		width, -hegiht, length,
		width, -hegiht, -length,
		width, hegiht, -length
		,
		//µÞ¸é
		width, -hegiht, -length,
		-width, -hegiht, -length,
		-width, hegiht, -length,

		width, -hegiht, -length,
		-width, hegiht, -length,
		width, hegiht, -length
		,
		//¿Þ¸é
		-width, -hegiht, -length,
		-width, -hegiht, length,
		-width, hegiht, length,

		-width, -hegiht, -length,
		-width, hegiht, length,
		-width, hegiht, -length
		,
		//À­¸é
		width, hegiht, length,
		width, hegiht, -length,
		-width, hegiht, length,

		-width, hegiht, length,
		width, hegiht, -length,
		-width, hegiht, -length
		,
		//¹Ø¸é
		width, -hegiht, length,
		-width, -hegiht, length,
		-width, -hegiht, -length,

		width, -hegiht, length,
		-width, -hegiht, -length,
		width, -hegiht, -length
	};
}

GLvoid genCol(std::vector<GLfloat>& col, const GLfloat& red, const GLfloat& green, const GLfloat& blue)
{
	for (int i = 0; i < 36; ++i)
	{
		col.push_back(red);
		col.push_back(green);
		col.push_back(blue);
	}
}

GLvoid setCol(std::vector<GLfloat>& col, const GLfloat& red, const GLfloat& green, const GLfloat& blue)
{
	for (int i = 0; i < col.size() /3; ++i)
	{
		col[3 * i] = red;
		col[3 * i + 1] = green;
		col[3 * i + 2] = blue;
	}
}
