/**************************************************************************************************************
*	Analysis of Algorithms Project 3:  The Itsy-Bitsy Spider
*
*	Programmed By John Santoro (U87402138)
*
*	This file contains a class which stores information pertaining to each chamber in the maze.
**************************************************************************************************************/

#ifndef MAZEVECTOR_H
#define MAZEVECTOR_H

#define INF 1000000000
#include <iostream>
#include <vector>

using namespace std;

class MazeVector
{
	friend class Maze;

private:
	int directionInfo;
	int distanceFlag;
	int visitedFlag;
	char moveDirection;
	vector<int> parentFlag;

public:
	// Default Constructor
	MazeVector() :directionInfo(0)
	{
		distanceFlag = INF;
		visitedFlag = 2;
		moveDirection = '-';
		parentFlag = vector<int>(3);

		parentFlag[0] = -1;  // NULL marker
	}

	// Constructor which takes direction info
	MazeVector(int DirectionInfo) :directionInfo(DirectionInfo)
	{
		distanceFlag = INF;
		visitedFlag = 2;
		moveDirection = '-';
		parentFlag = vector<int>(3);

		parentFlag[0] = -1;  // NULL marker
	}

	// Resets the variables in the mazeVector so that another BFS can be performed.
	void Reset()
	{
		distanceFlag = INF;
		visitedFlag = 2;

		parentFlag[0] = -1;  // NULL marker
	}

	// Prints all relevant info about this object
	void Print()
	{
		std::cout << "Direction:  " << directionInfo << ", Distance From Start:  " << distanceFlag << ", Visited?  " 
			<< (visitedFlag == 2 ? "OPEN" : (visitedFlag == 1 ? "VISITED" : "CLOSED")) << endl;
	}
};

#endif