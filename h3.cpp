/*
    Allan Spektor
    H3 takes the output of H1 and then converts the edge points into hough space, counts the votes,
    and then outputs the vote array to a txt and a visualization of the hough space as a pgm
*/

#include <cstdio>
#include <cstring>
#include <iostream>
#include <string>
#include <cmath>
#include <fstream>
#include <algorithm>
#include "image.h"

#define R2D 0.01745329251

using namespace std;
using namespace ComputerVisionProjects;

void houghTransform(Image & image, int ** voteArray);

int main(int argc, char **argv) 
{

	if (argc != 4) {
		printf("Usage: %s file1 file2 file3\n", argv[0]);
		return 0;
	}
	const string input_file(argv[1]);
	const string output_file(argv[2]); //out image
    const string output_votingArray(argv[3]);

	Image image;

	if (!ReadImage(input_file, &image)) {
		cout << "Can't open file " << input_file << endl;
		return 0;
	}

    ofstream outArray(output_votingArray);
    if(outArray.fail()) {
        cout << "Can't create " << output_votingArray << endl;
        return 0;
    }

    //create and initialize the vote array, (theta, r)
    int ** voteArray = new int * [181];
    int rMax = (int) sqrt(pow(image.num_rows(), 2) + pow(image.num_columns(), 2)) * 2;
    for(int i = 0; i < 181; i++) {
        voteArray[i] = new int [rMax];
        std::memset(voteArray[i], 0, rMax * 4); //initialize it all to 0
    }

    houghTransform(image, voteArray);

    Image outImage;
    outImage.AllocateSpaceAndSetSize(181, rMax);
    outImage.SetNumberGrayLevels(255);
    //write the vote array dimensions in header
    outArray << 181 << " " << rMax;
    for(int i = 0; i < 181; i++) {
        for(int q = 0; q < rMax; q++) {
            outImage.SetPixel(i, q, (int) min(voteArray[i][q], 255));
            outArray << " " << voteArray[i][q];
        }
        outArray << endl;
    }

	if (!WriteImage(output_file, outImage)) {
		cout << "Can't write to file " << output_file << endl;
		return 0;
	}

    outArray.close();

	cout << "done" << endl;
}

//transform is done using a resolution of half degrees
void houghTransform(Image & image, int ** voteArray) {
    int rMax = sqrt(pow(image.num_rows(), 2) + pow(image.num_columns(), 2));
    //iterate through entire image, and build up the vote Array
    cout << "running hough transform, please wait" << endl;
    cout << "rMax : " << rMax << endl;
    for(int i = 0; i < image.num_rows(); i++) {
        for(int j = 0; j < image.num_columns(); j++) {
            //cout << "working on : " << i << ", " << j << endl;
            if(image.GetPixel(i,j) > 0) {
                for(int q = 0; q < 181; q++) { //actual r value will be r - row# to make indices all positive
                    int rho = round(((i * cos(q * R2D) + j * sin(q * R2D))));
                    voteArray[q][rho + rMax]++;
                }
            }
        }
    }
    cout << "finished running hough transform" << endl;
}