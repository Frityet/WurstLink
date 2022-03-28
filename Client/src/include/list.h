//
// Created by Frityet on 2022-02-16.
//

#pragma once

#ifdef __cplusplus
#error C only header!
#endif

#include <stddef.h>
#include <stdint.h>
#include <string.h>
#include <stdlib.h>

#include "wurstlink_types.h"
#include "utilities.h"

#define ATTRIBUTE(...) __attribute__((__VA_ARGS__))

//May god forgive me for this
#define _LIST_1(type, ...)                                              new_list(2, sizeof(type), NULL)
#define _LIST_2(type, dtor)                                             new_list(2, sizeof(type), dtor)
#define _LIST_3(type, dtor, count)                                      new_list(count, sizeof(type), dtor)
//#define _LIST_2(type, count)                                        new_list(count, sizeof(type), NULL)
//#define _LIST_3(type, count, dtor)                                  new_list(count, sizeof(type), dtor)
#define _LIST_REDIRECTOR(n, type, dtor_or_count, func, FUNC, ...)   FUNC

/**
 * @brief Creates a list.
 * @param type      Type to be used with this list.
 * @param dtor      Function to be called on every item of the list once deallocation is to occur. Defaults to @c NULL.
 * @param count     Amount to preallocate with this call. Defaults to 2.
 * @returns Pointer to the allocated list.
 *
 * @example
 * @code
 * int *list1 = LIST(int);
 * MyType_t *list2 = LIST(MyType_t, mytype_deallocator);
 * uint64_t *list3 = LIST(MyType_t, NULL, 1024);
 * @endcode
 */
#define LIST(...) _LIST_REDIRECTOR(, ##__VA_ARGS__, _LIST_3(__VA_ARGS__), _LIST_2(__VA_ARGS__), _LIST_1(__VA_ARGS__))

/**
 * @brief Function type for freeing a list.
 * @param ptr pointer to the item in the list.
 */
typedef void FreeItem_f(void *ptr);

/**
 * @brief Holds all the information about the list.
 */
struct ListData {
    /**
     * @brief Total typesize of the list.
     */
    size_t  size;

    /**
     * @brief Size of the type.
     */
    size_t typesize;

    /**
     * @brief Amount items currently in this list.
     */
    size_t count;

    /**
     * @brief Max amount items this list can hold.
     */
    size_t max;

    /**
     * @brief Function to be called on every item in the list on deallocation of the list.
     */
    FreeItem_f  *free_item;

    /**
     * @brief Pointer to the raw data the list holds.
     */
    void        *data;
};

/**
 * @brief Gets the data header of the specified list.
 * @param ptr List to get the header of.
 * @return the header (ListData), or NULL if the pointer is not a list.
 */
static inline struct ListData *dataof_list(void *ptr)
{
    if (ptr == NULL) {
        return NULL;
    }
    struct ListData *list = ptr - sizeof(struct ListData);
    return list->data == ptr ? list : NULL;
}

/**
 * @brief Creates a new list
 * @param count     Amount of items to preallocate.
 * @param size      Size of each item.
 * @param free_item Function, which will be called upon deallocation on each item of the list.
 * @return
 */
ATTRIBUTE(used)
static void *new_list(size_t count, size_t size, FreeItem_f free_item)
{
    size_t total = count * size;
    struct ListData *list = malloc(total + sizeof(struct ListData));
    if (list == NULL) {
        return NULL;
    }
    list->size = total;
    list->typesize = size;
    list->count = 0;
    list->max = count;
    list->free_item = free_item;
    list->data = list + 1;
    return list->data;
}

/**
 * @brief Gets the total typesize of the list.
 * @param ptr Pointer to the list.
 * @return Size of the list, or @c NULL if the list pointer is invalid.
 */
static inline size_t sizeof_list(void *ptr)
{
    if (ptr == NULL) {
        return 0;
    }
    struct ListData *list = dataof_list(ptr);
    return list == NULL ? 0 : list->size;
}

/**
 * @brief Gets the amount of items in the list.
 * @param ptr Pointer to the list.
 * @return Amount of items in the list, or @c NULL if the list pointer is invalid.
 */
static inline size_t lengthof_list(void *ptr)
{
    if (ptr == NULL) {
        return 0;
    }
    struct ListData *list = dataof_list(ptr);
    return list == NULL ? 0 : list->count;
}

/**
 * @brief Gets the typesize of the type of one item in the list.
 * @param ptr Pointer to the list.
 * @return Typesize of one item in the list, or @c NULL if the list pointer is invalid.
 */
static inline size_t typesizeof_list(void *ptr)
{
    if (ptr == NULL) {
        return 0;
    }
    struct ListData *list = dataof_list(ptr);
    return list == NULL ? 0 : list->typesize;
}

