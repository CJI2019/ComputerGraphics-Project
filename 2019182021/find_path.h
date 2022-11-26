#pragma once
#include "make_Shader.h"
#include <iostream>
#include <queue>
#include <vector>
#include <list>


class path
{
private:
	bool wall; //1이면 미로로 갈수있다. 0이면 벽이다
	bool visit = false;
	int col;
	int row;

	path* prev = nullptr;

public:

	bool get_wall()const { return wall; }
	bool get_visit()const { return visit; }
	int get_col()const { return col; }
	int get_row()const { return row; }
	path* get_prev()const { return prev; }
	

	GLvoid set_wall(const bool& i_bool) { wall = i_bool; }
	GLvoid set_visit() { visit = true; }
	GLvoid set_col(const int& i_col) { col = i_col; }
	GLvoid set_row(const int& i_row) { row = i_row; }
	GLvoid set_prev(path* i_prev_path) { prev = i_prev_path; }
};

bool is_path(const path path_matrix[][25], int dist_col, int dist_row);

GLvoid find_path(std::vector<std::vector<mountain>>& m_list, const int& start_col, const int& start_row, const int& target_col, const int& target_row)
{
	path path_matrix[25][25];
	for (int i = 0; i < 25; ++i)
	{
		for (int j = 0; j < 25; ++j)
		{
			path_matrix[i][j].set_wall(m_list[i][j].maze_state);
			path_matrix[i][j].set_col(i);
			path_matrix[i][j].set_row(j);
		}
	}
	int start[2] = { start_col, start_row };
	
	int dist_col[] = { 0, 1, 0, -1 };
	int dist_row[] = { 1, 0, -1, 0 };

	std::list<path> final_path;
	std::queue<path> adjacent_queue;
	path_matrix[start[0]][start[1]].set_visit();
	adjacent_queue.push(path_matrix[start[0]][start[1]]);

	//큐가 빌때까지 반복한다.
	while (!adjacent_queue.empty())
	{
		path* now_path = &adjacent_queue.front();
		adjacent_queue.pop();

		if (now_path->get_col() == target_col && now_path->get_row() == target_row)
		{
			path found_path = *now_path;
			while (found_path.get_prev() != NULL)
			{
				final_path.push_front(found_path);
				found_path = *found_path.get_prev();
			}
			final_path.push_front(found_path);
			break;
		}

		for (int i = 0; i < 4; ++i)
		{
			int search_col = now_path->get_col() + dist_col[i];
			int search_row = now_path->get_row() + dist_row[i];
			if (is_path(path_matrix, search_col, search_row))
			{
				path_matrix[search_col][search_row].set_visit();
				path_matrix[search_col][search_row].set_prev(&path_matrix[now_path->get_col()][now_path->get_row()]);
				adjacent_queue.push(path_matrix[search_col][search_row]);

			}
		}
	}

	std::list<path>::iterator iter = final_path.begin();
	for (iter; iter != final_path.end(); iter++)
	{
		std::cout << "( " << iter->get_col() << ", " << iter->get_row() << ")\n ";
	}
}

bool is_path(const path path_matrix[][25], int dist_col, int dist_row)
{
	// maze 영역을 벗어나면 안됨.
	if (dist_row < 0 || dist_col < 0 || dist_row > 24 || dist_col > 24) {
		return false;
	}

	// 이미 방문한 노드도 안됨. wall 은 갈수 없음.
	if (path_matrix[dist_col][dist_row].get_visit() || !path_matrix[dist_col][dist_row].get_wall()) {
		return false;
	}
	return true;
}