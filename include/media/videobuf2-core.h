/*
 * videobuf2-core.h - V4L2 driver helper framework
 *
 * Copyright (C) 2010 Samsung Electronics
 *
 * Author: Pawel Osciak <pawel@osciak.com>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation.
 */
#ifndef _MEDIA_VIDEOBUF2_CORE_H
#define _MEDIA_VIDEOBUF2_CORE_H

#include <linux/mm_types.h>
#include <linux/mutex.h>
#include <linux/poll.h>
#include <linux/videodev2.h>
#include <linux/dma-buf.h>

struct vb2_alloc_ctx;
struct vb2_fileio_data;

<<<<<<< HEAD
=======
/**
 * struct vb2_mem_ops - memory handling/memory allocator operations
 * @alloc:	allocate video memory and, optionally, allocator private data,
 *		return NULL on failure or a pointer to allocator private,
 *		per-buffer data on success; the returned private structure
 *		will then be passed as buf_priv argument to other ops in this
 *		structure. Additional gfp_flags to use when allocating the
 *		are also passed to this operation. These flags are from the
 *		gfp_flags field of vb2_queue.
 * @put:	inform the allocator that the buffer will no longer be used;
 *		usually will result in the allocator freeing the buffer (if
 *		no other users of this buffer are present); the buf_priv
 *		argument is the allocator private per-buffer structure
 *		previously returned from the alloc callback
 * @get_userptr: acquire userspace memory for a hardware operation; used for
 *		 USERPTR memory types; vaddr is the address passed to the
 *		 videobuf layer when queuing a video buffer of USERPTR type;
 *		 should return an allocator private per-buffer structure
 *		 associated with the buffer on success, NULL on failure;
 *		 the returned private structure will then be passed as buf_priv
 *		 argument to other ops in this structure
 * @put_userptr: inform the allocator that a USERPTR buffer will no longer
 *		 be used
 * @attach_dmabuf: attach a shared struct dma_buf for a hardware operation;
 *		   used for DMABUF memory types; alloc_ctx is the alloc context
 *		   dbuf is the shared dma_buf; returns NULL on failure;
 *		   allocator private per-buffer structure on success;
 *		   this needs to be used for further accesses to the buffer
 * @detach_dmabuf: inform the exporter of the buffer that the current DMABUF
 *		   buffer is no longer used; the buf_priv argument is the
 *		   allocator private per-buffer structure previously returned
 *		   from the attach_dmabuf callback
 * @map_dmabuf: request for access to the dmabuf from allocator; the allocator
 *		of dmabuf is informed that this driver is going to use the
 *		dmabuf
 * @unmap_dmabuf: releases access control to the dmabuf - allocator is notified
 *		  that this driver is done using the dmabuf for now
 * @prepare:	called every time the buffer is passed from userspace to the
 *		driver, useful for cache synchronisation, optional
 * @finish:	called every time the buffer is passed back from the driver
 *		to the userspace, also optional
 * @vaddr:	return a kernel virtual address to a given memory buffer
 *		associated with the passed private structure or NULL if no
 *		such mapping exists
 * @cookie:	return allocator specific cookie for a given memory buffer
 *		associated with the passed private structure or NULL if not
 *		available
 * @num_users:	return the current number of users of a memory buffer;
 *		return 1 if the videobuf layer (or actually the driver using
 *		it) is the only user
 * @mmap:	setup a userspace mapping for a given memory buffer under
 *		the provided virtual memory region
 *
 * Required ops for USERPTR types: get_userptr, put_userptr.
 * Required ops for MMAP types: alloc, put, num_users, mmap.
 * Required ops for read/write access types: alloc, put, num_users, vaddr
 * Required ops for DMABUF types: attach_dmabuf, detach_dmabuf, map_dmabuf,
 *				  unmap_dmabuf.
 */
>>>>>>> common/android-3.10.y
struct vb2_mem_ops {
	void		*(*alloc)(void *alloc_ctx, unsigned long size, gfp_t gfp_flags);
	void		(*put)(void *buf_priv);
	struct dma_buf *(*get_dmabuf)(void *buf_priv);

