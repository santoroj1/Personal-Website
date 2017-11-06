/**************************************************************************************************************
*	Analysis of Algorithms Project 3:  The Itsy-Bitsy Spider
*	
*	Programmed By John Santoro (U87402138)
*	
*	This program determines the shortest path that the spider can take to get from the start of the maze to
*	the end of the maze.  
*
*	To compile, use g++ -std=c++0x *.cpp *.h and then use ./a.out to run.  Be sure that the input file is in
*	the same directory as the run file and is entitled "sample-input.txt"
**************************************************************************************************************/

#include <iostream>
#include "Maze.h"
#include <time.h>
#include <fstream>
#include <string>

using namespace std;

// Converts a Binary-Encoded string to its representation as an integer.
int ConvertOpcode(string Opcode)
{
	int composedOpcode = 0;
	int currentBinVal = 1;  // or 2^0...

	// Work from smallest to largest integer
	for (int i = Opcode.length() - 1; i >= 0; i--)
	{
		composedOpcode += (int)(Opcode[i] - 48) * currentBinVal;  // char - 48, when char is a number, returns the actual number.

		currentBinVal *= 2;
	}

	return composedOpcode;
}

int main()
{
	cout << "Analysis of Algorithms Project 3:  The Itsy-Bitsy Spider" << endl << endl;

	// -----------------------------------------------------
	// Test Code
	// -----------------------------------------------------
	
	/*srand(time(NULL));

	vector<vector<vector<int>>> v3 = vector<vector<vector<int>>>(3);

	for (int i = 0; i < v3.size(); i++)
	{
		v3[i] = vector<vector<int>>(6);

		for (int j = 0; j < v3[i].size(); j++)
		{
			v3[i][j] = vector<int>(4);

			for (int k = 0; k < v3[i][j].size(); k++)
			{
				v3[i][j][k] = rand() % 20;
			}
		}
	}*/

	// -----------------------------------------------------
	// End Test Code
	// -----------------------------------------------------

	// -----------------------------------------------------
	// Driver Program
	// -----------------------------------------------------

	ifstream infile("sample-input.txt");
	ofstream outFile("sample-output.txt");

	if (infile.is_open())
	{
		// Store current position in file
		int w = 0, z = 0, y = 0, x = 0;
		int dataPosition = 0;
		string input;
		
		// Problem instance count
		int problems;
		// 3D vector for data
		vector<vector<vector<int>>> v3;
		// Dimension vector
		vector<int> dimension = vector<int>(3);
		// Start
		vector<int> start = vector<int>(3);
		// End
		vector<int> endV = vector<int>(3);

		while (infile >> input)
		{
			// Get problem count
			if (dataPosition == 0 && w == 0 && z == 0 && y == 0 && x == 0)
			{
				problems = stoi(input);
				dataPosition++;
			}
			else 
			{
				if (dataPosition == 1)  // Get dimensions of current problem
				{
					dimension[x++] = stoi(input);

					if (x > 2)  // 3 dimension sizes acquired
					{
						x = 0;
						dataPosition++;

						// Initialize 3D vector.
						v3 = vector<vector<vector<int>>>(dimension[0]);

						for (int i = 0; i < v3.size(); i++)
						{
							v3[i] = vector<vector<int>>(dimension[1]);

							for (int j = 0; j < v3[i].size(); j++)
								v3[i][j] = vector<int>(dimension[2]);
						}
					}
				}
				else if (dataPosition == 2)  // Get starting point
				{
					start[x++] = stoi(input);

					if (x > 2)  // Start vector completed
					{
						x = 0;
						dataPosition++;
					}
				}
				else if (dataPosition == 3)  // Get end point
				{
					endV[x++] = stoi(input);

					if (x > 2)  // End vector completed
					{
						x = 0;
						dataPosition++;
					}
				}
				else if (dataPosition == 4)  // Get data
				{
					v3[z][y][x++] = ConvertOpcode(input);

					// Check to see if index is out-of-bounds + increment logically
					if (x >= dimension[2])
					{
						y++;
						x = 0;
						
						if (y >= dimension[1])
						{
							z++;
							y = 0;

							if (z >= dimension[0])  // End of section reached.  Process/print output.
							{
								z = 0;

								// Process + print output here. -----------------------------------------------------------
								Maze* mz = new Maze(v3);

								std::cout << "Maze #" << w + 1 << ":" << endl;
								/*mz->Print();
								std::cout << endl;*/
								std::cout << "Running Modified BFS on current maze:" << endl;
								string outputString = mz->BFS(start, endV, false);
								std::cout << "Path:  " << outputString;
								std::cout << endl << endl;
								/*std::cout << "Running Dijkstra's Algorithm on current maze:" << endl;
								string outputString = mz->DijkstrasAlgorithm(start, endV, true);*/

								outFile << outputString << endl;

								std::cout << endl;

								// End process + print output. ------------------------------------------------------------

								dataPosition = 1;
								w++;
							}
						}
					}
				}

				if (w >= problems)  // If w is >= the number of problems, break out of loop.
					break;
			}
		}

		
		outFile.close();
		infile.close();
	}

	std::cout << endl;

	// -----------------------------------------------------
	// End Driver Program
	// -----------------------------------------------------

	// cin.get();
  // std::cout << "Press enter to end run." << endl;
}