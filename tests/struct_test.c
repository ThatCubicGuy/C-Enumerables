#include "Collections/Enumerable.h"
#include "Collections/Generic/ListImplement.h"
#include "Collections/Generic/EnumerableImplement.h"
#include "Tests.h"

typedef struct employee_s {
    string Name;
    void* Coworkers;
    int Age;
} Employee;

ENUMERABLE_DEFINE(Employee)
LIST_DEFINE(Employee)
ENUMERABLE_DEFINE(string)
ENUMERABLE_DEFINE_SELECT(Employee, string)

void showData(Employee item)
{
    printf("%s, age %d\n", item.Name, item.Age);
}

void ShowAllData(const IEnumerable_Employee* source)
{
    foreach(Employee, employee, source, showData(employee));
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

string selectNames(Employee item)
{
    return item.Name;
}

int selectAge(Employee item)
{
    return item.Age;
}

IEnumerable_Employee* selectCoworkers(Employee item)
{
    return (IEnumerable_Employee*)item.Coworkers;
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
    List_Employee* workers = new(List_Employee)(4);
    joe.Coworkers = Enumerable_Employee_ToList(Enumerable_Employee_Append(Enumerable_Employee_Append(Enumerable_Employee_Append((IEnumerable_Employee*)workers, bidome), barack), obama));
    bidome.Coworkers = Enumerable_Employee_ToList(Enumerable_Employee_Append(Enumerable_Employee_Append(Enumerable_Employee_Append((IEnumerable_Employee*)workers, joe), barack), obama));
    barack.Coworkers = Enumerable_Employee_ToList(Enumerable_Employee_Append(Enumerable_Employee_Append(Enumerable_Employee_Append((IEnumerable_Employee*)workers, joe), bidome), obama));
    obama.Coworkers = Enumerable_Employee_ToList(Enumerable_Employee_Append(Enumerable_Employee_Append(Enumerable_Employee_Append((IEnumerable_Employee*)workers, joe), bidome), barack));
    List_Employee_Add(workers, joe);
    List_Employee_Add(workers, bidome);
    List_Employee_Add(workers, barack);
    List_Employee_Add(workers, obama);
    List_Employee* revWorkers = new(List_Employee)(4);
    List_Employee_Add(revWorkers, obama);
    List_Employee_Add(revWorkers, barack);
    List_Employee_Add(revWorkers, bidome);
    List_Employee_Add(revWorkers, joe);
    IEnumerable_Employee* concat = Enumerable_Employee_Concat((IEnumerable_Employee*)workers, (IEnumerable_Employee*)revWorkers);
    foreach(Employee, employee, concat, showData(employee));
    List_Employee_Remove(workers, obama);
    if (Enumerable_Employee_Contains((IEnumerable_Employee*)workers, obama)) {
        printf("Yup, he's in there.\n");
    } else printf("WARNING: HE HAS BREACHED CONTAINMENT\n");
    foreach(Employee, employee, concat, showData(employee));
    if (Enumerable_Employee_SequenceEqual((IEnumerable_Employee*)workers, (IEnumerable_Employee*)workers)) {
        printf("Sequence is equal to itself. Who would've guessed!\n");
    } else printf("U h  o h .\n");
    foreach(string, name, Enumerable_Employee_Select_string((IEnumerable_Employee*)workers, selectNames), {
        printf("Employee name: %s\n", name);
    });
}

ENUMERABLE_IMPLEMENT(Employee)
LIST_IMPLEMENT(Employee)
ENUMERABLE_IMPLEMENT(string)
ENUMERABLE_IMPLEMENT_SELECT(Employee, string)
