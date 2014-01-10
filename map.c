#include "map.h"
#include "pyramid.h"
#include "construct.h"
#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <Imlib2.h>
#include <pthread.h>


#define MIN(X,Y)	((X < Y) ? (X) : (Y))
#define MAX(X,Y)	((X > Y) ? (X) : (Y))

#define NUM_DELIM ','
#define ROW_DELIM '\n'

#define NUM_THREADS	10	

int get_num();
map_t *new_map(int side);
pt_t * new_pt(int x, int y);
int chk_pt(int x, int y, map_t *map);
void add_pt(pt_t *last,int x, int y);
void fill(map_t *map, int level);
void flatten(map_t *map, int level);
int chk_viable(map_t *map, int x, int y);
void free_map(map_t *map);

void print_pt(pt_t *pt){
	printf("(%d,%d,%d,%ld)\n",pt->x,pt->y,pt->val, pt->cost);

}

long int cost(map_t *orig_m, map_t *new_m){
	int side;
	int **omat,**nmat;
	int i,j;
	long int sink,raise;
	int old,new;

	side = orig_m->side;

	sink=0;
	raise=0;

	omat = orig_m->matrix;
	nmat = new_m->matrix;

	for(i=0; i<side; i++){
		for(j=0; j<side; j++){
			old = omat[i][j];
			new = nmat[i][j];
			if(old > new){
				sink++;
			}else if(new > old){
				raise++;
			}
		}
	}

	new_m->cost = MAX(sink,raise);

	return new_m->cost;
}

void mk_map_img(map_t *map){
	int i,j;
	Imlib_Image image;
	int side;
	char name[80];
	char text[80];
	int **mat;
	int val;

	mat = map->matrix;

	side = map->side;

	sprintf(text,"Cost:\t%ld\nPos:\t(%d,%d,%d)",map->cost,map->x,map->y,map->h);
	sprintf(name,"images/%ld_%d_%d_%d.jpg",map->cost,map->x,map->y,map->h);

	image = imlib_create_image(side,side);

	imlib_context_set_image(image);

	for(i=0; i < side; i++){
		for(j=0; j < side; j++){
			val = mat[i][j];
			imlib_context_set_color(val,val,val,255);
			imlib_image_draw_pixel(i,j,0);	
		}
	}
	
	imlib_save_image(name);
	imlib_free_image();

}

long int count(map_t *map){
	int side;
	register int i,j;
	int **mat;
	register int count;

	side = map->side;
	mat = map->matrix;

	count = 0;

	for(i=0; i<side; i++){
		for(j=0; j<side; j++){
			count += mat[i][j];
		}
	}
	map->count = count;
	return count;

}

int chk_slope(map_t *map, int x, int y){
	int val;
	int ref;

	ref = map->matrix[x][y];

	val = get_val(map,x,y+1);

	if(abs(val-ref) > 2){
		return 0;
	}

	val = get_val(map,x+1,y);

	if(abs(val-ref) > 2){
		return 0;
	}
	
	val = get_val(map,x,y-1);

	if(abs(val-ref) > 2){
		return 0;
	}

	val = get_val(map,x-1,y);

	if(abs(val-ref) > 2){
		return 0;
	}

	return 1;

}

map_t *trim(map_t *omap, map_t *nmap){
	int i,j;
	int side;
	int **omat,**nmat;
	int **tmat;
	map_t *tmap;
	int x,y,x1,y1;
	int notrim=0;

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
	while(notrim==0){
		notrim=1;
		for(i=0; i<side; i++){
			for(j=0; j<side; j++){
				if(tmat[i][j] > 0){
					nmat[i][j]--;
					if(chk_slope(nmap,i,j) == 0){
						nmat[i][j]++;
					}else if(notrim==1){
						notrim=0;
					}
				}else if(tmat[i][j] < 0){
					nmat[i][j]++;
					if(chk_slope(nmap,i,j)==0){
						nmat[i][j]--;
					}else if(notrim==1){
						notrim=0;
					}
				}
			}
		}
	}
	
	free_map(tmap);
	
	return nmap;

}


inline map_t *test_pos(map_t *map, int h, int x, int y){
	map_t *temp;
		
	cp_map(&temp,map);

	pyramid(temp,h,x,y,x+map->c_width-1,y+map->c_height-1);

	trim(map,temp);

	return temp;

}

int find_c_min(map_t *map){
	int i,j;
	int x,y;
	int **mat;
	int cw,ch;
	int xm,ym;
	register int low;

	mat = map->matrix;

	x = map->x;
	y = map->y;
	
	cw = map->c_width;
	ch = map->c_height;

	xm = x + cw;
	ym = y + ch;

	low = INT_MAX;

	for(i=x; i < xm; i++){
		for(j=y; j < ym; j++){
			if(mat[i][j] < low){
				low = mat[i][j];
			}	
		}
	}

	map->c_min = low;

	return low;

}

int find_c_max(map_t *map){
	int i,j;
	int x,y;
	register int **mat;
	int cw,ch;
	int xm,ym;
	register int high;

	mat = map->matrix;

	x = map->x;
	y = map->y;
	
	cw = map->c_width;
	ch = map->c_height;

	xm = x + cw;
	ym = y + ch;

	high = 0;

	for(i=x; i < xm; i++){
		for(j=y; j < ym; j++){
			if(mat[i][j] > high){
				high = mat[i][j];
			}	
		}
	}

	map->c_min = high;

	return high;

}

