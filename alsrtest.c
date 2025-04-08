#include "types.h"
#include "stat.h"
#include "user.h"

int global_var = 31;

int main(void) {
    int local_var;
    printf(1, "Code segment address: %p\n", main);
    printf(1, "Global variable address: %p\n", &global_var);
    printf(1, "Stack address: %p\n", &local_var);
    char *heap_ptr = malloc(100);
    if(heap_ptr == 0){
        printf(1, "Heap allocation failed\n");
    } else {
        printf(1, "Heap allocation address: %p\n", heap_ptr);
    }
    exit();
}
