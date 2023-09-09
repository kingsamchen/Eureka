
#if defined(_MSC_VER)
#pragma once
#endif

#ifndef COMPILER_HELPER_H_
#define COMPILER_HELPER_H_

#define FRIEND_TEST_WITH_PREFIX(class_name) \
    friend class class_name##Test

#endif  // COMPILER_HELPER_H_