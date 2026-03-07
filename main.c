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
    printf("--------------ARRAY--------------\n");
    test_arrays();
    printf("-------GENERIC LINKED LIST-------\n");
    test_generic_linked_lists();
    printf("\n----------TESTS CLEARED----------\n");
    return 0;
}