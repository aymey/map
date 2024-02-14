#ifndef __MAP_H__
#define __MAP_H__

#include <stddef.h>

#define INITIAL_CAPACITY 16

struct MapEntry {
    const char *key;
    void *value;
};

typedef struct {
    struct MapEntry *entries;
    size_t capacity;
    size_t length;
} Map;

Map *map_create(void);  // Creates a Map and return a pointer to it (NULL if out of memory)
void map_destroy(Map *map); // Free the map

void *map_get(Map *map, char *key);                     // Get value/NULL from a char key
const char *map_set(Map *map, char *key, void *value);  // Sets and retrieves a key

size_t map_length(Map *map);    // amount of items in map

#endif // __MAP_H__
