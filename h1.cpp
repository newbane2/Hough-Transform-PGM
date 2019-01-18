/*
    Allan Spektor
    H1
	Applies gausian smoothing kernel
	and then applies sobels kernel
	outputs image where pixels are replaced with edge amplitude
*/

#include <cstdio>
#include <iostream>
#include <string>
#include <cmath>
#include "image.h"

using namespace std;
using namespace ComputerVisionProjects;

void sobelOperator(Image & image, Image & oimage);
void gausianSmoothing(Image & image, Image & oimage);

int main(int argc, char **argv) 
{

	if (argc != 3) {
		printf("Usage: %s file1 file2\n", argv[0]);
		return 0;
	}
	const string input_file(argv[1]);
	const string output_file(argv[2]);

	Image image, oimage;

	if (!ReadImage(input_file, &image)) {
		cout << "Can't open file " << input_file << endl;
		return 0;
	}

    //reads in a second image as a copy
    if (!ReadImage(input_file, &oimage)) {
		cout << "Can't open file " << input_file << endl;
		return 0;
	}

	gausianSmoothing(image, oimage);
    sobelOperator(oimage, image);

	if (!WriteImage(output_file, image)) {
		cout << "Can't write to file " << output_file << endl;
		return 0;
	}

	cout << "done" << endl;
}

void sobelOperator(Image & image, Image & oimage) {
	int sobelX[3][3] = 
	{
		{-1, 0, 1},
		{-2, 0, 2},
		{-1, 0, 1}	
	};			
	int sobelY[3][3] =
	{
		{ 1, 2, 1},
		{ 0, 0, 0},
		{-1,-2,-1}
	};

	for(int i = 0; i < image.num_rows(); i++) {
		for(int j = 0; j < image.num_columns(); j++) {

			int xMag = 0, yMag = 0;
			for(int k = -1; k < 2; k++) {
				for(int q = -1; q < 2; q++) {
					//if out of bounds duplicate base pixel value
					int iOff = (i + k < 0 || i + k >= image.num_rows()) ? i : (i + k);
					int jOff = (j + q < 0 || j + q >= image.num_columns()) ? j : (j + q);
					xMag += image.GetPixel(iOff, jOff) * sobelX[k+1][q+1];
					yMag += image.GetPixel(iOff, jOff) * sobelY[k+1][q+1];
				}
			}

			oimage.SetPixel(i, j, sqrt(pow(xMag,2) + pow(yMag,2)));
		}
	}
}

//with smoothing a good threshold is 20
void gausianSmoothing(Image & image, Image & oimage) {
	int kernel[5][5] = 
	{
		{1, 4, 6, 4, 1},
		{4, 16, 24, 16, 4},
		{6, 24, 36, 24, 6},
		{4, 16, 24, 16, 4},
		{1, 4, 6, 4, 1}
	};			

	for(int i = 0; i < image.num_rows(); i++) {
		for(int j = 0; j < image.num_columns(); j++) {

			double value = 0; 
			for(int k = -2; k < 3; k++) {
				for(int q = -2; q < 3; q++) {
					//if out of bounds duplicate base pixel value
					int iOff = (i + k < 0 || i + k >= image.num_rows()) ? i : (i + k);
					int jOff = (j + q < 0 || j + q >= image.num_columns()) ? j : (j + q);
					value += (double) image.GetPixel(iOff, jOff) * (double)(1.0/256.0) * (double) kernel[k+2][q+2];
				}
			}
			oimage.SetPixel(i, j, round(value));
		}
	}
}