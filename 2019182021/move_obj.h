#pragma once
#include "make_Shader.h"
#include "cuboid.h"
#include "mountain.h"

class move_obj
{
private:
	GLuint vao;
	GLuint vbo[2];
		
	std::vector<GLfloat> vertex;
	std::vector<GLfloat> color;
	glm::mat4 transformation;

	glm::vec3 pos;
	glm::vec3 oldPos;
	GLfloat speed;
	GLboolean state;

	glm::vec3 look;
	glm::vec2 direction;

	glm::mat4 camera;
	glm::vec3 camera_eye;


public:
	move_obj()
	{
		pos = glm::vec3(-500.0f + mountain::width / 2, 10.0f, -500.0f + mountain::length / 2);
		oldPos = pos;
		speed = 10.0f;
		state = false;
		direction = glm::vec2(10.0f, 10.0f);
		look = glm::vec3(0.0f, 0.0f, 1.0f);

		vertex = std::vector<GLfloat>(108);
		color = std::vector<GLfloat>(108);
		makeCuboid(vertex, mountain::width / 4, mountain::length / 4, 10.0f);
		setCol(color, 0.3f, 0.5f, 0.8f);

		camera_eye = glm::vec3(pos.x, pos.y, pos.z + mountain::width / 4);

		glGenVertexArrays(1, &vao);
		glGenBuffers(2, vbo);

		glBindVertexArray(vao);

		glBindBuffer(GL_ARRAY_BUFFER, vbo[1]);
		glBufferData(GL_ARRAY_BUFFER, color.size() * sizeof(GLfloat), color.data(), GL_STATIC_DRAW);
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, 0);
		glEnableVertexAttribArray(1);

		glBindBuffer(GL_ARRAY_BUFFER, vbo[0]);
		glBufferData(GL_ARRAY_BUFFER, vertex.size() * sizeof(GLfloat), vertex.data(), GL_STATIC_DRAW);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
		glEnableVertexAttribArray(0);

		transformation = glm::mat4(1.0f);
		transformation = glm::translate(transformation, pos);


	}
	GLvoid set_dir(int key);
	GLvoid setDirection(int key, GLboolean down);
	GLvoid set_speed(const GLfloat& delta);

	GLboolean get_state() const { return state; }
	GLfloat get_speed() const { return speed; }
	glm::vec3 get_look() const { return look; }
	glm::mat4 get_camera() const { return camera; }
	glm::vec2 get_dir() const { return direction; }


	GLvoid draw(unsigned int& modelLocation);
	GLvoid reveal();
	GLvoid move(const std::vector<std::vector<mountain>>& mountainList);
	GLboolean collide(const mountain& mountain_obj);
};

GLvoid move_obj::set_dir(int key)
{
	if (key == GLUT_KEY_LEFT || key == GLUT_KEY_RIGHT)
		direction.x = 0;

	if (key == GLUT_KEY_UP || (key == GLUT_KEY_DOWN))
		direction.y = 0;
}

GLvoid move_obj::setDirection(int key, GLboolean down)
{
	if (down)
	{
		if (key == GLUT_KEY_LEFT)
		{
			direction.x -= 1;
		}
		else if (key == GLUT_KEY_RIGHT)
		{
			direction.x += 1;
		}
		else if (key == GLUT_KEY_UP)
		{
			direction.y -= 1;
		}
		else if (key == GLUT_KEY_DOWN)
		{
			direction.y += 1;
		}
	}
	else
	{
		if (key == GLUT_KEY_LEFT)
		{
			direction.x += 1;
		}
		else if (key == GLUT_KEY_RIGHT)
		{
			direction.x -= 1;
		}
		else if (key == GLUT_KEY_UP)
		{
			direction.y += 1;
		}
		else if (key == GLUT_KEY_DOWN)
		{
			direction.y -= 1;
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


GLvoid move_obj::reveal()
{
	state = true;
}

GLvoid move_obj::move(const std::vector<std::vector<mountain>>& mountainList)
{
	oldPos = pos;
	if(direction.x != 10.0f)
		pos.x += direction.x * speed;
	if(direction.y != 10.0f)
		pos.z += direction.y * speed;

	for (int i = 0; i < mountain::cNum; ++i)
	{
		for (int j = 0; j < mountain::rNum; ++j) 
		{
			if (collide(mountainList[i][j]))
			{
				pos.x = oldPos.x;
				pos.z = oldPos.z;
			}
		}
	}

	camera_eye = glm::vec3(pos.x, pos.y, pos.z);

	glm::vec3 temp(0.0f, 0.0f, 1.0f);
	if (oldPos.x != pos.x || oldPos.z != pos.z)
	{	
		look.x = pos.x - oldPos.x;
		look.z = pos.z - oldPos.z;
	}
	look = glm::normalize(look);
	GLfloat cos_rotate = glm::dot(glm::vec3(0.0f, 0.0f, 1.0f), look);

	transformation = glm::mat4(1.0f);
	transformation = glm::translate(transformation, pos);
	transformation = glm::rotate(transformation, glm::acos(cos_rotate), glm::vec3(0.0f, 1.0f, 0.0f));

	camera = glm::mat4(1.0f);
	camera = glm::lookAt(camera_eye, camera_eye + look, glm::vec3(0.0f, 1.0f, 0.0f));
}

GLboolean move_obj::collide(const mountain& mountain_obj)
{
	//자신과 충돌체크
	//만약 자신이 미로가 아니면 true리턴
	GLfloat minX = -500.0f + mountain::width * mountain_obj.get_index_r();
	GLfloat maxX = minX + mountain::width;
	GLfloat minZ = -500.0f + mountain::length * mountain_obj.get_index_c();
	GLfloat maxZ = minZ + mountain::length;
	
	if (pos.x - mountain::width / 4 < -500.0f || pos.x + mountain::width / 4 > 500.0f
		|| pos.z - mountain::length / 4 < -500.0f || pos.z + mountain::width / 4 > 500.0f)
		return true;

	if (mountain_obj.maze_state)
		return false;

	if (pos.x - mountain::width / 4 > maxX)
		return false;
	if (pos.x + mountain::width / 4 < minX)
		return false;
	if (pos.z - mountain::length / 4 > maxZ)
		return false;
	if (pos.z + mountain::length / 4 < minZ)
		return false;

	return true;
}