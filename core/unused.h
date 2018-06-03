/** UNUSED
 *  ======
 *  Utility macro for marking variables deliberately-unused. Nixes warnings.
 */

#pragma once

#define UNUSED(_var) do { (void)(true ? (void)0 : ( (void)(_var) ) ); } while(0)
