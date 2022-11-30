#pragma once
#include "make_Shader.h"
#include <unordered_map>


//class maze
//{
//private:
//
//
//public:
//	static GLuint set_num;
//
//};
//
//GLuint maze::set_num = 0;

struct Cell
{
	//원소의 행열인덱스
	int col, row;
	//그룹
	int group;
	//연결성?
	bool up, down, left, right;
};

struct Group
{
	//멤버
	std::vector<Cell*> member;
	//마지막 행
	std::vector<Cell*> last_rows;
};

//입력
int input;

int group_count = 1;
//현재 행
int cur_col = 0;
int draw_array[200][200];
Cell cells[90][90];
std::unordered_map<int, Group> groups;


void set_cell_group(int group);

void init()
{
	for (int i = 0; i < input; i++) 
	{
		for (int j = 0; j < input; j++) 
		{
			cells[i][j].row = j;
			cells[i][j].col = i;
		}
	}
}

void set_row()
{
	for (int i = 0; i < input; i++)
	{
		if (cells[cur_col][i].group == 0) // 그룹이 아직 지정되지 않았다면 새 그룹 생성
		{
			cells[cur_col][i].col = cur_col;
			cells[cur_col][i].row = i;
			cells[cur_col][i].group = group_count;

			//그룹에 삽입
			groups.insert(std::make_pair(group_count, Group()));
			groups[group_count].member.push_back(&cells[cur_col][i]);
			groups[group_count].last_rows.push_back(&cells[cur_col][i]);

			//그룹 카운트 +1
			group_count++;
		}
	}
}

void rnd_union()
{
	int cur_group = -1;
	int next_group = -1;
	int dir;
	bool flag;
	std::uniform_int_distribution<int> dis(0, 2);

	for (int i = 0; i < input; i++)
	{
		cur_group = cells[cur_col][i].group;
		flag = false;

		switch (dis(gen))
		{
		case 0: // 오른쪽 합치기
			//원소가 행의 끝이 아니고 같은 그룹이 아니라면 연결해준다
			next_group = cells[cur_col][i + 1].group;

			if (i + 1 < input && cur_group != next_group) {
				cells[cur_col][i + 1].left = true;
				cells[cur_col][i].right = true;

				groups[cur_group].last_rows.push_back(&cells[cur_col][i + 1]);

				flag = true;
			}
			break;
		case 1: // 왼쪽 합치기
			next_group = cells[cur_col][i - 1].group;

			if (i - 1 > -1 && cur_group != next_group) {
				cells[cur_col][i - 1].right = true;
				cells[cur_col][i].left = true;

				groups[cur_group].last_rows.push_back(&cells[cur_col][i - 1]);

				flag = true;
			}
			break;
		case 2: // 합치지 않기
			break;
		}
	
		if (flag)
		{
			groups[cur_group].member.insert(groups[cur_group].member.end(), groups[next_group].member.begin(), groups[next_group].member.end());
			groups.erase(next_group);
			set_cell_group(cur_group);
		}
	}

}

void set_cell_group(int group)
{
	for (int i = 0; i < groups[group].member.size(); i++)
	{
		groups[group].member[i]->group = group;
	}
}

//아래행으로 넓히기
void group_down()
{
	std::unordered_map<int, Group>::iterator iter;
	std::vector<Cell*> dest;
	int down_count = 0;
	int cur_row;

	for (iter = groups.begin(); iter != groups.end(); iter++)
	{
		down_count = 0;
		
		dest.clear();
		dest.assign(iter->second.last_rows.begin(), iter->second.last_rows.end());

		std::uniform_int_distribution<int> dis(0, 1);
		std::uniform_int_distribution<int> dis2(0, dest.size() - 1);


		groups[iter->first].last_rows.clear();

		for (int i = 0; i < dest.size(); i++)
		{
			//연결하거나 말거나 랜덤

			if (dis(gen))
			{
				cur_row = dest[i]->row;

				groups[iter->first].member.push_back(&cells[cur_col + 1][cur_row]);
				groups[iter->first].last_rows.push_back(&cells[cur_col + 1][cur_row]);

				cells[cur_col + 1][cur_row].up = true;
				cells[cur_col][cur_row].down = true;

				set_cell_group(iter->first);
				down_count++;
			}
		}

		//아무것도 아래로 안넓혔으면 하나는 적어도 해야함
		if (down_count == 0 && dest.size() != 0)
		{
			cur_row = iter->second.member[dis2(gen)]->row;

			groups[iter->first].member.push_back(&cells[cur_col + 1][cur_row]);
			groups[iter->first].last_rows.push_back(&cells[cur_col + 1][cur_row]);

			cells[cur_col + 1][cur_row].up = true;
			cells[cur_col][cur_row].down = true;

			set_cell_group(iter->first);
		}

	}

}

void last_line()
{
	for (int i = 0; i < input - 1; i++)
	{
		//마지막 행의 두개의 원소가 같은 그룹이 아니라면 연결
		if (cells[cur_col][i + 1].group != cells[cur_col][i].group)
		{
			cells[cur_col][i].right = true;
			cells[cur_col][i + 1].left = true;
		}
	}
}

void init_array()
{
	for (int i = 0; i < input * 2 + 1; i++)
	{
		for (int j = 0; j < input * 2 + 1; j++)
		{
			draw_array[i][j] = 1;
		}
	}
}

void set_array_block()
{
	for (int i = 0; i < input; i++)
	{
		for (int j = 0; j < input; j++)
		{
			draw_array[i * 2 + 1][j * 2 + 1] = 0;

			if (cells[i][j].up)
				draw_array[i * 2][j * 2 + 1] = 0;

			if (cells[i][j].down)
				draw_array[i * 2 + 2][j * 2 + 1] = 0;

			if (cells[i][j].left)
				draw_array[i * 2 + 1][j * 2] = 0;

			if (cells[i][j].right)
				draw_array[i * 2 + 1][j * 2 + 2] = 0;

		}
	}
}


void draw_maze()
{
	for (int i = 0; i < input * 2 + 1; i++)
	{
		for (int j = 0; j < input * 2 + 1; j++)
		{
			if (draw_array[i][j] > 0)
			{
				std::cout << "■";
			}
			else
				std::cout << "  ";
		}
		std::cout << std::endl;
	}
}


void print_group()
{
	for (int i = 0; i < input; i++)
	{
		for (int j = 0; j < input; j++)
		{
			{
				printf("%4d", cells[i][j].group);
			}
		}
		std::cout << std::endl;
	}
	std::cout << std::endl;
}

void print_maze() {
	init_array();
	set_array_block();
	draw_maze();
	//print_group();
}