	void		*(*get_userptr)(void *alloc_ctx, unsigned long vaddr,
					unsigned long size, int write);
	void		(*put_userptr)(void *buf_priv);

	void		(*prepare)(void *buf_priv);
	void		(*finish)(void *buf_priv);

	void		*(*attach_dmabuf)(void *alloc_ctx, struct dma_buf *dbuf,
				unsigned long size, int write);
	void		(*detach_dmabuf)(void *buf_priv);
	int		(*map_dmabuf)(void *buf_priv);
	void		(*unmap_dmabuf)(void *buf_priv);

	void		*(*vaddr)(void *buf_priv);
	void		*(*cookie)(void *buf_priv);

	unsigned int	(*num_users)(void *buf_priv);

	int		(*mmap)(void *buf_priv, struct vm_area_struct *vma);
};

struct vb2_plane {
	void			*mem_priv;
<<<<<<< HEAD
	int			mapped:1;
};

=======
	struct dma_buf		*dbuf;
	unsigned int		dbuf_mapped;
};

/**
 * enum vb2_io_modes - queue access methods
 * @VB2_MMAP:		driver supports MMAP with streaming API
 * @VB2_USERPTR:	driver supports USERPTR with streaming API
 * @VB2_READ:		driver supports read() style access
 * @VB2_WRITE:		driver supports write() style access
 * @VB2_DMABUF:		driver supports DMABUF with streaming API
 */
>>>>>>> common/android-3.10.y
enum vb2_io_modes {
	VB2_MMAP	= (1 << 0),
	VB2_USERPTR	= (1 << 1),
	VB2_READ	= (1 << 2),
	VB2_WRITE	= (1 << 3),
	VB2_DMABUF	= (1 << 4),
};

enum vb2_fileio_flags {
	VB2_FILEIO_READ_ONCE		= (1 << 0),
	VB2_FILEIO_WRITE_IMMEDIATELY	= (1 << 1),
};

enum vb2_buffer_state {
	VB2_BUF_STATE_DEQUEUED,
	VB2_BUF_STATE_QUEUED,
	VB2_BUF_STATE_ACTIVE,
	VB2_BUF_STATE_DONE,
	VB2_BUF_STATE_ERROR,
};

struct vb2_queue;

struct vb2_buffer {
	struct v4l2_buffer	v4l2_buf;
	struct v4l2_plane	v4l2_planes[VIDEO_MAX_PLANES];

	struct vb2_queue	*vb2_queue;

	unsigned int		num_planes;

	enum vb2_buffer_state	state;

	struct list_head	queued_entry;
	struct list_head	done_entry;

	struct vb2_plane	planes[VIDEO_MAX_PLANES];
	unsigned int		num_planes_mapped;
};

struct vb2_ops {
	int (*queue_setup)(struct vb2_queue *q, const struct v4l2_format *fmt,
				unsigned int *num_buffers, unsigned int *num_planes,
				unsigned int sizes[], void *alloc_ctxs[]);

	void (*wait_prepare)(struct vb2_queue *q);
	void (*wait_finish)(struct vb2_queue *q);

	int (*buf_init)(struct vb2_buffer *vb);
	int (*buf_prepare)(struct vb2_buffer *vb);
	int (*buf_finish)(struct vb2_buffer *vb);
	void (*buf_cleanup)(struct vb2_buffer *vb);

	int (*start_streaming)(struct vb2_queue *q, unsigned int count);
	int (*stop_streaming)(struct vb2_queue *q);

	void (*buf_queue)(struct vb2_buffer *vb);
};

<<<<<<< HEAD
=======
struct v4l2_fh;

