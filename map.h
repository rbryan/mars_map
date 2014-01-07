struct map_t{
	int side;
	int **matrix;
};
typedef struct map_t map_t;

struct pt_t{
	int x,y;
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