void *process_pixel( void *data){
	int i,j,k;
	int c_min,c_max;
	char *status;
	map_t *current;
	map_t *map;
	map_t **best;
	pthread_mutex_t *lock;

	map = ((thread_data *)data)->map;
	best = ((thread_data *)data)->best;
	lock = ((thread_data *)data)->lock;
	i = ((thread_data *)data)->i;
	j = ((thread_data *)data)->j;
	status = ((thread_data *)data)->status;
	

	map->x=i;
	map->y=j;
	c_min = find_c_min(map);
	c_max = find_c_max(map);
	for(k=c_min; k<c_max; k++){
		current = test_pos(map,k,i,j);
		construct(map,current);
		//if(chk_viable(current,current->x,current->y)==0) printf("Failed!\n");
		current->cost=cost(map,current);
		if(current->cost < (*best)->cost){
			
			pthread_mutex_lock(lock);

			free_map(*best);
			(*best) = current;
			(*best)->x = i;
			(*best)->y = j;
			(*best)->h = k;
			mk_map_img(*best);

			pthread_mutex_unlock(lock);
			//print_map(best);
			fprintf(stderr,"\n\nNEW BEST!!!:\n\tCost:\t%ld\n\tPos:\t (%d,%d,%d)\n\n",(*best)->cost,(*best)->x,(*best)->y,(*best)->h);
		}else{
			free_map(current);
		}
	}
	
	*status='\0';
	return NULL;

}

void find_best(map_t *map){
	int i,j,l;
	int side;
	thread_data *data;
	pthread_t *threads;
	char *thread_status;
	pthread_mutex_t lock;
	map_t *best;
		
	pthread_mutex_init(&lock,PTHREAD_MUTEX_NORMAL);
	
	best = calloc(1,sizeof(map_t*));
	thread_status = calloc(NUM_THREADS,1);
	threads = calloc(NUM_THREADS,sizeof(pthread_t));
	data = calloc(NUM_THREADS,sizeof(thread_data));

	side = map->side;

	best = new_map(map->side);
	best->cost = LONG_MAX;


	for(i=0; i<side; i++){
		for(j=0; j<side;){
			for(l=0;l<NUM_THREADS;l++){
				if(thread_status[l] == '\0'){
				
					fprintf(stderr,"\rCurrent Pixel\t(%d,%d).\t%f%% done.",i,j,(1.0*i*map->side+j)/(map->side*map->side));
					thread_status[l] = 'a';
					data[l].i = i;
					data[l].j = j;
					data[l].status = &(thread_status[l]);
					data[l].map = map;
					data[l].best = &best;
					data[l].lock = &lock;
					pthread_create(&threads[l],NULL,process_pixel,&(data[l]));
					pthread_detach(threads[l]);
					j++;
				}

				l++;

			}		
					
		}
	}
	
	
	while(1){
		for(i=0; i<NUM_THREADS; ){
			if(threads[i] != 0){
				i++;
			}
		}
	}	

	pthread_mutex_destroy(&lock);
	free(thread_status);
	free(threads);
	free(data);
	free(best);

}

inline int get_val(map_t *map, int x, int y){
	
	int side;

	
	side = map->side;

	if(x < 0){ 
		x = -x%side;
	}else{
		x = x % side;
	}
	if(y < 0){ 
		y = -y%side;
	}else{
		y = y % side;
	}
	return map->matrix[x][y];

}

inline void set_val(map_t *map, int val, int x, int y){

	int side;
	
	side = map->side;
	

	if(x < 0){ 
		x = -x%side;
	}else{
		x = x % side;
	}
	if(y < 0){ 
		y = -y%side;
	}else{
		y = y % side;
	}
	
	map->matrix[x][y] = val;
	

}

void free_map(map_t *map){
	int side;
	int i;
	int **mat;

	mat = map->matrix;
	side = map->side;
	
	for(i=0; i < side; i++){
		free(mat[i]);
	}
	free(mat);
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
	printf("\nBEST: (%d,%d,%d,%ld)\n",bestp->x,bestp->y,bestp->val,bestp->cost);
}


int chk_viable(map_t *map, int x, int y){

	int x1,y1;
	int i,j;
	int side;
	int level;
	int **mat;

	x1 = x+map->c_width;
	y1 = y+map->c_height;
	side = map->side;

	mat = map->matrix;

	level = mat[x][y];

	for(i=x; i < x1; i++){
		for(j=y; j < y1; j++){
			
			if(i >= side){
				i = i % side;
			}
		
			if(j >= side){
				j = j % side;
			}
	
			if(mat[i][j] != level) return 0;

		}
	}
	printf("Found viable at (%d,%d,%d)\n",x,y,level);
	return 1;

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
	(*dest)->x = src->x;
	(*dest)->y = src->y;

	for(i=0; i < side; i++){
		for(j=0; j < side; j++){
			(*dest)->matrix[i][j] = src->matrix[i][j];
		}
	}

}


void print_map(map_t *map){
	int i,j;
	printf("\n\nCost:\t%ld\nPos:\t(%d,%d,%d)\n\n",map->cost,map->x,map->y,map->h);
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
	
	//printf("Making new map struct.\n");
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
