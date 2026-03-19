#ifndef COLLECTIONS_GENERIC_LINKED_LIST
#define COLLECTIONS_GENERIC_LINKED_LIST

#define LINKED_LIST_DEFINE(T)                                               \
/* @brief A node inside a linked list. */                                   \
typedef struct LinkedNode_##T##_s {                                         \
    struct LinkedNode_##T##_s *Next;                                        \
    T Value;                                                                \
} *LinkedNode_##T;                                                          \
/* @brief A list of items stored by reference. */                           \
typedef struct LinkedList_##T##_s {                                         \
    struct IEnumerable_##T##_s _parent;                                     \
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
LinkedList_##T Enumerable_##T##_ToLinkedList(IEnumerable_##T source);       \
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
void LinkedList_##T##_Destroy(LinkedList_##T* source);                      \
/**                                                                         \
 * @brief Reverses the given list without creating a copy.                  \
 * @param source List to reverse.                                           \
 */                                                                         \
void LinkedList_##T##_Reverse(LinkedList_##T source);                       \
/**                                                                         \
 * @brief Sorts the given linked list without creating a copy.              \
 * @param source List to sort.                                              \
 */                                                                         \
void LinkedList_##T##_Sort(LinkedList_##T source, int (*comparer)(T, T));

// TODO: some fixes from SDA project

#endif