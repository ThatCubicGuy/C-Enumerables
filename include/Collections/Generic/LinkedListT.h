#ifndef COLLECTIONS_GENERIC_LINKED_LIST
#define COLLECTIONS_GENERIC_LINKED_LIST

#include "Collections/Generic/EnumerableT.h"

#define LINKED_LIST_DEFINE(T)                                               \
/* @brief A node inside a linked list. */                                   \
typedef struct generic_linked_node_##T##_s {                                \
    struct generic_linked_node_##T##_s *Next;                               \
    T Value;                                                                \
} *LinkedNode_##T;                                                          \
/* @brief A list of items stored by reference. */                           \
typedef struct generic_linked_list_##T##_s {                                \
    struct generic_enumerable_##T##_s _parent;                              \
    LinkedNode_##T _start;                                                  \
    LinkedNode_##T _end;                                                    \
    int Count;                                                              \
} *LinkedList_##T;                                                          \
/**                                                                         \
 * @brief Creates a new instance of the LinkedList enumerable.              \
 * @return A new LinkedList with no elements.                               \
 */                                                                         \
LinkedList_##T LinkedList_##T##__ctor();                                    \
/**                                                                         \
 * @brief Creates a LinkedList from a static array.                         \
 * @param itemCount Amount of items in the array.                           \
 * @param items Array of items.                                             \
 * @return A new LinkedList with elements from the array.                   \
 */                                                                         \
LinkedList_##T Enumerable_ToLinkedList_##T(IEnumerable_##T source);         \
/**                                                                         \
 * @brief Add an element to the end of the list.                            \
 * @param list List to add an element to.                                   \
 * @param item Item to add to the end of the list.                          \
 */                                                                         \
void LinkedList_##T##_Add(LinkedList_##T source, T item);                   \
/**                                                                         \
 * @brief Insert an item into the list at the given index.                  \
 * @param source List to insert the element into.                           \
 * @param item Item to insert into the list.                                \
 * @param index Index to insert the item at.                                \
 */                                                                         \
void LinkedList_##T##_Insert(LinkedList_##T source, T item, int index);     \
/**                                                                         \
 * @brief Removes all elements from the linked list.                        \
 * @param source Linked list to clear.                                      \
 */                                                                         \
void LinkedList_##T##_Clear(LinkedList_##T source);                         \
/**                                                                         \
 * @brief Frees up all memory occupied by a LinkedList<T>.                  \
 * @param source List to destroy.                                           \
 */                                                                         \
void DestroyLinkedList_##T(LinkedList_##T* source);

#endif