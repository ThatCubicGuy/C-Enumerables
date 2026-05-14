#ifndef COLLECTIONS_GENERIC_HASH_SET
#define COLLECTIONS_GENERIC_HASH_SET

#define HashSet(T) _HashSet_##T

#pragma region Define

#define IEqualityComparer(T) _IEqualityComparer_##T
#define EQUALITY_COMPARER_DEFINE(T)         \
typedef struct tag_IEqualityComparer_##T {  \
    bool (*Equals)(T, T);                   \
    size_t (*GetHashCode)(T);               \
} *IEqualityComparer(T);

#define IComparer(T) _IComparer_##T
#define COMPARER_DEFINE(T)          \
typedef struct tag_IComparer_##T {  \
    int (*Compare)(T, T);           \
} *IComparer(T);

#define MAX_HASH_SET_ARRAY_LENGTH 64
#define HASH_SET_DEFINE(T)                                          \
typedef struct HashSetEntry_##T##_s *HashSetEntry_##T;              \
typedef struct HashSet_##T##_s {                                    \
    struct _IEnumerable_##T##_s _parent;                            \
    int Count;                                                      \
    IEqualityComparer(T) Comparer;                                  \
    HashSetEntry_##T Values[MAX_HASH_SET_ARRAY_LENGTH];             \
} *HashSet(T);                                                      \
/**                                                                 \
 * @brief Creates a new HashSet with the given capacity.            \
 * @param capacity Initial capacity for the created list.           \
 * @return A new HashSet with the given capacity.                   \
 */                                                                 \
HashSet(T) new(HashSet(T))(IEqualityComparer(T) comparer);          \
/**                                                                 \
 * @brief Frees all memory relating to a hash set.                  \
 * @param source HashSet to destroy.                                \
 */                                                                 \
void HashSet_##T##_Destroy(HashSet(T)* source);                     \
/**                                                                 \
 * @brief Creates a HashSet from an IEnumerable.                    \
 * @param source Enumerable to take items from.                     \
 * @return A new hash set.                                          \
 */                                                                 \
HashSet(T) Enumerable_##T##_ToHashSet(IEnumerable(T) source);       \
/**                                                                 \
 * @brief Add an element to the HashSet<T>.                         \
 * @param list HashSet to add an element to.                        \
 * @param item Item to add to the set.                              \
 */                                                                 \
void HashSet_##T##_Add(HashSet(T) source, T item);                  \
/**                                                                 \
 * @brief Removes an element from the list.                         \
 * @param source HashSet to remove the element from.                \
 * @param item Item to remove from the list.                        \
 */                                                                 \
bool HashSet_##T##_Remove(HashSet(T) source, T item);               \
/**                                                                 \
 * @brief Removes all elements from the list that match a condition.\
 * @param source HashSet to remove the elements from.               \
 * @param item Condition for removing items from the list.          \
 */                                                                 \
void HashSet_##T##_RemoveWhere(HashSet(T) source, bool (*predicate)(T));\
/**                                                                 \
 * @brief Determines whether the given set contains a given element.\
 * @param source HashSet to check.                                  \
 * @param item Item to search for in the set.                       \
 */                                                                 \
bool HashSet_##T##_Contains(HashSet(T) source, T item);             \
/**                                                                 \
 * @brief Removes all elements from the list.                       \
 * @param source HashSet to clear.                                  \
 */                                                                 \
void HashSet_##T##_Clear(HashSet(T) source);                        \
/**                                                                 \
 * @brief Removes all elements in the specified                     \
 * collection from the current HashSet<T>.                          \
 * @param source HashSet to modify.                                 \
 * @param items Items to remove from the set.                       \
 */                                                                 \
void HashSet_##T##_ExceptWith(HashSet(T) source, IEnumerable(T) items);

#pragma endregion

#endif