#pragma once

#include <stack>
#include <vector>
#include <random>


enum DIRECTION
{
	TOP = 0,
	BOTTOM,
	LEFT,
	RIGHT,
};

class tile
{
public:
	int mX, mY;
	//true->막혀있다 false->뚫여있다
	bool wallOpen[4] = { true, true, true, true };
	bool visited = false;

	tile(int i_x, int i_y)
	{
		mX = i_x;
		mY = i_y;
	}

	tile() 
	{
		mX = 5;
		mY = 5;
	};

};

class maze
{
private:
	int mMazeWidth;
	int mMazeLength;

public:
	std::vector<std::vector<tile>> Maze;
	tile* curTile;
	bool reset;

	int visitedCellCount;

	std::stack<tile*> tileStack;
	static bool completeGenerate;

	void initialize(const int& w, const int& l);
	void generator();

	//주변 타일 검사
	tile* checkNeighborTiles(const tile& curTile)
	{
		//현재 타일 위치
		int x = curTile.mX;
		int y = curTile.mY;

		bool top = true;
		bool right = true;
		bool bottom = true;
		bool left = true;

		if (y == 0) top = false;
		if (x == mMazeWidth - 1)right = false;
		if (y == mMazeLength - 1)bottom = false;
		if (x == 0) left = false;

		//방문 하지 않은 이웃 타일 탐색

		//이웃 타일 저장용 벡터
		std::vector<tile*> nbTiles;

		 
		if (top) //탑이있고 있고 방문하지 않은 곳이라면 이웃타일 벡터에 넣는다
			if (!Maze[y - 1][x].visited)
				nbTiles.push_back(&Maze[y - 1][x]);

		if (right)
			if (!Maze[y][x + 1].visited)
				nbTiles.push_back(&Maze[y][x + 1]);

		if (bottom)
			if (!Maze[y + 1][x].visited)
				nbTiles.push_back(&Maze[y + 1][x]);

		if (left)
			if (!Maze[y][x - 1].visited)
				nbTiles.push_back(&Maze[y][x - 1]);

		//이웃한 타일중 갈수있고 방문하지 않은곳이 있다면
		if (!nbTiles.empty())
		{
			//이웃 벡터에서 아무거나 하나를 리턴한다
			int r = getRandom(0, nbTiles.size() - 1);
			return nbTiles[r];
		}
		else //없다면 -> 널포인터 리턴
			return nullptr;
	}

	int getRandom(int min, int max) {
		std::random_device ranDevice;
		std::mt19937 gen(ranDevice());
		std::uniform_int_distribution<int> dis(min, max);

		return (int)dis(gen);
	}

	void openWall(const int dir, int x, int y)
	{
		//방향에 따라
		switch (dir)
		{
		case TOP://top이라면 
			Maze[y][x].wallOpen[0] = false;
			Maze[y - 1][x].wallOpen[2] = false;
			break;
		case BOTTOM:
			Maze[y][x].wallOpen[2] = false;
			Maze[y + 1][x].wallOpen[0] = false;
			break;
		case RIGHT:
			Maze[y][x].wallOpen[1] = false;
			Maze[y][x + 1].wallOpen[3] = false;
			break;
		case LEFT:
			Maze[y][x].wallOpen[3] = false;
			Maze[y][x - 1].wallOpen[1] = false;
			break;
		}
	}

	void openWall(const tile& firstTile, const tile& secondTile)
	{
		//방향 체크
		int checkDirection;

		if (firstTile.mX == secondTile.mX) { //두타일의 x값이 같으면 위아래 비교
			checkDirection = firstTile.mY - secondTile.mY;
			if (checkDirection < 0) 
			{	//0보다 작다는 두번째 타일이 아래에 있다는 뜻 -> 바텀을 뚫어준다
				openWall(BOTTOM, firstTile.mX, firstTile.mY);
			}
			else 
			{	//탑을 뚫는다
				openWall(TOP, firstTile.mX, firstTile.mY);
			}
		}

		else { //y값이 같으면 
			checkDirection = firstTile.mX - secondTile.mX;
			if (checkDirection < 0)
			{
				openWall(RIGHT, firstTile.mX, firstTile.mY); 
			}
			else 
			{
				openWall(LEFT, firstTile.mX, firstTile.mY); 
			}
		}
	}

	void ResetMaze() {
		if (reset) return;
		while (!tileStack.empty()) {
			tileStack.pop();
		}
		Maze.clear();
		visitedCellCount = 1;
		maze::completeGenerate = false;
		reset = true;
	}
};

void maze::initialize(const int& w, const int& l)
{
	mMazeWidth = w;
	mMazeLength = l;
	visitedCellCount = 1;

	//초기화
	Maze = std::vector<std::vector<tile>>(mMazeLength);
	for (int i = 0; i < mMazeLength; ++i)
	{
		Maze[i] = std::vector<tile>(mMazeWidth);
	}

	for (int y = 0; y < mMazeLength; y++)
	{
		for (int x = 0; x < mMazeWidth; x++)
		{
			Maze[y][x].mX = x;
			Maze[y][x].mY = y;
			for (int i = 0; i < 4; ++i)
				Maze[y][x].wallOpen[i] = true;

			Maze[y][x].visited = false;
		}
	}
	completeGenerate = false;
	
	//일단 보류
	curTile = &Maze[0][0];

	reset = false;
}

void maze::generator()
{
	curTile->visited = true;
	tile* nextTile = this->checkNeighborTiles(*curTile);
	
	if (visitedCellCount < (mMazeLength * mMazeWidth))
	{
		if (nextTile != nullptr)
		{
			nextTile->visited = true;

			//stack push
			tileStack.push(curTile);

			openWall(*curTile, *nextTile);

			//갱신
			curTile = nextTile;

			visitedCellCount++;
		}
		else if (!tileStack.empty()) {
			curTile = tileStack.top();
			tileStack.pop();
		}
	}
	else
	{
		//std::cout << visitedCellCount<< " 미로 완성" << std::endl;
		maze::completeGenerate = true;
	}

}

bool maze::completeGenerate = false;