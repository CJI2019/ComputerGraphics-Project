#pragma once
#include "pac_man.h"
#include "move_obj.h"

class wander_pac_man : public pac_man
{
private:

	int direction[2] = {0, 0}; //index 0: x방향(row), index 1:z방향(col) ex) 1 0 -> +x방향 , 0 -1 -> -z방향
	int sight = 5; //최대 볼수있는 칸의 수
	int miss_time = 0; //놓친 시간
public:
	wander_pac_man();

	GLvoid set_col(const int& i_col) { col = i_col; }
	GLvoid set_row(const int& i_row) { row = i_row; }
	GLvoid set_old_col(const int& i_col) { old_col = i_col; }
	GLvoid set_old_row(const int& i_row) { old_row = i_row; }
	GLvoid set_path(const std::vector<std::vector<wall>>& w_list); //경로를 설정한다.
	bool set_path(const std::vector<std::vector<wall>>& w_list, const move_obj& object);
	GLvoid set_miss_time(const int& i_t) { miss_time = i_t; }
	GLvoid set_direction();

	GLvoid miss_time_gone();

	GLvoid move();
	std::vector<int> get_paths() const { return paths; }
	int get_miss_time() const { return miss_time; }

	GLvoid print_time()const { std::cout << miss_time << std::endl; }
	GLvoid reset();
	GLvoid print_paths()
	{
		std::cout << paths.size() << std::endl;
		for (int i = 0; i < paths.size(); i += 2)
		{
			std::cout << "(" << paths[i] << ", " << paths[i + 1] << ")->";
		}
		std::cout << std::endl;
	};
};

wander_pac_man::wander_pac_man()
{
	pac_man();

	std::uniform_int_distribution<int> dis(-1, 0);
	direction[0] = dis(gen);
	direction[1] = (direction[0] == 0) ? -1 : 0;
	sight = 5;
	look[0] = direction[0];
	look[1] = direction[1];
}

GLvoid wander_pac_man::reset()
{
	pac_man::reset();
	std::uniform_int_distribution<int> dis(-1, 0);
	direction[0] = dis(gen);
	direction[1] = (direction[0] == 0) ? -1 : 0;
	sight = 5;
	look[0] = direction[0];
	look[1] = direction[1];
}

GLvoid wander_pac_man::miss_time_gone()
{
	miss_time -= 1;
}


GLvoid wander_pac_man::move()
{
	int next_col = paths[0] - col;
	int next_row = paths[1] - row;

	if (next_col == 0 && next_row == 0)
	{
		if (look[0] == 0 && look[1] == 1)
		{
			if (pos.z < 20 + col * 40 - 500)
				pos.z += speed;
		}
		else if (look[0] == 0 && look[1] == -1)
		{
			if (pos.z > 20 + col * 40 - 500)
				pos.z -= speed;
		}
		else if (look[0] == 1 && look[1] == 0)
		{
			if (pos.x < 20 + row * 40 - 500)
				pos.x += speed;
		}
		else if (look[0] == -1 && look[1] == 0)
		{
			if (pos.x > 20 + row * 40 - 500)
				pos.x -= speed;
		}
	}
	else if (next_col == 0 && next_row == -1)
	{
		if (pos.z < 20 + col * 40 - 500)
		{
			pos.z += speed;
			look[0] = 0;
			look[1] = 1;
		}
		else if (pos.z > 20 + col * 40 - 500)
		{
			pos.z -= speed;
			look[0] = 0;
			look[1] = -1;
		}
		else
		{
			look[0] = -1;
			look[1] = 0;
			pos.x -= speed;
		}
	}
	else if (next_col == 0 && next_row == 1)
	{
		if (pos.z < 20 + col * 40 - 500)
		{
			look[0] = 0;
			look[1] = 1;
			pos.z += speed;
		}
		else if (pos.z > 20 + col * 40 - 500)
		{
			look[0] = 0;
			look[1] = -1;
			pos.z -= speed;
		}
		else
		{
			look[0] = 1;
			look[1] = 0;
			pos.x += speed;
		}
	}
	else if (next_col == -1 && next_row == 0)
	{
		if (pos.x < 20 + row * 40 - 500)
		{
			look[0] = 1;
			look[1] = 0;
			pos.x += speed;
		}
		else if (pos.x > 20 + row * 40 - 500)
		{
			look[0] = -1;
			look[1] = 0;
			pos.x -= speed;
		}
		else
		{
			look[0] = 0;
			look[1] = -1;
			pos.z -= speed;
		}
	}
	else if (next_col == 1 && next_row == 0)
	{
		if (pos.x < 20 + row * 40 - 500)
		{
			look[0] = 1;
			look[1] = 0;
			pos.x += speed;
		}
		else if (pos.x > 20 + row * 40 - 500)
		{
			look[0] = -1;
			look[1] = 0;
			pos.x -= speed;
		}
		else
		{
			look[0] = 0;
			look[1] = 1;
			pos.z += speed;
		}
	}

	col = (pos.z + 500) / 40;
	row = (pos.x + 500) / 40;

	transformation = glm::mat4(1.0f);
	transformation = glm::translate(transformation, pos);
	if (look[0] == 0 && look[1] == 1)
		return;
	else if (look[0] == 0 && look[1] == -1)
		transformation = glm::rotate(transformation, glm::radians(180.0f), { 0.0f, 1.0f, 0.0f });
	else if (look[0] == 1 && look[1] == 0)
		transformation = glm::rotate(transformation, glm::radians(90.0f), { 0.0f, 1.0f, 0.0f });
	else if (look[0] == -1 && look[1] == 0)
		transformation = glm::rotate(transformation, glm::radians(-90.0f), { 0.0f, 1.0f, 0.0f });
}

