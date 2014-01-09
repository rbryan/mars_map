struct map_t{
	int side;
	int c_width,c_height;
	long int count;
	long int cost;
	int x,y,h;
	int max,min;
	int **matrix;
};
typedef struct map_t map_t;

struct pt_t{
	int x,y;
	int val;
	long int cost;
	struct pt_t *next;
};
typedef struct pt_t pt_t;

struct extr_l{
	pt_t *maxima;
	pt_t *minima;
};

void load_map(map_t **map, int side);
map_t *new_map(int side);
struct extr_l *link_extrema(map_t *map);
int map_min(map_t *map);
int map_max(map_t *map);
void cp_map(map_t **dest, map_t *src);

void set_val(map_t *map, int val, int x, int y);

int get_val(map_t *map, int x, int y);
void print_map(map_t *map);
int chk_viable(map_t *map, int x, int y);

void find_best(map_t *map);
void fill(map_t *map, int level);