/**
 * @brief Gets the deallocator function of the list.
 * @param ptr Pointer to the list.
 * @return Deallocator of the list, or @c NULL if the list pointer is invalid.
 */
static inline FreeItem_f *deallocatorof_list(void *ptr)
{
    if (ptr == NULL) {
        return NULL;
    }
    struct ListData *list = dataof_list(ptr);
    return list == NULL ? NULL : list->free_item;
}

/**
 * @brief Frees the specified list.
 * @param ptr Pointer to the list to be freed.
 */
static void free_list(void *ptr)
{
    struct ListData *ld = dataof_list(ptr);
    if (ld == NULL) {
        return;
    }

    FreeItem_f *free_item = ld->free_item;
    if (free_item != NULL) {
        for (size_t i = 0; i < ld->count; ++i) {
            free_item(ptr + (ld->typesize * i));
        }
    }

    free(dataof_list(ptr));
}

/**
 * @brief Exponentially increases the typesize of the list.
 * @param ptr Address of the pointer to the list to grow.
 * @return Pointer to the new, grown, list (the pointer is also set to this value) or NULL if it could not be grown.
 */
ATTRIBUTE(used)
static void *grow_list(void **ptr)
{
    struct ListData *list = dataof_list(*ptr);
    if (list == NULL) {
//        fprintf(stderr, "List is null? (arg: %p, ptr: %p, list: %p)\n", ptr, *ptr, list);
        return NULL;
    }

    void *newlist = new_list(list->max * list->max, list->typesize, list->free_item);
    if (newlist == NULL) {
        return NULL;
    }

    /*
    Yes, this ACTUALLY worked
    for (size_t i = 0; i < list->count; ++i) {
        *((uint64_t *)(newlist + i * list->typesize)) = *((uint64_t *)(list->data + i * list->typesize));
    }
    */

    memcpy(newlist, list->data, list->count * list->typesize);

    dataof_list(newlist)->count = list->count;

    free_list(list->data);
    list->data = newlist;
    return list->data;
}

#define PUSH_LIST(_list, _item) push_to_list(&(_list), &(_item))

/**
 * @brief Adds an item to the list.
 * @param arg Address of the pointer to the list.
 * @param item Address of the item to add to the list.
 * @return True if successfully added to the list, false otherwise.
 *
 * @example
 * @code
 * int *list = LIST(int);
 * push_to_list(&list, ADDRESS_OF(10));
 * push_to_list(&list, ADDRESS_OF(4));
 * @endcode
 */
ATTRIBUTE(used)
static bool push_to_list(void *arg, void *item)
{
    void **ptr = (void **)arg;
    struct ListData *list = dataof_list(*ptr);
    if (list == NULL) {
        fprintf(stderr, "List is null?\n(ptr: %p, item: %p, list: %p)\n", *ptr, item, list);
        return false;
    }

    if (list->count + 1 >= list->max) {
//        printf("Growing list (count: %zu, max: %zu)\n", list->count, list->max);
        if ((list = dataof_list(grow_list(&list->data))) == NULL) {
//            fprintf(stderr, "Couldnt grow list?\n(ptr: %p, item: %p, list: %p)\n", *ptr, item, list);
            return false;
        }
//        printf("List grown to (count: %zu, max: %zu)\n", list->count, list->max);
    }

    memcpy(list->data + (list->count * list->typesize), item, list->typesize);
    list->count++;
    *ptr = list->data;
//    printf("Pushed item to list: (ptr: %p, item: %p, count: %zu)\n", *ptr, item, list->count);
    return true;
}

ATTRIBUTE(unused)
static void *copy_list(void *src)
{
    struct ListData *srcdata = dataof_list(src);
    if (srcdata == NULL) {
        return NULL;
    }

    void *newlist = new_list(srcdata->count, srcdata->typesize, srcdata->free_item);
    if (newlist == NULL) {
        return NULL;
    }

    memcpy(newlist, srcdata->data, srcdata->count * srcdata->typesize);

    return newlist;
}

#define REMOVE_ITEM(_list, _index) remove_from_list(&(_list), _index)
ATTRIBUTE(used)
static void remove_from_list(void *ptr, size_t index)
{
    void **list = (void **)ptr;
    size_t  len     = lengthof_list(*list),
        size    = typesizeof_list(*list);
    if (len == 0 || size == 0)
        return;

    void *new = new_list(len - 1, size, deallocatorof_list(*list));
    for (size_t i = 0; i < len; i++) {
        if (index == i)
            continue;
        push_to_list(&new, (*list) + i);
    }

    *list = new;
}

static void *list_to_array(void *ptr)
{
    struct ListData *list_data = dataof_list(ptr);

    int size = list_data->typesize * list_data->count;
    void *mem = malloc(size);
    if (mem == NULL)
        return NULL;

    memcpy(mem, list_data->data, size);
    return mem;
}
