#include "Collections/Enumerable.h"
#include "Collections/Generic/ListImplement.h"
#include "Collections/Generic/EnumerableImplement.h"
#include "Tests.h"
#include "Text/String.h"

typedef struct employee_s {
    string Name;
    void* Coworkers;
    int Age;
} Employee;
#ifndef EMPLOYEE_ENUMERABLE_DEFINED
#define EMPLOYEE_ENUMERABLE_DEFINED
ENUMERABLE_DEFINE(Employee)
ENUMERABLE_IMPLEMENT(Employee)
#endif
#ifndef EMPLOYEE_LIST_DEFINED
#define EMPLOYEE_LIST_DEFINED
LIST_DEFINE(Employee)
LIST_IMPLEMENT(Employee)
#endif
#ifdef STRING_ENUMERABLE_DEFINED
#ifndef EMPLOYEE_STRING_SELECT_DEFINED
#define EMPLOYEE_STRING_SELECT_DEFINED
ENUMERABLE_DEFINE_SELECT(Employee, string)
ENUMERABLE_IMPLEMENT_SELECT(Employee, string)
#endif
#endif
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

IEnumerable_Employee selectCoworkers(Employee item)
{
    return (IEnumerable_Employee)item.Coworkers;
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
    List_Employee workers = new(List_Employee)(4);
    joe.Coworkers = Enumerable_Employee_ToList(Enumerable_Employee_Append(Enumerable_Employee_Append(Enumerable_Employee_Append(base(workers), bidome), barack), obama));
    bidome.Coworkers = Enumerable_Employee_ToList(Enumerable_Employee_Append(Enumerable_Employee_Append(Enumerable_Employee_Append(base(workers), joe), barack), obama));
    barack.Coworkers = Enumerable_Employee_ToList(Enumerable_Employee_Append(Enumerable_Employee_Append(Enumerable_Employee_Append(base(workers), joe), bidome), obama));
    obama.Coworkers = Enumerable_Employee_ToList(Enumerable_Employee_Append(Enumerable_Employee_Append(Enumerable_Employee_Append(base(workers), joe), bidome), barack));
    List_Employee_Add(workers, joe);
    List_Employee_Add(workers, bidome);
    List_Employee_Add(workers, barack);
    List_Employee_Add(workers, obama);
    List_Employee revWorkers = new(List_Employee)(4);
    List_Employee_Add(revWorkers, obama);
    List_Employee_Add(revWorkers, barack);
    List_Employee_Add(revWorkers, bidome);
    List_Employee_Add(revWorkers, joe);
    IEnumerable_Employee concat = Enumerable_Employee_Concat(base(workers), base(revWorkers));
    foreach(Employee, employee, concat, showData(employee));
    List_Employee_Remove(workers, obama);
    if (Enumerable_Employee_Contains(base(workers), obama)) {
        printf("Yup, he's in there.\n");
    } else printf("WARNING: HE HAS BREACHED CONTAINMENT\n");
    foreach(Employee, employee, concat, showData(employee));
    if (Enumerable_Employee_SequenceEqual(base(workers), base(workers))) {
        printf("Sequence is equal to itself. Who would've guessed!\n");
    } else printf("U h  o h .\n");
    #ifdef STRING_ENUMERABLE_DEFINED
    foreach(string, name, Enumerable_Employee_Select_string(base(workers), selectNames), {
        printf("Employee name: %s\n", name);
    });
    printf("We have our list of victors: %s\n", string_Join(", ", Enumerable_Employee_Select_string(base(workers), selectNames)));
    #endif
}
