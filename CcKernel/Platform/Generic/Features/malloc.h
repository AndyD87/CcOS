/*
 * malloc.h
 *
 *  Created on: 10.07.2019
 *      Author: Dirmeier
 */

#ifndef CCOS_CCKERNEL_PLATFORM_GENERIC_MALLOC_H_
#define CCOS_CCKERNEL_PLATFORM_GENERIC_MALLOC_H_

#include "CcBase.h"

#ifndef CCOS_MEMORY_MANAGMENT_MALLOC_ONLY

CCEXTERNC void* malloc(size_t);
CCEXTERNC void free(void*);
CCEXTERNC void __malloc_lock( struct _reent *_r );
CCEXTERNC void __malloc_unlock( struct _reent *_r );

#ifdef __cplusplus
  void* operator new(size_t size);
  void operator delete(void*);
  void* operator new[](size_t size);
  void operator delete[](void*);
#endif

#endif // CCOS_MEMORY_MANAGMENT_MALLOC_ONLY

#endif /* CCOS_CCKERNEL_PLATFORM_GENERIC_MALLOC_H_ */
