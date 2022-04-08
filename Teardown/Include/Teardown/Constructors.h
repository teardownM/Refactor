#pragma once

namespace Teardown::Constructors {
    typedef void *(*tClassConstructor)(void *pAlloc, void *pParent);

    inline tClassConstructor Script;
}