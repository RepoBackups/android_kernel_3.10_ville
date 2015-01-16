/*
 * videobuf2-core.c - V4L2 driver helper framework
 *
 * Copyright (C) 2010 Samsung Electronics
 *
 * Author: Pawel Osciak <pawel@osciak.com>
 *	   Marek Szyprowski <m.szyprowski@samsung.com>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation.
 */

#include <linux/err.h>
#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/mm.h>
#include <linux/poll.h>
#include <linux/slab.h>
#include <linux/sched.h>

#include <media/v4l2-dev.h>
#include <media/v4l2-fh.h>
#include <media/v4l2-event.h>
#include <media/videobuf2-core.h>

static int debug;
module_param(debug, int, 0644);

#define dprintk(level, fmt, arg...)					\
	do {								\
		if (debug >= level)					\
			printk(KERN_DEBUG "vb2: " fmt, ## arg);		\
	} while (0)

#define call_memop(q, plane, op, args...)				\
	(((q)->mem_ops->op) ?						\
		((q)->mem_ops->op(args)) : 0)

#define call_qop(q, op, args...)					\
	(((q)->ops->op) ? ((q)->ops->op(args)) : 0)

<<<<<<< HEAD:drivers/media/video/videobuf2-core.c
#define V4L2_BUFFER_STATE_FLAGS	(V4L2_BUF_FLAG_MAPPED | V4L2_BUF_FLAG_QUEUED | \
				 V4L2_BUF_FLAG_DONE | V4L2_BUF_FLAG_ERROR)
=======
#define V4L2_BUFFER_MASK_FLAGS	(V4L2_BUF_FLAG_MAPPED | V4L2_BUF_FLAG_QUEUED | \
				 V4L2_BUF_FLAG_DONE | V4L2_BUF_FLAG_ERROR | \
				 V4L2_BUF_FLAG_PREPARED | \
				 V4L2_BUF_FLAG_TIMESTAMP_MASK)
>>>>>>> common/android-3.10.y:drivers/media/v4l2-core/videobuf2-core.c

