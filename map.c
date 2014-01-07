#include "map.h"
#include <stdio.h>
#include <stdlib.h>
#include <limits.h>


#define NUM_DELIM ','
#define ROW_DELIM '\n'

int get_num();
map_t *new_map(int side);
pt_t * new_pt(int x, int y);
int chk_pt(int x, int y, map_t *map);
void add_pt(pt_t *last,int x, int y);
void fill(map_t *map, int level);
void flatten(map_t *map, int level);
pt_t *find_viable(map_t *map);
int chk_viable(map_t *map, int x, int y);

void free_map(map_t *map){
	int side;
	int i;

	side = map->side;
	
	for(i=0; i < side; i++){
		free(map->matrix[i]);
	}
	free(map);

}

pt_t *get_last(pt_t *head){
	pt_t *current;
	current = head;
	while(current->next != NULL){
		current = current->next;
	}
	return current;
}

void print_best(pt_t *head){
	int best=0;
	pt_t *bestp;
	pt_t *current;
	current = head;

	best = current->val;
	bestp = current;

	while(current->next != NULL){
		current = current->next;
		if (best > current->val){
		       	best = current->val;
			bestp = current;
		}
	}
	printf("\nBEST: (%d,%d,%d,%d)\n",bestp->x,bestp->y,bestp->val,bestp->dir);
}

void set_level_dir(pt_t *head, int level, int dir){
	
	pt_t *current;
	current = head;
	do{
		current->val = level;
		current->dir = dir;
		current = current->next;

	}while(current->next != NULL);

	current->val = level;
	current->dir = dir;

	
}

pt_t *get_vlist(map_t *map){
	int mmax,mmin;
	int level;
	map_t *mapc;
	pt_t *lhead;
	pt_t *tail;
	pt_t *newseg;

	lhead = calloc(1,sizeof(pt_t));
	
	tail = lhead;

	mmax = map_max(map);
	mmin = map_min(map);
	
	cp_map(&mapc,map);

	for(level = mmin-1; level < mmax; level++){
		printf("Level %d going up.\n",level);
		fill(mapc,level);
		newseg = find_viable(map);
		if(newseg != NULL){
			set_level_dir(newseg,level,1);
			tail->next = newseg;
			tail = get_last(newseg);
		}
	}

	free(mapc);
	cp_map(&mapc,map);

	for(level = mmax+1; level > mmin; level--){

		printf("Level %d going down.\n",level);
		flatten(mapc,level);
		newseg = find_viable(map);
		if(newseg != NULL){
			set_level_dir(newseg,level,-1);
			tail->next = newseg;
			tail = get_last(newseg);
		}
	}
	return lhead->next;

}


int chk_viable(map_t *map, int x, int y){

	int w,h;
	int i,j;
	int side;
	int level;
	int **mat;

	w = map->c_width;
	h = map->c_height;
	side = map->side;

	mat = map->matrix;

	level = mat[x][y];

	for(i=0; i < w; i++){
		for(j=0; j < h; j++){
			
			if(i > side -1) i = i-side-2;
			if(j > side -1) j = j-side-2;

			if(mat[i][j] != level) return 0;

		}
	}
	printf("Found viable at (%d,%d,%d)\n",x,y,level);
	return 1;

}

pt_t *find_viable(map_t *map){
	int i,j;
	int side;
	pt_t *lhead;
	pt_t *last;

	lhead = calloc(1,sizeof(pt_t));

	last = lhead;

	side = map->side;

	for(i=0; i < side; i++){
		for(j=0; j < side; j++){
			if(chk_viable(map,i,j)) add_pt(last,i,j);
		}
	}

	return lhead->next;

}

void fill(map_t *map, int level){
	int i,j;
	int side;
	int **mat;

	side = map->side;
	mat = map->matrix;

	for(i=0; i<side; i++){
		for(j=0; j<side; j++){
			if(mat[i][j] < level) mat[i][j] = level;
		}
	}


}

void flatten(map_t *map, int level){

	int i,j;
	int side;
	int **mat;

	side = map->side;
	mat = map->matrix;

	for(i=0; i<side; i++){
		for(j=0; j<side; j++){
			if(mat[i][j] > level) mat[i][j] = level;
		}
	}

}