/**
 * struct vb2_queue - a videobuf queue
 *
 * @type:	queue type (see V4L2_BUF_TYPE_* in linux/videodev2.h
 * @io_modes:	supported io methods (see vb2_io_modes enum)
 * @io_flags:	additional io flags (see vb2_fileio_flags enum)
 * @lock:	pointer to a mutex that protects the vb2_queue struct. The
 *		driver can set this to a mutex to let the v4l2 core serialize
 *		the queuing ioctls. If the driver wants to handle locking
 *		itself, then this should be set to NULL. This lock is not used
 *		by the videobuf2 core API.
 * @owner:	The filehandle that 'owns' the buffers, i.e. the filehandle
 *		that called reqbufs, create_buffers or started fileio.
 *		This field is not used by the videobuf2 core API, but it allows
 *		drivers to easily associate an owner filehandle with the queue.
 * @ops:	driver-specific callbacks
 * @mem_ops:	memory allocator specific callbacks
 * @drv_priv:	driver private data
 * @buf_struct_size: size of the driver-specific buffer structure;
 *		"0" indicates the driver doesn't want to use a custom buffer
 *		structure type, so sizeof(struct vb2_buffer) will is used
 * @gfp_flags:	additional gfp flags used when allocating the buffers.
 *		Typically this is 0, but it may be e.g. GFP_DMA or __GFP_DMA32
 *		to force the buffer allocation to a specific memory zone.
 *
 * @memory:	current memory type used
 * @bufs:	videobuf buffer structures
 * @num_buffers: number of allocated/used buffers
 * @queued_list: list of buffers currently queued from userspace
 * @queued_count: number of buffers owned by the driver
 * @done_list:	list of buffers ready to be dequeued to userspace
 * @done_lock:	lock to protect done_list list
 * @done_wq:	waitqueue for processes waiting for buffers ready to be dequeued
 * @alloc_ctx:	memory type/allocator-specific contexts for each plane
 * @streaming:	current streaming state
 * @waiting_for_buffers: used in poll() to check if vb2 is still waiting for
 *		buffers. Only set for capture queues if qbuf has not yet been
 *		called since poll() needs to return POLLERR in that situation.
 * @fileio:	file io emulator internal data, used only if emulator is active
 */
>>>>>>> common/android-3.10.y
struct vb2_queue {
	enum v4l2_buf_type		type;
	unsigned int			io_modes;
	unsigned int			io_flags;
	struct mutex			*lock;
	struct v4l2_fh			*owner;

	const struct vb2_ops		*ops;
	const struct vb2_mem_ops	*mem_ops;
	void				*drv_priv;
	unsigned int			buf_struct_size;
	u32				timestamp_type;
	gfp_t				gfp_flags;

	enum v4l2_memory		memory;
	struct vb2_buffer		*bufs[VIDEO_MAX_FRAME];
	unsigned int			num_buffers;

	struct list_head		queued_list;

	atomic_t			queued_count;
	struct list_head		done_list;
	spinlock_t			done_lock;
	wait_queue_head_t		done_wq;

	void				*alloc_ctx[VIDEO_MAX_PLANES];
	unsigned int            plane_sizes[VIDEO_MAX_PLANES];

	unsigned int			streaming:1;
	unsigned int			waiting_for_buffers:1;

	struct vb2_fileio_data		*fileio;
};

void *vb2_plane_vaddr(struct vb2_buffer *vb, unsigned int plane_no);
void *vb2_plane_cookie(struct vb2_buffer *vb, unsigned int plane_no);

void vb2_buffer_done(struct vb2_buffer *vb, enum vb2_buffer_state state);
int vb2_wait_for_all_buffers(struct vb2_queue *q);

int vb2_querybuf(struct vb2_queue *q, struct v4l2_buffer *b);
int vb2_reqbufs(struct vb2_queue *q, struct v4l2_requestbuffers *req);

<<<<<<< HEAD
int vb2_queue_init(struct vb2_queue *q);
=======
int vb2_create_bufs(struct vb2_queue *q, struct v4l2_create_buffers *create);
int vb2_prepare_buf(struct vb2_queue *q, struct v4l2_buffer *b);

int __must_check vb2_queue_init(struct vb2_queue *q);
>>>>>>> common/android-3.10.y

void vb2_queue_release(struct vb2_queue *q);

int vb2_qbuf(struct vb2_queue *q, struct v4l2_buffer *b);
int vb2_expbuf(struct vb2_queue *q, struct v4l2_exportbuffer *eb);
int vb2_dqbuf(struct vb2_queue *q, struct v4l2_buffer *b, bool nonblocking);

