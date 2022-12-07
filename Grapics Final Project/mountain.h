#pragma once
//#include "make_Shader.h"
#include "all_include_file.h"
#include "my_maze.h"

class mountain
{
private:
	glm::mat4 transformation;
	
	GLint index_r;
	GLint index_c;

public:
	GLboolean maze_state;

	static objRead cuboid;
	static GLuint vao;
	static GLuint vbo[3];
	static GLfloat width;
	static GLfloat length;
	static GLboolean initAni;
	static GLint rNum;
	static GLint cNum;

	//cji
	glm::vec3 pos;

	mountain(const GLint& r, const GLint& c)
	{
		index_r = r;
		index_c = c;
		//true라면 미로의 길이다 즉 바닥으로 변한다
		maze_state = false;

		transformation = glm::mat4(1.0f);
		transformation = glm::translate(transformation,
						 glm::vec3((-500.0f + mountain::width / 2) + mountain::width * index_r, 0.0f, (-500.0f + mountain::length / 2) + mountain::length * index_c));

		//보석 위치를 얻어올 좌표값
		pos = { (-500.0f + mountain::width / 2) + mountain::width * index_r, 0.0f, (-500.0f + mountain::length / 2) + mountain::length * index_c };

		if (mountain::vao == 0)
		{
			mountain::cuboid.loadObj_normalize_center("cuboid.obj");

			glGenVertexArrays(1, &mountain::vao);
			glGenBuffers(3, mountain::vbo);

			glBindVertexArray(mountain::vao);

			glBindBuffer(GL_ARRAY_BUFFER, mountain::vbo[1]);
			glBufferData(GL_ARRAY_BUFFER, cuboid.outnormal.size() * sizeof(glm::vec3), cuboid.outnormal.data(), GL_STATIC_DRAW);
			glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, 0);
			glEnableVertexAttribArray(1);

			glBindBuffer(GL_ARRAY_BUFFER, mountain::vbo[0]);
			glBufferData(GL_ARRAY_BUFFER, cuboid.outvertex.size() * sizeof(glm::vec3), cuboid.outvertex.data(), GL_STATIC_DRAW);
			glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
			glEnableVertexAttribArray(0);
			
			glBindBuffer(GL_ARRAY_BUFFER, mountain::vbo[2]);
			glBufferData(GL_ARRAY_BUFFER, cuboid.outuv.size() * sizeof(glm::vec3), cuboid.outuv.data(), GL_STATIC_DRAW);
			glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 0, 0);
			glEnableVertexAttribArray(2);
		}
	}

	GLint get_index_r() const { return index_r; }
	GLint get_index_c() const { return index_c; }

	GLvoid draw(unsigned int& modelLocation);
	GLvoid drawMaze(unsigned int& modelLocation);
	friend GLvoid set_maze(const maze& completeMaze, std::vector<std::vector<mountain>>& mountainList);
	GLvoid set_height();
	//cji
};
GLvoid open_random_maze(std::vector<std::vector<mountain>>& mountainList, const int& i, const int& j, const int& wall_num);

objRead mountain::cuboid;
GLuint mountain::vao = 0;
GLuint mountain::vbo[3];
GLfloat mountain::width = 0.0f;
GLfloat mountain::length = 0.0f;
GLboolean mountain::initAni = false;
GLint mountain::rNum = 0;
GLint mountain::cNum = 0;


GLvoid mountain::draw(unsigned int& modelLocation)
{
	glUniformMatrix4fv(modelLocation, 1, GL_FALSE, glm::value_ptr(transformation));
	glBindVertexArray(mountain::vao);
	glDrawArrays(GL_TRIANGLES, 0, cuboid.outvertex.size());
}

GLvoid mountain::drawMaze(unsigned int& modelLocation)
{
	if (maze_state)
		return;
	glUniformMatrix4fv(modelLocation, 1, GL_FALSE, glm::value_ptr(transformation));
	glBindVertexArray(mountain::vao);
	glDrawArrays(GL_TRIANGLES, 0, cuboid.outvertex.size());
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
		for (int i = 0; i < mountain::rNum; ++i)
		{
			if(i % 2 == 0)
				mountainList[mountain::cNum - 1][i].maze_state = true;
		}
	}

	if (mountain::rNum % 2 == 0)
	{
		for (int i = 0; i < mountain::cNum; ++i)
		{
			if (i % 2 == 0)
				mountainList[i][mountain::rNum - 1].maze_state = true;
		}
	}

	for (int i = 0; i < mountain::cNum; ++i)
	{
		for (int j = 0; j < mountain::rNum; ++j)
		{
			int wall_num = 0;
			if (i == 0)
				wall_num++;
			else if (!mountainList[i - 1][j].maze_state)
				wall_num++;

			if (i == mountain::cNum - 1)
				wall_num++;
			else if (!mountainList[i + 1][j].maze_state)
				wall_num++;

			if (j == 0)
				wall_num++;
			else if (!mountainList[i][j - 1].maze_state)
				wall_num++;

			if (j == mountain::rNum - 1)
				wall_num++;
			else if (!mountainList[i][j + 1].maze_state)
				wall_num++;

			if (wall_num >= 3)
			{
				open_random_maze(mountainList, i, j, wall_num);
			}
		}
	}
}

GLvoid open_random_maze(std::vector<std::vector<mountain>>& mountainList, const int& i, const int& j, const int& wall_num)
{
	std::uniform_int_distribution<int> dis(0, wall_num);

	GLboolean complete_open = false;

	while (!complete_open)
	{
		switch (dis(gen))
		{
		case 0: //top 연다
			if (i == 0)
				break;
			else if (!mountainList[i - 1][j].maze_state)
			{
				mountainList[i - 1][j].maze_state = true;
				complete_open = true;
			}
			break;
		case 1: //left 연다
			if (j == 0)
				break;
			else if (!mountainList[i][j - 1].maze_state)
			{
				mountainList[i][j - 1].maze_state = true;
				complete_open = true;
			}			break;
		case 2: // bottom 연다
			if (i == mountain::cNum - 1)
				break;
			else if (!mountainList[i + 1][j].maze_state)
			{
				mountainList[i + 1][j].maze_state = true;
				complete_open = true;
			}			break;
		case 3: // right 연다
			if (j == mountain::rNum - 1)
				break;
			else if (!mountainList[i][j + 1].maze_state)
			{
				mountainList[i][j + 1].maze_state = true;
				complete_open = true;
			}
			break;
		default:
			break;
		}
	}
}