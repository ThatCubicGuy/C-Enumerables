#include "Enumerable.h"
#include "Array.h"
#include "Tests.h"

typedef struct car_s {
    char Manufacturer[16];
    int Year;
} Car;

object Car__ctor(void)
{
    Car* result = bare(Car);
    *result = (Car) {
        .Manufacturer = "Unknown",
        .Year = 1970
    };
    return result;
}

void showItem(object item)
{
    printf("%s (%d)\n ", ((const Car*)item)->Manufacturer, ((const Car*)item)->Year);
}

void setNumber(object item)
{
    *(int*)item = 3;
}

void test_arrays(void)
{
    Car* car = new(Car)();
    Array* arr = new(Array)(16);
    Array_FillConstructible(arr, Car__ctor);
    for (int i = 0; i < 7; ++i) {
        ((Car*)arr->Values[7])->Manufacturer[i] = "Toyota"[i];
    }
    ((Car*)arr->Values[7])->Year = 2023;
    printf("Array has values:\n");
    Enumerable_ForEach((IEnumerable*)arr, showItem);
}