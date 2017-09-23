/** Strigification
 *  ==============
 *  Macros to make manipulating strings in the preprocessor easier.
 */

#pragma once


/** Symbol Stringifier
 *  ------------------
 *  Uses the preprocessor to create a static string version of the passed symbol
 *  or macro. Usage:
 *      char const* msg = "This is a string literal defined at "
 *                        STRING(__FILE__) ":" STRING(__LINE__);
 */
#define __STRING_SECONDPASS(X) #X
#define STRING(X) __STRING_SECONDPASS(X)

/** Symbol Concatenator
 *  ------------------
 *  Mushes two symbols together into one at the preprocessor level.
 */
#define CONCAT_SYMBOL(a, b) CONCAT_SYMBOL_I(a, b)
#define CONCAT_SYMBOL_I(a, b) CONCAT_SYMBOL_II(~, a ## b)
#define CONCAT_SYMBOL_II(p, res) res
