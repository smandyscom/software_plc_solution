//---------------------------------------------------------
// FILENAME				: LDE_LIB0.h
// ORIGIN AYTHOR		: Hsien , 2012.09.26
// EDITOR				: Hsien
// NOTE					: Methods for Ladder-Diagram -> Structured Text Interpreting , expression part
//							including:	1. collect vertices from LD-array , which mixed with vertices and route descriptor
//										2. find-out relationships(so-called edge) in every pairs of vertices
//---------------------------------------------------------
#ifndef _LDE_LIB0_H
#define _LDE_LIB0_H
#include<STG_ERRHDL.h>
#include<LDE_ELEMENT.h>

struct edge_st{
	VERTEX_SN	from;
	VERTEX_SN	to;
};

typedef vector<vector<ldElement>>	LD_ARRAY	;
typedef vector<ldElement>			LD_ROW		;
typedef vector<vertice>				LD_DEVICE_LIST			;
typedef vector<edge_st>				EDGE_STREAM			;

//----------------------------------------------------------
//	warning: doing searchVertices before doing ld2EdgeStream
//----------------------------------------------------------
int collectVertices	(LD_ARRAY &sourceArray,LD_DEVICE_LIST &buffer);		// find out all vertices and store them into buf
																		// mark their's serial number simaltanously
int ld2EdgeStream	(LD_ARRAY &sourceArray,EDGE_STREAM &edgeStream);	// derive edge stream from ld array

#endif