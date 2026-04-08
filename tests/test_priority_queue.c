#include "Collections/Generic/EnumerableT.h"
#include "Tests.h"
#include "Defines.h"
#include "Collections/Generic/QueueT.h"
#include "Collections/Generic/QueueImplement.h"
#include "Collections/Generic/PriorityQueueT.h"
#include "Collections/Generic/PriorityQueueImplement.h"
#include "Collections/Generic/HeapT.h"
#include "Collections/Generic/HeapImplement.h"

typedef struct car_s {
    int year;
    string model;
} Car;

typedef struct car_priority_s {
    int rarity;
    int demand;
    int price;
} CarValue;

static void print_car(Car car)
{
    printf("%s (%d)\n", car.model, car.year);
}

static void print_car_value(CarValue value)
{
    printf("Rarity: %d; Demand: %d%%; Price: $%d\n", value.rarity, value.demand, value.price);
}

ENUMERABLE_DEFINE(Car)

QUEUE_DEFINE(Car)
HEAP_DEFINE(CarValue)
PRIORITYQUEUE_DEFINE(CarValue, Car)

static int ascending(int x, int y)
{
    if (x < y) return -1;
    if (x > y) return 1;
    return 0;
}

static int descending(int x, int y)
{
    if (x > y) return -1;
    if (x < y) return 1;
    return 0;
}

static int comp(CarValue a, CarValue b)
{
    if (a.rarity != b.rarity) return descending(a.rarity, b.rarity);
    if (a.demand != b.demand) return descending(a.demand, b.demand);
    return descending(a.price, b.price);
}

#define CARS {(Car){2023, "Skoda Octavia"}, (Car){2005, "Honda Civic"}, (Car){1986, "Toyota Corolla"}, {2017, "Nissan GT-R"}, {2009, "Peugeot"}, {1998, "Lancia 037"}, {2002, "Nissan Silvia"}}
#define CAR_VALUES {(CarValue){2, 10, 4000}, (CarValue){3, 20, 3000}, (CarValue){3, 40, 2000}, (CarValue){2, 20, 5000}, (CarValue){1, 15, 1500}, (CarValue){2, 30, 5000}, (CarValue){2, 30, 3000}}
#define CNT 7

void test_priority_queue(void)
{
    auto pq = new(PriorityQueue_CarValue_Car)(16, comp);
    auto q = new(Queue_Car)(16);
    auto h = new(Heap_CarValue)(16, 4, comp);
    Car cars[] = CARS;
    CarValue values[] = CAR_VALUES;
    for (int i = 0; i < CNT; ++i) {
        PriorityQueue_CarValue_Car_Enqueue(pq, cars[i], values[i]);
        Queue_Car_Enqueue(q, cars[i]);
        Heap_CarValue_Push(h, values[i]);
    }
    Car result;
    printf("Output cars in add order:\n");
    while (Queue_Car_TryDequeue(q, &result)) {
        print_car(result);
    }
    CarValue priority;
    printf("Output values in order:\n");
    while (Heap_CarValue_TryPop(h, &priority)) {
        print_car_value(priority);
    }
    printf("Output cars with priority in value order:\n");
    while (PriorityQueue_CarValue_Car_TryDequeue(pq, &result, &priority)) {
        print_car(result);
        print_car_value(priority);
    }
}

QUEUE_IMPLEMENT(Car)
HEAP_IMPLEMENT(CarValue)
PRIORITYQUEUE_IMPLEMENT(CarValue, Car)