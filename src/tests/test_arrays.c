#include "Collections/Enumerable.h"
#include "Collections/Array.h"
#include "Tests.h"
#include "String.h"
#include "Keywords.h"

typedef TAG(CarClass) {
    string Manufacturer;
    int Year;
} *CarClass;

ARRAY_DEFINE(CarClass);

typedef const TAG(CartRecord) {
    string Manufacturer;
    int MaxSpeed;
    float Acceleration;
} *CartRecord;

CarClass CarClass__ctor(void)
{
    var result = meminit(CarClass) {
        .Manufacturer = new(string)("Unknown"),
        .Year = 1970
    };
    return result;
}

CartRecord CartRecord__ctor(string manufacturer, int maxSpeed, float acceleration)
{
    CartRecord result = meminit(CartRecord) {
        .MaxSpeed = maxSpeed,
        .Acceleration = acceleration,
        .Manufacturer = new(string)(manufacturer)
    };
    return result;
}

void showItem(FILE* output, CarClass car)
{
    fprintf(output, "%s (%d)\n ", car->Manufacturer, car->Year);
}

void test_arrays(FILE* output)
{
    CarClass car = new(CarClass)();
    car->Manufacturer = new(string)("Toyota");
    car->Year = 2023;
    showItem(output, car);
    CartRecord cart = new(CartRecord)("Ferrari", 240, 0.8);
    fprintf(output, "Cart stuff: %s; %d, %.2f\n", cart->Manufacturer, cart->MaxSpeed, cart->Acceleration);
    Array arr = new_array(CarClass)(16);
    Array_CarClass_Initialize(arr);
    Array_CarClass_Set(arr, 7, car);
    fprintf(output, "Array value at 7:\n");
    showItem(output, Array_CarClass_Get(arr, 7));
    fprintf(output, "Array Length: %d; MaxLength: %d, _memberSize: %d\n", arr->Length, arr->MaxLength, arr->_memberSize);
    fprintf(output, "Array has values:\n");
    foreach_deref(CarClass, item, arr, showItem(output, item));
    Array_Destroy(&arr);
}

ARRAY_IMPLEMENT(CarClass);