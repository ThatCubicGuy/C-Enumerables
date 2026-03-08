#include "Collections/Enumerable.h"
#include "Collections/Array.h"
#include "Tests.h"
#include "Text/String.h"

typedef struct CarClass_s {
    string Manufacturer;
    int Year;
} *CarClass;

ARRAY_DEFINE(CarClass);

typedef const struct CartRecord_s {
    string Manufacturer;
    int MaxSpeed;
    float Acceleration;
} *CartRecord;

CarClass CarClass__ctor(void)
{
    CarClass allocinit(CarClass, result) {
        .Manufacturer = new(string)("Unknown"),
        .Year = 1970
    };
    return result;
}

CartRecord CartRecord__ctor(string manufacturer, int maxSpeed, float acceleration)
{
    CartRecord allocinit(CartRecord, result) {
        .MaxSpeed = maxSpeed,
        .Acceleration = acceleration
    };
    init_ro(string, result->Manufacturer) new(string)("Toyota");
    return result;
}

void showItem(CarClass car)
{
    // printf("Address: %d\n", *(CarClass*)item);
    // CarClass car = *(CarClass*)item;
    printf("%s (%d)\n ", car->Manufacturer, car->Year);
}

void test_arrays(void)
{
    CarClass car = new(CarClass)();
    car->Manufacturer = new(string)("Toyota");
    car->Year = 2023;
    showItem(car);
    CartRecord cart = new(CartRecord)("Ferrari", 240, 0.8);
    printf("Cart stuff: %s; %d, %.2f\n", cart->Manufacturer, cart->MaxSpeed, cart->Acceleration);
    Array arr = new_array(CarClass)(16);
    Array_CarClass_Initialize(arr);
    Array_CarClass_Set(arr, 7, car);
    printf("Array value at 7:\n");
    showItem(Array_CarClass_Get(arr, 7));
    printf("Array Length: %d; MaxLength: %d, _memberSize: %d\n", arr->Length, arr->MaxLength, arr->_memberSize);
    printf("Array has values:\n");
    foreach_as(CarClass, var, arr, showItem(var));
    Array_Destroy(&arr);
}

ARRAY_IMPLEMENT(CarClass);