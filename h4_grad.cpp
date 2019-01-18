/*
    Allan Spektor
    H4_Grad
    Same as H3 but it only draws the edge line segments on the actual edges
*/

#include <cstdio>
#include <iostream>
#include <string>
#include <fstream>
#include <cmath>
#include <algorithm>
#include <queue>
#include <set>
#include "image.h"

#define R2D 0.01745329251

using namespace std;
using namespace ComputerVisionProjects;

int main(int argc, char **argv) 
{

	if (argc != 5) {
		printf("Usage: %s input.pgm voteArray.txt threshold output.pgm \n", argv[0]);
		return 0;
	}
	const string input_file(argv[1]);
    const string vote_file(argv[2]);
    const int threshold = stoi(argv[3]);
	const string output_file(argv[4]);

	Image image;

	if (!ReadImage(input_file, &image)) {
		cout << "Can't open file " << input_file << endl;
		return 0;
	}

    cout << "image dimensions: " << image.num_rows() << ", " << image.num_columns() << endl;

    ifstream voteFile(vote_file);
    if(voteFile.fail()) {
        cout << "Can't open vote file " << vote_file << endl;
        return 0;
    }

    int vArrayHeight, vArrayWidth;
    voteFile >> vArrayHeight >> vArrayWidth;

    int ** voteArray = new int * [vArrayHeight];
    for(int i = 0; i < vArrayHeight; i++) {
        voteArray[i] = new int [vArrayWidth];
    }

    //to get back original i/j you should subtract rMax (vArrayWidth / 2)
    int rMax = vArrayWidth / 2;

    //load + filter the voteArray
    //cout << vArrayHeight << " : " << vArrayWidth << endl;
    for(int i = 0; i < vArrayHeight; i++) {
        for(int j = 0; j < vArrayWidth; j++) {
            voteFile >> voteArray[i][j];
            if(voteArray[i][j] < threshold) {
                voteArray[i][j] = 0; //0 out anything below threshold 
            }
        }       
    }

    //breadth first search, to find local maxima, and then if local maxima draw line
    for(int i = 0; i < vArrayHeight; i++) {
        for(int j = 0; j < vArrayWidth; j++) {
            if(voteArray[i][j]) {
                int m = 0;
                queue<pair<int,int>> points;
                set<pair<int,int>> visited;
                points.push(pair<int,int>(i,j));

                while(!points.empty()) {
                    pair<int,int> point = points.front();
                    const int & r = point.first;
                    const int & c = point.second;
                    points.pop();
                    
                    if(voteArray[r][c] == 0) continue;
                    if(visited.count(point) > 0) continue;
                    else visited.insert(point);

                    m = max(m, voteArray[r][c]);
                    
                    if(r > 0) {
                        points.push(pair<int,int>(r-1, c)); //north
                        if(c > 0) points.push(pair<int,int>(r-1, c-1)); //north west
                        if(c+1 < vArrayWidth) points.push(pair<int,int>(r-1, c+1)); //north east
                    }
                    if(r+1 < vArrayHeight) { 
                        points.push(pair<int,int>(r+1, c)); //south
                        if(c > 0) points.push(pair<int,int>(r+1, c-1)); //south west
                        if(c+1 < vArrayWidth) points.push(pair<int,int>(r+1, c+1)); //south east
                    }  

                    if(c > 0) points.push(pair<int,int>(r, c-1)); //west
                    if(c+1 < vArrayWidth) points.push(pair<int,int>(r, c+1)); //east
                }

                bool first = true;
                for(auto & p : visited) {
                    if(first && voteArray[p.first][p.second] == m && first) {
                        first = false;
                    } else {
                        voteArray[p.first][p.second] = 0;
                    }
                }

                //if it is a local maxima, find 2 points and then draw the line
                if(voteArray[i][j] == m) { 

                    int x1, x2, y1, y2; //start with arbitrary x keep searching until you find an inbound y
                    
                    if(sin(i * R2D) == 0.0) { //vertical line
                        int vL = (double)(j-rMax) / cos(i * R2D); // fixed x
                        if(vL >= 0 && vL < image.num_rows()) {
                            //cout << "Drawing x " << vL << endl;
                            DrawLine(vL, 0, vL, image.num_columns() - 1, 100, &image, true);
                        } else {
                            cout << "trying to draw fixed x " << vL << endl;
                        }
                    } else if(cos(i * R2D) == 0.0) { //horizontal
                        int hL = (double)(j-rMax) / sin(i * R2D); //fixed y
                        if(hL >= 0 && hL < image.num_columns()) {
                            //cout << "drawing y " << hL << endl;
                            DrawLine(0, hL, image.num_rows() - 1, hL, 100, &image, true);
                        } else {
                            cout << "trying to draw fixed y " << hL << endl;
                        }
                    } else {
                        //try to find a x that gives a valid y
                        for(x1 = 0; x1 < image.num_rows(); x1++) {
                            y1 = ((double)(j-rMax) - (double)x1 * cos(i * R2D)) / sin(i * R2D);
                            if(y1 >= 0 && y1 < image.num_columns()) break;
                        }

                        for(x2 = image.num_rows() - 1; x2 >= 0; x2--) {
                            y2 = ((double)(j-rMax) - (double)x2 * cos(i * R2D)) / sin(i * R2D);
                            if(y2 >= 0 && y2 < image.num_columns()) break;
                        }

                        if(y1 >= 0 && y1 < image.num_columns() && y2 >= 0 && y2 < image.num_columns()) {
                            //cout << "drawing " << x1 << ", " << y1 << " : " << x2 << ", " << y2 << endl;
                            DrawLine(x1,y1,x2,y2,100, &image, true);
                        } else {
                            cout << "failed drawing " << x1 << ", " << y1 << " : " << x2 << ", " << y2 << endl;
                        }
                    }
                }
            }
        }       
    }

	if (!WriteImage(output_file, image)) {
		cout << "Can't write to file " << output_file << endl;
		return 0;
	}

	cout << "done" << endl;
}