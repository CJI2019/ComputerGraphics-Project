#pragma once
#include "pac_man.h"
#include "move_obj.h"

class wander_pac_man : public pac_man
{
private:
	int old_col, old_row;

	int direction[2] = {0, 0}; //index 0: x����(row), index 1:z����(col) ex) 1 0 -> +x���� , 0 -1 -> -z����
	int sight = 5; //�ִ� �����ִ� ĭ�� ��
public:
	wander_pac_man();

	GLvoid set_col(const int& i_col) { col = i_col; }
	GLvoid set_row(const int& i_row) { row = i_row; }
	GLvoid set_path(const std::vector<std::vector<mountain>>& m_list); //��θ� �����Ѵ�.
	GLvoid set_path(const std::vector<std::vector<mountain>>& m_list, const move_obj& object);

	GLvoid target_in_sight();


	GLvoid move();
	std::vector<int> get_paths() { return paths; }

	GLvoid print_paths();
};

wander_pac_man::wander_pac_man()
{
	pac_man();

	old_col = col;
	old_row = row;
	std::uniform_int_distribution<int> dis(-1, 0);
	direction[0] = dis(gen);
	direction[1] = (direction[0] == 0) ? -1 : 0;
	sight = 5;
}

GLvoid wander_pac_man::move()
{
	int next_col = paths[0] - col;
	int next_row = paths[1] - row;

	if (next_col == 0 && next_row == -1)
	{
		if (pos.z < 12.5 + col * 25 - 500)
			pos.z += speed;
		else if (pos.z > 12.5 + col * 25 - 500)
			pos.z -= speed;
		else
			pos.x -= speed;

	}
	else if (next_col == 0 && next_row == 1)
	{
		if (pos.z < 12.5 + col * 25 - 500)
			pos.z += speed;
		else if (pos.z > 12.5 + col * 25 - 500)
			pos.z -= speed;
		else
			pos.x += speed;
	}
	else if (next_col == -1 && next_row == 0)
	{
		if (pos.x < 12.5 + row * 25 - 500)
			pos.x += speed;
		else if (pos.x > 12.5 + row * 25 - 500)
			pos.x -= speed;
		else
			pos.z -= speed;
	}
	else if (next_col == 1 && next_row == 0)
	{
		if (pos.x < 12.5 + row * 25 - 500)
			pos.x += speed;
		else if (pos.x > 12.5 + row * 25 - 500)
			pos.x -= speed;
		else
			pos.z += speed;
	}

	col = (pos.z + 500) / 25;
	row = (pos.x + 500) / 25;

	transformation = glm::mat4(1.0f);
	transformation = glm::translate(transformation, pos);
}

GLvoid wander_pac_man::set_path(const std::vector<std::vector<mountain>>& m_list)
{
	int next_row = row + direction[0];
	int next_col = col + direction[1];

	if (old_col == col && old_row == row) //ĭ�� �̵��� �������� ��� ������ �ٽ� ����
	{
		paths = std::vector<int>{ next_col, next_row };
		return;
	}

	if (direction[1] == 0) //���� or �������� �����ִ�
	{
		if (col == 0 && !m_list[col + 1][row].maze_state)
		{
			paths = std::vector<int>{ next_col, next_row };
		}
		else if (col == mountain::cNum - 1 && !m_list[col - 1][row].maze_state)
		{
			paths = std::vector<int>{ next_col, next_row };
		}
		else
		{
			std::uniform_int_distribution<int> dis(0, 2);
			while (true)
			{
				int temp_dir = dis(gen);
				if (temp_dir == 0) //����
				{
					if (row == 0 || row == mountain::rNum - 1)
						continue;

					if (!m_list[next_col][next_row].maze_state)
						continue;

					paths = std::vector<int>{ next_col, next_row };
					break;
				}
				else if(temp_dir == 1) //���� ������ �ٲ۴�
				{
					if (col == 0)
						continue;

					if (!m_list[col - 1][row].maze_state)
						continue;

					paths = std::vector<int>{ col - 1, row };
					direction[0] = 0;
					direction[1] = -1;
					break;
				}
				else if (temp_dir == 2) //�Ʒ��� ������ �ٲ۴�
				{
					if (col == mountain::cNum - 1)
						continue;

					if (!m_list[col + 1][row].maze_state)
						continue;

					paths = std::vector<int>{ col + 1, row };
					direction[0] = 0;
					direction[1] = 1;
					break;
				}
			}
		}
	}
	else if (direction[0] == 0) //�� or �Ʒ��� ������
	{
		if (row == 0 && !m_list[col][row + 1].maze_state)
		{
			paths = std::vector<int>{ next_col, next_row };
		}
		else if (row == mountain::rNum - 1 && !m_list[col][row - 1].maze_state)
		{
			paths = std::vector<int>{ next_col, next_row };
		}
		else
		{
			std::uniform_int_distribution<int> dis(0, 2);
			while (true)
			{
				int temp_dir = dis(gen);
				if (temp_dir == 0) //����
				{
					if (col == 0 || col == mountain::cNum - 1)
						continue;

					if (!m_list[next_col][next_row].maze_state)
						continue;

					paths = std::vector<int>{ next_col, next_row };
					break;
				}
				else if (temp_dir == 1) //�������� ������ �ٲ۴�
				{
					if (row == 0)
						continue;

					if (!m_list[col][row - 1].maze_state)
						continue;

					paths = std::vector<int>{ col, row - 1 };
					direction[0] = -1;
					direction[1] = 0;
					break;
				}
				else if (temp_dir == 2) //���������� ������ �ٲ۴�
				{
					if (row == mountain::rNum - 1)
						continue;

					if (!m_list[col][row + 1].maze_state)
						continue;

					paths = std::vector<int>{ col, row + 1 };
					direction[0] = 1;
					direction[1] = 0;
					break;
				}
			}
		}
	}
	old_col = col;
	old_row = row;
}

GLvoid wander_pac_man::set_path(const std::vector<std::vector<mountain>>& m_list, const move_obj& object)
{

}


GLvoid wander_pac_man::target_in_sight()
{
	if (direction[0] == 1 && direction[1] == 0) //����������
	{

	}
}