#pragma once
#include "make_Shader.h"
#include "cuboid.h"
#include "mountain.h"
#include "find_path.h"

class pac_man
{
protected:
	std::vector<GLfloat> color = std::vector<GLfloat>(108);
	glm::mat4 transformation;

	int col;
	int row;
	int old_col, old_row;

	glm::vec3 pos;
	GLfloat speed;
	std::vector<int> paths;
	int look[2];

	int stun_time;

	unsigned int vao = 0;
public:
	static objRead model;
	static unsigned int vbo_vertex;
	static unsigned int vbo_texture;
	static unsigned int vbo_normal;
	
	pac_man();

	int get_col()const { return col; }
	int get_row()const { return row; }

	GLvoid draw(unsigned int& modelLocation);
	GLvoid update();
	GLvoid stun();

};

objRead pac_man::model;
unsigned int pac_man::vbo_vertex;
unsigned int pac_man::vbo_texture;
unsigned int pac_man::vbo_normal;

pac_man::pac_man()
{
	col = 24;
	row = 24;
	old_col = col;
	old_row = row;

	pos.x = 480.0f;
	pos.y = 0.0f;
	pos.z = 480.0f;

	transformation = glm::mat4(1.0f);
	transformation = glm::translate(transformation, pos);
	speed = 1.00f;
	stun_time = 0;
	setCol(color, 1.0f, 1.0f, 0.0f);

	if (vao == 0)
	{
		glGenVertexArrays(1, &vao);
		if (pac_man::model.outnormal.empty())
		{
			pac_man::model.loadObj_normalize_center("test.obj");

			glGenBuffers(1, &pac_man::vbo_vertex);
			glGenBuffers(1, &pac_man::vbo_normal);
			glGenBuffers(1, &pac_man::vbo_texture);

			glBindVertexArray(vao);

			glBindBuffer(GL_ARRAY_BUFFER, pac_man::vbo_texture);
			glBufferData(GL_ARRAY_BUFFER, pac_man::model.outuv.size() * sizeof(GLfloat), pac_man::model.outuv.data(), GL_STATIC_DRAW);
			glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 0, 0);
			glEnableVertexAttribArray(2);

			glBindBuffer(GL_ARRAY_BUFFER, pac_man::vbo_normal);
			glBufferData(GL_ARRAY_BUFFER, pac_man::model.outnormal.size() * sizeof(GLfloat), pac_man::model.outnormal.data(), GL_STATIC_DRAW);
			glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, 0);
			glEnableVertexAttribArray(1);

			glBindBuffer(GL_ARRAY_BUFFER, pac_man::vbo_vertex);
			glBufferData(GL_ARRAY_BUFFER, pac_man::model.outvertex.size() * sizeof(glm::vec3), pac_man::model.outvertex.data(), GL_STATIC_DRAW);
			glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
			glEnableVertexAttribArray(0);
		}
		else
		{
			glBindVertexArray(vao);
			glBindBuffer(GL_ARRAY_BUFFER, pac_man::vbo_texture);
			glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 0, 0);
			glEnableVertexAttribArray(2);

			glBindBuffer(GL_ARRAY_BUFFER, pac_man::vbo_normal);
			glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, 0);
			glEnableVertexAttribArray(1);

			glBindBuffer(GL_ARRAY_BUFFER, pac_man::vbo_vertex);
			glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
			glEnableVertexAttribArray(0);
		}
	}
}

GLvoid pac_man::stun()
{
	if()
}


GLvoid pac_man::draw(unsigned int& modelLocation)
{
	glUniformMatrix4fv(modelLocation, 1, GL_FALSE, glm::value_ptr(transformation));
	glBindVertexArray(vao);
	glDrawArrays(GL_TRIANGLES, 0, pac_man::model.outvertex.size());
}