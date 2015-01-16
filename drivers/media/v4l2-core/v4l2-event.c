/*
 * v4l2-event.c
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

#include <linux/module.h>
#include <media/v4l2-dev.h>
#include <media/v4l2-fh.h>
#include <media/v4l2-event.h>

#include <linux/sched.h>
#include <linux/slab.h>

int v4l2_event_init(struct v4l2_fh *fh)
{
	fh->events = kzalloc(sizeof(*fh->events), GFP_KERNEL);
	if (fh->events == NULL)
		return -ENOMEM;

	init_waitqueue_head(&fh->events->wait);

	INIT_LIST_HEAD(&fh->events->free);
	INIT_LIST_HEAD(&fh->events->available);
	INIT_LIST_HEAD(&fh->events->subscribed);

	fh->events->sequence = -1;

	return 0;
}
EXPORT_SYMBOL_GPL(v4l2_event_init);

int v4l2_event_alloc(struct v4l2_fh *fh, unsigned int n)
{
	struct v4l2_events *events = fh->events;
	unsigned long flags;

	if (!events) {
		WARN_ON(1);
		return -ENOMEM;
	}

	while (events->nallocated < n) {
		struct v4l2_kevent *kev;

		kev = kzalloc(sizeof(*kev), GFP_KERNEL);
		if (kev == NULL)
			return -ENOMEM;

		spin_lock_irqsave(&fh->vdev->fh_lock, flags);
		list_add_tail(&kev->list, &events->free);
		events->nallocated++;
		spin_unlock_irqrestore(&fh->vdev->fh_lock, flags);
	}

	return 0;
}
EXPORT_SYMBOL_GPL(v4l2_event_alloc);

#define list_kfree(list, type, member)				\
	while (!list_empty(list)) {				\
		type *hi;					\
		hi = list_first_entry(list, type, member);	\
		list_del(&hi->member);				\
		kfree(hi);					\
	}

void v4l2_event_free(struct v4l2_fh *fh)
{
	struct v4l2_events *events = fh->events;

	if (!events)
		return;

	list_kfree(&events->free, struct v4l2_kevent, list);
	list_kfree(&events->available, struct v4l2_kevent, list);
	list_kfree(&events->subscribed, struct v4l2_subscribed_event, list);

	kfree(events);
	fh->events = NULL;
}
EXPORT_SYMBOL_GPL(v4l2_event_free);

static int __v4l2_event_dequeue(struct v4l2_fh *fh, struct v4l2_event *event)
{
	struct v4l2_events *events = fh->events;
	struct v4l2_kevent *kev;
	unsigned long flags;

	spin_lock_irqsave(&fh->vdev->fh_lock, flags);

	if (list_empty(&events->available)) {
		spin_unlock_irqrestore(&fh->vdev->fh_lock, flags);
		return -ENOENT;
	}

	WARN_ON(events->navailable == 0);

	kev = list_first_entry(&events->available, struct v4l2_kevent, list);
	list_move(&kev->list, &events->free);
	events->navailable--;

	kev->event.pending = events->navailable;
	*event = kev->event;

	spin_unlock_irqrestore(&fh->vdev->fh_lock, flags);

	return 0;
}

int v4l2_event_dequeue(struct v4l2_fh *fh, struct v4l2_event *event,
		       int nonblocking)
{
	struct v4l2_events *events = fh->events;
	int ret;

	if (nonblocking)
		return __v4l2_event_dequeue(fh, event);

	
	if (fh->vdev->lock)
		mutex_unlock(fh->vdev->lock);

	do {
		ret = wait_event_interruptible(events->wait,
					       events->navailable != 0);
		if (ret < 0)
			break;

		ret = __v4l2_event_dequeue(fh, event);
	} while (ret == -ENOENT);

	if (fh->vdev->lock)
		mutex_lock(fh->vdev->lock);

	return ret;
}
EXPORT_SYMBOL_GPL(v4l2_event_dequeue);

static struct v4l2_subscribed_event *v4l2_event_subscribed(
	struct v4l2_fh *fh, u32 type)
{
	struct v4l2_events *events = fh->events;
	struct v4l2_subscribed_event *sev;

	assert_spin_locked(&fh->vdev->fh_lock);

	list_for_each_entry(sev, &events->subscribed, list) {
		if (sev->type == type)
			return sev;
<<<<<<< HEAD:drivers/media/video/v4l2-event.c
=======

	return NULL;
}

static void __v4l2_event_queue_fh(struct v4l2_fh *fh, const struct v4l2_event *ev,
		const struct timespec *ts)
{
	struct v4l2_subscribed_event *sev;
	struct v4l2_kevent *kev;
	bool copy_payload = true;

	/* Are we subscribed? */
	sev = v4l2_event_subscribed(fh, ev->type, ev->id);
	if (sev == NULL)
		return;

	/*
	 * If the event has been added to the fh->subscribed list, but its
	 * add op has not completed yet elems will be 0, treat this as
	 * not being subscribed.
	 */
	if (!sev->elems)
		return;

	/* Increase event sequence number on fh. */
	fh->sequence++;

	/* Do we have any free events? */
	if (sev->in_use == sev->elems) {
		/* no, remove the oldest one */
		kev = sev->events + sev_pos(sev, 0);
		list_del(&kev->list);
		sev->in_use--;
		sev->first = sev_pos(sev, 1);
		fh->navailable--;
		if (sev->elems == 1) {
			if (sev->ops && sev->ops->replace) {
				sev->ops->replace(&kev->event, ev);
				copy_payload = false;
			}
		} else if (sev->ops && sev->ops->merge) {
			struct v4l2_kevent *second_oldest =
				sev->events + sev_pos(sev, 0);
			sev->ops->merge(&kev->event, &second_oldest->event);
		}
>>>>>>> common/android-3.10.y:drivers/media/v4l2-core/v4l2-event.c
	}

	return NULL;
}

