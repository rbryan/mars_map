#include "map.h"
#include <stdio.h>
#include <stdlib.h>

/**
 * Makes the count of a map add up to the count of the original.
 * @param	omap	original map
 * @param	nmap	new map
 */
void construct(map_t *omap, map_t *nmap){
	int i,j;
	int side;
	int **omat,**nmat;
	int **tmat;
	map_t *tmap;
	int x,y,x1,y1;
	long int diff;

	if(omap->count==0) count(omap);
	if(nmap->count==0) count(nmap);

	diff = nmap->count - omap->count;

	x = nmap->x;
	y = nmap->y;
	
	x1 = x+nmap->c_width-1;
	y1 = y+nmap->c_height-1;

	side = omap->side;

	omat = omap->matrix;
	nmat = nmap->matrix;

	tmap = new_map(side);

	tmat = tmap->matrix;
	if(x1 < 0){ 
		x1 = side+x1%side;
	}else{
		x1 = x1 % side;
	}
	if(y1 < 0){ 
		y1 = side+y1%side;
	}else{
		y1 = y1 % side;
	}

	for(i=0; i<side; i++){
		for(j=0; j<side; j++){

			if(!(i <= x1 && i >= x && j <= y1 && j >= y)){
				tmat[i][j] = (nmat[i][j]-omat[i][j]);
			}
		}
	}
	
	while(diff > 0){
		for(i=0; i<side; i++){
			for(j=0; j<side; j++){
				if(!(i <= x1 && i >= x && j <= y1 && j >= y)){
					if(tmat[i][j]==0){
						nmat[i][j]--;
						if(chk_slope(nmap,i,j)==0){
							nmat[i][j]++;
						}else{
							diff--;
							if(diff==0)break;
						}	
						if(diff==0)break;
					}
					if(diff==0)break;
				}
				if(diff==0)break;
		
			}
			if(diff==0)break;
		}
	}


	while(diff < 0){
		for(i=0; i<side; i++){
			for(j=0; j<side; j++){
				if(!(i <= x1 && i >= x && j <= y1 && j >= y)){
					if(tmat[i][j]==0){
						nmat[i][j]++;
						if(chk_slope(nmap,i,j)==0){
							nmat[i][j]--;

						}else{
							diff++;
							if(diff==0)break;
						}	
						if(diff==0)break;
					}
					if(diff==0)break;
				}
				if(diff==0)break;

			}
			if(diff==0)break;
		}
	}
	free_map(tmap);	

}