int map_max(map_t *map){
	
	int i,j;
	int max = 0;
	int current;

	for(i=0; i < map->side; i++){
		for(j=0; j < map->side; j++){
			current = map->matrix[i][j];
			if(current > max) max = current;
		}
	}

	return max;
}

int map_min(map_t *map){

	int i,j;
	int min = 10000;
	int current;

	for(i=0; i < map->side; i++){
		for(j=0; j < map->side; j++){
			current = map->matrix[i][j];
			if(current < min) min = current;
		}
	}

	return min;
}

void cp_map(map_t **dest, map_t *src){
	int side;
	int i,j;

	side = src->side;

	*dest = new_map(side);

	(*dest)->side = side;
	(*dest)->c_width = src->c_width;
	(*dest)->c_height = src->c_height;

	for(i=0; i < side; i++){
		for(j=0; j < side; j++){
			(*dest)->matrix[i][j] = src->matrix[i][j];
		}
	}

}


void print_map(map_t *map){
	int i,j;
	for(i=0; i < map->side; i++){
		for(j=0; j < map->side; j++){
			printf("%d",map->matrix[i][j]);
			if(j != map->side-1) printf(",");


		}
		printf("\n");
	}	

}

void load_map(map_t **map,int side){
	int i,j;
	int num;
	*map = new_map(side);
	printf("Loading map data from stdin.\n");
	for(i=0;i<side;i++){
		for(j=0;j<side;j++){
			num = get_num();
			//printf("loaded (%d,%d):\t%d\n",i,j,num);
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
//		printf("%c",temp);
		switch(temp){
			case(NUM_DELIM):
				string[count] = '\0';
				//printf("%s\n",string);
				num = strtol(&string[1],NULL,10);
				return num;
				break;
			case(ROW_DELIM):
				string[count] = '\0';
				num = strtol(&string[1],NULL,10);
				return num;
				break;
			default:
				string[count]=temp;
				break;

		}

	}

}

struct extr_l *link_extrema(map_t *map){
	int i,j;
	int type;
	pt_t *max_h, *min_h;
	pt_t *last_max,*last_min;

	max_h = calloc(1,sizeof(pt_t));
	min_h = calloc(1,sizeof(pt_t));
	
	last_max = max_h;
	last_min = min_h;

	struct extr_l *newlist;

	newlist = calloc(1,sizeof(struct extr_l));
	
	for(i=1; i < map->side-1; i++){
		for(j=1; j < map->side-1; j++){
			type = chk_pt(i,j,map);


			switch(type){
				case(1):
//					printf("Max: (%d,%d)\n",i,j);
					add_pt(last_max,i,j);
					break;
				case(0):
//					printf("MaxMin: (%d,%d)\n",i,j);
					add_pt(last_max,i,j);
					add_pt(last_min,i,j);
					break;
				case(-1):
//					printf("MaxMin: (%d,%d)\n",i,j);
					add_pt(last_min,i,j);
					break;
				default:
					printf("Error: chk_pt returned strange things at (%d,%d)\n",i,j);
					break;

			}			
			
		}
	}
	
	newlist->maxima = max_h->next;
	newlist->minima = min_h->next;

	return newlist;
	
}

void add_pt(pt_t *last,int x, int y){
	pt_t *newpt;
	newpt = malloc(sizeof(pt_t));
	newpt->x = x;
	newpt->y = y;
	last->next = newpt;

}

int chk_pt(int x, int y, map_t *map){
	int i,j;
	int min,max;
	int val;

	val = map->matrix[x][y];

	min = 100000;
	max = 0;

	for(i=x-1; i < x+2; i++){
		for(j=y-1; j < y+2; j++){
			
			if(i < 0) i = map->side -1;
			if(j < 0) j = map->side -1;
			if(i > map->side -1) i = 0;
			if(j > map->side -1) j = 0;

			if(map->matrix[i][j] > max) max = map->matrix[i][j];
			if(map->matrix[i][j] < min) min = map->matrix[i][j];

		}
	}

	if(val >= max) return 1;
	if(val <= min) return -1;
	if(val == max && val == min) return 0;
	
	return 0;
}

pt_t * new_pt(int x, int y){

	pt_t *newpt;

	newpt = calloc(1,sizeof(pt_t));
	newpt->x = x;
	newpt->y = y;
	
	return newpt;

}
