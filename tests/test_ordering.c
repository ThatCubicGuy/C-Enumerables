#include "Collections/Enumerable.h"
#include "Collections/Array.h"

typedef struct person_s {
    int age;
    int salary;
    int motivation;
} Person;

int sort_by_age(object left, object right)
{
    if (((Person*)left)->age < ((Person*)right)->age) return -1;
    if (((Person*)left)->age > ((Person*)right)->age) return 1;
    return 0;
}

int sort_by_salary(object left, object right)
{
    if (((Person*)left)->salary < ((Person*)right)->salary) return -1;
    if (((Person*)left)->salary > ((Person*)right)->salary) return 1;
    return 0;
}

int sort_by_motivation(object left, object right)
{
    if (((Person*)left)->motivation < ((Person*)right)->motivation) return -1;
    if (((Person*)left)->motivation > ((Person*)right)->motivation) return 1;
    return 0;
}

void test_ordering(void)
{
    // int x[16] = {125, 1, 65, 34, 22, 1, 94, 90, -1, 65, 3, 18, 8, 88, 8889, 2};
    // Person p[8] = {
    //     {29, 1600, 99999},
    //     {31, 2000, 9999},
    //     {31, 2, 999},
    //     {31, 32767, 99},
    //     {29, 1600, 9},
    //     {29, 1600, -9},
    //     {29, 1590, -99},
    //     {29, 1800, -999}
    // };
    // Array arr = new(Array)(sizeof(Person), 20);
    // // Array_Fill(arr, &p);
    // for (int i = 0; i < 8; ++i) {
    //     Array_Set(arr, i, p + i);
    // }
    // foreach_deref(Person, item, arr, {
    //     printf("Array item: { Age: %d, Salary: %d, Motivation: %d }\n", item.age, item.salary, item.motivation);
    // });
    // printf("ORDER!!!\n");
    // IOrderedEnumerable sorted = Enumerable_OrderBy((IEnumerable)arr, sort_by_age);
    // foreach_deref(Person, item, sorted, {
    //     printf("Array item: { Age: %d, Salary: %d }\n", item.age, item.salary);
    // });
    // printf("Order... BETTER !!!!!\n");
    // sorted = Enumerable_ThenBy(sorted, sort_by_salary);
    // foreach_deref(Person, item, sorted, {
    //     printf("Array item: { Age: %d, Salary: %d }\n", item.age, item.salary);
    // });
    // printf("Order... BETTEREST !!!!!!!!!!!!!\n");
    // sorted = Enumerable_ThenBy(sorted, sort_by_motivation);
    // foreach_deref(Person, item, sorted, {
    //     printf("Array item: { Age: %d, Salary: %d }\n", item.age, item.salary);
    // });
    // Array_Set(arr, 0, )
}
