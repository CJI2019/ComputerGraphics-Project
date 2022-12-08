#pragma once
#include "make_Shader.h"
#include "wall.h"
#include "find_path.h"
#include "move_obj.h"

struct model
{
	objRead obj_model;
	unsigned int vao;
	unsigned int vbo[3];
};

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


	unsigned int vao_body = 0;
	unsigned int vao_eye = 0;
	unsigned int vao_eyebrow = 0;
public:
	static int stun_time;
	static objRead body;
	static objRead eye;
	static objRead eyebrow;
	static unsigned int vbo_body[3];
	static unsigned int vbo_eye[3];
	static unsigned int vbo_eyebrow[3];
	
	pac_man();

	int get_col()const { return col; }
	int get_row()const { return row; }

	GLvoid draw(unsigned int& modelLocation);
	GLvoid update();
	GLfloat* get_bb();
	bool colide(GLfloat object[])
	{
		
		GLfloat* pac_man_bb = get_bb();

		if (pac_man_bb[0] > object[1]) return false;
		if (pac_man_bb[1] < object[0]) return false;
		if (pac_man_bb[2] > object[3]) return false;
		if (pac_man_bb[3] < object[2]) return false;

		return true;
	}
	bool stun();
	GLvoid reset();

};
int pac_man::stun_time = 0;
objRead pac_man::body;
objRead pac_man::eye;
objRead pac_man::eyebrow;
unsigned int pac_man::vbo_body[3];
unsigned int pac_man::vbo_eye[3];
unsigned int pac_man::vbo_eyebrow[3];

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

	if (vao_body == 0)
	{
		glGenVertexArrays(1, &vao_body);
		glGenVertexArrays(1, &vao_eye);
		glGenVertexArrays(1, &vao_eyebrow);
		if (pac_man::body.outnormal.empty())
		{
			pac_man::body.loadObj_normalize_center("pac_body.obj");

			glGenBuffers(3, pac_man::vbo_body);

			glBindVertexArray(vao_body);

			glBindBuffer(GL_ARRAY_BUFFER, pac_man::vbo_body[2]);
			glBufferData(GL_ARRAY_BUFFER, pac_man::body.outuv.size() * sizeof(glm::vec2), pac_man::body.outuv.data(), GL_STATIC_DRAW);
			glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 0, 0);
			glEnableVertexAttribArray(2);

			glBindBuffer(GL_ARRAY_BUFFER, pac_man::vbo_body[1]);
			glBufferData(GL_ARRAY_BUFFER, pac_man::body.outnormal.size() * sizeof(glm::vec3), pac_man::body.outnormal.data(), GL_STATIC_DRAW);
			glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, 0);
			glEnableVertexAttribArray(1);

			glBindBuffer(GL_ARRAY_BUFFER, pac_man::vbo_body[0]);
			glBufferData(GL_ARRAY_BUFFER, pac_man::body.outvertex.size() * sizeof(glm::vec3), pac_man::body.outvertex.data(), GL_STATIC_DRAW);
			glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
			glEnableVertexAttribArray(0);

			pac_man::eye.loadObj_normalize_center("pac_eye.obj");

			glGenBuffers(3, pac_man::vbo_eye);

			glBindVertexArray(vao_eye);

			glBindBuffer(GL_ARRAY_BUFFER, pac_man::vbo_eye[2]);
			glBufferData(GL_ARRAY_BUFFER, pac_man::eye.outuv.size() * sizeof(glm::vec2), pac_man::eye.outuv.data(), GL_STATIC_DRAW);
			glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 0, 0);
			glEnableVertexAttribArray(2);

			glBindBuffer(GL_ARRAY_BUFFER, pac_man::vbo_eye[1]);
			glBufferData(GL_ARRAY_BUFFER, pac_man::eye.outnormal.size() * sizeof(glm::vec3), pac_man::eye.outnormal.data(), GL_STATIC_DRAW);
			glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, 0);
			glEnableVertexAttribArray(1);

			glBindBuffer(GL_ARRAY_BUFFER, pac_man::vbo_eye[0]);
			glBufferData(GL_ARRAY_BUFFER, pac_man::eye.outvertex.size() * sizeof(glm::vec3), pac_man::eye.outvertex.data(), GL_STATIC_DRAW);
			glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
			glEnableVertexAttribArray(0);

			pac_man::eyebrow.loadObj_normalize_center("pac_eyebrow.obj");

			glGenBuffers(3, pac_man::vbo_eyebrow);

			glBindVertexArray(vao_eyebrow);

			glBindBuffer(GL_ARRAY_BUFFER, pac_man::vbo_eyebrow[2]);
			glBufferData(GL_ARRAY_BUFFER, pac_man::eyebrow.outuv.size() * sizeof(glm::vec2), pac_man::eyebrow.outuv.data(), GL_STATIC_DRAW);
			glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 0, 0);
			glEnableVertexAttribArray(2);

			glBindBuffer(GL_ARRAY_BUFFER, pac_man::vbo_eyebrow[1]);
			glBufferData(GL_ARRAY_BUFFER, pac_man::eyebrow.outnormal.size() * sizeof(glm::vec3), pac_man::eyebrow.outnormal.data(), GL_STATIC_DRAW);
			glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, 0);
			glEnableVertexAttribArray(1);

			glBindBuffer(GL_ARRAY_BUFFER, pac_man::vbo_eyebrow[0]);
			glBufferData(GL_ARRAY_BUFFER, pac_man::eyebrow.outvertex.size() * sizeof(glm::vec3), pac_man::eyebrow.outvertex.data(), GL_STATIC_DRAW);
			glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
			glEnableVertexAttribArray(0);
		}
		else
		{
			glBindVertexArray(vao_body);
			glBindBuffer(GL_ARRAY_BUFFER, pac_man::vbo_body[2]);
			glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 0, 0);
			glEnableVertexAttribArray(2);

			glBindBuffer(GL_ARRAY_BUFFER, pac_man::vbo_body[1]);
			glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, 0);
			glEnableVertexAttribArray(1);

			glBindBuffer(GL_ARRAY_BUFFER, pac_man::vbo_body[0]);
			glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
			glEnableVertexAttribArray(0);

			glBindVertexArray(vao_eye);
			glBindBuffer(GL_ARRAY_BUFFER, pac_man::vbo_eye[2]);
			glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 0, 0);
			glEnableVertexAttribArray(2);

			glBindBuffer(GL_ARRAY_BUFFER, pac_man::vbo_eye[1]);
			glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, 0);
			glEnableVertexAttribArray(1);

			glBindBuffer(GL_ARRAY_BUFFER, pac_man::vbo_eye[0]);
			glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
			glEnableVertexAttribArray(0);

			glBindVertexArray(vao_eyebrow);
			glBindBuffer(GL_ARRAY_BUFFER, pac_man::vbo_eyebrow[2]);
			glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 0, 0);
			glEnableVertexAttribArray(2);

			glBindBuffer(GL_ARRAY_BUFFER, pac_man::vbo_eyebrow[1]);
			glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, 0);
			glEnableVertexAttribArray(1);

			glBindBuffer(GL_ARRAY_BUFFER, pac_man::vbo_eyebrow[0]);
			glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
			glEnableVertexAttribArray(0);
		}
	}
}

