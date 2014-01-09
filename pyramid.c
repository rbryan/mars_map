#include <stdio.h>
#include <stdlib.h>
#include "map.h"

#define MIN(X,Y) ((X < Y) ? (X) : (Y))
#define MAX(X,Y) ((X > Y) ? (X) : (Y))

inline void pyramid(map_t *map, int h, int x, int y, int x1, int y1){
	int z,zc;
	int tx,ty;
	int bx,by;
	int i,j;
	
	tx = x-h;
	ty = y-h;

	bx = x1+h;
	by = y1+h;
	

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



