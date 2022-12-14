#pragma once
#include "all_include_file.h"
#include "cuboid.h"
#include "wall.h"
#include "state.h"
#include "Jewel.h"

class move_obj
{
private:
	GLuint vao;
	GLuint vbo[2];

	std::vector<GLfloat> vertex;
	std::vector<GLfloat> normal;
	glm::mat4 transformation;

	glm::vec3 pos;
	glm::vec3 oldPos;
	GLfloat speed;

	glm::vec3 look;
	glm::vec3 direction[4];

	glm::mat4 camera;
	glm::vec3 camera_eye;

	int row;
	int col;

public:
	move_obj()
	{
		pos = glm::vec3(-500.0f + wall::width / 2, 10.0f, -500.0f + wall::length / 2);
		oldPos = pos;
		speed = 0.9f;
		look = glm::vec3(0.0f, 0.0f, 1.0f);

		vertex = std::vector<GLfloat>(108);
		makeCuboid(vertex, wall::width / 4, wall::length / 4, 10.0f);
		set_normal();
		camera_eye = glm::vec3(pos.x, pos.y, pos.z + wall::width / 4);

		glGenVertexArrays(1, &vao);
		glGenBuffers(2, vbo);

		glBindVertexArray(vao);

		glBindBuffer(GL_ARRAY_BUFFER, vbo[1]);
		glBufferData(GL_ARRAY_BUFFER, normal.size() * sizeof(GLfloat), normal.data(), GL_STATIC_DRAW);
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, 0);
		glEnableVertexAttribArray(1);

		glBindBuffer(GL_ARRAY_BUFFER, vbo[0]);
		glBufferData(GL_ARRAY_BUFFER, vertex.size() * sizeof(GLfloat), vertex.data(), GL_STATIC_DRAW);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
		glEnableVertexAttribArray(0);

		transformation = glm::mat4(1.0f);
		transformation = glm::translate(transformation, pos);

	}

	GLvoid set_normal()
	{
		normal = {
		0.0f,0.0f,1.0f,
		0.0f,0.0f,1.0f,
		0.0f,0.0f,1.0f,
		0.0f,0.0f,1.0f,
		0.0f,0.0f,1.0f,
		0.0f,0.0f,1.0f,

		1.0f,0.0f,0.0f,
		1.0f,0.0f,0.0f,
		1.0f,0.0f,0.0f,
		1.0f,0.0f,0.0f,
		1.0f,0.0f,0.0f,
		1.0f,0.0f,0.0f,

		0.0f,0.0f,-1.0f,
		0.0f,0.0f,-1.0f,
		0.0f,0.0f,-1.0f,
		0.0f,0.0f,-1.0f,
		0.0f,0.0f,-1.0f,
		0.0f,0.0f,-1.0f,

		-1.0f,0.0f,0.0f,
		-1.0f,0.0f,0.0f,
		-1.0f,0.0f,0.0f,
		-1.0f,0.0f,0.0f,
		-1.0f,0.0f,0.0f,
		-1.0f,0.0f,0.0f,

		0.0f,1.0f,0.0f,
		0.0f,1.0f,0.0f,
		0.0f,1.0f,0.0f,
		0.0f,1.0f,0.0f,
		0.0f,1.0f,0.0f,
		0.0f,1.0f,0.0f,

		0.0f,-1.0f,0.0f,
		0.0f,-1.0f,0.0f,
		0.0f,-1.0f,0.0f,
		0.0f,-1.0f,0.0f,
		0.0f,-1.0f,0.0f,
		0.0f,-1.0f,0.0f
		};
	}

	GLvoid setDirection(int key, GLboolean down);
	GLvoid set_speed(const GLfloat& delta);

	GLfloat get_speed() const { return speed; }
	glm::vec3 get_look() const { return look; }
	glm::mat4 get_camera() const { return camera; }
	glm::vec3 get_pos() const { return pos; }
	int get_col() const { return col; }
	int get_row() const { return row; }

	GLvoid draw(unsigned int& modelLocation);
	GLvoid move(const std::vector<std::vector<wall>>& wallList, Jewel** jewel);
	GLboolean collide(const wall& wall_obj);
	GLfloat* get_bb();
	GLvoid reset();

	GLvoid change_camera_look(const glm::vec3& lookvector);
};

