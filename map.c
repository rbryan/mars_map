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

#define NUM_THREADS	4	

int get_num();
map_t *new_map(int side);
void fill(map_t *map, int level);
void flatten(map_t *map, int level);
int chk_viable(map_t *map, int x, int y);
void free_map(map_t *map);

/**
 * This function takes a file name and attempts to load 
 * that file as a map and return a pointer to said map.
 * @param name	This is the name of the file to be loaded.
 * @return 	Returns a pointer to the loaded map.
 */
map_t *ld_map_img(const char *name){
	int w,h;
	int side;
	int i,j;
	Imlib_Color col;

	map_t *newmap;

	Imlib_Image image;

	image = imlib_load_image(name);
	imlib_context_set_image(image);

	w = imlib_image_get_width();
	h = imlib_image_get_height();

	side = MIN(w,h); 

	newmap = new_map(side);

	for(i=0; i<side; i++){
		for(j=0; j<side; j++){
			imlib_image_query_pixel(i,j,&col);
			newmap->matrix[i][j] = (int) col.red;
		}
	}
	return newmap;


}
/**
 * This funciton takes in a pointer to the original map
 * and a pointer to the new map. It then computes the cost
 * of transforming the old into the new returning the cost
 * as a long int.
 * @param	orig_m	original map
 * @param	new_m	new map
 * @return 	the cost to build the map	
 */
long int cost(map_t *orig_m, map_t *new_m){
	int side;
	int **omat,**nmat;
	int i,j;
	long int cost;

	side = orig_m->side;

	cost = 0;

	omat = orig_m->matrix;
	nmat = new_m->matrix;

	for(i=0; i<side; i++){
		for(j=0; j<side; j++){
			cost += abs(nmat[i][j]-omat[i][j]);			
		}
	}

	new_m->cost = cost;

	return cost;
}
/**
 * This function takes in a pointer to a map and outputs the
 * map as a .png. The file name is in the format
 *  <Cost>_<x>_<y>_<h>.png
 *  @param	map	the map to output as an image
 */
