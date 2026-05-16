#include "Collections/Enumerable.h"
#include "Collections/Generic/ListImplement.h"
#include "Collections/Generic/EnumerableImplement.h"
#include "Tests.h"
#include <stdio.h>
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

void ShowAllData(IEnumerable(Employee) source)
{
    foreach_deref(Employee, employee, source, {
        showData(employee);
    });
}

void ShowStrings(IEnumerable source)
{
    foreach (string str in source) {
        printf("%s\n", str);
    }
}

void ShowNumbers(IEnumerable source)
{
    foreach_deref(int, item, source, {
        printf("%d ", item);
    });
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

IEnumerable(Employee) selectCoworkers(Employee item)
{
    return (IEnumerable(Employee))item.Coworkers;
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
    List(Employee) workers = new(List(Employee))(4);
    joe.Coworkers = Enumerable_Employee_ToList(Enumerable_Employee_Append(Enumerable_Employee_Append(Enumerable_Employee_Append((IEnumerable(Employee))(workers), bidome), barack), obama));
    bidome.Coworkers = Enumerable_Employee_ToList(Enumerable_Employee_Append(Enumerable_Employee_Append(Enumerable_Employee_Append((IEnumerable(Employee))(workers), joe), barack), obama));
    barack.Coworkers = Enumerable_Employee_ToList(Enumerable_Employee_Append(Enumerable_Employee_Append(Enumerable_Employee_Append((IEnumerable(Employee))(workers), joe), bidome), obama));
    obama.Coworkers = Enumerable_Employee_ToList(Enumerable_Employee_Append(Enumerable_Employee_Append(Enumerable_Employee_Append((IEnumerable(Employee))(workers), joe), bidome), barack));
    List_Employee_Add(workers, joe);
    List_Employee_Add(workers, bidome);
    List_Employee_Add(workers, barack);
    List_Employee_Add(workers, obama);
    List(Employee) revWorkers = new(List(Employee))(4);
    List_Employee_Add(revWorkers, obama);
    List_Employee_Add(revWorkers, barack);
    List_Employee_Add(revWorkers, bidome);
    List_Employee_Add(revWorkers, joe);
    IEnumerable(Employee) concat = Enumerable_Employee_Concat((IEnumerable(Employee))workers, (IEnumerable(Employee))revWorkers);
    foreach (Employee employee in concat) showData(employee);
    List_Employee_Remove(workers, obama);
    if (Enumerable_Employee_Contains((IEnumerable(Employee))workers, obama)) {
        printf("Yup, he's in there.\n");
    } else printf("WARNING: HE HAS BREACHED CONTAINMENT\n");
    foreach (Employee employee in concat) showData(employee);
    if (Enumerable_Employee_SequenceEqual((IEnumerable(Employee))workers, (IEnumerable(Employee))workers)) {
        printf("Sequence is equal to itself. Who would've guessed!\n");
    } else printf("U h  o h .\n");
    #ifdef STRING_ENUMERABLE_DEFINED
    foreach (string name in Enumerable_Employee_Select_string((IEnumerable(Employee))workers, selectNames)) {
        printf("Employee name: %s\n", name);
    }
    printf("We have our list of victors: %s\n", string_Join(", ", Enumerable_Employee_Select_string((IEnumerable(Employee))(workers), selectNames)));
    #endif
    List_Employee_Destroy(&workers);
    List_Employee_Destroy(&revWorkers);
}
