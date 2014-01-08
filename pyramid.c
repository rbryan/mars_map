#include <stdio.h>
#include <stdlib.h>
#include "map.h"

#define MIN(X,Y) ((X < Y) ? (X) : (Y))
#define MAX(X,Y) ((X > Y) ? (X) : (Y))

void pyramid(map_t *map, int h, int x, int y){
	int z,zc;
	int tx,ty;
	int bx,by;
	int i,j;

	tx = x-h;
	ty = y-h;

	bx = x+h;
	by = y+h;

	for(i=tx; i < bx; i++){
		for(j=ty; j < by; j++){
			
			z = MAX(x-i,y-j);
			zc = get_val(map,i,j);
			z = MAX(z,zc);
			set_val(map,z,i,j);			
		}
	}

}