void mk_map_img(map_t *map){
	int i,j;
	Imlib_Image image;
	int side;
	char name[80];
	int **mat;
	int val;

	mat = map->matrix;

	side = map->side;

	sprintf(name,"%ld_%d_%d_%d.png",map->cost,map->x,map->y,map->h);

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
/**
 * This function takes in a pointer to a map and counts the 
 * number of dirt particles in the entire map. The count is 
 * returned as a long int.
 * @param	map	the map to count
 * @return	the count of the map
 */
long int count(map_t *map){
	register int side;
	register int i,j;
	register int **mat;
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
/**
 * This function takes a pointer to a map, and x and y coordinates.
 * The function returns 1 if the slope at that position on the map
 * matches the rules and 0 if not.
 * @param	map	the map to check the point on
 * @param	x	the x coordinate of the point
 * @param	y	the y coordinate of the point
 * @return	1 if valid 0 if not
 */
inline int chk_slope(map_t *map, int x, int y){
	register int val;
	register int ref;

	ref = get_val(map,x,y);

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

int chk_slope_map(map_t *map){
	int i,j;
	int side;

	side = map->side;

	for(i=0; i<side; i++){
		for(j=0; j<side; j++){
			if(chk_slope(map,i,j)){
				continue;
			}else{
				return 0;
			}
	
		}
		
	}

	return 1;


}
/**
 * This function takes a pointer to the original map and a pointer to
 * the transformed map. It returns a map that has been trimmed of all 
 * unneeded changes.
 * @param	omap	original map
 * @param	nmap	new map
 * @return	trimmed map
 */

map_t *trim(map_t *omap, map_t *nmap){
	int i,j;
	int side;
	int **omat,**nmat;
	int **tmat;
	map_t *tmap;
	int x,y,x1,y1;
	int no_trim=0;

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
	while(no_trim==0){
		no_trim=1;
		for(i=0; i<side; i++){
			for(j=0; j<side; j++){
				if(tmat[i][j] > 0){
					nmat[i][j]--;
					if(chk_slope(nmap,i,j) == 0){
						nmat[i][j]++;
					}else if(no_trim==1){
						no_trim=0;
					}
				}else if(tmat[i][j] < 0){
					nmat[i][j]++;
					if(chk_slope(nmap,i,j)==0){
						nmat[i][j]--;
					}else if(no_trim==1){
						no_trim=0;
					}
				}
			}
		}
	}
	
	free_map(tmap);
	
	return nmap;

}

/**
 * This function takes a pointer to the original input map and three position
 * coordinates. It processes a position (x,y,h) on the map.
 * @param	map	the map to test the position on
 * @param	h	the height coordinate of the position
 * @param	x	the x coordinate of the position
 * @param	y	the y coordinate of the position
 * @return 	the map after the court is built at (x,y,h)
 */
inline map_t *test_pos(map_t *map, int h, int x, int y){
	map_t *temp;
		
	cp_map(&temp,map);

	temp->x = x;
	temp->y = y;

	pyramid(temp,h,x,y,x+map->c_width-1,y+map->c_height-1);

	trim(map,temp);
	construct(map,temp);

	return temp;

}
/**
 * This function takes a pointer to a map and a position.
 * It finds the minimum value within the area of a court placed
 * at the position defined by x and y.
 * @param 	map	the map on which to find the min
 * @param	x	the x coordinate of the court position
 * @param	y	the y coordinate of the court position
 * @return	the minimum value
 */
int find_c_min(map_t *map,int x, int y){
	int i,j;
	int **mat;
	int xm,ym;
	register int low;

	mat = map->matrix;

	xm = x + map->c_width - 1;
	ym = y + map->c_height - 1;

	xm = wrap(xm,map->side);
	ym = wrap(ym,map->side);

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

/**
 * This function takes a pointer to a map to process along with
 * an x and y coordinate of a position to process. It returns the
 * maximum hight within an area bounded by a court at that position.
 * @param	map	the map to process
 * @param	x	the x position
 * @param	y	the y position
 * @return	the max value
 */

int find_c_max(map_t *map,int x, int y){
	int i,j;
	register int **mat;
	int xm,ym;
	register int high;

	mat = map->matrix;

	
	xm = x + map->c_width - 1;
	ym = y + map->c_height - 1;

	xm = wrap(xm,map->side);
	ym = wrap(ym,map->side);

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
/**
 * This function is called by create_pthread(). It processes
 * every height on a pixel between c_max and c_min.
 * @param	data	a void pointer to the data structure.
 * @return	a void pointer that is never used.
 */
void *process_pixel( void *data){
	int k;
	int c_min,c_max;
	char *status;
	map_t *current;
	map_t *map;
	map_t **best;
	pthread_mutex_t *lock;

	map = ((thread_data *)data)->map;
	best = ((thread_data *)data)->best;
	lock = ((thread_data *)data)->lock;
	const int i = ((thread_data *)data)->i;
	const int j = ((thread_data *)data)->j;
	status = ((thread_data *)data)->status;
	

	c_min = find_c_min(map,i,j);
	c_max = find_c_max(map,i,j);
	for(k=c_min; k<c_max; k++){
		current = test_pos(map,k,i,j);
		//if(chk_viable(current,current->x,current->y)==0) {
		//	fprintf(stderr,"Failed!\n");
		//}
		current->cost=cost(map,current);
		
		pthread_mutex_lock(lock);
		if(current->cost < (*best)->cost){
			

			free_map(*best);
			(*best) = current;
			(*best)->x = i;
			(*best)->y = j;
			(*best)->h = k;
			mk_map_img(*best);
				
			fprintf(stderr,"\n\nBEST:\n\tCost:\t%ld\n\tPos:\t (%d,%d,%d)\n\n",(*best)->cost,(*best)->x,(*best)->y,(*best)->h);
		}else{
			free_map(current);
		}
		pthread_mutex_unlock(lock);
		

	}
	
	*status='\0';
	return NULL;

}
/**
 *Finds the best position on a map.
 *@param	map	map to process
 */
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
				
					fprintf(stderr,"\rCurrent Pixel\t(%d,%d)",i,j);
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
					if(j==side) break;
				}

			}		
					
		}
	}
	
	
	for(i=0; i<NUM_THREADS; ){
		if(threads[i] != 0){
			i++;
		}
	}

	if(chk_viable(best,best->x,best->y)==0){
		for(i=0;i<3;i++)fprintf(stderr,"\nWRONG (EXPLITIVE) VIABILITY!!\n");
	}
	if(count(map) != count(best)){
		for(i=0;i<3;i++)fprintf(stderr,"\nWRONG (EXPLITIVE) COUNT!!!\n");
	}
	printf("\n\nBEST:\n\tCost:\t%ld\n\tPos:\t (%d,%d,%d)\n\n",cost(map,best),best->x,best->y,best->h);


	pthread_mutex_destroy(&lock);
	free(thread_status);
	free(threads);
	free(data);
	free(best);

}
/**
 * wraps a variable to the other side of a map. This way
 * the map loops.
 * @param	x	variable to wrap
 * @param	side	length of a side of the map
 * @return	wrapped variable
 */
inline int wrap(register int x, register int side){
	
	if(x%side==0)return 0;
	if(x < 0){ 
		x = side+(x%side);
	}else{
		x = x % side;
	}
	return x;
	
}
/**
 * gets a value on the map using the wrap function for safety
 * @param	map	map to get from
 * @param	x	x position
 * @param	y	y position
 * @return	value of the map at (x,y)
 */

inline int get_val(map_t *map, int x, int y){
	
	register int side;

	
	side = map->side;
	
	x = wrap(x,side);
	y = wrap(y,side);
	
	return map->matrix[x][y];

}
/**
 * sets a value on a map using the wrap function for safety
 * @param	map	map to set value on
 * @param	x	x position
 * @param	y	y position
 */
inline void set_val(map_t *map, int val, int x, int y){

	int side;
	
	side = map->side;
	

	x = wrap(x,side);
	y = wrap(y,side);

	map->matrix[x][y] = val;
	

}
/**
 * frees a map.
 * @param	map	map to free
 */
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

/**
 * checks if a court position on a map is flat.
 * @param	map	map to check on
 * @param	x	x position
 * @param	y	y position
 * @return	1 if valid, 0 if not.
 */

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

	x1 = wrap(x1,side);
	y1 = wrap(y1,side);

	for(i=x; i < x1; i++){
		for(j=y; j < y1; j++){
			
			i = wrap(i,side);
			j = wrap(j,side);

			if(mat[i][j] != level) return 0;

		}
	}
	return 1;

}
/**
 * fills a map to a certain level. Imagine a rising
 * water table.
 * @param 	map	map to fill
 * @param	level 	level to fill to
 */

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
/**
 * flattens a map to a level. Imagine lopping off the 
 * tops of mountains above a certain level.
 * @param	map	map to flatten
 * @param	level	level to flatten to.
 */

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
/**
 * finds the maximum height on a map
 * @param	map	map to search
 * @return	value of maximum
 */

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
/**
 * finds the minimum height on a map.
 * @param	map	map to search
 * @return	minimum level
 */
int map_min(map_t *map){

	int i,j;
	int min = INT_MAX;
	int current;

	for(i=0; i < map->side; i++){
		for(j=0; j < map->side; j++){
			current = map->matrix[i][j];
			if(current < min) min = current;
		}
	}

	return min;
}
/**
 * copies a map into a new map pointer.
 * @param	dest	pointer to a destination pointer to put the new map in.
 * @param	src	source map
 */
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

/**
 * prints a map to stdout
 * @param	map	pointer to map to print
 */

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
/**
 * loads a map from stdin
 * @param	map	map pointer to load into.
 * @param	side	length of a side of the map.
 */
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
/**
 * callocs a new map.
 * @param	side	length of a side of the map.
 * @return	a pointer to the new map.
 */
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
/**
 * get a number from stdin. Used when loading a new map.
 * @return 	the number retrieved.
 */
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





