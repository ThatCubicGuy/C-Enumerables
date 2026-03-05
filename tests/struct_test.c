#include "Enumerable.h"
#include "List.h"
#include "tests.h"

typedef struct employee_s {
    char Name[16];
    List* Coworkers;
    int Age;
} Employee;

void ShowNames(const IEnumerable* source)
{
    IEnumerator* e = source->GetEnumerator(source);
    while (e->MoveNext(e)) {
        printf("%s, age %d\n", ((Employee*)e->Current)->Name, ((Employee*)e->Current)->Age);
    }
    e->Dispose(e);
}

void test_with_structs(void) {
    Employee joe = (Employee) {
        .Name = "Joe",
    }, bidome = (Employee) {
        .Name = "Bidome",
    }, barack = (Employee) {
        .Name = "Barack"
    }, obama = (Employee) {
        .Name = "Obama"
    };
    List* workers = CreateList(4);
    List_Add(workers, &joe);
    List_Add(workers, &bidome);
    List_Add(workers, &barack);
    List_Add(workers, &obama);
    joe.Coworkers = workers;
    bidome.Coworkers = workers;
    barack.Coworkers = workers;
    obama.Coworkers = workers;
    ShowNames((IEnumerable*)workers);
}