void v4l2_event_queue(struct video_device *vdev, const struct v4l2_event *ev)
{
	struct v4l2_fh *fh;
	unsigned long flags;
	struct timespec timestamp;

	ktime_get_ts(&timestamp);

	spin_lock_irqsave(&vdev->fh_lock, flags);

	list_for_each_entry(fh, &vdev->fh_list, list) {
		struct v4l2_events *events = fh->events;
		struct v4l2_kevent *kev;

		
		if (!v4l2_event_subscribed(fh, ev->type))
			continue;

		
		events->sequence++;

		
		if (list_empty(&events->free)) {
			
			pr_err("%s, no free event queues", __func__);
			continue;
		}
		
		kev = list_first_entry(&events->free, struct v4l2_kevent, list);
		kev->event.type = ev->type;
		kev->event.u = ev->u;
		kev->event.timestamp = timestamp;
		kev->event.sequence = events->sequence;
		list_move_tail(&kev->list, &events->available);

		events->navailable++;

<<<<<<< HEAD:drivers/media/video/v4l2-event.c
		wake_up_all(&events->wait);
	}

	spin_unlock_irqrestore(&vdev->fh_lock, flags);
}
EXPORT_SYMBOL_GPL(v4l2_event_queue);

int v4l2_event_pending(struct v4l2_fh *fh)
{
	if (!(fh) || !(fh)->events) return 0;
	return fh->events->navailable;
}
EXPORT_SYMBOL_GPL(v4l2_event_pending);

