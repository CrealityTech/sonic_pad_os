/*
 * ion_head.h
 *
 * Copyright(c) 2013-2015 Allwinnertech Co., Ltd.
 *      http://www.allwinnertech.com
 *
 * Author: liugang <liugang@allwinnertech.com>
 *
 * sunxi ion test head file
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 */
#ifndef ION_HEAD__H
#define ION_HEAD__H

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/time.h>
#include <time.h>
#include <sys/mman.h>
#include <sys/mman.h>

typedef int ion_user_handle_t;

/**
 * enum ion_heap_types - list of all possible types of heaps
 * @ION_HEAP_TYPE_SYSTEM:	 memory allocated via vmalloc
 * @ION_HEAP_TYPE_SYSTEM_CONTIG: memory allocated via kmalloc
 * @ION_HEAP_TYPE_CARVEOUT:	 memory allocated from a prereserved
 *				 carveout heap, allocations are physically
 *				 contiguous
 * @ION_HEAP_TYPE_DMA:		 memory allocated via DMA API
 * @ION_NUM_HEAPS:		 helper for iterating over heaps, a bit mask
 *				 is used to identify the heaps, so only 32
 *				 total heap types are supported
 */
enum ion_heap_type {
	ION_HEAP_TYPE_SYSTEM,
	ION_HEAP_TYPE_SYSTEM_CONTIG,
	ION_HEAP_TYPE_CARVEOUT,
	ION_HEAP_TYPE_CHUNK,
	ION_HEAP_TYPE_DMA,
	ION_HEAP_TYPE_CUSTOM,
	ION_HEAP_TYPE_SECURE, /* allwinner add */
			       /*
			       * must be last so device specific heaps always
			       * are at the end of this enum
			       */
};

#define ION_HEAP_SYSTEM_MASK		(1 << ION_HEAP_TYPE_SYSTEM)
#define ION_HEAP_SYSTEM_CONTIG_MASK	(1 << ION_HEAP_TYPE_SYSTEM_CONTIG)
#define ION_HEAP_CARVEOUT_MASK		(1 << ION_HEAP_TYPE_CARVEOUT)
#define ION_HEAP_TYPE_DMA_MASK		(1 << ION_HEAP_TYPE_DMA)
#define ION_NUM_HEAP_IDS		(sizeof(unsigned int) * 8)

/**
 * allocation flags - the lower 16 bits are used by core ion, the upper 16
 * bits are reserved for use by the heaps themselves.
 */

/*
 * mappings of this buffer should be cached, ion will do cache maintenance
 * when the buffer is mapped for dma
 */

#define ION_FLAG_CACHED 1

/*
 * mappings of this buffer will created at mmap time, if this is set
 * caches must be managed manually
 */
#define ION_FLAG_CACHED_NEEDS_SYNC 2

/**
 * DOC: Ion Userspace API
 *
 * create a client by opening /dev/ion
 * most operations handled via following ioctls
 *
 */

/**
 * struct ion_allocation_data - metadata passed from userspace for allocations
 * @len:		size of the allocation
 * @align:		required alignment of the allocation
 * @heap_id_mask:	mask of heap ids to allocate from
 * @flags:		flags passed to heap
 * @handle:		pointer that will be populated with a cookie to use to
 *			refer to this allocation
 *
 * Provided by userspace as an argument to the ioctl
 */
struct ion_allocation_data {
	size_t len;
	size_t align;
	unsigned int heap_id_mask;
	unsigned int flags;
	ion_user_handle_t handle;
};

/**
 * struct ion_fd_data - metadata passed to/from userspace for a handle/fd pair
 * @handle:	a handle
 * @fd:		a file descriptor representing that handle
 *
 * For ION_IOC_SHARE or ION_IOC_MAP userspace populates the handle field with
 * the handle returned from ion alloc, and the kernel returns the file
 * descriptor to share or map in the fd field.  For ION_IOC_IMPORT, userspace
 * provides the file descriptor and the kernel returns the handle.
 */
struct ion_fd_data {
	ion_user_handle_t handle;
	int fd;
};

/**
 * struct ion_handle_data - a handle passed to/from the kernel
 * @handle:	a handle
 */
struct ion_handle_data {
	ion_user_handle_t handle;
};

/**
 * struct ion_custom_data - metadata passed to/from userspace for a custom ioctl
 * @cmd:	the custom ioctl function to call
 * @arg:	additional data to pass to the custom ioctl, typically a user
 *		pointer to a predefined structure
 *
 * This works just like the regular cmd and arg fields of an ioctl.
 */
struct ion_custom_data {
	unsigned int cmd;
	unsigned long arg;
};

#define MAX_HEAP_NAME			32

/**
 * struct ion_heap_data - data about a heap
 * @name - first 32 characters of the heap name
 * @type - heap type
 * @heap_id - heap id for the heap
 */
#if 0
struct ion_heap_data {
	char name[MAX_HEAP_NAME];
	u32 type;
	u32 heap_id;
	u32 reserved0;
	u32 reserved1;
	u32 reserved2;
};
#endif

/**
 * struct ion_heap_query - collection of data about all heaps
 * @cnt - total number of heaps to be copied
 * @heaps - buffer to copy heap data
 */
#if 0
struct ion_heap_query {
	u32 cnt; /* Total number of heaps to be copied */
	u32 reserved0; /* align to 64bits */
	u64 heaps; /* buffer to be populated */
	u32 reserved1;
	u32 reserved2;
};
#endif

#define ION_IOC_MAGIC		'I'
/**
 * DOC: ION_IOC_ALLOC - allocate memory
 *
 * Takes an ion_allocation_data struct and returns it with the handle field
 * populated with the opaque handle for the allocation.
 */