static int __vb2_buf_mem_alloc(struct vb2_buffer *vb,
				unsigned long *plane_sizes)
{
	struct vb2_queue *q = vb->vb2_queue;
	void *mem_priv;
	int plane;

<<<<<<< HEAD:drivers/media/video/videobuf2-core.c
	
	for (plane = 0; plane < vb->num_planes; ++plane) {
		mem_priv = call_memop(q, plane, alloc, q->alloc_ctx[plane],
					plane_sizes[plane]);
=======
	/*
	 * Allocate memory for all planes in this buffer
	 * NOTE: mmapped areas should be page aligned
	 */
	for (plane = 0; plane < vb->num_planes; ++plane) {
		unsigned long size = PAGE_ALIGN(q->plane_sizes[plane]);

		mem_priv = call_memop(q, alloc, q->alloc_ctx[plane],
				      size, q->gfp_flags);
>>>>>>> common/android-3.10.y:drivers/media/v4l2-core/videobuf2-core.c
		if (IS_ERR_OR_NULL(mem_priv))
			goto free;

		
		vb->planes[plane].mem_priv = mem_priv;
		vb->v4l2_planes[plane].length = plane_sizes[plane];
	}

	return 0;
free:
	
	for (; plane > 0; --plane)
		call_memop(q, plane, put, vb->planes[plane - 1].mem_priv);

	return -ENOMEM;
}

static void __vb2_buf_mem_free(struct vb2_buffer *vb)
{
	struct vb2_queue *q = vb->vb2_queue;
	unsigned int plane;

	for (plane = 0; plane < vb->num_planes; ++plane) {
		call_memop(q, plane, put, vb->planes[plane].mem_priv);
		vb->planes[plane].mem_priv = NULL;
		dprintk(3, "Freed plane %d of buffer %d\n",
				plane, vb->v4l2_buf.index);
	}
}

static void __vb2_buf_userptr_put(struct vb2_buffer *vb)
{
	struct vb2_queue *q = vb->vb2_queue;
	unsigned int plane;

	for (plane = 0; plane < vb->num_planes; ++plane) {
		void *mem_priv = vb->planes[plane].mem_priv;

		if (mem_priv) {
			call_memop(q, plane, put_userptr, mem_priv);
			vb->planes[plane].mem_priv = NULL;
		}
	}
}

<<<<<<< HEAD:drivers/media/video/videobuf2-core.c
static void __setup_offsets(struct vb2_queue *q)
=======
/**
 * __vb2_plane_dmabuf_put() - release memory associated with
 * a DMABUF shared plane
 */
static void __vb2_plane_dmabuf_put(struct vb2_queue *q, struct vb2_plane *p)
{
	if (!p->mem_priv)
		return;

	if (p->dbuf_mapped)
		call_memop(q, unmap_dmabuf, p->mem_priv);

	call_memop(q, detach_dmabuf, p->mem_priv);
	dma_buf_put(p->dbuf);
	memset(p, 0, sizeof(*p));
}

/**
 * __vb2_buf_dmabuf_put() - release memory associated with
 * a DMABUF shared buffer
 */
static void __vb2_buf_dmabuf_put(struct vb2_buffer *vb)
{
	struct vb2_queue *q = vb->vb2_queue;
	unsigned int plane;

	for (plane = 0; plane < vb->num_planes; ++plane)
		__vb2_plane_dmabuf_put(q, &vb->planes[plane]);
}

/**
 * __setup_offsets() - setup unique offsets ("cookies") for every plane in
 * every buffer on the queue
 */
static void __setup_offsets(struct vb2_queue *q, unsigned int n)
>>>>>>> common/android-3.10.y:drivers/media/v4l2-core/videobuf2-core.c
{
	unsigned int buffer, plane;
	struct vb2_buffer *vb;
	unsigned long off = 0;

	for (buffer = 0; buffer < q->num_buffers; ++buffer) {
		vb = q->bufs[buffer];
		if (!vb)
			continue;

		for (plane = 0; plane < vb->num_planes; ++plane) {
			vb->v4l2_planes[plane].m.mem_offset = off;

			dprintk(3, "Buffer %d, plane %d offset 0x%08lx\n",
					buffer, plane, off);

			off += vb->v4l2_planes[plane].length;
			off = PAGE_ALIGN(off);
		}
	}
}

static int __vb2_queue_alloc(struct vb2_queue *q, enum v4l2_memory memory,
			     unsigned int num_buffers, unsigned int num_planes,
			     unsigned long plane_sizes[])
{
	unsigned int buffer;
	struct vb2_buffer *vb;
	int ret;

	for (buffer = 0; buffer < num_buffers; ++buffer) {
		
		vb = kzalloc(q->buf_struct_size, GFP_KERNEL);
		if (!vb) {
			dprintk(1, "Memory alloc for buffer struct failed\n");
			break;
		}

		
		if (V4L2_TYPE_IS_MULTIPLANAR(q->type))
			vb->v4l2_buf.length = num_planes;

		vb->state = VB2_BUF_STATE_DEQUEUED;
		vb->vb2_queue = q;
		vb->num_planes = num_planes;
		vb->v4l2_buf.index = buffer;
		vb->v4l2_buf.type = q->type;
		vb->v4l2_buf.memory = memory;

		
		if (memory == V4L2_MEMORY_MMAP) {
			ret = __vb2_buf_mem_alloc(vb, plane_sizes);
			if (ret) {
				dprintk(1, "Failed allocating memory for "
						"buffer %d\n", buffer);
				kfree(vb);
				break;
			}
			ret = call_qop(q, buf_init, vb);
			if (ret) {
				dprintk(1, "Buffer %d %p initialization"
					" failed\n", buffer, vb);
				__vb2_buf_mem_free(vb);
				kfree(vb);
				break;
			}
		}

		q->bufs[buffer] = vb;
	}

	q->num_buffers = buffer;

	__setup_offsets(q);

	dprintk(1, "Allocated %d buffers, %d plane(s) each\n",
			q->num_buffers, num_planes);

	return buffer;
}

static void __vb2_free_mem(struct vb2_queue *q)
{
	unsigned int buffer;
	struct vb2_buffer *vb;

	for (buffer = 0; buffer < q->num_buffers; ++buffer) {
		vb = q->bufs[buffer];
		if (!vb)
			continue;

		
		if (q->memory == V4L2_MEMORY_MMAP)
			__vb2_buf_mem_free(vb);
		else if (q->memory == V4L2_MEMORY_DMABUF)
			__vb2_buf_dmabuf_put(vb);
		else
			__vb2_buf_userptr_put(vb);
	}
}

static void __vb2_queue_free(struct vb2_queue *q)
{
	unsigned int buffer;

	
	if (q->ops->buf_cleanup) {
		for (buffer = 0; buffer < q->num_buffers; ++buffer) {
			if (NULL == q->bufs[buffer])
				continue;
			q->ops->buf_cleanup(q->bufs[buffer]);
		}
	}

	
	__vb2_free_mem(q);

	
	for (buffer = 0; buffer < q->num_buffers; ++buffer) {
		kfree(q->bufs[buffer]);
		q->bufs[buffer] = NULL;
	}

	q->num_buffers = 0;
	q->memory = 0;
}

static int __verify_planes_array(struct vb2_buffer *vb, struct v4l2_buffer *b)
{
<<<<<<< HEAD:drivers/media/video/videobuf2-core.c
	
=======
	if (!V4L2_TYPE_IS_MULTIPLANAR(b->type))
		return 0;

	/* Is memory for copying plane information present? */
>>>>>>> common/android-3.10.y:drivers/media/v4l2-core/videobuf2-core.c
	if (NULL == b->m.planes) {
		dprintk(1, "Multi-planar buffer passed but "
			   "planes array not provided\n");
		return -EINVAL;
	}

	if (b->length < vb->num_planes || b->length > VIDEO_MAX_PLANES) {
		dprintk(1, "Incorrect planes array length, "
			   "expected %d, got %d\n", vb->num_planes, b->length);
		return -EINVAL;
	}

	return 0;
}

<<<<<<< HEAD:drivers/media/video/videobuf2-core.c
static int __fill_v4l2_buffer(struct vb2_buffer *vb, struct v4l2_buffer *b)
{
	struct vb2_queue *q = vb->vb2_queue;
	int ret = 0;

	
=======
/**
 * __buffer_in_use() - return true if the buffer is in use and
 * the queue cannot be freed (by the means of REQBUFS(0)) call
 */
static bool __buffer_in_use(struct vb2_queue *q, struct vb2_buffer *vb)
{
	unsigned int plane;
	for (plane = 0; plane < vb->num_planes; ++plane) {
		void *mem_priv = vb->planes[plane].mem_priv;
		/*
		 * If num_users() has not been provided, call_memop
		 * will return 0, apparently nobody cares about this
		 * case anyway. If num_users() returns more than 1,
		 * we are not the only user of the plane's memory.
		 */
		if (mem_priv && call_memop(q, num_users, mem_priv) > 1)
			return true;
	}
	return false;
}

/**
 * __buffers_in_use() - return true if any buffers on the queue are in use and
 * the queue cannot be freed (by the means of REQBUFS(0)) call
 */
static bool __buffers_in_use(struct vb2_queue *q)
{
	unsigned int buffer;
	for (buffer = 0; buffer < q->num_buffers; ++buffer) {
		if (__buffer_in_use(q, q->bufs[buffer]))
			return true;
	}
	return false;
}

/**
 * __fill_v4l2_buffer() - fill in a struct v4l2_buffer with information to be
 * returned to userspace
 */
static void __fill_v4l2_buffer(struct vb2_buffer *vb, struct v4l2_buffer *b)
{
	struct vb2_queue *q = vb->vb2_queue;

	/* Copy back data such as timestamp, flags, etc. */
>>>>>>> common/android-3.10.y:drivers/media/v4l2-core/videobuf2-core.c
	memcpy(b, &vb->v4l2_buf, offsetof(struct v4l2_buffer, m));
	b->reserved2 = vb->v4l2_buf.reserved2;
	b->reserved = vb->v4l2_buf.reserved;

	if (V4L2_TYPE_IS_MULTIPLANAR(q->type)) {
<<<<<<< HEAD:drivers/media/video/videobuf2-core.c
		ret = __verify_planes_array(vb, b);
		if (ret)
			return ret;

=======
		/*
		 * Fill in plane-related data if userspace provided an array
		 * for it. The caller has already verified memory and size.
		 */
		b->length = vb->num_planes;
>>>>>>> common/android-3.10.y:drivers/media/v4l2-core/videobuf2-core.c
		memcpy(b->m.planes, vb->v4l2_planes,
			b->length * sizeof(struct v4l2_plane));
	} else {
		b->length = vb->v4l2_planes[0].length;
		b->bytesused = vb->v4l2_planes[0].bytesused;
		if (q->memory == V4L2_MEMORY_MMAP)
			b->m.offset = vb->v4l2_planes[0].m.mem_offset;
		else if (q->memory == V4L2_MEMORY_USERPTR)
			b->m.userptr = vb->v4l2_planes[0].m.userptr;
		else if (q->memory == V4L2_MEMORY_DMABUF)
			b->m.fd = vb->v4l2_planes[0].m.fd;
	}

<<<<<<< HEAD:drivers/media/video/videobuf2-core.c
	b->flags &= ~V4L2_BUFFER_STATE_FLAGS;
=======
	/*
	 * Clear any buffer state related flags.
	 */
	b->flags &= ~V4L2_BUFFER_MASK_FLAGS;
	b->flags |= q->timestamp_type;
>>>>>>> common/android-3.10.y:drivers/media/v4l2-core/videobuf2-core.c

	switch (vb->state) {
	case VB2_BUF_STATE_QUEUED:
	case VB2_BUF_STATE_ACTIVE:
		b->flags |= V4L2_BUF_FLAG_QUEUED;
		break;
	case VB2_BUF_STATE_ERROR:
		b->flags |= V4L2_BUF_FLAG_ERROR;
		
	case VB2_BUF_STATE_DONE:
		b->flags |= V4L2_BUF_FLAG_DONE;
		break;
	case VB2_BUF_STATE_DEQUEUED:
		
		break;
	}

	if (vb->num_planes_mapped == vb->num_planes)
		b->flags |= V4L2_BUF_FLAG_MAPPED;
<<<<<<< HEAD:drivers/media/video/videobuf2-core.c

	return ret;
=======
>>>>>>> common/android-3.10.y:drivers/media/v4l2-core/videobuf2-core.c
}

int vb2_querybuf(struct vb2_queue *q, struct v4l2_buffer *b)
{
	struct vb2_buffer *vb;
	int ret;

	if (b->type != q->type) {
		dprintk(1, "querybuf: wrong buffer type\n");
		return -EINVAL;
	}

	if (b->index >= q->num_buffers) {
		dprintk(1, "querybuf: buffer index out of range\n");
		return -EINVAL;
	}
	vb = q->bufs[b->index];
	ret = __verify_planes_array(vb, b);
	if (!ret)
		__fill_v4l2_buffer(vb, b);
	return ret;
}
EXPORT_SYMBOL(vb2_querybuf);

static int __verify_userptr_ops(struct vb2_queue *q)
{
	if (!(q->io_modes & VB2_USERPTR) || !q->mem_ops->get_userptr ||
	    !q->mem_ops->put_userptr)
		return -EINVAL;

	return 0;
}

static int __verify_mmap_ops(struct vb2_queue *q)
{
	if (!(q->io_modes & VB2_MMAP) || !q->mem_ops->alloc ||
	    !q->mem_ops->put || !q->mem_ops->mmap)
		return -EINVAL;

	return 0;
}

<<<<<<< HEAD:drivers/media/video/videobuf2-core.c
static bool __buffers_in_use(struct vb2_queue *q)
{
	unsigned int buffer, plane;
	struct vb2_buffer *vb;

	for (buffer = 0; buffer < q->num_buffers; ++buffer) {
		vb = q->bufs[buffer];
		for (plane = 0; plane < vb->num_planes; ++plane) {
			if (call_memop(q, plane, num_users,
					vb->planes[plane].mem_priv) > 1)
				return true;
		}
	}

	return false;
}

int vb2_reqbufs(struct vb2_queue *q, struct v4l2_requestbuffers *req)
{
	unsigned int num_buffers, num_planes;
	unsigned long plane_sizes[VIDEO_MAX_PLANES];
	int ret = 0;
	
	num_buffers = 0;
	num_planes = 0;

	if (q->fileio) {
		dprintk(1, "reqbufs: file io in progress\n");
		return -EBUSY;
	}

	if (req->memory != V4L2_MEMORY_MMAP
			&& req->memory != V4L2_MEMORY_USERPTR) {
		dprintk(1, "reqbufs: unsupported memory type\n");
		return -EINVAL;
	}

	if (req->type != q->type) {
		dprintk(1, "reqbufs: requested type is incorrect\n");
		return -EINVAL;
	}
=======
/**
 * __verify_dmabuf_ops() - verify that all memory operations required for
 * DMABUF queue type have been provided
 */
static int __verify_dmabuf_ops(struct vb2_queue *q)
{
	if (!(q->io_modes & VB2_DMABUF) || !q->mem_ops->attach_dmabuf ||
	    !q->mem_ops->detach_dmabuf  || !q->mem_ops->map_dmabuf ||
	    !q->mem_ops->unmap_dmabuf)
		return -EINVAL;

	return 0;
}

/**
 * __verify_memory_type() - Check whether the memory type and buffer type
 * passed to a buffer operation are compatible with the queue.
 */
static int __verify_memory_type(struct vb2_queue *q,
		enum v4l2_memory memory, enum v4l2_buf_type type)
{
	if (memory != V4L2_MEMORY_MMAP && memory != V4L2_MEMORY_USERPTR &&
	    memory != V4L2_MEMORY_DMABUF) {
		dprintk(1, "reqbufs: unsupported memory type\n");
		return -EINVAL;
	}

	if (type != q->type) {
		dprintk(1, "reqbufs: requested type is incorrect\n");
		return -EINVAL;
	}

	/*
	 * Make sure all the required memory ops for given memory type
	 * are available.
	 */
	if (memory == V4L2_MEMORY_MMAP && __verify_mmap_ops(q)) {
		dprintk(1, "reqbufs: MMAP for current setup unsupported\n");
		return -EINVAL;
	}

	if (memory == V4L2_MEMORY_USERPTR && __verify_userptr_ops(q)) {
		dprintk(1, "reqbufs: USERPTR for current setup unsupported\n");
		return -EINVAL;
	}

	if (memory == V4L2_MEMORY_DMABUF && __verify_dmabuf_ops(q)) {
		dprintk(1, "reqbufs: DMABUF for current setup unsupported\n");
		return -EINVAL;
	}

	/*
	 * Place the busy tests at the end: -EBUSY can be ignored when
	 * create_bufs is called with count == 0, but count == 0 should still
	 * do the memory and type validation.
	 */
	if (q->fileio) {
		dprintk(1, "reqbufs: file io in progress\n");
		return -EBUSY;
	}
	return 0;
}

/**
 * __reqbufs() - Initiate streaming
 * @q:		videobuf2 queue
 * @req:	struct passed from userspace to vidioc_reqbufs handler in driver
 *
 * Should be called from vidioc_reqbufs ioctl handler of a driver.
 * This function:
 * 1) verifies streaming parameters passed from the userspace,
 * 2) sets up the queue,
 * 3) negotiates number of buffers and planes per buffer with the driver
 *    to be used during streaming,
 * 4) allocates internal buffer structures (struct vb2_buffer), according to
 *    the agreed parameters,
 * 5) for MMAP memory type, allocates actual video memory, using the
 *    memory handling/allocation routines provided during queue initialization
 *
 * If req->count is 0, all the memory will be freed instead.
 * If the queue has been allocated previously (by a previous vb2_reqbufs) call
 * and the queue is not busy, memory will be reallocated.
 *
 * The return values from this function are intended to be directly returned
 * from vidioc_reqbufs handler in driver.
 */
static int __reqbufs(struct vb2_queue *q, struct v4l2_requestbuffers *req)
{
	unsigned int num_buffers, allocated_buffers, num_planes = 0;
	int ret;
>>>>>>> common/android-3.10.y:drivers/media/v4l2-core/videobuf2-core.c

	if (q->streaming) {
		dprintk(1, "reqbufs: streaming active\n");
		return -EBUSY;
	}

<<<<<<< HEAD:drivers/media/video/videobuf2-core.c
	if (req->memory == V4L2_MEMORY_MMAP && __verify_mmap_ops(q)) {
		dprintk(1, "reqbufs: MMAP for current setup unsupported\n");
		return -EINVAL;
	}

	if (req->memory == V4L2_MEMORY_USERPTR && __verify_userptr_ops(q)) {
		dprintk(1, "reqbufs: USERPTR for current setup unsupported\n");
		return -EINVAL;
	}

	if (q->memory == req->memory && req->count == q->num_buffers)
		return 0;

=======
>>>>>>> common/android-3.10.y:drivers/media/v4l2-core/videobuf2-core.c
	if (req->count == 0 || q->num_buffers != 0 || q->memory != req->memory) {
		if (q->memory == V4L2_MEMORY_MMAP && __buffers_in_use(q)) {
			dprintk(1, "reqbufs: memory in use, cannot free\n");
			return -EBUSY;
		}

		__vb2_queue_free(q);

		if (req->count == 0)
			return 0;
	}

	num_buffers = min_t(unsigned int, req->count, VIDEO_MAX_FRAME);
	memset(plane_sizes, 0, sizeof(plane_sizes));
	memset(q->alloc_ctx, 0, sizeof(q->alloc_ctx));
	q->memory = req->memory;

	ret = call_qop(q, queue_setup, q, NULL, &num_buffers,
				&num_planes, q->plane_sizes, q->alloc_ctx);

	if (ret)
		return ret;

	
	ret = __vb2_queue_alloc(q, req->memory, num_buffers, num_planes,
				plane_sizes);
	if (ret == 0) {
		dprintk(1, "Memory allocation failed\n");
		return -ENOMEM;
	}

	if (ret < num_buffers) {
		unsigned int orig_num_buffers;

		orig_num_buffers = num_buffers = ret;
		ret = call_qop(q, queue_setup, q, NULL, &num_buffers,
			       &num_planes, q->plane_sizes, q->alloc_ctx);
		if (ret)
			goto free_mem;

		if (orig_num_buffers < num_buffers) {
			ret = -ENOMEM;
			goto free_mem;
		}

		ret = num_buffers;
	}

<<<<<<< HEAD:drivers/media/video/videobuf2-core.c
	req->count = ret;

	return 0;

free_mem:
	__vb2_queue_free(q);
	return ret;
}
EXPORT_SYMBOL_GPL(vb2_reqbufs);
=======
	/*
	 * Return the number of successfully allocated buffers
	 * to the userspace.
	 */
	req->count = allocated_buffers;
	q->waiting_for_buffers = !V4L2_TYPE_IS_OUTPUT(q->type);

	return 0;
}

/**
 * vb2_reqbufs() - Wrapper for __reqbufs() that also verifies the memory and
 * type values.
 * @q:		videobuf2 queue
 * @req:	struct passed from userspace to vidioc_reqbufs handler in driver
 */
int vb2_reqbufs(struct vb2_queue *q, struct v4l2_requestbuffers *req)
{
	int ret = __verify_memory_type(q, req->memory, req->type);

	return ret ? ret : __reqbufs(q, req);
}
EXPORT_SYMBOL_GPL(vb2_reqbufs);

/**
 * __create_bufs() - Allocate buffers and any required auxiliary structs
 * @q:		videobuf2 queue
 * @create:	creation parameters, passed from userspace to vidioc_create_bufs
 *		handler in driver
 *
 * Should be called from vidioc_create_bufs ioctl handler of a driver.
 * This function:
 * 1) verifies parameter sanity
 * 2) calls the .queue_setup() queue operation
 * 3) performs any necessary memory allocations
 *
 * The return values from this function are intended to be directly returned
 * from vidioc_create_bufs handler in driver.
 */
static int __create_bufs(struct vb2_queue *q, struct v4l2_create_buffers *create)
{
	unsigned int num_planes = 0, num_buffers, allocated_buffers;
	int ret;

	if (q->num_buffers == VIDEO_MAX_FRAME) {
		dprintk(1, "%s(): maximum number of buffers already allocated\n",
			__func__);
		return -ENOBUFS;
	}

	if (!q->num_buffers) {
		memset(q->plane_sizes, 0, sizeof(q->plane_sizes));
		memset(q->alloc_ctx, 0, sizeof(q->alloc_ctx));
		q->memory = create->memory;
		q->waiting_for_buffers = !V4L2_TYPE_IS_OUTPUT(q->type);
	}

	num_buffers = min(create->count, VIDEO_MAX_FRAME - q->num_buffers);

	/*
	 * Ask the driver, whether the requested number of buffers, planes per
	 * buffer and their sizes are acceptable
	 */
	ret = call_qop(q, queue_setup, q, &create->format, &num_buffers,
		       &num_planes, q->plane_sizes, q->alloc_ctx);
	if (ret)
		return ret;

	/* Finally, allocate buffers and video memory */
	ret = __vb2_queue_alloc(q, create->memory, num_buffers,
				num_planes);
	if (ret == 0) {
		dprintk(1, "Memory allocation failed\n");
		return -ENOMEM;
	}

	allocated_buffers = ret;

	/*
	 * Check if driver can handle the so far allocated number of buffers.
	 */
	if (ret < num_buffers) {
		num_buffers = ret;

		/*
		 * q->num_buffers contains the total number of buffers, that the
		 * queue driver has set up
		 */
		ret = call_qop(q, queue_setup, q, &create->format, &num_buffers,
			       &num_planes, q->plane_sizes, q->alloc_ctx);

		if (!ret && allocated_buffers < num_buffers)
			ret = -ENOMEM;

		/*
		 * Either the driver has accepted a smaller number of buffers,
		 * or .queue_setup() returned an error
		 */
	}

	q->num_buffers += allocated_buffers;

	if (ret < 0) {
		__vb2_queue_free(q, allocated_buffers);
		return -ENOMEM;
	}

	/*
	 * Return the number of successfully allocated buffers
	 * to the userspace.
	 */
	create->count = allocated_buffers;

	return 0;
}

/**
 * vb2_create_bufs() - Wrapper for __create_bufs() that also verifies the
 * memory and type values.
 * @q:		videobuf2 queue
 * @create:	creation parameters, passed from userspace to vidioc_create_bufs
 *		handler in driver
 */
int vb2_create_bufs(struct vb2_queue *q, struct v4l2_create_buffers *create)
{
	int ret = __verify_memory_type(q, create->memory, create->format.type);

	create->index = q->num_buffers;
	if (create->count == 0)
		return ret != -EBUSY ? ret : 0;
	return ret ? ret : __create_bufs(q, create);
}
EXPORT_SYMBOL_GPL(vb2_create_bufs);
>>>>>>> common/android-3.10.y:drivers/media/v4l2-core/videobuf2-core.c

void *vb2_plane_vaddr(struct vb2_buffer *vb, unsigned int plane_no)
{
	struct vb2_queue *q = vb->vb2_queue;

	if (plane_no > vb->num_planes)
		return NULL;

	return call_memop(q, plane_no, vaddr, vb->planes[plane_no].mem_priv);

}
EXPORT_SYMBOL_GPL(vb2_plane_vaddr);

void *vb2_plane_cookie(struct vb2_buffer *vb, unsigned int plane_no)
{
	struct vb2_queue *q = vb->vb2_queue;

	if (plane_no > vb->num_planes)
		return NULL;

	return call_memop(q, plane_no, cookie, vb->planes[plane_no].mem_priv);
}
EXPORT_SYMBOL_GPL(vb2_plane_cookie);

void vb2_buffer_done(struct vb2_buffer *vb, enum vb2_buffer_state state)
{
	struct vb2_queue *q = vb->vb2_queue;
	unsigned long flags;
	unsigned int plane;

	if (vb->state != VB2_BUF_STATE_ACTIVE)
		return;

	if (state != VB2_BUF_STATE_DONE && state != VB2_BUF_STATE_ERROR)
		return;

	dprintk(4, "Done processing on buffer %d, state: %d\n",
			vb->v4l2_buf.index, state);

	/* sync buffers */
	for (plane = 0; plane < vb->num_planes; ++plane)
		call_memop(q, finish, vb->planes[plane].mem_priv);

	
	spin_lock_irqsave(&q->done_lock, flags);
	vb->state = state;
	list_add_tail(&vb->done_entry, &q->done_list);
	atomic_dec(&q->queued_count);
	spin_unlock_irqrestore(&q->done_lock, flags);

	
	wake_up(&q->done_wq);
}
EXPORT_SYMBOL_GPL(vb2_buffer_done);

<<<<<<< HEAD:drivers/media/video/videobuf2-core.c
static int __fill_vb2_buffer(struct vb2_buffer *vb, struct v4l2_buffer *b,
=======
/**
 * __fill_vb2_buffer() - fill a vb2_buffer with information provided in a
 * v4l2_buffer by the userspace. The caller has already verified that struct
 * v4l2_buffer has a valid number of planes.
 */
static void __fill_vb2_buffer(struct vb2_buffer *vb, const struct v4l2_buffer *b,
>>>>>>> common/android-3.10.y:drivers/media/v4l2-core/videobuf2-core.c
				struct v4l2_plane *v4l2_planes)
{
	unsigned int plane;

	if (V4L2_TYPE_IS_MULTIPLANAR(b->type)) {
<<<<<<< HEAD:drivers/media/video/videobuf2-core.c
		ret = __verify_planes_array(vb, b);
		if (ret)
			return ret;

		
=======
		/* Fill in driver-provided information for OUTPUT types */
>>>>>>> common/android-3.10.y:drivers/media/v4l2-core/videobuf2-core.c
		if (V4L2_TYPE_IS_OUTPUT(b->type)) {
			for (plane = 0; plane < vb->num_planes; ++plane) {
				v4l2_planes[plane].bytesused =
					b->m.planes[plane].bytesused;
				v4l2_planes[plane].data_offset =
					b->m.planes[plane].data_offset;
			}
		}

		if (b->memory == V4L2_MEMORY_USERPTR) {
			for (plane = 0; plane < vb->num_planes; ++plane) {
				v4l2_planes[plane].m.userptr =
					b->m.planes[plane].m.userptr;
				v4l2_planes[plane].length =
					b->m.planes[plane].length;
			}
		}
		if (b->memory == V4L2_MEMORY_DMABUF) {
			for (plane = 0; plane < vb->num_planes; ++plane) {
				v4l2_planes[plane].m.fd =
					b->m.planes[plane].m.fd;
				v4l2_planes[plane].length =
					b->m.planes[plane].length;
				v4l2_planes[plane].data_offset =
					b->m.planes[plane].data_offset;
			}
		}
	} else {
<<<<<<< HEAD:drivers/media/video/videobuf2-core.c
		if (V4L2_TYPE_IS_OUTPUT(b->type))
=======
		/*
		 * Single-planar buffers do not use planes array,
		 * so fill in relevant v4l2_buffer struct fields instead.
		 * In videobuf we use our internal V4l2_planes struct for
		 * single-planar buffers as well, for simplicity.
		 */
		if (V4L2_TYPE_IS_OUTPUT(b->type)) {
>>>>>>> common/android-3.10.y:drivers/media/v4l2-core/videobuf2-core.c
			v4l2_planes[0].bytesused = b->bytesused;
			v4l2_planes[0].data_offset = 0;
		}

		if (b->memory == V4L2_MEMORY_USERPTR) {
			v4l2_planes[0].m.userptr = b->m.userptr;
			v4l2_planes[0].length = b->length;
		}

		if (b->memory == V4L2_MEMORY_DMABUF) {
			v4l2_planes[0].m.fd = b->m.fd;
			v4l2_planes[0].length = b->length;
			v4l2_planes[0].data_offset = 0;
		}

	}

	vb->v4l2_buf.field = b->field;
	vb->v4l2_buf.timestamp = b->timestamp;
	vb->v4l2_buf.flags = b->flags & ~V4L2_BUFFER_MASK_FLAGS;
}

static int __qbuf_userptr(struct vb2_buffer *vb, struct v4l2_buffer *b)
{
	struct v4l2_plane planes[VIDEO_MAX_PLANES];
	struct vb2_queue *q = vb->vb2_queue;
	void *mem_priv;
	unsigned int plane;
	int ret;
	int write = !V4L2_TYPE_IS_OUTPUT(q->type);

<<<<<<< HEAD:drivers/media/video/videobuf2-core.c
	
	ret = __fill_vb2_buffer(vb, b, planes);
	if (ret)
		return ret;
=======
	/* Copy relevant information provided by the userspace */
	__fill_vb2_buffer(vb, b, planes);
>>>>>>> common/android-3.10.y:drivers/media/v4l2-core/videobuf2-core.c

	for (plane = 0; plane < vb->num_planes; ++plane) {
		
		if (vb->v4l2_planes[plane].m.userptr == planes[plane].m.userptr
		    && vb->v4l2_planes[plane].length == planes[plane].length)
			continue;

		dprintk(3, "qbuf: userspace address for plane %d changed, "
				"reacquiring memory\n", plane);

		
		if (vb->planes[plane].mem_priv)
			call_memop(q, plane, put_userptr,
					vb->planes[plane].mem_priv);

		vb->planes[plane].mem_priv = NULL;

		
		if (q->mem_ops->get_userptr) {
			mem_priv = q->mem_ops->get_userptr(q->alloc_ctx[plane],
							planes[plane].m.userptr,
							planes[plane].length,
							write);
			if (IS_ERR(mem_priv)) {
				dprintk(1, "qbuf: failed acquiring userspace "
						"memory for plane %d\n", plane);
				ret = PTR_ERR(mem_priv);
				goto err;
			}
			vb->planes[plane].mem_priv = mem_priv;
		}
	}

	ret = call_qop(q, buf_init, vb);
	if (ret) {
		dprintk(1, "qbuf: buffer initialization failed\n");
		goto err;
	}

	for (plane = 0; plane < vb->num_planes; ++plane)
		vb->v4l2_planes[plane] = planes[plane];

	return 0;
err:
	
	for (plane = 0; plane < vb->num_planes; ++plane) {
		if (vb->planes[plane].mem_priv)
			call_memop(q, plane, put_userptr,
				   vb->planes[plane].mem_priv);
		vb->planes[plane].mem_priv = NULL;
		vb->v4l2_planes[plane].m.userptr = 0;
		vb->v4l2_planes[plane].length = 0;
	}

	return ret;
}

static int __qbuf_mmap(struct vb2_buffer *vb, struct v4l2_buffer *b)
{
	__fill_vb2_buffer(vb, b, vb->v4l2_planes);
	return 0;
}

/**
 * __qbuf_dmabuf() - handle qbuf of a DMABUF buffer
 */
static int __qbuf_dmabuf(struct vb2_buffer *vb, const struct v4l2_buffer *b)
{
	struct v4l2_plane planes[VIDEO_MAX_PLANES];
	struct vb2_queue *q = vb->vb2_queue;
	void *mem_priv;
	unsigned int plane;
	int ret;
	int write = !V4L2_TYPE_IS_OUTPUT(q->type);

	/* Verify and copy relevant information provided by the userspace */
	__fill_vb2_buffer(vb, b, planes);

	for (plane = 0; plane < vb->num_planes; ++plane) {
		struct dma_buf *dbuf = dma_buf_get(planes[plane].m.fd);

		if (IS_ERR_OR_NULL(dbuf)) {
			dprintk(1, "qbuf: invalid dmabuf fd for plane %d\n",
				plane);
			ret = -EINVAL;
			goto err;
		}

		/* use DMABUF size if length is not provided */
		if (planes[plane].length == 0)
			planes[plane].length = dbuf->size;

		if (planes[plane].length < planes[plane].data_offset +
		    q->plane_sizes[plane]) {
			ret = -EINVAL;
			goto err;
		}

		/* Skip the plane if already verified */
		if (dbuf == vb->planes[plane].dbuf &&
		    vb->v4l2_planes[plane].length == planes[plane].length) {
			dma_buf_put(dbuf);
			continue;
		}

		dprintk(1, "qbuf: buffer for plane %d changed\n", plane);

		/* Release previously acquired memory if present */
		__vb2_plane_dmabuf_put(q, &vb->planes[plane]);
		memset(&vb->v4l2_planes[plane], 0, sizeof(struct v4l2_plane));

		/* Acquire each plane's memory */
		mem_priv = call_memop(q, attach_dmabuf, q->alloc_ctx[plane],
			dbuf, planes[plane].length, write);
		if (IS_ERR(mem_priv)) {
			dprintk(1, "qbuf: failed to attach dmabuf\n");
			ret = PTR_ERR(mem_priv);
			dma_buf_put(dbuf);
			goto err;
		}

		vb->planes[plane].dbuf = dbuf;
		vb->planes[plane].mem_priv = mem_priv;
	}

	/* TODO: This pins the buffer(s) with  dma_buf_map_attachment()).. but
	 * really we want to do this just before the DMA, not while queueing
	 * the buffer(s)..
	 */
	for (plane = 0; plane < vb->num_planes; ++plane) {
		ret = call_memop(q, map_dmabuf, vb->planes[plane].mem_priv);
		if (ret) {
			dprintk(1, "qbuf: failed to map dmabuf for plane %d\n",
				plane);
			goto err;
		}
		vb->planes[plane].dbuf_mapped = 1;
	}

	/*
	 * Call driver-specific initialization on the newly acquired buffer,
	 * if provided.
	 */
	ret = call_qop(q, buf_init, vb);
	if (ret) {
		dprintk(1, "qbuf: buffer initialization failed\n");
		goto err;
	}

	/*
	 * Now that everything is in order, copy relevant information
	 * provided by userspace.
	 */
	for (plane = 0; plane < vb->num_planes; ++plane)
		vb->v4l2_planes[plane] = planes[plane];

	return 0;
err:
	/* In case of errors, release planes that were already acquired */
	__vb2_buf_dmabuf_put(vb);

	return ret;
}

static void __enqueue_in_driver(struct vb2_buffer *vb)
{
	struct vb2_queue *q = vb->vb2_queue;
	unsigned int plane;

	vb->state = VB2_BUF_STATE_ACTIVE;
	atomic_inc(&q->queued_count);

	/* sync buffers */
	for (plane = 0; plane < vb->num_planes; ++plane)
		call_memop(q, prepare, vb->planes[plane].mem_priv);

	q->ops->buf_queue(vb);
}

<<<<<<< HEAD:drivers/media/video/videobuf2-core.c
int vb2_qbuf(struct vb2_queue *q, struct v4l2_buffer *b)
=======
static int __buf_prepare(struct vb2_buffer *vb, const struct v4l2_buffer *b)
{
	struct vb2_queue *q = vb->vb2_queue;
	int ret;

	switch (q->memory) {
	case V4L2_MEMORY_MMAP:
		ret = __qbuf_mmap(vb, b);
		break;
	case V4L2_MEMORY_USERPTR:
		ret = __qbuf_userptr(vb, b);
		break;
	case V4L2_MEMORY_DMABUF:
		ret = __qbuf_dmabuf(vb, b);
		break;
	default:
		WARN(1, "Invalid queue type\n");
		ret = -EINVAL;
	}

	if (!ret)
		ret = call_qop(q, buf_prepare, vb);
	if (ret)
		dprintk(1, "qbuf: buffer preparation failed: %d\n", ret);
	else
		vb->state = VB2_BUF_STATE_PREPARED;

	return ret;
}

/**
 * vb2_prepare_buf() - Pass ownership of a buffer from userspace to the kernel
 * @q:		videobuf2 queue
 * @b:		buffer structure passed from userspace to vidioc_prepare_buf
 *		handler in driver
 *
 * Should be called from vidioc_prepare_buf ioctl handler of a driver.
 * This function:
 * 1) verifies the passed buffer,
 * 2) calls buf_prepare callback in the driver (if provided), in which
 *    driver-specific buffer initialization can be performed,
 *
 * The return values from this function are intended to be directly returned
 * from vidioc_prepare_buf handler in driver.
 */
int vb2_prepare_buf(struct vb2_queue *q, struct v4l2_buffer *b)
>>>>>>> common/android-3.10.y:drivers/media/v4l2-core/videobuf2-core.c
{
	struct vb2_buffer *vb;
	int ret = 0;

	if (q->fileio) {
		dprintk(1, "qbuf: file io in progress\n");
		return -EBUSY;
	}

	if (b->type != q->type) {
		dprintk(1, "qbuf: invalid buffer type\n");
		return -EINVAL;
	}

	if (b->index >= q->num_buffers) {
		dprintk(1, "qbuf: buffer index out of range\n");
		return -EINVAL;
	}

	vb = q->bufs[b->index];
	if (NULL == vb) {
		
		dprintk(1, "qbuf: buffer is NULL\n");
		return -EINVAL;
	}

	if (b->memory != q->memory) {
		dprintk(1, "qbuf: invalid memory type\n");
		return -EINVAL;
	}

	if (vb->state != VB2_BUF_STATE_DEQUEUED) {
		dprintk(1, "qbuf: buffer already in use\n");
		return -EINVAL;
	}
<<<<<<< HEAD:drivers/media/video/videobuf2-core.c

	if (q->memory == V4L2_MEMORY_MMAP)
		ret = __qbuf_mmap(vb, b);
	else if (q->memory == V4L2_MEMORY_USERPTR)
		ret = __qbuf_userptr(vb, b);
	else {
		WARN(1, "Invalid queue type\n");
		return -EINVAL;
	}

	if (ret)
		return ret;
=======
	ret = __verify_planes_array(vb, b);
	if (ret < 0)
		return ret;
	ret = __buf_prepare(vb, b);
	if (ret < 0)
		return ret;

	__fill_v4l2_buffer(vb, b);

	return 0;
}
EXPORT_SYMBOL_GPL(vb2_prepare_buf);

/**
 * vb2_qbuf() - Queue a buffer from userspace
 * @q:		videobuf2 queue
 * @b:		buffer structure passed from userspace to vidioc_qbuf handler
 *		in driver
 *
 * Should be called from vidioc_qbuf ioctl handler of a driver.
 * This function:
 * 1) verifies the passed buffer,
 * 2) if necessary, calls buf_prepare callback in the driver (if provided), in
 *    which driver-specific buffer initialization can be performed,
 * 3) if streaming is on, queues the buffer in driver by the means of buf_queue
 *    callback for processing.
 *
 * The return values from this function are intended to be directly returned
 * from vidioc_qbuf handler in driver.
 */
int vb2_qbuf(struct vb2_queue *q, struct v4l2_buffer *b)
{
	struct rw_semaphore *mmap_sem = NULL;
	struct vb2_buffer *vb;
	int ret = 0;

	/*
	 * In case of user pointer buffers vb2 allocator needs to get direct
	 * access to userspace pages. This requires getting read access on
	 * mmap semaphore in the current process structure. The same
	 * semaphore is taken before calling mmap operation, while both mmap
	 * and qbuf are called by the driver or v4l2 core with driver's lock
	 * held. To avoid a AB-BA deadlock (mmap_sem then driver's lock in
	 * mmap and driver's lock then mmap_sem in qbuf) the videobuf2 core
	 * release driver's lock, takes mmap_sem and then takes again driver's
	 * lock.
	 *
	 * To avoid race with other vb2 calls, which might be called after
	 * releasing driver's lock, this operation is performed at the
	 * beggining of qbuf processing. This way the queue status is
	 * consistent after getting driver's lock back.
	 */
	if (q->memory == V4L2_MEMORY_USERPTR) {
		mmap_sem = &current->mm->mmap_sem;
		call_qop(q, wait_prepare, q);
		down_read(mmap_sem);
		call_qop(q, wait_finish, q);
	}

	if (q->fileio) {
		dprintk(1, "qbuf: file io in progress\n");
		ret = -EBUSY;
		goto unlock;
	}

	if (b->type != q->type) {
		dprintk(1, "qbuf: invalid buffer type\n");
		ret = -EINVAL;
		goto unlock;
	}

	if (b->index >= q->num_buffers) {
		dprintk(1, "qbuf: buffer index out of range\n");
		ret = -EINVAL;
		goto unlock;
	}

	vb = q->bufs[b->index];
	if (NULL == vb) {
		/* Should never happen */
		dprintk(1, "qbuf: buffer is NULL\n");
		ret = -EINVAL;
		goto unlock;
	}

	if (b->memory != q->memory) {
		dprintk(1, "qbuf: invalid memory type\n");
		ret = -EINVAL;
		goto unlock;
	}
	ret = __verify_planes_array(vb, b);
	if (ret)
		goto unlock;
>>>>>>> common/android-3.10.y:drivers/media/v4l2-core/videobuf2-core.c

	ret = call_qop(q, buf_prepare, vb);
	if (ret) {
		dprintk(1, "qbuf: buffer preparation failed\n");
		return ret;
	}

	list_add_tail(&vb->queued_entry, &q->queued_list);
	q->waiting_for_buffers = false;
	vb->state = VB2_BUF_STATE_QUEUED;

	if (q->streaming)
		__enqueue_in_driver(vb);

	dprintk(1, "qbuf of buffer %d succeeded\n", vb->v4l2_buf.index);
	return 0;
}
EXPORT_SYMBOL_GPL(vb2_qbuf);

static int __vb2_wait_for_done_vb(struct vb2_queue *q, int nonblocking)
{

	for (;;) {
		int ret;

		if (!q->streaming) {
			dprintk(1, "Streaming off, will not wait for buffers\n");
			return -EINVAL;
		}

		if (!list_empty(&q->done_list)) {
			break;
		}

		if (nonblocking) {
			dprintk(1, "Nonblocking and no buffers to dequeue, "
								"will not wait\n");
			return -EAGAIN;
		}

		call_qop(q, wait_prepare, q);

		dprintk(3, "Will sleep waiting for buffers\n");
		ret = wait_event_interruptible(q->done_wq,
				!list_empty(&q->done_list) || !q->streaming);

		call_qop(q, wait_finish, q);
		if (ret) {
			dprintk(1, "Sleep was interrupted\n");
			return ret;
		}
	}
	return 0;
}

static int __vb2_get_done_vb(struct vb2_queue *q, struct vb2_buffer **vb,
				struct v4l2_buffer *b, int nonblocking)
{
	unsigned long flags;
	int ret;

	ret = __vb2_wait_for_done_vb(q, nonblocking);
	if (ret)
		return ret;

	spin_lock_irqsave(&q->done_lock, flags);
	*vb = list_first_entry(&q->done_list, struct vb2_buffer, done_entry);
	/*
	 * Only remove the buffer from done_list if v4l2_buffer can handle all
	 * the planes.
	 */
	ret = __verify_planes_array(*vb, b);
	if (!ret)
		list_del(&(*vb)->done_entry);
	spin_unlock_irqrestore(&q->done_lock, flags);

	return ret;
}

int vb2_wait_for_all_buffers(struct vb2_queue *q)
{
	if (!q->streaming) {
		dprintk(1, "Streaming off, will not wait for buffers\n");
		return -EINVAL;
	}
	wait_event(q->done_wq, !atomic_read(&q->queued_count));
	return 0;
}
EXPORT_SYMBOL_GPL(vb2_wait_for_all_buffers);

<<<<<<< HEAD:drivers/media/video/videobuf2-core.c
=======
/**
 * __vb2_dqbuf() - bring back the buffer to the DEQUEUED state
 */
static void __vb2_dqbuf(struct vb2_buffer *vb)
{
	struct vb2_queue *q = vb->vb2_queue;
	unsigned int i;

	/* nothing to do if the buffer is already dequeued */
	if (vb->state == VB2_BUF_STATE_DEQUEUED)
		return;

	vb->state = VB2_BUF_STATE_DEQUEUED;

	/* unmap DMABUF buffer */
	if (q->memory == V4L2_MEMORY_DMABUF)
		for (i = 0; i < vb->num_planes; ++i) {
			if (!vb->planes[i].dbuf_mapped)
				continue;
			call_memop(q, unmap_dmabuf, vb->planes[i].mem_priv);
			vb->planes[i].dbuf_mapped = 0;
		}
}

/**
 * vb2_dqbuf() - Dequeue a buffer to the userspace
 * @q:		videobuf2 queue
 * @b:		buffer structure passed from userspace to vidioc_dqbuf handler
 *		in driver
 * @nonblocking: if true, this call will not sleep waiting for a buffer if no
 *		 buffers ready for dequeuing are present. Normally the driver
 *		 would be passing (file->f_flags & O_NONBLOCK) here
 *
 * Should be called from vidioc_dqbuf ioctl handler of a driver.
 * This function:
 * 1) verifies the passed buffer,
 * 2) calls buf_finish callback in the driver (if provided), in which
 *    driver can perform any additional operations that may be required before
 *    returning the buffer to userspace, such as cache sync,
 * 3) the buffer struct members are filled with relevant information for
 *    the userspace.
 *
 * The return values from this function are intended to be directly returned
 * from vidioc_dqbuf handler in driver.
 */
>>>>>>> common/android-3.10.y:drivers/media/v4l2-core/videobuf2-core.c
int vb2_dqbuf(struct vb2_queue *q, struct v4l2_buffer *b, bool nonblocking)
{
	struct vb2_buffer *vb = NULL;
	int ret;

	if (q->fileio) {
		dprintk(1, "dqbuf: file io in progress\n");
		return -EBUSY;
	}

	if (b->type != q->type) {
		dprintk(1, "dqbuf: invalid buffer type\n");
		return -EINVAL;
	}
	ret = __vb2_get_done_vb(q, &vb, b, nonblocking);
	if (ret < 0)
		return ret;

	ret = call_qop(q, buf_finish, vb);
	if (ret) {
		dprintk(1, "dqbuf: buffer finish failed\n");
		return ret;
	}

	switch (vb->state) {
	case VB2_BUF_STATE_DONE:
		dprintk(3, "dqbuf: Returning done buffer\n");
		break;
	case VB2_BUF_STATE_ERROR:
		dprintk(3, "dqbuf: Returning done buffer with errors\n");
		break;
	default:
		dprintk(1, "dqbuf: Invalid buffer state\n");
		return -EINVAL;
	}

	
	__fill_v4l2_buffer(vb, b);
	
	list_del(&vb->queued_entry);
	/* go back to dequeued state */
	__vb2_dqbuf(vb);

	dprintk(1, "dqbuf of buffer %d, with state %d\n",
			vb->v4l2_buf.index, vb->state);

	return 0;
}
EXPORT_SYMBOL_GPL(vb2_dqbuf);

static void __vb2_queue_cancel(struct vb2_queue *q)
{
	unsigned int i;

	if (q->streaming)
		call_qop(q, stop_streaming, q);
	q->streaming = 0;

	INIT_LIST_HEAD(&q->queued_list);
	INIT_LIST_HEAD(&q->done_list);
	atomic_set(&q->queued_count, 0);
	wake_up_all(&q->done_wq);

	for (i = 0; i < q->num_buffers; ++i)
		__vb2_dqbuf(q->bufs[i]);
}

int vb2_streamon(struct vb2_queue *q, enum v4l2_buf_type type)
{
	struct vb2_buffer *vb;
	int ret;

	if (q->fileio) {
		dprintk(1, "streamon: file io in progress\n");
		return -EBUSY;
	}

	if (type != q->type) {
		dprintk(1, "streamon: invalid stream type\n");
		return -EINVAL;
	}

	if (q->streaming) {
		dprintk(1, "streamon: already streaming\n");
		return -EBUSY;
	}

	list_for_each_entry(vb, &q->queued_list, queued_entry)
		__enqueue_in_driver(vb);

	ret = call_qop(q, start_streaming, q, atomic_read(&q->queued_count));
	if (ret) {
		dprintk(1, "streamon: driver refused to start streaming\n");
		__vb2_queue_cancel(q);
		return ret;
	}

	q->streaming = 1;

	dprintk(3, "Streamon successful\n");
	return 0;
}
EXPORT_SYMBOL_GPL(vb2_streamon);


int vb2_streamoff(struct vb2_queue *q, enum v4l2_buf_type type)
{
	if (q->fileio) {
		dprintk(1, "streamoff: file io in progress\n");
		return -EBUSY;
	}

	if (type != q->type) {
		dprintk(1, "streamoff: invalid stream type\n");
		return -EINVAL;
	}

	if (!q->streaming) {
		dprintk(1, "streamoff: not streaming\n");
		return -EINVAL;
	}

	__vb2_queue_cancel(q);
	q->waiting_for_buffers = !V4L2_TYPE_IS_OUTPUT(q->type);

	dprintk(3, "Streamoff successful\n");
	return 0;
}
EXPORT_SYMBOL_GPL(vb2_streamoff);

static int __find_plane_by_offset(struct vb2_queue *q, unsigned long off,
			unsigned int *_buffer, unsigned int *_plane)
{
	struct vb2_buffer *vb;
	unsigned int buffer, plane;

	for (buffer = 0; buffer < q->num_buffers; ++buffer) {
		vb = q->bufs[buffer];

		for (plane = 0; plane < vb->num_planes; ++plane) {
			if (vb->v4l2_planes[plane].m.mem_offset == off) {
				*_buffer = buffer;
				*_plane = plane;
				return 0;
			}
		}
	}

	return -EINVAL;
}

<<<<<<< HEAD:drivers/media/video/videobuf2-core.c
=======
/**
 * vb2_expbuf() - Export a buffer as a file descriptor
 * @q:		videobuf2 queue
 * @eb:		export buffer structure passed from userspace to vidioc_expbuf
 *		handler in driver
 *
 * The return values from this function are intended to be directly returned
 * from vidioc_expbuf handler in driver.
 */
int vb2_expbuf(struct vb2_queue *q, struct v4l2_exportbuffer *eb)
{
	struct vb2_buffer *vb = NULL;
	struct vb2_plane *vb_plane;
	int ret;
	struct dma_buf *dbuf;

	if (q->memory != V4L2_MEMORY_MMAP) {
		dprintk(1, "Queue is not currently set up for mmap\n");
		return -EINVAL;
	}

	if (!q->mem_ops->get_dmabuf) {
		dprintk(1, "Queue does not support DMA buffer exporting\n");
		return -EINVAL;
	}

	if (eb->flags & ~O_CLOEXEC) {
		dprintk(1, "Queue does support only O_CLOEXEC flag\n");
		return -EINVAL;
	}

	if (eb->type != q->type) {
		dprintk(1, "qbuf: invalid buffer type\n");
		return -EINVAL;
	}

	if (eb->index >= q->num_buffers) {
		dprintk(1, "buffer index out of range\n");
		return -EINVAL;
	}

	vb = q->bufs[eb->index];

	if (eb->plane >= vb->num_planes) {
		dprintk(1, "buffer plane out of range\n");
		return -EINVAL;
	}

	vb_plane = &vb->planes[eb->plane];

	dbuf = call_memop(q, get_dmabuf, vb_plane->mem_priv);
	if (IS_ERR_OR_NULL(dbuf)) {
		dprintk(1, "Failed to export buffer %d, plane %d\n",
			eb->index, eb->plane);
		return -EINVAL;
	}

	ret = dma_buf_fd(dbuf, eb->flags);
	if (ret < 0) {
		dprintk(3, "buffer %d, plane %d failed to export (%d)\n",
			eb->index, eb->plane, ret);
		dma_buf_put(dbuf);
		return ret;
	}

	dprintk(3, "buffer %d, plane %d exported as %d descriptor\n",
		eb->index, eb->plane, ret);
	eb->fd = ret;

	return 0;
}
EXPORT_SYMBOL_GPL(vb2_expbuf);

/**
 * vb2_mmap() - map video buffers into application address space
 * @q:		videobuf2 queue
 * @vma:	vma passed to the mmap file operation handler in the driver
 *
 * Should be called from mmap file operation handler of a driver.
 * This function maps one plane of one of the available video buffers to
 * userspace. To map whole video memory allocated on reqbufs, this function
 * has to be called once per each plane per each buffer previously allocated.
 *
 * When the userspace application calls mmap, it passes to it an offset returned
 * to it earlier by the means of vidioc_querybuf handler. That offset acts as
 * a "cookie", which is then used to identify the plane to be mapped.
 * This function finds a plane with a matching offset and a mapping is performed
 * by the means of a provided memory operation.
 *
 * The return values from this function are intended to be directly returned
 * from the mmap handler in driver.
 */
>>>>>>> common/android-3.10.y:drivers/media/v4l2-core/videobuf2-core.c
int vb2_mmap(struct vb2_queue *q, struct vm_area_struct *vma)
{
	unsigned long off = vma->vm_pgoff << PAGE_SHIFT;
	struct vb2_plane *vb_plane;
	struct vb2_buffer *vb;
	unsigned int buffer, plane;
	int ret;
	unsigned long length;

	if (q->memory != V4L2_MEMORY_MMAP) {
		dprintk(1, "Queue is not currently set up for mmap\n");
		return -EINVAL;
	}

	if (!(vma->vm_flags & VM_SHARED)) {
		dprintk(1, "Invalid vma flags, VM_SHARED needed\n");
		return -EINVAL;
	}
	if (V4L2_TYPE_IS_OUTPUT(q->type)) {
		if (!(vma->vm_flags & VM_WRITE)) {
			dprintk(1, "Invalid vma flags, VM_WRITE needed\n");
			return -EINVAL;
		}
	} else {
		if (!(vma->vm_flags & VM_READ)) {
			dprintk(1, "Invalid vma flags, VM_READ needed\n");
			return -EINVAL;
		}
	}

	ret = __find_plane_by_offset(q, off, &buffer, &plane);
	if (ret)
		return ret;

	vb = q->bufs[buffer];
	vb_plane = &vb->planes[plane];

<<<<<<< HEAD:drivers/media/video/videobuf2-core.c
	ret = q->mem_ops->mmap(vb_plane->mem_priv, vma);
=======
	/*
	 * MMAP requires page_aligned buffers.
	 * The buffer length was page_aligned at __vb2_buf_mem_alloc(),
	 * so, we need to do the same here.
	 */
	length = PAGE_ALIGN(vb->v4l2_planes[plane].length);
	if (length < (vma->vm_end - vma->vm_start)) {
		dprintk(1,
			"MMAP invalid, as it would overflow buffer length\n");
		return -EINVAL;
	}

	ret = call_memop(q, mmap, vb->planes[plane].mem_priv, vma);
>>>>>>> common/android-3.10.y:drivers/media/v4l2-core/videobuf2-core.c
	if (ret)
		return ret;

	vb_plane->mapped = 1;
	vb->num_planes_mapped++;

	dprintk(3, "Buffer %d, plane %d successfully mapped\n", buffer, plane);
	return 0;
}
EXPORT_SYMBOL_GPL(vb2_mmap);

static int __vb2_init_fileio(struct vb2_queue *q, int read);
static int __vb2_cleanup_fileio(struct vb2_queue *q);

<<<<<<< HEAD:drivers/media/video/videobuf2-core.c
=======
/**
 * vb2_poll() - implements poll userspace operation
 * @q:		videobuf2 queue
 * @file:	file argument passed to the poll file operation handler
 * @wait:	wait argument passed to the poll file operation handler
 *
 * This function implements poll file operation handler for a driver.
 * For CAPTURE queues, if a buffer is ready to be dequeued, the userspace will
 * be informed that the file descriptor of a video device is available for
 * reading.
 * For OUTPUT queues, if a buffer is ready to be dequeued, the file descriptor
 * will be reported as available for writing.
 *
 * If the driver uses struct v4l2_fh, then vb2_poll() will also check for any
 * pending events.
 *
 * The return values from this function are intended to be directly returned
 * from poll handler in driver.
 */
>>>>>>> common/android-3.10.y:drivers/media/v4l2-core/videobuf2-core.c
unsigned int vb2_poll(struct vb2_queue *q, struct file *file, poll_table *wait)
{
	struct video_device *vfd = video_devdata(file);
	unsigned long req_events = poll_requested_events(wait);
	struct vb2_buffer *vb = NULL;
	unsigned int res = 0;
	unsigned long flags;

	if (test_bit(V4L2_FL_USES_V4L2_FH, &vfd->flags)) {
		struct v4l2_fh *fh = file->private_data;

		if (v4l2_event_pending(fh))
			res = POLLPRI;
		else if (req_events & POLLPRI)
			poll_wait(file, &fh->wait, wait);
	}

	if (!V4L2_TYPE_IS_OUTPUT(q->type) && !(req_events & (POLLIN | POLLRDNORM)))
		return res;
	if (V4L2_TYPE_IS_OUTPUT(q->type) && !(req_events & (POLLOUT | POLLWRNORM)))
		return res;

	if (q->num_buffers == 0 && q->fileio == NULL) {
		if (!V4L2_TYPE_IS_OUTPUT(q->type) && (q->io_modes & VB2_READ) &&
				(req_events & (POLLIN | POLLRDNORM))) {
			if (__vb2_init_fileio(q, 1))
				return res | POLLERR;
		}
<<<<<<< HEAD:drivers/media/video/videobuf2-core.c
		if (V4L2_TYPE_IS_OUTPUT(q->type) && (q->io_modes & VB2_WRITE)) {
			ret = __vb2_init_fileio(q, 0);
			if (ret)
				return POLLERR;
			return POLLOUT | POLLWRNORM;
		}
	}

	if (list_empty(&q->queued_list))
		return POLLERR;
=======
		if (V4L2_TYPE_IS_OUTPUT(q->type) && (q->io_modes & VB2_WRITE) &&
				(req_events & (POLLOUT | POLLWRNORM))) {
			if (__vb2_init_fileio(q, 0))
				return res | POLLERR;
			/*
			 * Write to OUTPUT queue can be done immediately.
			 */
			return res | POLLOUT | POLLWRNORM;
		}
	}

	/*
	 * There is nothing to wait for if the queue isn't streaming.
	 */
	if (!vb2_is_streaming(q))
		return res | POLLERR;
	/*
	 * For compatibility with vb1: if QBUF hasn't been called yet, then
	 * return POLLERR as well. This only affects capture queues, output
	 * queues will always initialize waiting_for_buffers to false.
	 */
	if (q->waiting_for_buffers)
		return res | POLLERR;
>>>>>>> common/android-3.10.y:drivers/media/v4l2-core/videobuf2-core.c

	if (list_empty(&q->done_list))
		poll_wait(file, &q->done_wq, wait);

	spin_lock_irqsave(&q->done_lock, flags);
	if (!list_empty(&q->done_list))
		vb = list_first_entry(&q->done_list, struct vb2_buffer,
					done_entry);
	spin_unlock_irqrestore(&q->done_lock, flags);

	if (vb && (vb->state == VB2_BUF_STATE_DONE
			|| vb->state == VB2_BUF_STATE_ERROR)) {
		return (V4L2_TYPE_IS_OUTPUT(q->type)) ?
				res | POLLOUT | POLLWRNORM :
				res | POLLIN | POLLRDNORM;
	}
	return res;
}
EXPORT_SYMBOL_GPL(vb2_poll);

int vb2_queue_init(struct vb2_queue *q)
{
	/*
	 * Sanity check
	 */
	if (WARN_ON(!q)			  ||
	    WARN_ON(!q->ops)		  ||
	    WARN_ON(!q->mem_ops)	  ||
	    WARN_ON(!q->type)		  ||
	    WARN_ON(!q->io_modes)	  ||
	    WARN_ON(!q->ops->queue_setup) ||
	    WARN_ON(!q->ops->buf_queue)   ||
	    WARN_ON(q->timestamp_type & ~V4L2_BUF_FLAG_TIMESTAMP_MASK))
		return -EINVAL;

	/* Warn that the driver should choose an appropriate timestamp type */
	WARN_ON(q->timestamp_type == V4L2_BUF_FLAG_TIMESTAMP_UNKNOWN);

	INIT_LIST_HEAD(&q->queued_list);
	INIT_LIST_HEAD(&q->done_list);
	spin_lock_init(&q->done_lock);
	init_waitqueue_head(&q->done_wq);

	if (q->buf_struct_size == 0)
		q->buf_struct_size = sizeof(struct vb2_buffer);

	return 0;
}
EXPORT_SYMBOL_GPL(vb2_queue_init);

void vb2_queue_release(struct vb2_queue *q)
{
	__vb2_cleanup_fileio(q);
	__vb2_queue_cancel(q);
	__vb2_queue_free(q);
}
EXPORT_SYMBOL_GPL(vb2_queue_release);

struct vb2_fileio_buf {
	void *vaddr;
	unsigned int size;
	unsigned int pos;
	unsigned int queued:1;
};

struct vb2_fileio_data {
	struct v4l2_requestbuffers req;
	struct v4l2_buffer b;
	struct vb2_fileio_buf bufs[VIDEO_MAX_FRAME];
	unsigned int index;
	unsigned int q_count;
	unsigned int dq_count;
	unsigned int flags;
};

static int __vb2_init_fileio(struct vb2_queue *q, int read)
{
	struct vb2_fileio_data *fileio;
	int i, ret;
	unsigned int count = 0;

	if ((read && !(q->io_modes & VB2_READ)) ||
	   (!read && !(q->io_modes & VB2_WRITE)))
		BUG();

	if (!q->mem_ops->vaddr)
		return -EBUSY;

	if (q->streaming || q->num_buffers > 0)
		return -EBUSY;

	count = 1;

	dprintk(3, "setting up file io: mode %s, count %d, flags %08x\n",
		(read) ? "read" : "write", count, q->io_flags);

	fileio = kzalloc(sizeof(struct vb2_fileio_data), GFP_KERNEL);
	if (fileio == NULL)
		return -ENOMEM;

	fileio->flags = q->io_flags;

	fileio->req.count = count;
	fileio->req.memory = V4L2_MEMORY_MMAP;
	fileio->req.type = q->type;
	ret = vb2_reqbufs(q, &fileio->req);
	if (ret)
		goto err_kfree;

	if (q->bufs[0]->num_planes != 1) {
		ret = -EBUSY;
		goto err_reqbufs;
	}

	for (i = 0; i < q->num_buffers; i++) {
		fileio->bufs[i].vaddr = vb2_plane_vaddr(q->bufs[i], 0);
		if (fileio->bufs[i].vaddr == NULL)
			goto err_reqbufs;
		fileio->bufs[i].size = vb2_plane_size(q->bufs[i], 0);
	}

	if (read) {
		for (i = 0; i < q->num_buffers; i++) {
			struct v4l2_buffer *b = &fileio->b;
			memset(b, 0, sizeof(*b));
			b->type = q->type;
			b->memory = q->memory;
			b->index = i;
			ret = vb2_qbuf(q, b);
			if (ret)
				goto err_reqbufs;
			fileio->bufs[i].queued = 1;
		}

		ret = vb2_streamon(q, q->type);
		if (ret)
			goto err_reqbufs;
	}

	q->fileio = fileio;

	return ret;

err_reqbufs:
	fileio->req.count = 0;
	vb2_reqbufs(q, &fileio->req);

err_kfree:
	kfree(fileio);
	return ret;
}

static int __vb2_cleanup_fileio(struct vb2_queue *q)
{
	struct vb2_fileio_data *fileio = q->fileio;

	if (fileio) {
		q->fileio = NULL;

		vb2_streamoff(q, q->type);
		fileio->req.count = 0;
		vb2_reqbufs(q, &fileio->req);
		kfree(fileio);
		dprintk(3, "file io emulator closed\n");
	}
	return 0;
}

static size_t __vb2_perform_fileio(struct vb2_queue *q, char __user *data, size_t count,
		loff_t *ppos, int nonblock, int read)
{
	struct vb2_fileio_data *fileio;
	struct vb2_fileio_buf *buf;
	int ret, index;

	dprintk(3, "file io: mode %s, offset %ld, count %zd, %sblocking\n",
		read ? "read" : "write", (long)*ppos, count,
		nonblock ? "non" : "");

	if (!data)
		return -EINVAL;

	if (!q->fileio) {
		ret = __vb2_init_fileio(q, read);
		dprintk(3, "file io: vb2_init_fileio result: %d\n", ret);
		if (ret)
			return ret;
	}
	fileio = q->fileio;

	q->fileio = NULL;

	index = fileio->index;
	buf = &fileio->bufs[index];

	if (buf->queued) {
		struct vb2_buffer *vb;

		memset(&fileio->b, 0, sizeof(fileio->b));
		fileio->b.type = q->type;
		fileio->b.memory = q->memory;
		fileio->b.index = index;
		ret = vb2_dqbuf(q, &fileio->b, nonblock);
		dprintk(5, "file io: vb2_dqbuf result: %d\n", ret);
		if (ret)
			goto end;
		fileio->dq_count += 1;

		vb = q->bufs[index];
		buf->size = vb2_get_plane_payload(vb, 0);
		buf->queued = 0;
	}

	if (buf->pos + count > buf->size) {
		count = buf->size - buf->pos;
		dprintk(5, "reducing read count: %zd\n", count);
	}

	dprintk(3, "file io: copying %zd bytes - buffer %d, offset %u\n",
		count, index, buf->pos);
	if (read)
		ret = copy_to_user(data, buf->vaddr + buf->pos, count);
	else
		ret = copy_from_user(buf->vaddr + buf->pos, data, count);
	if (ret) {
		dprintk(3, "file io: error copying data\n");
		ret = -EFAULT;
		goto end;
	}

	buf->pos += count;
	*ppos += count;

	if (buf->pos == buf->size ||
	   (!read && (fileio->flags & VB2_FILEIO_WRITE_IMMEDIATELY))) {
		if (read && (fileio->flags & VB2_FILEIO_READ_ONCE) &&
		    fileio->dq_count == 1) {
			dprintk(3, "file io: read limit reached\n");
			q->fileio = fileio;
			return __vb2_cleanup_fileio(q);
		}

		memset(&fileio->b, 0, sizeof(fileio->b));
		fileio->b.type = q->type;
		fileio->b.memory = q->memory;
		fileio->b.index = index;
		fileio->b.bytesused = buf->pos;
		ret = vb2_qbuf(q, &fileio->b);
		dprintk(5, "file io: vb2_dbuf result: %d\n", ret);
		if (ret)
			goto end;

		buf->pos = 0;
		buf->queued = 1;
		buf->size = q->bufs[0]->v4l2_planes[0].length;
		fileio->q_count += 1;

		fileio->index = (index + 1) % q->num_buffers;

		if (!read && !q->streaming) {
			ret = vb2_streamon(q, q->type);
			if (ret)
				goto end;
		}
	}

	if (ret == 0)
		ret = count;
end:
	q->fileio = fileio;
	return ret;
}

size_t vb2_read(struct vb2_queue *q, char __user *data, size_t count,
		loff_t *ppos, int nonblocking)
{
	return __vb2_perform_fileio(q, data, count, ppos, nonblocking, 1);
}
EXPORT_SYMBOL_GPL(vb2_read);

size_t vb2_write(struct vb2_queue *q, char __user *data, size_t count,
		loff_t *ppos, int nonblocking)
{
	return __vb2_perform_fileio(q, data, count, ppos, nonblocking, 0);
}
EXPORT_SYMBOL_GPL(vb2_write);


/*
 * The following functions are not part of the vb2 core API, but are helper
 * functions that plug into struct v4l2_ioctl_ops, struct v4l2_file_operations
 * and struct vb2_ops.
 * They contain boilerplate code that most if not all drivers have to do
 * and so they simplify the driver code.
 */

/* The queue is busy if there is a owner and you are not that owner. */
static inline bool vb2_queue_is_busy(struct video_device *vdev, struct file *file)
{
	return vdev->queue->owner && vdev->queue->owner != file->private_data;
}

/* vb2 ioctl helpers */

int vb2_ioctl_reqbufs(struct file *file, void *priv,
			  struct v4l2_requestbuffers *p)
{
	struct video_device *vdev = video_devdata(file);
	int res = __verify_memory_type(vdev->queue, p->memory, p->type);

	if (res)
		return res;
	if (vb2_queue_is_busy(vdev, file))
		return -EBUSY;
	res = __reqbufs(vdev->queue, p);
	/* If count == 0, then the owner has released all buffers and he
	   is no longer owner of the queue. Otherwise we have a new owner. */
	if (res == 0)
		vdev->queue->owner = p->count ? file->private_data : NULL;
	return res;
}
EXPORT_SYMBOL_GPL(vb2_ioctl_reqbufs);

int vb2_ioctl_create_bufs(struct file *file, void *priv,
			  struct v4l2_create_buffers *p)
{
	struct video_device *vdev = video_devdata(file);
	int res = __verify_memory_type(vdev->queue, p->memory, p->format.type);

	p->index = vdev->queue->num_buffers;
	/* If count == 0, then just check if memory and type are valid.
	   Any -EBUSY result from __verify_memory_type can be mapped to 0. */
	if (p->count == 0)
		return res != -EBUSY ? res : 0;
	if (res)
		return res;
	if (vb2_queue_is_busy(vdev, file))
		return -EBUSY;
	res = __create_bufs(vdev->queue, p);
	if (res == 0)
		vdev->queue->owner = file->private_data;
	return res;
}
EXPORT_SYMBOL_GPL(vb2_ioctl_create_bufs);

int vb2_ioctl_prepare_buf(struct file *file, void *priv,
			  struct v4l2_buffer *p)
{
	struct video_device *vdev = video_devdata(file);

	if (vb2_queue_is_busy(vdev, file))
		return -EBUSY;
	return vb2_prepare_buf(vdev->queue, p);
}
EXPORT_SYMBOL_GPL(vb2_ioctl_prepare_buf);

int vb2_ioctl_querybuf(struct file *file, void *priv, struct v4l2_buffer *p)
{
	struct video_device *vdev = video_devdata(file);

	/* No need to call vb2_queue_is_busy(), anyone can query buffers. */
	return vb2_querybuf(vdev->queue, p);
}
EXPORT_SYMBOL_GPL(vb2_ioctl_querybuf);

int vb2_ioctl_qbuf(struct file *file, void *priv, struct v4l2_buffer *p)
{
	struct video_device *vdev = video_devdata(file);

	if (vb2_queue_is_busy(vdev, file))
		return -EBUSY;
	return vb2_qbuf(vdev->queue, p);
}
EXPORT_SYMBOL_GPL(vb2_ioctl_qbuf);

int vb2_ioctl_dqbuf(struct file *file, void *priv, struct v4l2_buffer *p)
{
	struct video_device *vdev = video_devdata(file);

	if (vb2_queue_is_busy(vdev, file))
		return -EBUSY;
	return vb2_dqbuf(vdev->queue, p, file->f_flags & O_NONBLOCK);
}
EXPORT_SYMBOL_GPL(vb2_ioctl_dqbuf);

int vb2_ioctl_streamon(struct file *file, void *priv, enum v4l2_buf_type i)
{
	struct video_device *vdev = video_devdata(file);

	if (vb2_queue_is_busy(vdev, file))
		return -EBUSY;
	return vb2_streamon(vdev->queue, i);
}
EXPORT_SYMBOL_GPL(vb2_ioctl_streamon);

int vb2_ioctl_streamoff(struct file *file, void *priv, enum v4l2_buf_type i)
{
	struct video_device *vdev = video_devdata(file);

	if (vb2_queue_is_busy(vdev, file))
		return -EBUSY;
	return vb2_streamoff(vdev->queue, i);
}
EXPORT_SYMBOL_GPL(vb2_ioctl_streamoff);

int vb2_ioctl_expbuf(struct file *file, void *priv, struct v4l2_exportbuffer *p)
{
	struct video_device *vdev = video_devdata(file);

	if (vb2_queue_is_busy(vdev, file))
		return -EBUSY;
	return vb2_expbuf(vdev->queue, p);
}
EXPORT_SYMBOL_GPL(vb2_ioctl_expbuf);

/* v4l2_file_operations helpers */

int vb2_fop_mmap(struct file *file, struct vm_area_struct *vma)
{
	struct video_device *vdev = video_devdata(file);

	return vb2_mmap(vdev->queue, vma);
}
EXPORT_SYMBOL_GPL(vb2_fop_mmap);

int vb2_fop_release(struct file *file)
{
	struct video_device *vdev = video_devdata(file);

	if (file->private_data == vdev->queue->owner) {
		vb2_queue_release(vdev->queue);
		vdev->queue->owner = NULL;
	}
	return v4l2_fh_release(file);
}
EXPORT_SYMBOL_GPL(vb2_fop_release);

ssize_t vb2_fop_write(struct file *file, char __user *buf,
		size_t count, loff_t *ppos)
{
	struct video_device *vdev = video_devdata(file);
	struct mutex *lock = vdev->queue->lock ? vdev->queue->lock : vdev->lock;
	int err = -EBUSY;

	if (lock && mutex_lock_interruptible(lock))
		return -ERESTARTSYS;
	if (vb2_queue_is_busy(vdev, file))
		goto exit;
	err = vb2_write(vdev->queue, buf, count, ppos,
		       file->f_flags & O_NONBLOCK);
	if (vdev->queue->fileio)
		vdev->queue->owner = file->private_data;
exit:
	if (lock)
		mutex_unlock(lock);
	return err;
}
EXPORT_SYMBOL_GPL(vb2_fop_write);

ssize_t vb2_fop_read(struct file *file, char __user *buf,
		size_t count, loff_t *ppos)
{
	struct video_device *vdev = video_devdata(file);
	struct mutex *lock = vdev->queue->lock ? vdev->queue->lock : vdev->lock;
	int err = -EBUSY;

	if (lock && mutex_lock_interruptible(lock))
		return -ERESTARTSYS;
	if (vb2_queue_is_busy(vdev, file))
		goto exit;
	err = vb2_read(vdev->queue, buf, count, ppos,
		       file->f_flags & O_NONBLOCK);
	if (vdev->queue->fileio)
		vdev->queue->owner = file->private_data;
exit:
	if (lock)
		mutex_unlock(lock);
	return err;
}
EXPORT_SYMBOL_GPL(vb2_fop_read);

unsigned int vb2_fop_poll(struct file *file, poll_table *wait)
{
	struct video_device *vdev = video_devdata(file);
	struct vb2_queue *q = vdev->queue;
	struct mutex *lock = q->lock ? q->lock : vdev->lock;
	unsigned long req_events = poll_requested_events(wait);
	unsigned res;
	void *fileio;
	bool must_lock = false;

	/* Try to be smart: only lock if polling might start fileio,
	   otherwise locking will only introduce unwanted delays. */
	if (q->num_buffers == 0 && q->fileio == NULL) {
		if (!V4L2_TYPE_IS_OUTPUT(q->type) && (q->io_modes & VB2_READ) &&
				(req_events & (POLLIN | POLLRDNORM)))
			must_lock = true;
		else if (V4L2_TYPE_IS_OUTPUT(q->type) && (q->io_modes & VB2_WRITE) &&
				(req_events & (POLLOUT | POLLWRNORM)))
			must_lock = true;
	}

	/* If locking is needed, but this helper doesn't know how, then you
	   shouldn't be using this helper but you should write your own. */
	WARN_ON(must_lock && !lock);

	if (must_lock && lock && mutex_lock_interruptible(lock))
		return POLLERR;

	fileio = q->fileio;

	res = vb2_poll(vdev->queue, file, wait);

	/* If fileio was started, then we have a new queue owner. */
	if (must_lock && !fileio && q->fileio)
		q->owner = file->private_data;
	if (must_lock && lock)
		mutex_unlock(lock);
	return res;
}
EXPORT_SYMBOL_GPL(vb2_fop_poll);

#ifndef CONFIG_MMU
unsigned long vb2_fop_get_unmapped_area(struct file *file, unsigned long addr,
		unsigned long len, unsigned long pgoff, unsigned long flags)
{
	struct video_device *vdev = video_devdata(file);

	return vb2_get_unmapped_area(vdev->queue, addr, len, pgoff, flags);
}
EXPORT_SYMBOL_GPL(vb2_fop_get_unmapped_area);
#endif

/* vb2_ops helpers. Only use if vq->lock is non-NULL. */

void vb2_ops_wait_prepare(struct vb2_queue *vq)
{
	mutex_unlock(vq->lock);
}
EXPORT_SYMBOL_GPL(vb2_ops_wait_prepare);

void vb2_ops_wait_finish(struct vb2_queue *vq)
{
	mutex_lock(vq->lock);
}
EXPORT_SYMBOL_GPL(vb2_ops_wait_finish);

MODULE_DESCRIPTION("Driver helper framework for Video for Linux 2");
MODULE_AUTHOR("Pawel Osciak <pawel@osciak.com>, Marek Szyprowski");
MODULE_LICENSE("GPL");
