#ifndef COLLECTIONS_LINKED_LIST
#define COLLECTIONS_LINKED_LIST

#include "Collections/Enumerable.h"

/// @brief A node inside a linked list.
typedef struct linked_node_s {
    struct linked_node_s *Next;
    object Value;
} LinkedNode;

/// @brief A list of items stored by reference.
typedef struct linked_list_s {
    IEnumerable _parent;
    LinkedNode *_start;
    LinkedNode *_end;
    int Count;
} LinkedList;

/// @brief Creates a new instance of the LinkedList enumerable.
/// @return A new LinkedList with no elements.
LinkedList* LinkedList__ctor();

/// @brief Creates a LinkedList from a static array.
/// @param itemCount Amount of items in the array.
/// @param items Array of items.
/// @return A new LinkedList with elements from the array.
LinkedList* CreateLinkedListFromArray(int itemCount, object items[]);

/// @brief Add an element to the end of the list.
/// @param list List to add an element to.
/// @param item Item to add to the end of the list.
void LinkedList_Add(LinkedList* source, object item);

/// @brief Insert an item into the list at the given index.
/// @param source List to insert the element into.
/// @param item Item to insert into the list.
/// @param index Index to insert the item at.
void LinkedList_Insert(LinkedList* source, object item, int index);

/// @brief Removes all elements from the linked list.
/// @param source Linked list to clear.
void LinkedList_Clear(LinkedList* source);

/// @brief Frees up all memory occupied by a linked list.
/// @param source List to destroy.
void DestroyLinkedList(LinkedList** source);

#endif