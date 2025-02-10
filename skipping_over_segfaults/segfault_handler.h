#pragma once
#include <csignal>
#include <setjmp.h>
#include <stdexcept>


/**
 * Please dont use this in actual release software. This is useful for operations that you know can and will
 *  throw segfaults, and you want to write illustrative example programs to show users what *not* to do.
 */
namespace detail
{
    void naive_sigsegv_handler(int)
    {
        throw std::runtime_error("");
    }

    template <typename T>
    bool segfault(T const & callable)
    {
        signal(SIGSEGV, detail::naive_sigsegv_handler);
        sigjmp_buf mark;
        if (sigsetjmp(mark, 1))
        {
            try{ callable(); } catch(...){ siglongjmp(mark,-1); }
            return false;
        }
        return true;
    }
}

