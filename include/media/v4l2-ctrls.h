/*
    V4L2 controls support header.

    Copyright (C) 2010  Hans Verkuil <hverkuil@xs4all.nl>

    This program is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation; either version 2 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program; if not, write to the Free Software
    Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 */

#ifndef _V4L2_CTRLS_H
#define _V4L2_CTRLS_H

#include <linux/list.h>
<<<<<<< HEAD
#include <linux/device.h>
#include <linux/mutex.h>
#include <linux/videodev2.h>

=======
#include <linux/mutex.h>
#include <linux/videodev2.h>

/* forward references */
struct file;
>>>>>>> common/android-3.10.y
struct v4l2_ctrl_handler;
struct v4l2_ctrl;
struct video_device;
struct v4l2_subdev;

struct v4l2_ctrl_ops {
	int (*g_volatile_ctrl)(struct v4l2_ctrl *ctrl);
	int (*try_ctrl)(struct v4l2_ctrl *ctrl);
	int (*s_ctrl)(struct v4l2_ctrl *ctrl);
};

<<<<<<< HEAD
=======
typedef void (*v4l2_ctrl_notify_fnc)(struct v4l2_ctrl *ctrl, void *priv);

/** struct v4l2_ctrl - The control structure.
  * @node:	The list node.
  * @ev_subs:	The list of control event subscriptions.
  * @handler:	The handler that owns the control.
  * @cluster:	Point to start of cluster array.
  * @ncontrols:	Number of controls in cluster array.
  * @done:	Internal flag: set for each processed control.
  * @is_new:	Set when the user specified a new value for this control. It
  *		is also set when called from v4l2_ctrl_handler_setup. Drivers
  *		should never set this flag.
  * @is_private: If set, then this control is private to its handler and it
  *		will not be added to any other handlers. Drivers can set
  *		this flag.
  * @is_auto:   If set, then this control selects whether the other cluster
  *		members are in 'automatic' mode or 'manual' mode. This is
  *		used for autogain/gain type clusters. Drivers should never
  *		set this flag directly.
  * @has_volatiles: If set, then one or more members of the cluster are volatile.
  *		Drivers should never touch this flag.
  * @call_notify: If set, then call the handler's notify function whenever the
  *		control's value changes.
  * @manual_mode_value: If the is_auto flag is set, then this is the value
  *		of the auto control that determines if that control is in
  *		manual mode. So if the value of the auto control equals this
  *		value, then the whole cluster is in manual mode. Drivers should
  *		never set this flag directly.
  * @ops:	The control ops.
  * @id:	The control ID.
  * @name:	The control name.
  * @type:	The control type.
  * @minimum:	The control's minimum value.
  * @maximum:	The control's maximum value.
  * @default_value: The control's default value.
  * @step:	The control's step value for non-menu controls.
  * @menu_skip_mask: The control's skip mask for menu controls. This makes it
  *		easy to skip menu items that are not valid. If bit X is set,
  *		then menu item X is skipped. Of course, this only works for
  *		menus with <= 32 menu items. There are no menus that come
  *		close to that number, so this is OK. Should we ever need more,
  *		then this will have to be extended to a u64 or a bit array.
  * @qmenu:	A const char * array for all menu items. Array entries that are
  *		empty strings ("") correspond to non-existing menu items (this
  *		is in addition to the menu_skip_mask above). The last entry
  *		must be NULL.
  * @flags:	The control's flags.
  * @cur:	The control's current value.
  * @val:	The control's new s32 value.
  * @val64:	The control's new s64 value.
  * @string:	The control's new string value.
  * @priv:	The control's private pointer. For use by the driver. It is
  *		untouched by the control framework. Note that this pointer is
  *		not freed when the control is deleted. Should this be needed
  *		then a new internal bitfield can be added to tell the framework
  *		to free this pointer.
  */
>>>>>>> common/android-3.10.y
struct v4l2_ctrl {
	
	struct list_head node;
	struct v4l2_ctrl_handler *handler;
	struct v4l2_ctrl **cluster;
	unsigned ncontrols;
	unsigned int done:1;

