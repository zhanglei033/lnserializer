#pragma once
#include "lnassert.h"
#include "lnplatform.h"

struct lnmalloc_info_t
{
    std::atomic_int alloc_count;
    std::atomic_int dealloc_count;
};

static inline LN_NODISCARD lnmalloc_info_t& lnmalloc_info_get_instance() LN_NOEXCEPT
{
    static lnmalloc_info_t info;
    return info;
}

LN_NODISCARD static inline int lnmalloc_alloc_count() LN_NOEXCEPT
{
    return lnmalloc_info_get_instance().alloc_count.load();
}

LN_NODISCARD static inline int lnmalloc_free_count() LN_NOEXCEPT
{
    return lnmalloc_info_get_instance().dealloc_count.load();
}

LN_NODISCARD static inline void* lnmalloc(size_t size) LN_NOEXCEPT
{
    if (size == 0) return nullptr;
    lnmalloc_info_get_instance().alloc_count++;
    void* ptr = malloc(size);
    LN_ASSERT(ptr, "lnmalloc failed.");
    memset(ptr, 0, size);
    return ptr;
}

static inline void xfree(void* ptr) LN_NOEXCEPT
{
    if (ptr)
    {
        free(ptr);
        lnmalloc_info_get_instance().dealloc_count++;
    }
}