#include "map.h"
#include <stdlib.h>

Map *map_create(void) {
    Map *map = malloc(sizeof(Map));
    if(!map)
        return NULL;

    map->length = 0;
    map->capacity = INITIAL_CAPACITY;
    map->entries = calloc(map->entries, sizeof(struct MapEntry));
    if(!map->entries) {
        free(map);
        return NULL;
    }

    return map;
}

void map_destroy(Map *map) {
    while(map->length--)
        free(&map->entries[map->length]);

    free(map->entries);
    free(map);
}
