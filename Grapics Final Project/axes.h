#pragma once
#include <vector>
#include "make_Shader.h"

struct axes_coordination
{
	glm::mat4 transformation = glm::mat4(1.0f);

	std::vector<GLfloat> axes_vertex =
	{
		//xรเ
		-1000.0f,0.0f,0.0f,
		1000.0f,0.0f,0.0f,
		//yรเ
		0.0f,-1000.0f,0.0f,
		0.0f,1000.0f,0.0f,
		//zรเ
		0.0f,0.0f,-1000.0f,
		0.0f,0.0f,1000.0f
	};

	std::vector<GLfloat> axes_color =
	{
		1.0f,0.0f,1.0f,
		0.0f,1.0f,0.0f,
		
		0.5f,0.5f,1.0f,
		0.5f,0.5f,1.0f,

		1.0f,1.0f,1.0f,
		0.0f,1.0f,1.0f
	};
};
