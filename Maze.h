/**************************************************************************************************************
*	Analysis of Algorithms Project 3:  The Itsy-Bitsy Spider
*
*	Programmed By John Santoro (U87402138)
*
*	This file contains the class which is used to represent the maze and search it.
**************************************************************************************************************/

#ifndef MAZE_H
#define MAZE_H

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <queue>
#include <stack>
#include "MazeVector.h"

using namespace std;

class Maze
{
private:
	// The "maze" consists of a 3D vector, where each value contains an integer that will be decomposed
	// to determine possible moves from the square.  Vector order:  z --> y --> x.
	vector<vector<vector<MazeVector>>> mazeRepresentation;

	// Resets flags associated with running algorithms
	void ResetFlags()
	{
		for (int i = 0; i < mazeRepresentation.size(); i++)
			for (int j = 0; j < mazeRepresentation[i].size(); j++)
				for (int k = 0; k < mazeRepresentation[i][j].size(); k++)
					mazeRepresentation[i][j][k].Reset();
	}

	// Reconstructs the path if one is found
	string ReconstructPath(vector<int> Goal)
	{
		stack<char> pStack;
		string directionString = "";

		vector<int> currentCoord(3);

		currentCoord[0] = Goal[0];
		currentCoord[1] = Goal[1];
		currentCoord[2] = Goal[2];

		while (mazeRepresentation[currentCoord[0]][currentCoord[1]][currentCoord[2]].parentFlag[0] != -1)  // Push path to stack
		{
			pStack.push(mazeRepresentation[currentCoord[0]][currentCoord[1]][currentCoord[2]].moveDirection);

			currentCoord = mazeRepresentation[currentCoord[0]][currentCoord[1]][currentCoord[2]].parentFlag;
		}
		while (!pStack.empty())
		{
			directionString.push_back(pStack.top());
			directionString.push_back(' ');

			pStack.pop();
		}

		return directionString;
	}

public:
	// Constructor -- This takes 3D vector of integers to populate a 3D vector of MazeVector objects
	Maze(vector<vector<vector<int>>> Input)
	{
		mazeRepresentation = vector<vector<vector<MazeVector>>>(Input.size());

		for (int i = 0; i < Input.size(); i++)
		{
			mazeRepresentation[i] = vector<vector<MazeVector>>(Input[i].size());

			for (int j = 0; j < Input[i].size(); j++)
			{
				mazeRepresentation[i][j] = vector<MazeVector>(Input[i][j].size());

				for (int k = 0; k < Input[i][j].size(); k++)
				{
					mazeRepresentation[i][j][k] = MazeVector(Input[i][j][k]);
				}
			}
		}
	}

