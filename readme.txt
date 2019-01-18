The task is to develop a vision system that recognizes lines in an image using the Hough Transform.
Such a system can be used to automatically interpret engineering drawings etc. 
Three gray-level images are provided to you: 
hough_simple_1.pgm, hough_simple_2.pgm and hough_complex_1.pgm.

to run h1
./h1.exe input.pgm output1.pgm

to run h2
./h2.exe output1.pgm threshold output2.pgm
for simple threshold of 20 is good

to run h3
./h3.exe output2.pgm output3.pgm voteArray.txt

to run h4
./h4.exe output2.pgm voteArray.txt threshold output4.pgm 

for simple1 threshold of 360 seems to be good

to run h4_grad
./h4_grad.exe output2.pgm voteArray.txt threshold output4.pgm 
for simple1 threshold of 360 seems to be good
