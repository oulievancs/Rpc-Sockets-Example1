/*
 *	Author: Oulis Evangelos
 *	Class:	Friday 2-4
 *	Subject:	Distributed Systems
 *	Description:	Remote Procedure Call x-File
 *	File Name:	remoteproc.x
 */
 
typedef int data<>;

typedef float new_array<>;

struct result1 {
	int max;
	int min;
};

struct argument1 {
	int data<>;
	float r;
};


program MATHPROG {
	version MATHVERBS {
		float MeshTimh(data) = 1;
		result1 MaxMin(data) = 2;
		new_array PowerArray(argument1) = 3;
	} = 1;
} = 0x20000008;