GLfloat* pac_man::get_bb()
{
	GLfloat bb[4] = {
	pos.x - 20, //left
	pos.x + 20, //right
	pos.z - 20, //bottom
	pos.z + 20  //top
	};

	return bb;
}



bool pac_man::stun()
{
	if (stun_time <= 0)
		return true;
	else
		stun_time -= 1;

	glm::mat4 temp = transformation;
	transformation = glm::mat4(1.0f);
	transformation = temp * transformation;
	transformation = glm::rotate(transformation, glm::radians(4.0f), { 0.0f, 1.0f, 0.0f });

	return false;
}

GLvoid pac_man::reset()
{
	col = 24;
	row = 24;
	old_col = col;
	old_row = row;

	pos.x = 480.0f;
	pos.y = 0.0f;
	pos.z = 480.0f;
}


GLvoid pac_man::draw(unsigned int& modelLocation)
{
	glUniformMatrix4fv(modelLocation, 1, GL_FALSE, glm::value_ptr(transformation));

	glBindTexture(GL_TEXTURE_2D, texture[2]);
	glBindVertexArray(vao_eye);
	glDrawArrays(GL_TRIANGLES, 0, pac_man::eye.outvertex.size());

	glBindTexture(GL_TEXTURE_2D, texture[3]);
	glBindVertexArray(vao_body);
	glDrawArrays(GL_TRIANGLES, 0, pac_man::body.outvertex.size());

	glBindTexture(GL_TEXTURE_2D, texture[4]);
	glBindVertexArray(vao_eyebrow);
	glDrawArrays(GL_TRIANGLES, 0, pac_man::eyebrow.outvertex.size());
}