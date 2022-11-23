#pragma once
#include "make_Shader.h"
#include "cuboid.h"
#include "my_maze.h"
//#include "move_obj.h"

class mountain
{
private:
	GLuint vao;
	GLuint vbo[2];

	std::vector<GLfloat> vertex = std::vector<GLfloat>(108);
	std::vector<GLfloat> color = std::vector<GLfloat>(108);;
	glm::mat4 transformation;
	
	GLint index_r;
	GLint index_c;
	GLfloat max_height;
	GLfloat speed;
	GLfloat now_height;
	GLboolean state_up;

public:
	GLboolean maze_state;

	static GLfloat width;
	static GLfloat length;
	static GLboolean initAni;
	static GLint rNum;
	static GLint cNum;

	mountain(const GLint& r, const GLint& c)
	{
		std::uniform_int_distribution<int> dis(50, 300);
		std::uniform_int_distribution<int> dis_speed(1, 10);
		max_height = static_cast<GLfloat>(dis(gen));
		speed = static_cast<GLfloat>(dis_speed(gen)) * 0.01;
		now_height = 0.0f;
		state_up = true;

		index_r = r;
		index_c = c;
		//true라면 미로의 길이다 즉 바닥으로 변한다
		maze_state = false;

		makeCuboid(vertex, mountain::width / 2, mountain::length / 2, max_height);

		GLfloat tempCol = dis(gen) * 0.001;

		setCol(color, tempCol, tempCol, tempCol);

		transformation = glm::mat4(1.0f);
		transformation = glm::translate(transformation,
						 glm::vec3((-500.0f + mountain::width / 2) + mountain::width * index_r, 0.0f, (-500.0f + mountain::length / 2) + mountain::length * index_c));

		transformation = glm::scale(transformation, glm::vec3(1.0f, now_height, 1.0f));

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
	}

	GLvoid set_speed(const GLfloat& i_speed) { speed += i_speed; }

	GLint get_index_r() const { return index_r; }
	GLint get_index_c() const { return index_c; }
	GLfloat get_speed() const { return speed; }

	GLvoid draw(unsigned int& modelLocation);
	GLvoid drawMaze(unsigned int& modelLocation);
	GLvoid init_animation();
	GLvoid animation();
	friend GLvoid set_maze(const maze& completeMaze, std::vector<std::vector<mountain>>& mountainList);
	GLvoid set_height();
};

GLfloat mountain::width = 0.0f;
GLfloat mountain::length = 0.0f;
GLboolean mountain::initAni = false;
GLint mountain::rNum = 0;
GLint mountain::cNum = 0;


GLvoid mountain::draw(unsigned int& modelLocation)
{
	glUniformMatrix4fv(modelLocation, 1, GL_FALSE, glm::value_ptr(transformation));
	glBindVertexArray(vao);
	glDrawArrays(GL_TRIANGLES, 0, vertex.size() / 3);
}

GLvoid mountain::drawMaze(unsigned int& modelLocation)
{
	if (maze_state)
		return;
	glUniformMatrix4fv(modelLocation, 1, GL_FALSE, glm::value_ptr(transformation));
	glBindVertexArray(vao);
	glDrawArrays(GL_TRIANGLES, 0, vertex.size() / 3);
}


GLvoid mountain::init_animation()
{
	now_height += 0.1f;
	if (now_height >= 1.0f)
	{
		state_up = false;
		mountain::initAni = true;
		now_height = 1.0f;
	}
	transformation = glm::mat4(1.0f);
	transformation = glm::translate(transformation,
		glm::vec3((-500.0f + mountain::width / 2) + mountain::width * index_r, max_height * now_height, (-500.0f + mountain::length / 2) + mountain::length * index_c));
	transformation = glm::scale(transformation, glm::vec3(1.0f, now_height, 1.0f));
}

GLvoid mountain::animation()
{
	if (state_up)
	{
		now_height += speed;
if (now_height >= 1.0f)
{
	now_height = 1.0f;
	state_up = false;
}
	}
	else if (!state_up)
	{
	now_height -= speed;
	if (now_height <= 0.0f)
	{
		now_height = 0.0f;
		state_up = true;
	}
	}
	transformation = glm::mat4(1.0f);
	transformation = glm::translate(transformation,
		glm::vec3((-500.0f + mountain::width / 2) + mountain::width * index_r, max_height * now_height, (-500.0f + mountain::length / 2) + mountain::length * index_c));
	transformation = glm::scale(transformation, glm::vec3(1.0f, now_height, 1.0f));
}

GLvoid set_maze(const maze& completeMaze, std::vector<std::vector<mountain>>& mountainList)
{
	for (int i = 0; i < mountain::cNum; ++i)
	{
		for (int j = 0; j < mountain::rNum; ++j)
		{
			if (i % 2 == 1 || j % 2 == 1)
				mountainList[i][j].maze_state = false;
			else
				mountainList[i][j].maze_state = true;
		}
	}

	for (int i = 0; i < (mountain::cNum + 1) / 2; ++i)
	{
		for (int j = 0; j < (mountain::rNum + 1) / 2; ++j)
		{
			//0 top  1 right  2 bottom  3 left
			if (!completeMaze.Maze[i][j].wallOpen[0])
				mountainList[i * 2 - 1][j * 2].maze_state = true;

			if (!completeMaze.Maze[i][j].wallOpen[1])
				mountainList[i * 2][j * 2 + 1].maze_state = true;

			if (!completeMaze.Maze[i][j].wallOpen[2])
				mountainList[i * 2 + 1][j * 2].maze_state = true;

			if (!completeMaze.Maze[i][j].wallOpen[3])
				mountainList[i * 2][j * 2 - 1].maze_state = true;
		}
	}

	//마지막 탈출방
	mountainList[mountain::cNum - 1][mountain::rNum - 1].maze_state = true;

	if (mountain::cNum % 2 == 0 && mountain::rNum % 2 == 0)
	{
		std::uniform_int_distribution<int> dis(0, 1);

		if (dis(gen) == 0) //마지막 top이 열림
			mountainList[mountain::cNum - 2][mountain::rNum - 1].maze_state = true;
		else
			mountainList[mountain::cNum - 1][mountain::rNum - 2].maze_state = true;
	}

	if (mountain::cNum % 2 == 0)
	{
		std::uniform_int_distribution<int> dis(1, mountain::rNum - 4);
		int random_loop = dis(gen);
		for (int i = 0; i < random_loop; ++i)
			mountainList[mountain::cNum - 1][dis(gen)].maze_state = true;
	}

	if (mountain::rNum % 2 == 0)
	{
		std::uniform_int_distribution<int> dis(1, mountain::cNum - 4);
		int random_loop = dis(gen);
		for (int i = 0; i < random_loop; ++i)
			mountainList[dis(gen)][mountain::rNum - 1].maze_state = true;

	}
}

GLvoid mountain::set_height()
{
	if (!maze_state)
	{
		transformation = glm::mat4(1.0f);
		transformation = glm::translate(transformation,
			glm::vec3((-500.0f + mountain::width / 2) + mountain::width * index_r, max_height * 0.2f, (-500.0f + mountain::length / 2) + mountain::length * index_c));
		transformation = glm::scale(transformation, glm::vec3(1.0f, 0.2f, 1.0f));
	}
}

