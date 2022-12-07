#pragma once
#include "make_Shader.h"


class map_floor
{
private:
	int row_num, col_num;
	std::vector<GLfloat> vertex;
	//std::vector<GLfloat> color;
	std::vector<GLfloat> normal;
	glm::mat4 transformation;

	GLfloat width, length;

	unsigned int vao;
	unsigned int vbo[3];

public:
	map_floor(const unsigned int& i_row, const unsigned int& i_col);
	 
	GLvoid set_floor(const int& i_width, const int& i_height);
	GLvoid set_vertex();
	//GLvoid set_color();
	GLvoid set_normal();
	GLvoid draw(unsigned int& modelLocation);

	std::vector<GLfloat> get_vertex() const { return vertex; }
	//std::vector<GLfloat> get_color() const { return color; }
	std::vector<GLfloat> get_normal() const { return normal; }
	glm::mat4 get_transformation() { return transformation; }
	GLfloat* get_ptr_transformation() { return &transformation[0][0]; }
	GLfloat get_width() const { return width; }
	GLfloat get_length() const { return length; }

	GLvoid reset()
	{
		row_num = 0;
		col_num = 0;
		transformation = glm::mat4(1.0f);
	}
};

map_floor::map_floor(const unsigned int& i_row = 5, const unsigned int& i_col = 5)
	:row_num(i_row), col_num(i_col), transformation(glm::mat4(1.0f))
{
	row_num = i_row;
	col_num = i_col;

	width = 1000.0f / row_num;
	length = 1000.0f / col_num;

	set_vertex();
	//set_color();
	set_normal();
	transformation = glm::mat4(1.0f);

	glGenVertexArrays(1, &vao);
	glGenBuffers(3, vbo);

	glBindVertexArray(vao);

	glBindBuffer(GL_ARRAY_BUFFER, vbo[1]);
	glBufferData(GL_ARRAY_BUFFER, normal.size() * sizeof(GLfloat), normal.data(), GL_STATIC_DRAW);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(1);

	glBindBuffer(GL_ARRAY_BUFFER, vbo[0]);
	glBufferData(GL_ARRAY_BUFFER, vertex.size() * sizeof(GLfloat), vertex.data(), GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(0);
}

GLvoid map_floor::set_floor(const int& i_row, const int& i_col)
{
	row_num = i_row;
	col_num = i_col;

	width = 1000.0f / row_num;
	length = 1000.0f / col_num;
}

GLvoid map_floor::set_vertex()
{
	vertex.push_back(500.0f);
	vertex.push_back(0.0f);
	vertex.push_back(500.0f);

	vertex.push_back(500.0f);
	vertex.push_back(0.0f);
	vertex.push_back(-500.0f);

	vertex.push_back(-500.0f);
	vertex.push_back(0.0f);
	vertex.push_back(500.0f);

	vertex.push_back(500.0f);
	vertex.push_back(0.0f);
	vertex.push_back(-500.0f);

	vertex.push_back(-500.0f);
	vertex.push_back(0.0f);
	vertex.push_back(-500.0f);

	vertex.push_back(-500.0f);
	vertex.push_back(0.0f);
	vertex.push_back(500.0f);
}

//GLvoid map_floor::set_color()
//{
//	color.push_back(0.7f);
//	color.push_back(0.7f);
//	color.push_back(0.7f);
//
//	color.push_back(0.7f);
//	color.push_back(0.7f);
//	color.push_back(0.7f);
//
//	color.push_back(0.7f);
//	color.push_back(0.7f);
//	color.push_back(0.7f);
//
//	color.push_back(0.7f);
//	color.push_back(0.7f);
//	color.push_back(0.7f);
//
//	color.push_back(0.7f);
//	color.push_back(0.7f);
//	color.push_back(0.7f);
//
//	color.push_back(0.7f);
//	color.push_back(0.7f);
//	color.push_back(0.7f);
//}

GLvoid map_floor::set_normal()
{
	normal.push_back(0.0f);
	normal.push_back(1.0f);
	normal.push_back(0.0f);

	normal.push_back(0.0f);
	normal.push_back(1.0f);
	normal.push_back(0.0f);

	normal.push_back(0.0f);
	normal.push_back(1.0f);
	normal.push_back(0.0f);

	normal.push_back(0.0f);
	normal.push_back(1.0f);
	normal.push_back(0.0f);

	normal.push_back(0.0f);
	normal.push_back(1.0f);
	normal.push_back(0.0f);

	normal.push_back(0.0f);
	normal.push_back(1.0f);
	normal.push_back(0.0f);
}


GLvoid map_floor::draw(unsigned int& modelLocation)
{
	glUniformMatrix4fv(modelLocation, 1, GL_FALSE, glm::value_ptr(transformation));
	glBindVertexArray(vao);
	glDrawArrays(GL_TRIANGLES, 0, vertex.size() / 3);
}