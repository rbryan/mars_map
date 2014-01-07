#include <stdio.h>
#include <stdlib.h>
#include "map.h"

int main( int argc, char **argv){
	
	int c_height,c_width,m_side;
	map_t *map;
	int max,min;
	pt_t *list;

	struct extr_l *extrema;

	if(argc < 4){
		printf("Usage: mars <map side len> <court width> <court height>\n");
	}else{
		c_height = atoi(argv[3]);
		c_width = atoi(argv[2]);
		m_side = atoi(argv[1]);
	}
	printf("Using:\n\tMap Size %dx%d\n\tCourt size:%dx%d", m_side,m_side,c_height,c_width);	
	load_map(&map, m_side);
	map->c_width = c_width;
	map->c_height = c_height;
	printf("Linking Extrema...\n");
	extrema = link_extrema(map);
	get_vlist(map);
	exit(1);

	

}