int vb2_streamon(struct vb2_queue *q, enum v4l2_buf_type type);
int vb2_streamoff(struct vb2_queue *q, enum v4l2_buf_type type);

int vb2_mmap(struct vb2_queue *q, struct vm_area_struct *vma);
unsigned int vb2_poll(struct vb2_queue *q, struct file *file, poll_table *wait);
size_t vb2_read(struct vb2_queue *q, char __user *data, size_t count,
		loff_t *ppos, int nonblock);
size_t vb2_write(struct vb2_queue *q, char __user *data, size_t count,
		loff_t *ppos, int nonblock);

static inline bool vb2_is_streaming(struct vb2_queue *q)
{
	return q->streaming;
}

static inline bool vb2_is_busy(struct vb2_queue *q)
{
	return (q->num_buffers > 0);
}

static inline void *vb2_get_drv_priv(struct vb2_queue *q)
{
	return q->drv_priv;
}

static inline void vb2_set_plane_payload(struct vb2_buffer *vb,
				 unsigned int plane_no, unsigned long size)
{
	if (plane_no < vb->num_planes)
		vb->v4l2_planes[plane_no].bytesused = size;
}

static inline unsigned long vb2_get_plane_payload(struct vb2_buffer *vb,
				 unsigned int plane_no)
{
	if (plane_no < vb->num_planes)
		return vb->v4l2_planes[plane_no].bytesused;
	return 0;
}

static inline unsigned long
vb2_plane_size(struct vb2_buffer *vb, unsigned int plane_no)
{
	if (plane_no < vb->num_planes)
		return vb->v4l2_planes[plane_no].length;
	return 0;
}

<<<<<<< HEAD
#endif 
=======
/*
 * The following functions are not part of the vb2 core API, but are simple
 * helper functions that you can use in your struct v4l2_file_operations,
 * struct v4l2_ioctl_ops and struct vb2_ops. They will serialize if vb2_queue->lock
 * or video_device->lock is set, and they will set and test vb2_queue->owner
 * to check if the calling filehandle is permitted to do the queuing operation.
 */

/* struct v4l2_ioctl_ops helpers */

int vb2_ioctl_reqbufs(struct file *file, void *priv,
			  struct v4l2_requestbuffers *p);
int vb2_ioctl_create_bufs(struct file *file, void *priv,
			  struct v4l2_create_buffers *p);
int vb2_ioctl_prepare_buf(struct file *file, void *priv,
			  struct v4l2_buffer *p);
int vb2_ioctl_querybuf(struct file *file, void *priv, struct v4l2_buffer *p);
int vb2_ioctl_qbuf(struct file *file, void *priv, struct v4l2_buffer *p);
int vb2_ioctl_dqbuf(struct file *file, void *priv, struct v4l2_buffer *p);
int vb2_ioctl_streamon(struct file *file, void *priv, enum v4l2_buf_type i);
int vb2_ioctl_streamoff(struct file *file, void *priv, enum v4l2_buf_type i);
int vb2_ioctl_expbuf(struct file *file, void *priv,
	struct v4l2_exportbuffer *p);

/* struct v4l2_file_operations helpers */

int vb2_fop_mmap(struct file *file, struct vm_area_struct *vma);
int vb2_fop_release(struct file *file);
ssize_t vb2_fop_write(struct file *file, char __user *buf,
		size_t count, loff_t *ppos);
ssize_t vb2_fop_read(struct file *file, char __user *buf,
		size_t count, loff_t *ppos);
unsigned int vb2_fop_poll(struct file *file, poll_table *wait);
#ifndef CONFIG_MMU
unsigned long vb2_fop_get_unmapped_area(struct file *file, unsigned long addr,
		unsigned long len, unsigned long pgoff, unsigned long flags);
#endif

/* struct vb2_ops helpers, only use if vq->lock is non-NULL. */

void vb2_ops_wait_prepare(struct vb2_queue *vq);
void vb2_ops_wait_finish(struct vb2_queue *vq);

#endif /* _MEDIA_VIDEOBUF2_CORE_H */
>>>>>>> common/android-3.10.y
