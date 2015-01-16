/*
 * v4l2-event.h
 *
 * V4L2 events.
 *
 * Copyright (C) 2009--2010 Nokia Corporation.
 *
 * Contact: Sakari Ailus <sakari.ailus@iki.fi>
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * version 2 as published by the Free Software Foundation.
 *
 * This program is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA
 * 02110-1301 USA
 */

#ifndef V4L2_EVENT_H
#define V4L2_EVENT_H

#include <linux/types.h>
#include <linux/videodev2.h>
#include <linux/wait.h>

struct v4l2_fh;
<<<<<<< HEAD
=======
struct v4l2_subdev;
struct v4l2_subscribed_event;
>>>>>>> common/android-3.10.y
struct video_device;

struct v4l2_kevent {
	struct list_head	list;
	struct v4l2_event	event;
};

<<<<<<< HEAD
struct v4l2_subscribed_event {
	struct list_head	list;
	u32			type;
=======
/** struct v4l2_subscribed_event_ops - Subscribed event operations.
  * @add:	Optional callback, called when a new listener is added
  * @del:	Optional callback, called when a listener stops listening
  * @replace:	Optional callback that can replace event 'old' with event 'new'.
  * @merge:	Optional callback that can merge event 'old' into event 'new'.
  */
struct v4l2_subscribed_event_ops {
	int  (*add)(struct v4l2_subscribed_event *sev, unsigned elems);
	void (*del)(struct v4l2_subscribed_event *sev);
	void (*replace)(struct v4l2_event *old, const struct v4l2_event *new);
	void (*merge)(const struct v4l2_event *old, struct v4l2_event *new);
};

/** struct v4l2_subscribed_event - Internal struct representing a subscribed event.
  * @list:	List node for the v4l2_fh->subscribed list.
  * @type:	Event type.
  * @id:	Associated object ID (e.g. control ID). 0 if there isn't any.
  * @flags:	Copy of v4l2_event_subscription->flags.
  * @fh:	Filehandle that subscribed to this event.
  * @node:	List node that hooks into the object's event list (if there is one).
  * @ops:	v4l2_subscribed_event_ops
  * @elems:	The number of elements in the events array.
  * @first:	The index of the events containing the oldest available event.
  * @in_use:	The number of queued events.
  * @events:	An array of @elems events.
  */
struct v4l2_subscribed_event {
	struct list_head	list;
	u32			type;
	u32			id;
	u32			flags;
	struct v4l2_fh		*fh;
	struct list_head	node;
	const struct v4l2_subscribed_event_ops *ops;
	unsigned		elems;
	unsigned		first;
	unsigned		in_use;
	struct v4l2_kevent	events[];
>>>>>>> common/android-3.10.y
};

struct v4l2_events {
	wait_queue_head_t	wait;
	struct list_head	subscribed; 
	struct list_head	free; 
	struct list_head	available; 
	unsigned int		navailable;
	unsigned int		nallocated; 
	u32			sequence;
};

int v4l2_event_init(struct v4l2_fh *fh);
int v4l2_event_alloc(struct v4l2_fh *fh, unsigned int n);
void v4l2_event_free(struct v4l2_fh *fh);
int v4l2_event_dequeue(struct v4l2_fh *fh, struct v4l2_event *event,
		       int nonblocking);
void v4l2_event_queue(struct video_device *vdev, const struct v4l2_event *ev);
int v4l2_event_pending(struct v4l2_fh *fh);
int v4l2_event_subscribe(struct v4l2_fh *fh,
<<<<<<< HEAD
			 struct v4l2_event_subscription *sub);
int v4l2_event_unsubscribe(struct v4l2_fh *fh,
			   struct v4l2_event_subscription *sub);

#endif 
=======
			 const struct v4l2_event_subscription *sub, unsigned elems,
			 const struct v4l2_subscribed_event_ops *ops);
int v4l2_event_unsubscribe(struct v4l2_fh *fh,
			   const struct v4l2_event_subscription *sub);
void v4l2_event_unsubscribe_all(struct v4l2_fh *fh);
int v4l2_event_subdev_unsubscribe(struct v4l2_subdev *sd, struct v4l2_fh *fh,
				  struct v4l2_event_subscription *sub);
#endif /* V4L2_EVENT_H */
>>>>>>> common/android-3.10.y
