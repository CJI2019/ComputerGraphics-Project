#pragma once
#include"pac_man.h"
#include "move_obj.h"

class chase_pac_man : public pac_man
{
public:
	chase_pac_man();

	GLvoid set_col(const int& i_col) { col = i_col; }
	GLvoid set_row(const int& i_row) { row = i_row; }
	GLvoid set_path(const std::vector<std::vector<mountain>>& m_list, const int& target_col, const int& target_row); //경로를 설정한다.
	GLvoid set_path(const std::vector<std::vector<mountain>>& m_list, const move_obj& object);

	GLvoid move();
	std::vector<int> get_paths() { return paths; }

	GLvoid print_paths();
};

chase_pac_man::chase_pac_man()
{
	pac_man();
}

GLvoid chase_pac_man::set_path(const std::vector<std::vector<mountain>>& m_list,const int& target_col, const int& target_row)
{
	paths = find_path(m_list, col, row, target_col, target_row);
}
GLvoid chase_pac_man::set_path(const std::vector<std::vector<mountain>>& m_list, const move_obj& object)
{
	paths = find_path(m_list, col, row, object.get_col(), object.get_row());
	old_col = col;
	old_row = row;
}

GLvoid chase_pac_man::move()
{
	int next_col = paths[0] - col;
	int next_row = paths[1] - row;

	//왼쪽이동
	if (next_col == 0 && next_row == -1)
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



GLvoid chase_pac_man::print_paths()
{
	std::cout << paths.size() << std::endl;
	for (int i = 0; i < paths.size(); i += 2)
	{
		std::cout << "(" << paths[i] << ", " << paths[i + 1] << ")->";
	}
	std::cout << std::endl;
}