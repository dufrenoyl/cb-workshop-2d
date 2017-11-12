#pragma once

struct CBCookieRemove
{
    bool success;

    static CBCookieRemove* fromPointer(const void *cookie)
    {
        return const_cast<CBCookieRemove*>(
                    reinterpret_cast<const CBCookieRemove*>(cookie));
    }
};
