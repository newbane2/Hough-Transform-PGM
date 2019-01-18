########################################
##
## Makefile
## LINUX compilation 
##
##############################################





#FLAGS
C++FLAG = -g -std=c++11

MATH_LIBS = -lm

EXEC_DIR=.


.cc.o:
	g++ $(C++FLAG) $(INCLUDES)  -c $< -o $@


#Including
INCLUDES=  -I. 

#-->All libraries (without LEDA)
LIBS_ALL =  -L/usr/lib -L/usr/local/lib 


#First Program (ListTest)

Cpp_OBJ=image.o image_demo.o

PROGRAM_NAME=image_demo

$(PROGRAM_NAME): $(Cpp_OBJ)
	g++ $(C++FLAG) -o $(EXEC_DIR)/$@ $(Cpp_OBJ) $(INCLUDES) $(LIBS_ALL)


all: 
	make h1
	make h2
	make h3 
	make h4

h1: 
	g++ $(C++FLAG) -I.   -c image.cc -o image.o
	g++ $(C++FLAG) -I.   -c h1.cpp -o h1.o
	g++ $(C++FLAG) -o h1 h1.cpp image.cc

h2: 
	g++ $(C++FLAG) -I.   -c image.cc -o image.o
	g++ $(C++FLAG) -I.   -c h2.cpp -o h2.o
	g++ $(C++FLAG) -o h2 h2.cpp image.cc
	
h3:
	g++ $(C++FLAG) -I.   -c image.cc -o image.o
	g++ $(C++FLAG) -I.   -c h3.cpp -o h3.o
	g++ $(C++FLAG) -o h3 h3.cpp image.cc

h4:
	g++ $(C++FLAG) -I.   -c image.cc -o image.o
	g++ $(C++FLAG) -I.   -c h4.cpp -o h4.o
	g++ $(C++FLAG) -o h4 h4.cpp image.cc
	
h4_grad:
	g++ $(C++FLAG) -I.   -c image.cc -o image.o
	g++ $(C++FLAG) -I.   -c h4_grad.cpp -o h4_grad.o
	g++ $(C++FLAG) -o h4_grad h4_grad.cpp image.cc	

clean:
	(rm -f *.o; rm h1)
	(rm -f *.o; rm h2)
	(rm -f *.o; rm h3)
	(rm -f *.o; rm h4)
	(rm -f *.o; rm h4_grad)
(:
