#include <pthread.h>

struct map_t{
	int side;
	int c_width,c_height;
	long int count;
	long int cost;
	int x,y,h;
	int max,min;
	int c_max,c_min;
	int **matrix;
};
typedef struct map_t map_t;

struct thread_data{
	int i;
	int j;
	char *status;
	map_t *map;
	map_t **best;
	pthread_mutex_t *lock;
};
typedef struct thread_data thread_data;

void load_map(map_t **map, int side);
map_t *new_map(int side);
int map_min(map_t *map);
int map_max(map_t *map);
void cp_map(map_t **dest, map_t *src);

void set_val(map_t *map, int val, int x, int y);

int get_val(map_t *map, int x, int y);
void print_map(map_t *map);
int chk_viable(map_t *map, int x, int y);

void find_best(map_t *map);
void fill(map_t *map, int level);

long int count(map_t *map);
inline map_t *test_pos(map_t *map, int h, int x, int y);

int chk_slope(map_t *map, int x, int y);

void free_map(map_t *map);

map_t *ld_map_img(const char *name);
void mk_map_img(map_t *map);
inline int wrap(register int x, register int side);
long int cost(map_t *orig_m, map_t *new_m);
int chk_slope_map(map_t *map);
inline map_t *test_pos(map_t *map, int h, int x, int y);
