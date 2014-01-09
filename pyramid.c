#include <stdio.h>
#include <stdlib.h>
#include "map.h"

#define MIN(X,Y) ((X < Y) ? (X) : (Y))
#define MAX(X,Y) ((X > Y) ? (X) : (Y))

inline void pyramid(map_t *map, int h, int x, int y){
	int z,zc;
	int tx,ty;
	int bx,by;
	int i,j;

	tx = x-abs(h);
	ty = y-abs(h);

	bx = x+abs(h);
	by = y+abs(h);
	

	for(i=tx; i < bx; i++){
		for(j=ty; j < by; j++){
			if(h >= 0){
				z = h-MAX(2*abs(x-i),2*abs(y-j));
				zc = get_val(map,i,j);
				z = MAX(z,zc);
			}else{
				z = 255-(abs(h)-MAX(2*abs(x-i),2*abs(y-j)));
				zc = get_val(map,i,j);
				z = MIN(z,zc);
			}
			set_val(map,z,i,j);			
		}
	}

}



