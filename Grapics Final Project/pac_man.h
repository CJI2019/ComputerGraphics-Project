#pragma once
#include "make_Shader.h"
#include "cuboid.h"
#include "mountain.h"
#include "find_path.h"

class pac_man
{
protected:
	unsigned int vao;
	unsigned int vbo_vertex;
	unsigned int vbo_texture;
	unsigned int vbo_normal;

	std::vector<GLfloat> vertex = std::vector<GLfloat>(108); //임시 직육면체
	std::vector<GLfloat> color = std::vector<GLfloat>(108);
	glm::mat4 transformation;

	int col;
	int row;
	int old_col, old_row;

	glm::vec3 pos;
	GLfloat speed;
	std::vector<int> paths;

public:
	pac_man();

	int get_col()const { return col; }
	int get_row()const { return row; }

	GLvoid draw(unsigned int& modelLocation);
	GLvoid update();
	//virtual GLvoid move();
};

pac_man::pac_man()
{
	makeCuboid(vertex, 10.0, 10.0f);
	setCol(color, 0.0f, 1.0f, 0.5f);

	col = 39;
	row = 39;
	old_col = col;
	old_row = row;

	pos.x = 487.5;
	pos.y = 0.0f;
	pos.z = 487.5;

	transformation = glm::mat4(1.0f);
	transformation = glm::translate(transformation, pos);
	speed = 1.25f;

	glGenVertexArrays(1, &vao);
	glGenBuffers(1, &vbo_vertex);
	glGenBuffers(1, &vbo_normal);
	glGenBuffers(1, &vbo_texture);

	glBindVertexArray(vao);

	//지금은 노말로 하는중 아직 조명 안함
	glBindBuffer(GL_ARRAY_BUFFER, vbo_normal);
	glBufferData(GL_ARRAY_BUFFER, color.size() * sizeof(GLfloat), color.data(), GL_STATIC_DRAW);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(1);

	glBindBuffer(GL_ARRAY_BUFFER, vbo_vertex);
	glBufferData(GL_ARRAY_BUFFER, vertex.size() * sizeof(GLfloat), vertex.data(), GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(0);
}

GLvoid pac_man::draw(unsigned int& modelLocation)
{
	glUniformMatrix4fv(modelLocation, 1, GL_FALSE, glm::value_ptr(transformation));
	glBindVertexArray(vao);
	glDrawArrays(GL_TRIANGLES, 0, vertex.size() / 3);
}