	unsigned int is_new:1;
	unsigned int is_private:1;
<<<<<<< HEAD
	unsigned int is_volatile:1;
=======
	unsigned int is_auto:1;
	unsigned int has_volatiles:1;
	unsigned int call_notify:1;
	unsigned int manual_mode_value:8;
>>>>>>> common/android-3.10.y

	const struct v4l2_ctrl_ops *ops;
	u32 id;
	const char *name;
	enum v4l2_ctrl_type type;
	s32 minimum, maximum, default_value;
	union {
		u32 step;
		u32 menu_skip_mask;
	};
	union {
		const char * const *qmenu;
		const s64 *qmenu_int;
	};
	unsigned long flags;
	union {
		s32 val;
		s64 val64;
		char *string;
	} cur;
	union {
		s32 val;
		s64 val64;
		char *string;
	};
	void *priv;
};

struct v4l2_ctrl_ref {
	struct list_head node;
	struct v4l2_ctrl_ref *next;
	struct v4l2_ctrl *ctrl;
};

<<<<<<< HEAD
=======
/** struct v4l2_ctrl_handler - The control handler keeps track of all the
  * controls: both the controls owned by the handler and those inherited
  * from other handlers.
  * @_lock:	Default for "lock".
  * @lock:	Lock to control access to this handler and its controls.
  *		May be replaced by the user right after init.
  * @ctrls:	The list of controls owned by this handler.
  * @ctrl_refs:	The list of control references.
  * @cached:	The last found control reference. It is common that the same
  *		control is needed multiple times, so this is a simple
  *		optimization.
  * @buckets:	Buckets for the hashing. Allows for quick control lookup.
  * @notify:	A notify callback that is called whenever the control changes value.
  *		Note that the handler's lock is held when the notify function
  *		is called!
  * @notify_priv: Passed as argument to the v4l2_ctrl notify callback.
  * @nr_of_buckets: Total number of buckets in the array.
  * @error:	The error code of the first failed control addition.
  */
>>>>>>> common/android-3.10.y
struct v4l2_ctrl_handler {
	struct mutex _lock;
	struct mutex *lock;
	struct list_head ctrls;
	struct list_head ctrl_refs;
	struct v4l2_ctrl_ref *cached;
	struct v4l2_ctrl_ref **buckets;
	v4l2_ctrl_notify_fnc notify;
	void *notify_priv;
	u16 nr_of_buckets;
	int error;
};

struct v4l2_ctrl_config {
	const struct v4l2_ctrl_ops *ops;
	u32 id;
	const char *name;
	enum v4l2_ctrl_type type;
	s32 min;
	s32 max;
	u32 step;
	s32 def;
	u32 flags;
	u32 menu_skip_mask;
	const char * const *qmenu;
	const s64 *qmenu_int;
	unsigned int is_private:1;
	unsigned int is_volatile:1;
};

void v4l2_ctrl_fill(u32 id, const char **name, enum v4l2_ctrl_type *type,
		    s32 *min, s32 *max, s32 *step, s32 *def, u32 *flags);


<<<<<<< HEAD
int v4l2_ctrl_handler_init(struct v4l2_ctrl_handler *hdl,
			   unsigned nr_of_controls_hint);
=======
/** v4l2_ctrl_handler_init_class() - Initialize the control handler.
  * @hdl:	The control handler.
  * @nr_of_controls_hint: A hint of how many controls this handler is
  *		expected to refer to. This is the total number, so including
  *		any inherited controls. It doesn't have to be precise, but if
  *		it is way off, then you either waste memory (too many buckets
  *		are allocated) or the control lookup becomes slower (not enough
  *		buckets are allocated, so there are more slow list lookups).
  *		It will always work, though.
  * @key:	Used by the lock validator if CONFIG_LOCKDEP is set.
  * @name:	Used by the lock validator if CONFIG_LOCKDEP is set.
  *
  * Returns an error if the buckets could not be allocated. This error will
  * also be stored in @hdl->error.
  *
  * Never use this call directly, always use the v4l2_ctrl_handler_init
  * macro that hides the @key and @name arguments.
  */
int v4l2_ctrl_handler_init_class(struct v4l2_ctrl_handler *hdl,
				 unsigned nr_of_controls_hint,
				 struct lock_class_key *key, const char *name);

