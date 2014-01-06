struct map_t{
	int side;
	int **matrix;
};
typedef struct map_t map_t;

void load_map(map_t **map, int side);
map_t *new_map(int side);