#define ION_IOC_ALLOC		_IOWR(ION_IOC_MAGIC, 0, \
				      struct ion_allocation_data)
/**
 * DOC: ION_IOC_FREE - free memory
 *
 * Takes an ion_handle_data struct and frees the handle.
 */
#define ION_IOC_FREE		_IOWR(ION_IOC_MAGIC, 1, struct ion_handle_data)
/**
 * DOC: ION_IOC_MAP - get a file descriptor to mmap
 *
 * Takes an ion_fd_data struct with the handle field populated with a valid
 * opaque handle.  Returns the struct with the fd field set to a file
 * descriptor open in the current address space.  This file descriptor
 * can then be used as an argument to mmap.
 */
#define ION_IOC_MAP		_IOWR(ION_IOC_MAGIC, 2, struct ion_fd_data)
/**
 * DOC: ION_IOC_SHARE - creates a file descriptor to use to share an allocation
 *
 * Takes an ion_fd_data struct with the handle field populated with a valid
 * opaque handle.  Returns the struct with the fd field set to a file
 * descriptor open in the current address space.  This file descriptor
 * can then be passed to another process.  The corresponding opaque handle can
 * be retrieved via ION_IOC_IMPORT.
 */
#define ION_IOC_SHARE		_IOWR(ION_IOC_MAGIC, 4, struct ion_fd_data)

/**
 * DOC: ION_IOC_IMPORT - imports a shared file descriptor
 *
 * Takes an ion_fd_data struct with the fd field populated with a valid file
 * descriptor obtained from ION_IOC_SHARE and returns the struct with the handle
 * filed set to the corresponding opaque handle.
 */
#define ION_IOC_IMPORT		_IOWR(ION_IOC_MAGIC, 5, struct ion_fd_data)

/**
 * DOC: ION_IOC_SYNC - syncs a shared file descriptors to memory
 *
 * Deprecated in favor of using the dma_buf api's correctly (syncing
 * will happen automatically when the buffer is mapped to a device).
 * If necessary should be used after touching a cached buffer from the cpu,
 * this will make the buffer in memory coherent.
 */
#define ION_IOC_SYNC		_IOWR(ION_IOC_MAGIC, 7, struct ion_fd_data)
/**
 * DOC: ION_IOC_CUSTOM - call architecture specific ion ioctl
 *
 * Takes the argument of the architecture specific ioctl to call and
 * passes appropriate userdata for that ioctl
 */
#define ION_IOC_CUSTOM		_IOWR(ION_IOC_MAGIC, 6, struct ion_custom_data)

/**
 * DOC: ION_IOC_HEAP_QUERY - information about available heaps
 *
 * Takes an ion_heap_query structure and populates information about
 * available Ion heaps.
 */
#define ION_IOC_HEAP_QUERY     _IOWR(ION_IOC_MAGIC, 8, \
					struct ion_heap_query)
					
/* extend cmd of sunxi platform */
#define ION_IOC_SUNXI_FLUSH_RANGE       5
#define ION_IOC_SUNXI_FLUSH_ALL         6
#define ION_IOC_SUNXI_PHYS_ADDR         7
#define ION_IOC_SUNXI_DMA_COPY          8
#define ION_IOC_SUNXI_DUMP              9
#define ION_IOC_SUNXI_TEE_ADDR              17

/* used for ION_IOC_SUNXI_FLUSH_RANGE cmd */
typedef struct {
	long 	start;			/* start virtual address */
	long 	end;			/* end virtual address */
}sunxi_cache_range;

/* used for ION_IOC_SUNXI_PHYS_ADDR cmd */
typedef struct {
	ion_user_handle_t handle;
	unsigned int phys_addr;
	unsigned int size;
}sunxi_phys_data;

#define ION_TEST_ID_BASE                     	 100
#define ION_TEST_FUNC_ALLOC                      (ION_TEST_ID_BASE + 1)
#define ION_TEST_FUNC_FREE                       (ION_TEST_ID_BASE + 2)
#define ION_TEST_FUNC_SHARE                      (ION_TEST_ID_BASE + 3)
#define ION_TEST_FUNC_IMPORT                     (ION_TEST_ID_BASE + 4)
#define ION_TEST_FUNC_SYNC                       (ION_TEST_ID_BASE + 5)
#define ION_TEST_FUNC_FLUSH_RANGE                (ION_TEST_ID_BASE + 6)
#define ION_TEST_FUNC_PHYS_ADDR                  (ION_TEST_ID_BASE + 7)
#define ION_TEST_FUNC_DMA_COPY                   (ION_TEST_ID_BASE + 8)
#define ION_TEST_FUNC_DUMP_CARVEOUT_BITMAP       (ION_TEST_ID_BASE + 9)

int ion_function_alloc(void);
int ion_function_free(void);
int ion_function_share(void);
int ion_function_import(void);
int ion_function_sync(void);
int ion_function_flush_range(void);
int ion_function_phys_addr(void);
int ion_function_dma_copy(void);
int ion_function_dump_carveout_bitmap(void);

#define ION_DEV_NAME		"/dev/ion"

#define SZ_64M			0x04000000
#define SZ_4M			0x00400000
#define SZ_1M			0x00100000
#define SZ_64K			0x00010000
#define SZ_4K			0x00001000
#define ION_ALLOC_SIZE		(SZ_4M + SZ_1M - SZ_64K)
#define ION_ALLOC_ALIGN		(SZ_4K)

#endif /* ION_HEAD__H */
