#ifndef __ARIES_C_H__
#define __ARIES_C_H__


#ifndef bool
typedef unsigned char bool;
#endif

#ifndef true
#define true    ((bool) 1)
#endif

#ifndef false
#define false   ((bool) 0)
#endif


/*-------------------intN----------------------------*/
typedef signed char int8;               /* == 8 bits */
typedef signed short int16;             /* == 16 bits */
typedef signed int int32;               /* == 32 bits */


/*-------------------uintN----------------------------*/
typedef unsigned char uint8;    /* == 8 bits */
typedef unsigned short uint16;  /* == 16 bits */
typedef unsigned int uint32;    /* == 32 bits */

/*-------------------64 bit----------------------------*/
typedef long long int int64;
typedef unsigned long long int uint64;

#endif
