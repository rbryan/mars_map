#include "map.h"
#include <stdio.h>
#include <stdlib.h>


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

	for(i=0; i<side; i++){
		for(j=0; j<side; j++){
			if(!(i <= x1 && i >= x && j <= y1 && j >= y)){
				tmat[i][j] = (nmat[i][j]-omat[i][j]);
			}
		}
	}
	if(diff > 0){
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
							}	
						}
					}
			
				}
			}
		}
	}

	if(diff < 0){
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
							}	
						}",omap->count,nmap->count);	
					}
			
				}
			}
		}
	}
	/////////////
	//	REMOVE FOR SPEED!!! 
	/////////////
	if(count(nmap) != omap->count)printf("Miscount:\n%ld:%ld\nDiff:\t%ld",omap->count,nmap->count,diff);	
	free_map(tmap);	

}
