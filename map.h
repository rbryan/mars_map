struct map_t{
	int side;
	int c_width,c_height;
	int **matrix;
};
typedef struct map_t map_t;

struct pt_t{
	int x,y;
	int val,dir;
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
pt_t *get_vlist(map_t *map);
