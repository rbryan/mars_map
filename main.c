#include <stdio.h>
#include <stdlib.h>
#include "map.h"

int main( int argc, char **argv){
	
	int c_height,c_width,m_side;
	map_t *map;

	if(argc < 4){
		printf("Usage: mars <map side len> <court width> <court height>\n");
	}else{
		c_height = atoi(argv[3]);
		c_width = atoi(argv[2]);
		m_side = atoi(argv[1]);
	}
	printf("here\n");	
	load_map(&map, m_side);

	exit(1);

	

}
