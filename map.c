#include <Imlib2.h>
#include "map.h"
#include <stdio.h>
#include <stdlib.h>

#define NUM_DELIM ','
#define ROW_DELIM '\n'

int get_num();
map_t *new_map(int side);

void print_map(map_t *map){
	

}
void load_map(map_t **map,int side){
	int i=0,j=0;
	int num;
	*map = new_map(side);
	printf("Loading map data from stdin.\n");
	for(;i<side;i++){
		for(;j<side;j++){
			num = get_num();
			if(num < 0 && (i==(side-1) || j==(side-1))){
				printf("Problem!!!: Hit edge too early!\n");
				num = -num;
			}	
			printf("loaded (%d,%d):\t%d\n",i,j,num);
			(*map)->matrix[i][j] = num;

		}
	}

}

map_t *new_map(int side){
	printf("Making new map struct.\n");
	int i;
	map_t *newmap;
	newmap = calloc(1,sizeof(map_t));
	newmap->matrix = calloc(side,sizeof(map_t*));
	for(i=0;i<side;i++){
		newmap->matrix[i] = calloc(side,sizeof(int));
	}
	newmap->side = side;
	return newmap;

}

int get_num(){
	char string[5];
	char temp;
	int num;
	int count=0;
	while(1){
		count++;
		temp = (char)getchar();
		switch(temp){
			case(NUM_DELIM):
				string[count+1] = '\0';
				num = atoi(string);
				return num;
				break;
			case(ROW_DELIM):
				string[count+1] = '\0';
				num = -atoi(string);
				return num;
				break;
			default:
				string[count]=temp;
				break;

		}

	}
}
