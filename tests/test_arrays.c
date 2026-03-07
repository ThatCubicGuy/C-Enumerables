#include "Collections/Enumerable.h"
#include "Collections/Array.h"
#include "Tests.h"

typedef struct car_s {
    char Manufacturer[16];
    int Year;
} *CarClass;

ARRAY_DEFINE(CarClass);

typedef const struct cart_s {
    char Manufacturer[16];
    int MaxSpeed;
    float Acceleration;
} *CartRecord;

CarClass CarClass__ctor(void)
{
    CarClass result = alloc(struct car_s);
    *result = (struct car_s) {
        .Manufacturer = "Unknown",
        .Year = 1970
    };
    return result;
}

CartRecord CartRecord__ctor(const char* manufacturer, int maxSpeed, float acceleration)
{
    CartRecord result = alloc(struct cart_s);
    *(struct cart_s*)result = (struct cart_s) {
        .MaxSpeed = maxSpeed,
        .Acceleration = acceleration
    };
    MemCopyToNull(result->Manufacturer, manufacturer);
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
    MemCopyToNull(car->Manufacturer, "Toyota");
    car->Year = 2023;
    showItem(car);
    CartRecord cart = new(CartRecord)("Ferrari", 240, 0.8);
    printf("Cart stuff: %s; %d, %.2f\n", cart->Manufacturer, cart->MaxSpeed, cart->Acceleration);
    Array* arr = new_array(CarClass)(16);
    Array_CarClass_Initialize(arr);
    Array_CarClass_Set(arr, 7, car);
    printf("Array value at 7:\n");
    showItem(Array_CarClass_Get(arr, 7));
    printf("Array Length: %d; MaxLength: %d, _memberSize: %d\n", arr->Length, arr->MaxLength, arr->_memberSize);
    printf("Array has values:\n");
    foreach_ref(CarClass, var, arr, showItem(var));
}

ARRAY_IMPLEMENT(CarClass);