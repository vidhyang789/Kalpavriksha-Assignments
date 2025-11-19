#include <stdio.h>
#include "lruCache.h"

int main() {
    LRUCache *cache = NULL;
    char *command = malloc(sizeof(char) * 100);

    while (1) {
        scanf("%s", command);

        if (strcmp(command, "createCache") == 0) {
            int cap;
            scanf("%d", &cap);
            cache = lruCreate(cap);
        }

        else if (strcmp(command, "put") == 0) {
            int key;
            char *value = malloc(sizeof(char)*100);
            scanf("%d %s", &key, value);
            lruPut(cache, key, value);
        }

        else if (strcmp(command, "get") == 0) {
            int key;
            scanf("%d", &key);
            char *result = lruGet(cache, key);
            if (strcmp(result,"-1") == 1){
                printf("%s\n", result);
            }
            else{
                printf("NULL\n");
            }
        }

        else if (strcmp(command, "exit") == 0) {
            printf("Exiting...\n");
            break;
        }

        else {
            printf("Unknown command: %s\n", command);
        }
    }

    if (cache){
        lruFree(cache);
    }

    return 0;
}
