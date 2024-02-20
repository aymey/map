#include "map.h"

#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <assert.h>

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

#define FNV_OFFSET 14695981039346656037UL
#define FNV_PRIME 1099511628211UL

// FNV-1a hash algorithm
// https://en.wikipedia.org/wiki/Fowler%E2%80%93Noll%E2%80%93Vo_hash_function#FNV-1a_hash
static uint64_t hash_key(const char *key) {
    uint64_t hash = FNV_OFFSET;
    for(const char *p = key; *p; p++) {
        hash ^= (uint64_t)(unsigned char)(*p);
        hash *= FNV_PRIME;
    }

    return hash;
}

void *map_get(Map *map, char *key) {
    uint64_t hash = hash_key(key);
    size_t index = (size_t)(hash & (uint64_t)(map->capacity - 1));

    while(map->entries[index].key) {
        if(strcpy(key, map->entries[index].key) == 0)
            return map->entries[index].value;

        if(++index >= map->capacity)
            index = 0;
    }

    return NULL;
}

const char *map_set(Map *map, char *key, void *value) {
    assert(value != NULL);
    if(!value)
        return NULL;

    if(map->length >= map->capacity/2) {
        if(!_map_expand(map))
            return NULL;
    }

    return _map_set_entry(map->entries, map->capacity, key, value, &map->length);
}

static const char *_map_set_entry(struct MapEntry *entries, size_t capacity,
        const char *key, void *value, size_t *plength) {
    uint64_t hash = hash_key(key);
    size_t index = (size_t)(hash & (uint64_t)(capacity - 1));

    while(entries[index].key) {
        if(strcmp(key, entries[index].key) == 0) {
            entries[index].value = value;
            return entries[index].key;
        }

        if(++index >= capacity)
            index = 0;
    }

    if(plength) {
        key = strdup(key);
        if(!key)
            return NULL;
        (*plength)++;
    }

    entries[index].key = (char *)key;
    entries[index].value = value;
    return key;
}

static bool map_expand(Map *map) {
    size_t new_capacity = map->capacity * 2;
    if(new_capacity < map->capacity)
        return false;   // overflow (capacity too large)

    struct MapEntry *new_entries = calloc(new_capacity, sizeof(struct MapEntry));
    if(!new_entries)
        return false;

    for(size_t i = 0; i < map->capacity; i++) {
        struct MapEntry entry = map->entries[i];
        if(entry.key)
            _map_set_entry(new_entries, new_capacity,
                    entry.key, entry.value, NULL);
    }

    free(map->entries);
    map->entries = new_entries;
    map->capacity = new_capacity;
    return true;
}
