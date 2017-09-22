/** C++ Default Constructor Removal
 *  ===============================
 *  A macro to disallow the Copy Constructor and Copy Assignment Operator.
 *  This should be used in the `private:` declaration for a class.
 *
 *  NB. If there is a user-defined Copy Ctor (even a deleted one) there will be
 *  no implicitly defined Move Ctor (or Move Assignment Operator), so deleting
 *  those members is, at best, redundant.
 */

#pragma once


#define DISALLOW_COPY_AND_ASSIGN(TypeName)   \
    TypeName(const TypeName&) = delete;      \
    void operator=(const TypeName&) = delete
