#include "Tests.h"

int Main(void)
{
    printf("\n----------NUMBERS TESTS----------\n");
    test_with_numbers(stdout);
    printf("-------------STRUCTS-------------\n");
    test_with_structs(stdout);
    printf("-----------LINKED LIST-----------\n");
    test_linked_lists(stdout);
    printf("-------GENERIC LINKED LIST-------\n");
    test_generic_linked_lists(stdout);
    printf("--------------ARRAY--------------\n");
    test_arrays(stdout);
    printf("--------------INDEX--------------\n");
    test_index(stdout);
    printf("--------------STACK--------------\n");
    test_stacks(stdout);
    printf("--------------QUEUE--------------\n");
    test_queues(stdout);
    printf("-------DOUBLY LINKED LISTS-------\n");
    test_doubly_linked_list(stdout);
    printf("--------------HEAPS--------------\n");
    test_heaps(stdout);
    printf("---------PRIORITY QUEUES---------\n");
    test_priority_queue(stdout);
    printf("-----------DICTIONARIES----------\n");
    test_dictionary(stdout);
    printf("-------------HASH SET------------\n");
    test_hash_set(stdout);
    printf("------------HASH SET 2------------\n");
    test_hash_set_string(stdout);
    printf("\n----------TESTS CLEARED----------\n");
    printf("Unless...\n");
    test_stack_trace(stdout);
    return 0;
}
