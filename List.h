#ifndef GENERIC_LIST
#define GENERIC_LIST

#include "Enumerable.h"

typedef struct generic_list_s {
    IEnumerable _parent;
    int Capacity;
    int Count;
    object *_items;
} List;

/// @brief Creates a new List with the given capacity.
/// @param capacity Initial capacity for the created list.
/// @return A new List with the given capacity.
List* CreateList(int capacity);

/// @brief Add an element to the end of the list.
/// @param list List to add an element to.
/// @param item Item to add to the end of the list.
void List_Add(List* source, object item);

/// @brief Insert an item into the list at the given index.
/// @param source List to insert the element into.
/// @param item Item to insert into the list.
/// @param index Index to insert the item at.
void List_Insert(List* source, object item, int index);

/// @brief Ensures the given list has the capacity to hold a given amount of items.
/// @param source List to ensure the capacity for.
/// @param capacity Capacity to ensure.
void List_EnsureCapacity(List* source, int capacity);

/// @brief Sets the capacity of the list to the amount of elements, if that amount is less than 90% of current capacity.
/// @param source List to trim the excess of.
void List_TrimExcess(List* source);

#endif