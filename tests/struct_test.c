#include "Enumerable.h"
#include "List.h"
#include "tests.h"

typedef struct employee_s {
    char Name[16];
    List* Coworkers;
    int Age;
} Employee;

void showData(object item)
{
    Employee* employee = (Employee*)item;
    printf("%s, age %d\n", employee->Name, employee->Age);
}

void ShowAllData(const IEnumerable* source)
{
    IEnumerator* e = source->GetEnumerator(source);
    while (e->MoveNext(e)) {
        showData(e->Current);
    }
    e->Dispose(e);
}

void ShowStrings(const IEnumerable* source)
{
    IEnumerator* e = source->GetEnumerator(source);
    while (e->MoveNext(e)) {
        printf("%s\n", (char*)e->Current);
    }
    e->Dispose(e);
}

void ShowNumbers(const IEnumerable* source)
{
    IEnumerator* e = source->GetEnumerator(source);
    while (e->MoveNext(e)) {
        printf("%d ", *(int*)e->Current);
    }
    e->Dispose(e);
    printf("\n");
}

object selectNames(object item)
{
    return ((Employee*)item)->Name;
}

object selectAge(object item)
{
    return &((Employee*)item)->Age;
}

IEnumerable* selectCoworkers(object item)
{
    return (IEnumerable*)((Employee*)item)->Coworkers;
}

void test_with_structs(void) {
    Employee joe = (Employee) {
        .Name = "Joe",
        .Age = 101
    }, bidome = (Employee) {
        .Name = "Bidome",
        .Age = 505
    }, barack = (Employee) {
        .Name = "Barack",
        .Age = 69
    }, obama = (Employee) {
        .Name = "Obama",
        .Age = 420
    };
    List* workers = CreateList(4);
    joe.Coworkers = Enumerable_ToList(Enumerable_Append(Enumerable_Append(Enumerable_Append((IEnumerable*)workers, &bidome), &barack), &obama));
    bidome.Coworkers = Enumerable_ToList(Enumerable_Append(Enumerable_Append(Enumerable_Append((IEnumerable*)workers, &joe), &barack), &obama));
    barack.Coworkers = Enumerable_ToList(Enumerable_Append(Enumerable_Append(Enumerable_Append((IEnumerable*)workers, &joe), &bidome), &obama));
    obama.Coworkers = Enumerable_ToList(Enumerable_Append(Enumerable_Append(Enumerable_Append((IEnumerable*)workers, &joe), &bidome), &barack));
    List_Add(workers, &joe);
    List_Add(workers, &bidome);
    List_Add(workers, &barack);
    List_Add(workers, &obama);
    List* revWorkers = CreateList(4);
    List_Add(revWorkers, &obama);
    List_Add(revWorkers, &barack);
    List_Add(revWorkers, &bidome);
    List_Add(revWorkers, &joe);
    IEnumerable* concat = Enumerable_Concat((IEnumerable*)workers, (IEnumerable*)revWorkers);
    Enumerable_ForEach(concat, showData);
    List_Remove(workers, &obama);
    if (Enumerable_Contains((IEnumerable*)workers, &obama)) {
        printf("Yup, he's in there.\n");
    } else printf("WARNING: HE HAS BREACHED CONTAINMENT\n");
    Enumerable_ForEach(concat, showData);
    if (Enumerable_SequenceEqual((IEnumerable*)workers, (IEnumerable*)workers)) {
        printf("Sequence is equal to itself. Who would've guessed!\n");
    } else printf("U h  o h .\n");
}