GLvoid wander_pac_man::set_path(const std::vector<std::vector<wall>>& w_list)
{
	int next_row = row + direction[0];
	int next_col = col + direction[1];

	if (old_col == col && old_row == row) //칸의 이동이 없었으면 경로 설정을 다시 안함
	{
		paths = std::vector<int>{ next_col, next_row };
		return;
	}

	if (direction[1] == 0) //왼쪽 or 오른쪽으로 가야한다
	{
		std::uniform_int_distribution<int> dis(0, 2);
		while (true)
		{
			int temp_dir = dis(gen);
			if (temp_dir == 0) //직진
			{
				if (row == 0 || row == wall::rNum - 1)
					continue;

				if (!w_list[next_col][next_row].maze_state)
					continue;

				paths = std::vector<int>{ next_col, next_row };
				break;
			}
			else if (temp_dir == 1) //위로 방향을 바꾼다
			{
				if (col == 0)
					continue;

				if (!w_list[col - 1][row].maze_state)
					continue;

				paths = std::vector<int>{ col - 1, row };
				look[0] = direction[0] = 0;
				look[1] = direction[1] = -1;
				break;
			}
			else if (temp_dir == 2) //아래로 방향을 바꾼다
			{
				if (col == wall::cNum - 1)
					continue;

				if (!w_list[col + 1][row].maze_state)
					continue;

				paths = std::vector<int>{ col + 1, row };
				look[0] = direction[0] = 0;
				look[1] = direction[1] = 1;
				break;
			}
		}

	}
	else if (direction[0] == 0) //위 or 아래로 가는중
	{

		std::uniform_int_distribution<int> dis(0, 2);
		while (true)
		{
			int temp_dir = dis(gen);
			if (temp_dir == 0) //직진
			{
				if (col == 0 || col == wall::cNum - 1)
					continue;

				if (!w_list[next_col][next_row].maze_state)
					continue;

				paths = std::vector<int>{ next_col, next_row };
				break;
			}
			else if (temp_dir == 1) //왼쪽으로 방향을 바꾼다
			{
				if (row == 0)
					continue;

				if (!w_list[col][row - 1].maze_state)
					continue;

				paths = std::vector<int>{ col, row - 1 };
				look[0] = direction[0] = -1;
				look[1] = direction[1] = 0;
				break;
			}
			else if (temp_dir == 2) //오른쪽으로 방향을 바꾼다
			{
				if (row == wall::rNum - 1)
					continue;

				if (!w_list[col][row + 1].maze_state)
					continue;

				paths = std::vector<int>{ col, row + 1 };
				look[0] = direction[0] = 1;
				look[1] = direction[1] = 0;
				break;
			}
		}
	}
	old_col = col;
	old_row = row;
}

bool wander_pac_man::set_path(const std::vector<std::vector<wall>>& w_list, const move_obj& object)
{
	paths = find_path(w_list, col, row, object.get_col(), object.get_row());

	if (look[0] == 0 && look[1] == -1) //위를보고 있음
	{
		for (int i = 0; i < sight; ++i)
		{		
			if (object.get_col() == col - i && object.get_row() == row)
				return true;

			if (col - i == 0)
				return false;

			if (!w_list[col - i][row].maze_state)
				return false;
		}
	}
	else if (look[0] == 0 && look[1] == 1) //아래를 보고있음
	{
		for (int i = 0; i < sight; ++i)
		{
			if (object.get_col() == col + i && object.get_row() == row)
				return true;

			if (col + i == wall::cNum - 1)
				return false;
			if (!w_list[col + i][row].maze_state)
				return false;

		}
	}
	else if (look[0] == -1 && look[1] == 0) //왼쪽 보고 있음
	{
		for (int i = 0; i < sight; ++i)
		{

			if (object.get_col() == col && object.get_row() == row - i)
				return true;

			if (row - i == 0)
				return false;
			if (!w_list[col][row - i].maze_state)
				return false;
		}
	}
	else if (look[0] == 1 && look[1] == 0)
	{
		for (int i = 0; i < sight; ++i)
		{
			if (object.get_col() == col && object.get_row() == row + 1)
				return true;
			
			if (row + i == wall::rNum - 1)
				return false;

			if (!w_list[col][row + 1].maze_state)
				return false;
		}
	}
	return false;
}