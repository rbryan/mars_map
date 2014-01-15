#include <stdio.h>
#include <stdlib.h>
#include "map.h"
#include "pyramid.h"

int main( int argc, char **argv){
	
	int c_height,c_width;
	map_t *map;

	map = NULL;
	
	c_height = 0;
	c_width = 0;

	if(argc < 4){
		printf("Usage: mars <map> <court width> <court height> \n");
	}else{
		c_height = atoi(argv[3]);
		c_width = atoi(argv[2]);
		map = ld_map_img(argv[1]);
	}
	
	map->c_width = c_width;
	map->c_height = c_height;
	map->min = map_min(map);
	map->max = map_max(map);
	map->count = count(map);	

	/*if(chk_slope_map(map)){
		printf("Yay, it's right!\n");
	}*/
	int k;	
	map_t *best;
	map_t *current;
	best = new_map(map->side);
	best->cost = 1000000;
	//find_best(map);
	
	for(k=1; k<256; k++){
		current = test_pos(map,k,13,509);
		current->cost = cost(map,current);
		printf("%ld\t%d\n",current->cost,k);
		if(current->cost < best->cost){
			free_map(best);
			best = current;
			best->cost = cost(map,best);
			best->h = k;
		}
		
	}
	printf("%ld (%d,%d,%d)\n",best->cost,best->x,best->y,best->h);
	
	exit(1);

	

}