int v4l2_event_subscribe(struct v4l2_fh *fh,
			 struct v4l2_event_subscription *sub)
{
	struct v4l2_events *events = fh->events;
	struct v4l2_subscribed_event *sev;
=======
int v4l2_event_subscribe(struct v4l2_fh *fh,
			 const struct v4l2_event_subscription *sub, unsigned elems,
			 const struct v4l2_subscribed_event_ops *ops)
{
	struct v4l2_subscribed_event *sev, *found_ev;
>>>>>>> common/android-3.10.y:drivers/media/v4l2-core/v4l2-event.c
	unsigned long flags;

<<<<<<< HEAD:drivers/media/video/v4l2-event.c
	if (fh->events == NULL) {
		WARN_ON(1);
		return -ENOMEM;
	}
=======
	if (sub->type == V4L2_EVENT_ALL)
		return -EINVAL;

	if (elems < 1)
		elems = 1;
>>>>>>> common/android-3.10.y:drivers/media/v4l2-core/v4l2-event.c

	sev = kmalloc(sizeof(*sev), GFP_KERNEL);
	if (!sev)
		return -ENOMEM;
<<<<<<< HEAD:drivers/media/video/v4l2-event.c
=======
	for (i = 0; i < elems; i++)
		sev->events[i].sev = sev;
	sev->type = sub->type;
	sev->id = sub->id;
	sev->flags = sub->flags;
	sev->fh = fh;
	sev->ops = ops;
>>>>>>> common/android-3.10.y:drivers/media/v4l2-core/v4l2-event.c

	spin_lock_irqsave(&fh->vdev->fh_lock, flags);

	if (v4l2_event_subscribed(fh, sub->type) == NULL) {
		INIT_LIST_HEAD(&sev->list);
		sev->type = sub->type;

		list_add(&sev->list, &events->subscribed);
		sev = NULL;
	}

	spin_unlock_irqrestore(&fh->vdev->fh_lock, flags);

<<<<<<< HEAD:drivers/media/video/v4l2-event.c
	kfree(sev);
=======
	if (found_ev) {
		kfree(sev);
		return 0; /* Already listening */
	}

	if (sev->ops && sev->ops->add) {
		int ret = sev->ops->add(sev, elems);
		if (ret) {
			sev->ops = NULL;
			v4l2_event_unsubscribe(fh, sub);
			return ret;
		}
	}

	/* Mark as ready for use */
	sev->elems = elems;
>>>>>>> common/android-3.10.y:drivers/media/v4l2-core/v4l2-event.c

	return 0;
}
EXPORT_SYMBOL_GPL(v4l2_event_subscribe);

static void v4l2_event_unsubscribe_all(struct v4l2_fh *fh)
{
	struct v4l2_events *events = fh->events;
	struct v4l2_subscribed_event *sev;
	unsigned long flags;

	do {
		sev = NULL;

		spin_lock_irqsave(&fh->vdev->fh_lock, flags);
		if (!list_empty(&events->subscribed)) {
			sev = list_first_entry(&events->subscribed,
				       struct v4l2_subscribed_event, list);
			list_del(&sev->list);
		}
		spin_unlock_irqrestore(&fh->vdev->fh_lock, flags);
		kfree(sev);
	} while (sev);
}

int v4l2_event_unsubscribe(struct v4l2_fh *fh,
			   const struct v4l2_event_subscription *sub)
{
	struct v4l2_subscribed_event *sev;
	unsigned long flags;

	if (sub->type == V4L2_EVENT_ALL) {
		v4l2_event_unsubscribe_all(fh);
		return 0;
	}

	spin_lock_irqsave(&fh->vdev->fh_lock, flags);

	sev = v4l2_event_subscribed(fh, sub->type);
	if (sev != NULL)
		list_del(&sev->list);

	spin_unlock_irqrestore(&fh->vdev->fh_lock, flags);
<<<<<<< HEAD:drivers/media/video/v4l2-event.c
=======

	if (sev && sev->ops && sev->ops->del)
		sev->ops->del(sev);
>>>>>>> common/android-3.10.y:drivers/media/v4l2-core/v4l2-event.c

	kfree(sev);

	return 0;
}
EXPORT_SYMBOL_GPL(v4l2_event_unsubscribe);

int v4l2_event_subdev_unsubscribe(struct v4l2_subdev *sd, struct v4l2_fh *fh,
				  struct v4l2_event_subscription *sub)
{
	return v4l2_event_unsubscribe(fh, sub);
}
EXPORT_SYMBOL_GPL(v4l2_event_subdev_unsubscribe);