#ifdef CONFIG_LOCKDEP
#define v4l2_ctrl_handler_init(hdl, nr_of_controls_hint)		\
(									\
	({								\
		static struct lock_class_key _key;			\
		v4l2_ctrl_handler_init_class(hdl, nr_of_controls_hint,	\
					&_key,				\
					KBUILD_BASENAME ":"		\
					__stringify(__LINE__) ":"	\
					"(" #hdl ")->_lock");		\
	})								\
)
#else
#define v4l2_ctrl_handler_init(hdl, nr_of_controls_hint)		\
	v4l2_ctrl_handler_init_class(hdl, nr_of_controls_hint, NULL, NULL)
#endif
>>>>>>> common/android-3.10.y

void v4l2_ctrl_handler_free(struct v4l2_ctrl_handler *hdl);

int v4l2_ctrl_handler_setup(struct v4l2_ctrl_handler *hdl);

void v4l2_ctrl_handler_log_status(struct v4l2_ctrl_handler *hdl,
				  const char *prefix);

struct v4l2_ctrl *v4l2_ctrl_new_custom(struct v4l2_ctrl_handler *hdl,
			const struct v4l2_ctrl_config *cfg, void *priv);

struct v4l2_ctrl *v4l2_ctrl_new_std(struct v4l2_ctrl_handler *hdl,
			const struct v4l2_ctrl_ops *ops,
			u32 id, s32 min, s32 max, u32 step, s32 def);

struct v4l2_ctrl *v4l2_ctrl_new_std_menu(struct v4l2_ctrl_handler *hdl,
			const struct v4l2_ctrl_ops *ops,
			u32 id, s32 max, s32 mask, s32 def);

<<<<<<< HEAD
struct v4l2_ctrl *v4l2_ctrl_add_ctrl(struct v4l2_ctrl_handler *hdl,
					  struct v4l2_ctrl *ctrl);

=======
/** v4l2_ctrl_new_std_menu_items() - Create a new standard V4L2 menu control
  * with driver specific menu.
  * @hdl:	The control handler.
  * @ops:	The control ops.
  * @id:	The control ID.
  * @max:	The control's maximum value.
  * @mask:	The control's skip mask for menu controls. This makes it
  *		easy to skip menu items that are not valid. If bit X is set,
  *		then menu item X is skipped. Of course, this only works for
  *		menus with <= 32 menu items. There are no menus that come
  *		close to that number, so this is OK. Should we ever need more,
  *		then this will have to be extended to a u64 or a bit array.
  * @def:	The control's default value.
  * @qmenu:	The new menu.
  *
  * Same as v4l2_ctrl_new_std_menu(), but @qmenu will be the driver specific
  * menu of this control.
  *
  */
struct v4l2_ctrl *v4l2_ctrl_new_std_menu_items(struct v4l2_ctrl_handler *hdl,
			const struct v4l2_ctrl_ops *ops, u32 id, s32 max,
			s32 mask, s32 def, const char * const *qmenu);

/** v4l2_ctrl_new_int_menu() - Create a new standard V4L2 integer menu control.
  * @hdl:	The control handler.
  * @ops:	The control ops.
  * @id:	The control ID.
  * @max:	The control's maximum value.
  * @def:	The control's default value.
  * @qmenu_int:	The control's menu entries.
  *
  * Same as v4l2_ctrl_new_std_menu(), but @mask is set to 0 and it additionaly
  * takes as an argument an array of integers determining the menu items.
  *
  * If @id refers to a non-integer-menu control, then this function will return NULL.
  */
struct v4l2_ctrl *v4l2_ctrl_new_int_menu(struct v4l2_ctrl_handler *hdl,
			const struct v4l2_ctrl_ops *ops,
			u32 id, s32 max, s32 def, const s64 *qmenu_int);

/** v4l2_ctrl_add_ctrl() - Add a control from another handler to this handler.
  * @hdl:	The control handler.
  * @ctrl:	The control to add.
  *
  * It will return NULL if it was unable to add the control reference.
  * If the control already belonged to the handler, then it will do
  * nothing and just return @ctrl.
  */
struct v4l2_ctrl *v4l2_ctrl_add_ctrl(struct v4l2_ctrl_handler *hdl,
					  struct v4l2_ctrl *ctrl);

/** v4l2_ctrl_add_handler() - Add all controls from handler @add to
  * handler @hdl.
  * @hdl:	The control handler.
  * @add:	The control handler whose controls you want to add to
  *		the @hdl control handler.
  * @filter:	This function will filter which controls should be added.
  *
  * Does nothing if either of the two handlers is a NULL pointer.
  * If @filter is NULL, then all controls are added. Otherwise only those
  * controls for which @filter returns true will be added.
  * In case of an error @hdl->error will be set to the error code (if it
  * wasn't set already).
  */
>>>>>>> common/android-3.10.y
int v4l2_ctrl_add_handler(struct v4l2_ctrl_handler *hdl,
			  struct v4l2_ctrl_handler *add,
			  bool (*filter)(const struct v4l2_ctrl *ctrl));

/** v4l2_ctrl_radio_filter() - Standard filter for radio controls.
  * @ctrl:	The control that is filtered.
  *
  * This will return true for any controls that are valid for radio device
  * nodes. Those are all of the V4L2_CID_AUDIO_* user controls and all FM
  * transmitter class controls.
  *
  * This function is to be used with v4l2_ctrl_add_handler().
  */
bool v4l2_ctrl_radio_filter(const struct v4l2_ctrl *ctrl);

void v4l2_ctrl_cluster(unsigned ncontrols, struct v4l2_ctrl **controls);


struct v4l2_ctrl *v4l2_ctrl_find(struct v4l2_ctrl_handler *hdl, u32 id);

void v4l2_ctrl_activate(struct v4l2_ctrl *ctrl, bool active);

void v4l2_ctrl_grab(struct v4l2_ctrl *ctrl, bool grabbed);

<<<<<<< HEAD
=======
/** v4l2_ctrl_modify_range() - Update the range of a control.
  * @ctrl:	The control to update.
  * @min:	The control's minimum value.
  * @max:	The control's maximum value.
  * @step:	The control's step value
  * @def:	The control's default value.
  *
  * Update the range of a control on the fly. This works for control types
  * INTEGER, BOOLEAN, MENU, INTEGER MENU and BITMASK. For menu controls the
  * @step value is interpreted as a menu_skip_mask.
  *
  * An error is returned if one of the range arguments is invalid for this
  * control type.
  *
  * This function assumes that the control handler is not locked and will
  * take the lock itself.
  */
int v4l2_ctrl_modify_range(struct v4l2_ctrl *ctrl,
			s32 min, s32 max, u32 step, s32 def);

/** v4l2_ctrl_lock() - Helper function to lock the handler
  * associated with the control.
  * @ctrl:	The control to lock.
  */
>>>>>>> common/android-3.10.y
static inline void v4l2_ctrl_lock(struct v4l2_ctrl *ctrl)
{
	mutex_lock(ctrl->handler->lock);
}

static inline void v4l2_ctrl_unlock(struct v4l2_ctrl *ctrl)
{
	mutex_unlock(ctrl->handler->lock);
}

<<<<<<< HEAD
=======
/** v4l2_ctrl_notify() - Function to set a notify callback for a control.
  * @ctrl:	The control.
  * @notify:	The callback function.
  * @priv:	The callback private handle, passed as argument to the callback.
  *
  * This function sets a callback function for the control. If @ctrl is NULL,
  * then it will do nothing. If @notify is NULL, then the notify callback will
  * be removed.
  *
  * There can be only one notify. If another already exists, then a WARN_ON
  * will be issued and the function will do nothing.
  */
void v4l2_ctrl_notify(struct v4l2_ctrl *ctrl, v4l2_ctrl_notify_fnc notify, void *priv);

/** v4l2_ctrl_g_ctrl() - Helper function to get the control's value from within a driver.
  * @ctrl:	The control.
  *
  * This returns the control's value safely by going through the control
  * framework. This function will lock the control's handler, so it cannot be
  * used from within the &v4l2_ctrl_ops functions.
  *
  * This function is for integer type controls only.
  */
>>>>>>> common/android-3.10.y
s32 v4l2_ctrl_g_ctrl(struct v4l2_ctrl *ctrl);

int v4l2_ctrl_s_ctrl(struct v4l2_ctrl *ctrl, s32 val);

<<<<<<< HEAD
=======
/** v4l2_ctrl_g_ctrl_int64() - Helper function to get a 64-bit control's value from within a driver.
  * @ctrl:	The control.
  *
  * This returns the control's value safely by going through the control
  * framework. This function will lock the control's handler, so it cannot be
  * used from within the &v4l2_ctrl_ops functions.
  *
  * This function is for 64-bit integer type controls only.
  */
s64 v4l2_ctrl_g_ctrl_int64(struct v4l2_ctrl *ctrl);

/** v4l2_ctrl_s_ctrl_int64() - Helper function to set a 64-bit control's value from within a driver.
  * @ctrl:	The control.
  * @val:	The new value.
  *
  * This set the control's new value safely by going through the control
  * framework. This function will lock the control's handler, so it cannot be
  * used from within the &v4l2_ctrl_ops functions.
  *
  * This function is for 64-bit integer type controls only.
  */
int v4l2_ctrl_s_ctrl_int64(struct v4l2_ctrl *ctrl, s64 val);

/* Internal helper functions that deal with control events. */
extern const struct v4l2_subscribed_event_ops v4l2_ctrl_sub_ev_ops;
void v4l2_ctrl_replace(struct v4l2_event *old, const struct v4l2_event *new);
void v4l2_ctrl_merge(const struct v4l2_event *old, struct v4l2_event *new);

/* Can be used as a vidioc_log_status function that just dumps all controls
   associated with the filehandle. */
int v4l2_ctrl_log_status(struct file *file, void *fh);

/* Can be used as a vidioc_subscribe_event function that just subscribes
   control events. */
int v4l2_ctrl_subscribe_event(struct v4l2_fh *fh,
				const struct v4l2_event_subscription *sub);

/* Can be used as a poll function that just polls for control events. */
unsigned int v4l2_ctrl_poll(struct file *file, struct poll_table_struct *wait);
>>>>>>> common/android-3.10.y

int v4l2_queryctrl(struct v4l2_ctrl_handler *hdl, struct v4l2_queryctrl *qc);
int v4l2_querymenu(struct v4l2_ctrl_handler *hdl, struct v4l2_querymenu *qm);
int v4l2_g_ctrl(struct v4l2_ctrl_handler *hdl, struct v4l2_control *ctrl);
int v4l2_s_ctrl(struct v4l2_ctrl_handler *hdl, struct v4l2_control *ctrl);
int v4l2_g_ext_ctrls(struct v4l2_ctrl_handler *hdl, struct v4l2_ext_controls *c);
int v4l2_try_ext_ctrls(struct v4l2_ctrl_handler *hdl, struct v4l2_ext_controls *c);
int v4l2_s_ext_ctrls(struct v4l2_ctrl_handler *hdl, struct v4l2_ext_controls *c);

int v4l2_subdev_queryctrl(struct v4l2_subdev *sd, struct v4l2_queryctrl *qc);
int v4l2_subdev_querymenu(struct v4l2_subdev *sd, struct v4l2_querymenu *qm);
int v4l2_subdev_g_ext_ctrls(struct v4l2_subdev *sd, struct v4l2_ext_controls *cs);
int v4l2_subdev_try_ext_ctrls(struct v4l2_subdev *sd, struct v4l2_ext_controls *cs);
int v4l2_subdev_s_ext_ctrls(struct v4l2_subdev *sd, struct v4l2_ext_controls *cs);
int v4l2_subdev_g_ctrl(struct v4l2_subdev *sd, struct v4l2_control *ctrl);
int v4l2_subdev_s_ctrl(struct v4l2_subdev *sd, struct v4l2_control *ctrl);

/* Can be used as a subscribe_event function that just subscribes control
   events. */
int v4l2_ctrl_subdev_subscribe_event(struct v4l2_subdev *sd, struct v4l2_fh *fh,
				     struct v4l2_event_subscription *sub);

/* Log all controls owned by subdev's control handler. */
int v4l2_ctrl_subdev_log_status(struct v4l2_subdev *sd);

#endif
