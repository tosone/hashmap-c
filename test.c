#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>

#include "hashmap.h"

#define KEY_MAX_LENGTH 256

enum event_type {
    event_type1,
    event_type2,
};

typedef struct data_struct_s {
    enum event_type type;
    void (*callback)();
} data_struct_t;
pthread_mutex_t data_struct_mutex = PTHREAD_MUTEX_INITIALIZER;

void callback_func() { printf("%s\n", "callback called."); }

int callback_func_itea(any_t key, any_t value) {
    printf("%s\n", "callback itea.");
    data_struct_t *value_get = (data_struct_t *)value;
    int *key_get             = (int *)key;
    printf("%d\n", value_get->type == *key_get);
    if (value_get->callback != NULL) {
        value_get->callback();
    }
    return 0;
}

int main(int argc, char const *argv[]) {
    int error;
    map_t callback_list;
    callback_list = hashmap_new();
    data_struct_t *value;
    value           = (data_struct_t *)malloc(sizeof(data_struct_t));
    value->type     = event_type1;
    value->callback = &callback_func;
    char key[KEY_MAX_LENGTH];
    sprintf(key, "%d", hashmap_length(callback_list));
    pthread_mutex_lock(&data_struct_mutex);
    hashmap_put(callback_list, key, value);
    pthread_mutex_unlock(&data_struct_mutex);

    printf("hashmap length: %d\n", hashmap_length(callback_list));

    data_struct_t *value_get;
    value_get = (data_struct_t *)malloc(sizeof(data_struct_t));
    hashmap_get(callback_list, key, (void **)(&value_get));
    if (value_get->callback != NULL) {
        value_get->callback();
    }

    enum event_type raw_event_type1 = event_type1;
    any_t type_get                  = (void *)&raw_event_type1;
    hashmap_iterate(callback_list, callback_func_itea, type_get);
    hashmap_remove(callback_list, key);
    free(value_get);
    return 0;
}