	// Do a Modified BFS to determine the shortest path to the goal.
	string BFS(vector<int> Start, vector<int> Goal, bool ShowOutput)
	{
		// Start at start
		mazeRepresentation[Start[0]][Start[1]][Start[2]].visitedFlag = 0;
		mazeRepresentation[Start[0]][Start[1]][Start[2]].distanceFlag = 0;

		if (ShowOutput)
		{
			cout << "<" << Start[2] << ", " << Start[1] << ", " << Start[0] << ">:  ";
			mazeRepresentation[Start[0]][Start[1]][Start[2]].Print();
		}

		queue<vector<int>> matrixPositionq;  // Stores the current coordinates of the position in the maze (used for traversal)
		queue<int> currentLevelq;

		currentLevelq.push(0);
		matrixPositionq.push(Start);

		while (matrixPositionq.size() > 0 && currentLevelq.size() > 0)
		{
			// Get queue info + pop front
			int currentLevel = currentLevelq.front();
			vector<int> zyx = matrixPositionq.front();  // Indeces appear in z-y-x order.
			vector<int> nextzyx(3);

			// Current vector is function of coordinates

			currentLevelq.pop();
			matrixPositionq.pop();

			// Get next move.  i = 0 <==> North?; i = 1 <==> East?; i = 2 <==> South?; i = 3 <==> West?;
			// i = 4 <==> Up?; i = 5 <==> Down?  Note that this loop decomposes the direction to the 
			// appropriate boolean value.
			int valueRemaining = mazeRepresentation[zyx[0]][zyx[1]][zyx[2]].directionInfo;
			int currentDiv = 32;  // or 2^5

			for (int i = 0; i < 6; i++)
			{
				bool moveInDirection = (valueRemaining / currentDiv == 1);  // Read number from front to back.

				if (moveInDirection)
					valueRemaining -= currentDiv;

				currentDiv /= 2;  // 2^(i++)

				// Try perform move
				if (moveInDirection)
				{
					if (i == 0)  // North move
					{
						if (zyx[1] - 1 >= 0 && mazeRepresentation[zyx[0]][zyx[1] - 1][zyx[2]].visitedFlag == 2)
						{
							mazeRepresentation[zyx[0]][zyx[1] - 1][zyx[2]].visitedFlag = 0;
							mazeRepresentation[zyx[0]][zyx[1] - 1][zyx[2]].distanceFlag = currentLevel + 1;

							nextzyx[0] = zyx[0];
							nextzyx[1] = zyx[1] - 1;
							nextzyx[2] = zyx[2];

							// Set move direction flag of current vertex
							mazeRepresentation[nextzyx[0]][nextzyx[1]][nextzyx[2]].moveDirection = 'N';

							// Set parent flag
							mazeRepresentation[nextzyx[0]][nextzyx[1]][nextzyx[2]].parentFlag = zyx;

							if (ShowOutput)
							{
								cout << "[N] <" << zyx[2] << ", " << zyx[1] - 1 << ", " << zyx[0] << ">:  ";
								mazeRepresentation[zyx[0]][zyx[1] - 1][zyx[2]].Print();
							}

							// Check to see whether this is the goal
							if (nextzyx == Goal)
							{
								cout << "Path of length " << currentLevel + 1 << " found." << endl;
								return ReconstructPath(Goal);
							}

							matrixPositionq.push(nextzyx);
							currentLevelq.push(currentLevel + 1);
						}
					}
					else if (i == 1)  // East move
					{
						if (zyx[2] + 1 < mazeRepresentation[0][0].size() && mazeRepresentation[zyx[0]][zyx[1]][zyx[2] + 1].visitedFlag == 2)
						{
							mazeRepresentation[zyx[0]][zyx[1]][zyx[2] + 1].visitedFlag = 0;
							mazeRepresentation[zyx[0]][zyx[1]][zyx[2] + 1].distanceFlag = currentLevel + 1;

							nextzyx[0] = zyx[0];
							nextzyx[1] = zyx[1];
							nextzyx[2] = zyx[2] + 1;

							// Set move direction flag of current vertex
							mazeRepresentation[nextzyx[0]][nextzyx[1]][nextzyx[2]].moveDirection = 'E';

							// Set parent flag
							mazeRepresentation[nextzyx[0]][nextzyx[1]][nextzyx[2]].parentFlag = zyx;

							if (ShowOutput)
							{
								cout << "[E] <" << zyx[2] + 1 << ", " << zyx[1] << ", " << zyx[0] << ">:  ";
								mazeRepresentation[zyx[0]][zyx[1]][zyx[2] + 1].Print();
							}

							// Check to see whether this is the goal
							if (nextzyx == Goal)
							{
								cout << "Path of length " << currentLevel + 1 << " found." << endl;
								return ReconstructPath(Goal);
							}

							matrixPositionq.push(nextzyx);
							currentLevelq.push(currentLevel + 1);
						}
					}
					else if (i == 2)  // South move
					{
						if (zyx[1] + 1 < mazeRepresentation[0].size() && mazeRepresentation[zyx[0]][zyx[1] + 1][zyx[2]].visitedFlag == 2)
						{
							mazeRepresentation[zyx[0]][zyx[1] + 1][zyx[2]].visitedFlag = 0;
							mazeRepresentation[zyx[0]][zyx[1] + 1][zyx[2]].distanceFlag = currentLevel + 1;

							nextzyx[0] = zyx[0];
							nextzyx[1] = zyx[1] + 1;
							nextzyx[2] = zyx[2];

							// Set move direction flag of current vertex
							mazeRepresentation[nextzyx[0]][nextzyx[1]][nextzyx[2]].moveDirection = 'S';

							// Set parent flag
							mazeRepresentation[nextzyx[0]][nextzyx[1]][nextzyx[2]].parentFlag = zyx;

							if (ShowOutput)
							{
								cout << "[S] <" << zyx[2] << ", " << zyx[1] + 1 << ", " << zyx[0] << ">:  ";
								mazeRepresentation[zyx[0]][zyx[1] + 1][zyx[2]].Print();
							}

							// Check to see whether this is the goal
							if (nextzyx == Goal)
							{
								cout << "Path of length " << currentLevel + 1 << " found." << endl;
								return ReconstructPath(Goal);
							}

							matrixPositionq.push(nextzyx);
							currentLevelq.push(currentLevel + 1);
						}
					}
					else if (i == 3)  // West move
					{
						if (zyx[2] - 1 >= 0 && mazeRepresentation[zyx[0]][zyx[1]][zyx[2] - 1].visitedFlag == 2)
						{
							mazeRepresentation[zyx[0]][zyx[1]][zyx[2] - 1].visitedFlag = 0;
							mazeRepresentation[zyx[0]][zyx[1]][zyx[2] - 1].distanceFlag = currentLevel + 1;

							nextzyx[0] = zyx[0];
							nextzyx[1] = zyx[1];
							nextzyx[2] = zyx[2] - 1;

							// Set move direction flag of current vertex
							mazeRepresentation[nextzyx[0]][nextzyx[1]][nextzyx[2]].moveDirection = 'W';

							// Set parent flag
							mazeRepresentation[nextzyx[0]][nextzyx[1]][nextzyx[2]].parentFlag = zyx;

							if (ShowOutput)
							{
								cout << "[W] <" << zyx[2] - 1 << ", " << zyx[1] << ", " << zyx[0] << ">:  ";
								mazeRepresentation[zyx[0]][zyx[1]][zyx[2] - 1].Print();
							}

							// Check to see whether this is the goal
							if (nextzyx == Goal)
							{
								cout << "Path of length " << currentLevel + 1 << " found." << endl;
								return ReconstructPath(Goal);
							}

							matrixPositionq.push(nextzyx);
							currentLevelq.push(currentLevel + 1);
						}
					}
					else if (i == 4)  // Up move
					{
						if (zyx[0] + 1 < mazeRepresentation.size() && mazeRepresentation[zyx[0] + 1][zyx[1]][zyx[2]].visitedFlag == 2)
						{
							mazeRepresentation[zyx[0] + 1][zyx[1]][zyx[2]].visitedFlag = 0;
							mazeRepresentation[zyx[0] + 1][zyx[1]][zyx[2]].distanceFlag = currentLevel + 1;

							nextzyx[0] = zyx[0] + 1;
							nextzyx[1] = zyx[1];
							nextzyx[2] = zyx[2];

							// Set move direction flag of current vertex
							mazeRepresentation[nextzyx[0]][nextzyx[1]][nextzyx[2]].moveDirection = 'U';

							// Set parent flag
							mazeRepresentation[nextzyx[0]][nextzyx[1]][nextzyx[2]].parentFlag = zyx;

							if (ShowOutput)
							{
								cout << "[U] <" << zyx[2] << ", " << zyx[1] << ", " << zyx[0] + 1 << ">:  ";
								mazeRepresentation[zyx[0] + 1][zyx[1]][zyx[2]].Print();
							}

							// Check to see whether this is the goal
							if (nextzyx == Goal)
							{
								cout << "Path of length " << currentLevel + 1 << " found." << endl;
								return ReconstructPath(Goal);
							}

							matrixPositionq.push(nextzyx);
							currentLevelq.push(currentLevel + 1);
						}
					}
					else if (i == 5)  // Down move
					{
						if (zyx[0] - 1 >= 0 && mazeRepresentation[zyx[0] - 1][zyx[1]][zyx[2]].visitedFlag == 2)
						{
							mazeRepresentation[zyx[0] - 1][zyx[1]][zyx[2]].visitedFlag = 0;
							mazeRepresentation[zyx[0] - 1][zyx[1]][zyx[2]].distanceFlag = currentLevel + 1;

							nextzyx[0] = zyx[0] - 1;
							nextzyx[1] = zyx[1];
							nextzyx[2] = zyx[2];

							// Set move direction flag of current vertex
							mazeRepresentation[nextzyx[0]][nextzyx[1]][nextzyx[2]].moveDirection = 'D';

							// Set parent flag
							mazeRepresentation[nextzyx[0]][nextzyx[1]][nextzyx[2]].parentFlag = zyx;

							if (ShowOutput)
							{
								cout << "[D] <" << zyx[2] << ", " << zyx[1] << ", " << zyx[0] - 1 << ">:  ";
								mazeRepresentation[zyx[0] - 1][zyx[1]][zyx[2]].Print();
							}

							// Check to see whether this is the goal
							if (nextzyx == Goal)
							{
								cout << "Path of length " << currentLevel + 1 << " found." << endl;
								return ReconstructPath(Goal);
							}

							matrixPositionq.push(nextzyx);
							currentLevelq.push(currentLevel + 1);
						}
					}
				}
			}
		}

		// Reset flags after running algorithm
		ResetFlags();
		return "Can't find path.";
	}

	// Prints the maze
	void Print()
	{
		for (int i = 0; i < mazeRepresentation.size(); i++)
		{
			cout << "-------------L" << i + 1 << "-------------" << endl;

			for (int j = 0; j < mazeRepresentation[i].size(); j++)
			{
				for (int k = 0; k < mazeRepresentation[i][j].size(); k++)
				{
					cout << (mazeRepresentation[i][j][k].directionInfo >= 10 ? "" : " ")
						<< mazeRepresentation[i][j][k].directionInfo << "  ";
				}

				cout << endl;
			}
		}
	}
};

#endif