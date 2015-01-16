/*
 * videobuf2-vmalloc.c - vmalloc memory allocator for videobuf2
 *
 * Copyright (C) 2010 Samsung Electronics
 *
 * Author: Pawel Osciak <pawel@osciak.com>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation.
 */

#include <linux/module.h>
#include <linux/mm.h>
#include <linux/slab.h>
#include <linux/vmalloc.h>

#include <media/videobuf2-core.h>
#include <media/videobuf2-vmalloc.h>
#include <media/videobuf2-memops.h>

struct vb2_vmalloc_buf {
	void				*vaddr;
	unsigned long			size;
	atomic_t			refcount;
	struct vb2_vmarea_handler	handler;
	struct dma_buf			*dbuf;
};

static void vb2_vmalloc_put(void *buf_priv);

static void *vb2_vmalloc_alloc(void *alloc_ctx, unsigned long size, gfp_t gfp_flags)
{
	struct vb2_vmalloc_buf *buf;

<<<<<<< HEAD:drivers/media/video/videobuf2-vmalloc.c
	buf = kzalloc(sizeof *buf, GFP_KERNEL);
=======
	buf = kzalloc(sizeof(*buf), GFP_KERNEL | gfp_flags);
>>>>>>> common/android-3.10.y:drivers/media/v4l2-core/videobuf2-vmalloc.c
	if (!buf)
		return NULL;

	buf->size = size;
	buf->vaddr = vmalloc_user(buf->size);
	buf->handler.refcount = &buf->refcount;
	buf->handler.put = vb2_vmalloc_put;
	buf->handler.arg = buf;

	if (!buf->vaddr) {
		printk(KERN_ERR "vmalloc of size %ld failed\n", buf->size);
		kfree(buf);
		return NULL;
	}

	atomic_inc(&buf->refcount);
	printk(KERN_DEBUG "Allocated vmalloc buffer of size %ld at vaddr=%p\n",
			buf->size, buf->vaddr);

	return buf;
}

static void vb2_vmalloc_put(void *buf_priv)
{
	struct vb2_vmalloc_buf *buf = buf_priv;

	if (atomic_dec_and_test(&buf->refcount)) {
		printk(KERN_DEBUG "%s: Freeing vmalloc mem at vaddr=%p\n",
			__func__, buf->vaddr);
		vfree(buf->vaddr);
		kfree(buf);
	}
}

static void *vb2_vmalloc_vaddr(void *buf_priv)
{
	struct vb2_vmalloc_buf *buf = buf_priv;

	BUG_ON(!buf);

	if (!buf->vaddr) {
		printk(KERN_ERR "Address of an unallocated plane requested\n");
		return NULL;
	}

	return buf->vaddr;
}

static unsigned int vb2_vmalloc_num_users(void *buf_priv)
{
	struct vb2_vmalloc_buf *buf = buf_priv;
	return atomic_read(&buf->refcount);
}

static int vb2_vmalloc_mmap(void *buf_priv, struct vm_area_struct *vma)
{
	struct vb2_vmalloc_buf *buf = buf_priv;
	int ret;

	if (!buf) {
		printk(KERN_ERR "No memory to map\n");
		return -EINVAL;
	}

	ret = remap_vmalloc_range(vma, buf->vaddr, 0);
	if (ret) {
		printk(KERN_ERR "Remapping vmalloc memory, error: %d\n", ret);
		return ret;
	}

	vma->vm_flags		|= VM_DONTEXPAND;

	vma->vm_private_data	= &buf->handler;
	vma->vm_ops		= &vb2_common_vm_ops;

	vma->vm_ops->open(vma);

	return 0;
}

/*********************************************/
/*       callbacks for DMABUF buffers        */
/*********************************************/

static int vb2_vmalloc_map_dmabuf(void *mem_priv)
{
	struct vb2_vmalloc_buf *buf = mem_priv;

	buf->vaddr = dma_buf_vmap(buf->dbuf);

	return buf->vaddr ? 0 : -EFAULT;
}

static void vb2_vmalloc_unmap_dmabuf(void *mem_priv)
{
	struct vb2_vmalloc_buf *buf = mem_priv;

	dma_buf_vunmap(buf->dbuf, buf->vaddr);
	buf->vaddr = NULL;
}

static void vb2_vmalloc_detach_dmabuf(void *mem_priv)
{
	struct vb2_vmalloc_buf *buf = mem_priv;

	if (buf->vaddr)
		dma_buf_vunmap(buf->dbuf, buf->vaddr);

	kfree(buf);
}

static void *vb2_vmalloc_attach_dmabuf(void *alloc_ctx, struct dma_buf *dbuf,
	unsigned long size, int write)
{
	struct vb2_vmalloc_buf *buf;

	if (dbuf->size < size)
		return ERR_PTR(-EFAULT);

	buf = kzalloc(sizeof(*buf), GFP_KERNEL);
	if (!buf)
		return ERR_PTR(-ENOMEM);

	buf->dbuf = dbuf;
	buf->write = write;
	buf->size = size;

	return buf;
}


const struct vb2_mem_ops vb2_vmalloc_memops = {
	.alloc		= vb2_vmalloc_alloc,
	.put		= vb2_vmalloc_put,
<<<<<<< HEAD:drivers/media/video/videobuf2-vmalloc.c
=======
	.get_userptr	= vb2_vmalloc_get_userptr,
	.put_userptr	= vb2_vmalloc_put_userptr,
	.map_dmabuf	= vb2_vmalloc_map_dmabuf,
	.unmap_dmabuf	= vb2_vmalloc_unmap_dmabuf,
	.attach_dmabuf	= vb2_vmalloc_attach_dmabuf,
	.detach_dmabuf	= vb2_vmalloc_detach_dmabuf,
>>>>>>> common/android-3.10.y:drivers/media/v4l2-core/videobuf2-vmalloc.c
	.vaddr		= vb2_vmalloc_vaddr,
	.mmap		= vb2_vmalloc_mmap,
	.num_users	= vb2_vmalloc_num_users,
};
EXPORT_SYMBOL_GPL(vb2_vmalloc_memops);

MODULE_DESCRIPTION("vmalloc memory handling routines for videobuf2");
MODULE_AUTHOR("Pawel Osciak <pawel@osciak.com>");
MODULE_LICENSE("GPL");
