#include <stdio.h>
#include <stdlib.h>
#include "map.h"

#define MIN(X,Y) ((X < Y) ? (X) : (Y))
#define MAX(X,Y) ((X > Y) ? (X) : (Y))

/**
 * Constructs a plateau with a top just the right size for a court.
 * The plateau can be recessed down into the ground or protruding up.
 * @param	map	the map to build on
 * @param	h	the hight of the desired court
 * @param	x	the x position
 * @param	y	the y position
 * @param	x1	the x position of the corner across from (x,y)
 * @param	y1	the y position of the corner across from (x,y)
 */
inline void pyramid(map_t *map, int h, int x, int y, int x1, int y1){
	int z,zc;
	int tx,ty;
	int bx,by;
	int i,j;

	z=0;
	zc=0;
	
	tx = x-h;
	ty = y-h;

	bx = x1+h;
	by = y1+h;
	
	x1 = wrap(x1,map->side);
	y1 = wrap(y1,map->side);
		

	for(i=tx; i < bx; i++){
		for(j=ty; j < by; j++){
			if(i >= x && i <= x1 && j >= y && j <= y1){
				z = h;
			}else if(i<x && j<y){
				z = h-2*(abs(x-i)+abs(y-j));
			}else if(i>x1 && j<y){
				z = h-2*(abs(x1-i)+abs(y-j));
			}else if(i > x1 && j > y1){
				z = h-2*(abs(x1-i)+abs(y1-j));
			}else if(i < x && j > y1){
				z = h-2*(abs(x-i)+abs(y1-j));
			}else if(j < y && i >= x && i <= x1){
				z = h-2*abs(y-j);
			}else if(i > x1 && j >= y && j <= y1){
				z = h-2*abs(x1-i);
			}else if(j > y1 && i <=x1 && i >= x){
				z = h-2*abs(y1-j);
			}else if(i < x && j >= y && j <= y1){
				z = h-2*abs(x-i);
			}
			
			zc = get_val(map,i,j);


			if(z < zc){
				z += 2*(h-z);
				z = MIN(z,zc);	
			}


			set_val(map,z,i,j);			
		}
	}

}



