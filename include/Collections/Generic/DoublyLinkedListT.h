#ifndef COLLECTIONS_GENERIC_DOUBLY_LINKED_LIST
#define COLLECTIONS_GENERIC_DOUBLY_LINKED_LIST

#define DOUBLY_LINKED_LIST_DEFINE(T)                                        \
/* @brief A node inside a linked list. */                                   \
typedef struct DoublyLinkedNode_##T##_s {                                   \
    struct DoublyLinkedNode_##T##_s *Next;                                  \
    struct DoublyLinkedNode_##T##_s *Prev;                                  \
    T Value;                                                                \
} *DoublyLinkedNode_##T;                                                    \
/* @brief A list of items stored by reference. */                           \
typedef struct DoublyLinkedList_##T##_s {                                   \
    struct IEnumerable_##T##_s _parent;                                     \
    DoublyLinkedNode_##T _start;                                            \
    int Count;                                                              \
} *DoublyLinkedList_##T;                                                    \
/**                                                                         \
 * @brief Creates a new instance of the LinkedList enumerable.              \
 * @return A new LinkedList with no elements.                               \
 */                                                                         \
DoublyLinkedList_##T DoublyLinkedList_##T##__ctor();                        \
/**                                                                         \
 * @brief Creates a LinkedList from a static array.                         \
 * @param itemCount Amount of items in the array.                           \
 * @param items Array of items.                                             \
 * @return A new LinkedList with elements from the array.                   \
 */                                                                         \
DoublyLinkedList_##T Enumerable_##T##_ToDoublyLinkedList(IEnumerable_##T source); \
/**                                                                         \
 * @brief Add an element to the end of the list.                            \
 * @param source List to add an element to.                                 \
 * @param item Item to add to the end of the list.                          \
 */                                                                         \
void DoublyLinkedList_##T##_Add(DoublyLinkedList_##T source, T item);       \
/**                                                                         \
 * @brief Removes an element from the list.                                 \
 * @param source List to remove the element from.                           \
 * @param item Item to remove from the list.                                \
 */                                                                         \
void DoublyLinkedList_##T##_Remove(DoublyLinkedList_##T source, T item);    \
/**                                                                         \
 * @brief Insert an item into the list at the given index.                  \
 * @param source List to insert the element into.                           \
 * @param item Item to insert into the list.                                \
 * @param index Index to insert the item at.                                \
 */                                                                         \
void DoublyLinkedList_##T##_Insert(DoublyLinkedList_##T source, T item, int index);\
/**                                                                         \
 * @brief Removes all elements from the doubly linked list.                 \
 * @param source Linked list to clear.                                      \
 */                                                                         \
void DoublyLinkedList_##T##_Clear(DoublyLinkedList_##T source);             \
/**                                                                         \
 * @brief Frees up all memory occupied by a DoublyLinkedList<T>.            \
 * @param source List to destroy.                                           \
 */                                                                         \
void DoublyLinkedList_##T##_Destroy(DoublyLinkedList_##T* source);          \
/**                                                                         \
 * @brief Sorts the elements of a DoublyLinkedList<T>.                      \
 * @param source List to sort.                                              \
 */                                                                         \
void DoublyLinkedList_##T##_Sort(DoublyLinkedList_##T source, int (*comparer)(T, T));

#endif
