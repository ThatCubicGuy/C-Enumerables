#include "Defines.h"
#include "Tests.h"

int main(void)
{
    printf("\n----------NUMBERS TESTS----------\n");
    test_with_numbers();
    printf("-------------STRUCTS-------------\n");
    test_with_structs();
    printf("-----------LINKED LIST-----------\n");
    test_linked_lists();
    printf("-------GENERIC LINKED LIST-------\n");
    test_generic_linked_lists();
    printf("--------------ARRAY--------------\n");
    test_arrays();
    printf("--------------INDEX--------------\n");
    test_index();
    printf("--------------STACK--------------\n");
    test_stacks();
    printf("--------------QUEUE--------------\n");
    test_queues();
    printf("-------DOUBLY LINKED LISTS-------\n");
    test_doubly_linked_list();
    printf("--------------HEAPS--------------\n");
    test_heaps();
    printf("---------PRIORITY QUEUES---------\n");
    test_priority_queue();
    printf("\n----------TESTS CLEARED----------\n");
    return 0;
}
