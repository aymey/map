#ifndef __MAP_H__
#define __MAP_H__

#include <stddef.h>
#include <stdbool.h>

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

static const char *_map_set_entry(struct MapEntry *entries, size_t capacity,
        const char *key, void *value, size_t *plength); // set entry without expanding map
static bool _map_expand(Map *map);                      // expand a map to twice its size

size_t map_length(Map *map);    // amount of items in map

#endif // __MAP_H__
