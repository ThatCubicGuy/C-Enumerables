#ifndef COLLECTIONS_GENERIC_LIST
#define COLLECTIONS_GENERIC_LIST

#pragma region Define

#define LIST_DEFINE(T)                                              \
typedef struct List_##T##_s {                                       \
    struct IEnumerable_##T##_s _parent;                             \
    int Capacity;                                                   \
    int Count;                                                      \
    T* Values;                                                      \
} *List_##T;                                                        \
/**                                                                 \
 * @brief Creates a new List with the given capacity.               \
 * @param capacity Initial capacity for the created list.           \
 * @return A new List with the given capacity.                      \
 */                                                                 \
List_##T List_##T##__ctor(int capacity);                            \
/**                                                                 \
 * @brief Frees all memory relating to a list.                      \
 * @param list List to destroy.                                     \
 */                                                                 \
void List_##T##_Destroy(List_##T* list);                            \
/**                                                                 \
 * @brief Creates a List from an IEnumerable.                       \
 * @param source Enumerable to take items from.                     \
 * @return A new list.                                              \
 */                                                                 \
List_##T Enumerable_##T##_ToList(IEnumerable_##T source);           \
/**                                                                 \
 * @brief Add an element to the end of the list.                    \
 * @param list List to add an element to.                           \
 * @param item Item to add to the end of the list.                  \
 */                                                                 \
void List_##T##_Add(List_##T source, T item);                       \
/**                                                                 \
 * @brief Removes an element from the list.                         \
 * @param source List to remove the element from.                   \
 * @param item Item to remove from the list.                        \
 */                                                                 \
void List_##T##_Remove(List_##T source, T item);                    \
/**                                                                 \
 * @brief Removes all elements from the list.                       \
 * @param source List to clear.                                     \
 */                                                                 \
void List_##T##_Clear(List_##T source);                             \
/**                                                                 \
 * @brief Insert an item into the list at the given index.          \
 * @param source List to insert the element into.                   \
 * @param item Item to insert into the list.                        \
 * @param index Index to insert the item at.                        \
 */                                                                 \
void List_##T##_Insert(List_##T source, T item, int index);         \
/**                                                                 \
 * @brief Performs an action on each element of the list.           \
 * @param source List to execute the action on.                     \
 * @param action Action to execute on each item.                    \
 */                                                                 \
void List_##T##_ForEach(List_##T source, void (*action)(T*));       \
/**                                                                 \
 * @brief Ensures the given list has the capacity                   \
 * to hold a given amount of items.                                 \
 * @param source List to ensure the capacity for.                   \
 * @param capacity Capacity to ensure.                              \
 */                                                                 \
void List_##T##_EnsureCapacity(List_##T source, int capacity);      \
/**                                                                 \
 * @brief Sets the capacity of the list to the amount of elements,  \
 * if that amount is less than 90% of current capacity.             \
 * @param source List to trim the excess of.                        \
 */                                                                 \
void List_##T##_TrimExcess(List_##T source);                        \
/**                                                                 \
 * @brief Sorts the elements of a list according to a comparer.     \
 * @param source List that contains elements to sort.               \
 * @param comparer Comparer to order the elements by.               \
 */                                                                 \
void List_##T##_Sort(List_##T source, int (*comparer)(T, T));

#pragma endregion

#endif