GLvoid move_obj::setDirection(int key, GLboolean down)
{
	if (down)
	{
		if (key == 'a')
		{
			STATE::dir[0] = true;
		}
		else if (key == 'd')
		{
			STATE::dir[1] = true;
		}
		else if (key == 'w')
		{
			STATE::dir[2] = true;
		}
		else if (key == 's')
		{
			STATE::dir[3] = true;
		}
	}
	else
	{
		if (key == 'a')
		{
			STATE::dir[0] = false;
		}
		else if (key == 'd')
		{
			STATE::dir[1] = false;
		}
		else if (key == 'w')
		{
			STATE::dir[2] = false;
		}
		else if (key == 's')
		{
			STATE::dir[3] = false;
		}
	}
}
GLvoid move_obj::set_speed(const GLfloat& delta)
{
	speed += delta;
}


GLvoid move_obj::draw(unsigned int& modelLocation)
{
	glUniformMatrix4fv(modelLocation, 1, GL_FALSE, glm::value_ptr(transformation));
	glBindVertexArray(vao);
	glDrawArrays(GL_TRIANGLES, 0, vertex.size() / 3);
}


GLvoid move_obj::move(const std::vector<std::vector<wall>>& wallList, Jewel** jewel)
{
	direction[0] = -(glm::normalize(glm::cross(look, glm::vec3(0.0f, 1.0f, 0.0f)))); //left
	direction[1] = glm::normalize(glm::cross(look, glm::vec3(0.0f, 1.0f, 0.0f))); //right
	direction[2] = look; //front
	direction[3] = -look; //back

	oldPos = pos;
	for (int i = 0; i < 4; ++i) {
		if (STATE::dir[i]) {
			pos.x += direction[i].x * speed;
			pos.z += direction[i].z * speed;
		}
	}

	for (int i = 0; i < wall::cNum; ++i)
	{
		for (int j = 0; j < wall::rNum; ++j)
		{
			if (collide(wallList[i][j])) {
				pos.x = oldPos.x;
				pos.z = oldPos.z;

				for (int k = 0; k < 4; ++k) {
					if (STATE::dir[k]) {
						pos.x += direction[k].x * speed;
					}
				}

				if (collide(wallList[i][j])) {
					pos.x = oldPos.x;
					pos.z = oldPos.z;

					for (int k = 0; k < 4; ++k) {
						if (STATE::dir[k]) {
							pos.z += direction[k].z * speed;
						}
					}

					if (collide(wallList[i][j])) {
						pos.x = oldPos.x;
						pos.z = oldPos.z;
					}
				}
			}
			if (jewel[i][j].collision(get_bb())) {
				jewel[i][j].status_draw = false;
			}
			jewel[i][j].respawn();
			jewel[i][j].update();
		}
	}

	col = (pos.z + 500) / 40;
	row = (pos.x + 500) / 40;

	camera_eye = glm::vec3(pos.x, pos.y, pos.z);

	glm::vec3 temp(0.0f, 0.0f, 1.0f);
	transformation = glm::mat4(1.0f);
	transformation = glm::translate(transformation, pos);

	camera = glm::mat4(1.0f);
	camera = glm::lookAt(camera_eye, camera_eye + look, glm::vec3(0.0f, 1.0f, 0.0f));
}

GLfloat* move_obj::get_bb()
{
	GLfloat invisiblepart = 10.0f;//?????? ???????? ?????? ?? ???? ???? ?????? ???? ????
	GLfloat bb[4] = {
		pos.x - wall::width / invisiblepart,
		pos.x + wall::width / invisiblepart,
		pos.z - wall::length / invisiblepart,
		pos.z + wall::length / invisiblepart
	};
	return bb;
}

GLboolean move_obj::collide(const wall& wall_obj)
{
	//?????? ????????
	//???? ?????? ?????? ?????? true????
	GLfloat minX = -500.0f + wall::width * wall_obj.get_index_r();
	GLfloat maxX = minX + wall::width;
	GLfloat minZ = -500.0f + wall::length * wall_obj.get_index_c();
	GLfloat maxZ = minZ + wall::length;

	GLfloat* bb = get_bb();

	//?? ?????? ?? ????????.
	if (bb[0] < -500.0f || bb[1] > 500.0f
		|| bb[2] < -500.0f || bb[3] > 500.0f)
		return true;

	if (wall_obj.maze_state)
		return false;

	if (bb[0] > maxX)
		return false;
	if (bb[1] < minX)
		return false;
	if (bb[2] > maxZ)
		return false;
	if (bb[3] < minZ)
		return false;

	return true;
}

GLvoid move_obj::reset()
{
	pos = glm::vec3(-500.0f + wall::width / 2, 10.0f, -500.0f + wall::length / 2);
	oldPos = pos;
	speed = 0.9f;

	col = 0;
	row = 0;
}

GLvoid move_obj::change_camera_look(const glm::vec3& lookvector)
{
	look = lookvector;
}