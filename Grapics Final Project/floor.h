#pragma once
#include "make_Shader.h"


class map_floor
{

private:
	int row_num, col_num;
	std::vector<GLfloat> vertex = std::vector<GLfloat>(108);
	std::vector<GLfloat> color = std::vector<GLfloat>(108);
	glm::mat4 transformation;

	GLfloat width, length;

public:
	map_floor(const unsigned int& i_row = 5, const unsigned int& i_col = 5)
		:row_num(i_row), col_num(i_col), transformation(glm::mat4(1.0f))
	{
		set_vertex();
		set_color();
		transformation = glm::mat4(1.0f);
	}
	 
	GLvoid set_floor(const int& i_width, const int& i_height);
	GLvoid set_vertex();
	GLvoid set_color();
	GLvoid draw(unsigned int& modelLocation);

	std::vector<GLfloat> get_vertex() const { return vertex; }
	std::vector<GLfloat> get_color() const { return color; }
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

GLvoid map_floor::set_color()
{
	color.push_back(0.7f);
	color.push_back(0.7f);
	color.push_back(0.7f);

	color.push_back(0.7f);
	color.push_back(0.7f);
	color.push_back(0.7f);

	color.push_back(0.7f);
	color.push_back(0.7f);
	color.push_back(0.7f);

	color.push_back(0.7f);
	color.push_back(0.7f);
	color.push_back(0.7f);

	color.push_back(0.7f);
	color.push_back(0.7f);
	color.push_back(0.7f);

	color.push_back(0.7f);
	color.push_back(0.7f);
	color.push_back(0.7f);
}

GLvoid map_floor::draw(unsigned int& modelLocation)
{
	//glUniformMatrix4fv(modelLocation, 1, GL_FALSE, mapFloor.get_ptr_transformation());
	//glBindVertexArray(vao_floor);
	//glDrawArrays(GL_TRIANGLES, 0, mapFloor.get_vertex().size() / 3);
}