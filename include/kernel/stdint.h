#ifndef KERNEL_STDINT_H
#define KERNEL_STDINT_H

/* Some nice typedefs, to standardise sizes across platforms.
*  These typedefs are written for 32-bit X86. */
/*typedef unsigned long long	uint64_t;
typedef long long		int64_t;
*/typedef unsigned int		uint32_t;
typedef int			int32_t;
typedef unsigned short		uint16_t;
typedef short			int16_t;
typedef unsigned char		uint8_t;
typedef char			int8_t;

#endif
