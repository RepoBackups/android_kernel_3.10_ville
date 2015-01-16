/*
    V4L2 sub-device support header.

    Copyright (C) 2008  Hans Verkuil <hverkuil@xs4all.nl>

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

#ifndef _V4L2_SUBDEV_H
#define _V4L2_SUBDEV_H

#include <linux/types.h>
#include <linux/v4l2-subdev.h>
#include <media/media-entity.h>
#include <media/v4l2-common.h>
#include <media/v4l2-dev.h>
#include <media/v4l2-fh.h>
#include <media/v4l2-mediabus.h>

#define V4L2_SUBDEV_IR_RX_NOTIFY		_IOW('v', 0, u32)
#define V4L2_SUBDEV_IR_RX_FIFO_SERVICE_REQ	0x00000001
#define V4L2_SUBDEV_IR_RX_END_OF_RX_DETECTED	0x00000002
#define V4L2_SUBDEV_IR_RX_HW_FIFO_OVERRUN	0x00000004
#define V4L2_SUBDEV_IR_RX_SW_FIFO_OVERRUN	0x00000008

#define V4L2_SUBDEV_IR_TX_NOTIFY		_IOW('v', 1, u32)
#define V4L2_SUBDEV_IR_TX_FIFO_SERVICE_REQ	0x00000001

struct v4l2_device;
struct v4l2_ctrl_handler;
struct v4l2_event_subscription;
struct v4l2_fh;
struct v4l2_subdev;
struct v4l2_subdev_fh;
struct tuner_setup;
struct v4l2_mbus_frame_desc;

struct v4l2_decode_vbi_line {
	u32 is_second_field;	
	u8 *p; 			
	u32 line;		
	u32 type;		
};



#define V4L2_SUBDEV_IO_PIN_DISABLE	(1 << 0) 
#define V4L2_SUBDEV_IO_PIN_OUTPUT	(1 << 1)
#define V4L2_SUBDEV_IO_PIN_INPUT	(1 << 2)
#define V4L2_SUBDEV_IO_PIN_SET_VALUE	(1 << 3) 
#define V4L2_SUBDEV_IO_PIN_ACTIVE_LOW	(1 << 4) 

struct v4l2_subdev_io_pin_config {
	u32 flags;	
	u8 pin;		
	u8 function;	
	u8 value;	
	u8 strength;	
};

<<<<<<< HEAD
=======
/*
   s_io_pin_config: configure one or more chip I/O pins for chips that
	multiplex different internal signal pads out to IO pins.  This function
	takes a pointer to an array of 'n' pin configuration entries, one for
	each pin being configured.  This function could be called at times
	other than just subdevice initialization.

   init: initialize the sensor registers to some sort of reasonable default
	values. Do not use for new drivers and should be removed in existing
	drivers.

   load_fw: load firmware.

   reset: generic reset command. The argument selects which subsystems to
	reset. Passing 0 will always reset the whole chip. Do not use for new
	drivers without discussing this first on the linux-media mailinglist.
	There should be no reason normally to reset a device.

   s_gpio: set GPIO pins. Very simple right now, might need to be extended with
	a direction argument if needed.

   s_power: puts subdevice in power saving mode (on == 0) or normal operation
	mode (on == 1).

   interrupt_service_routine: Called by the bridge chip's interrupt service
	handler, when an interrupt status has be raised due to this subdev,
	so that this subdev can handle the details.  It may schedule work to be
	performed later.  It must not sleep.  *Called from an IRQ context*.
 */
>>>>>>> common/android-3.10.y
struct v4l2_subdev_core_ops {
	int (*g_chip_ident)(struct v4l2_subdev *sd, struct v4l2_dbg_chip_ident *chip);
	int (*log_status)(struct v4l2_subdev *sd);
	int (*s_io_pin_config)(struct v4l2_subdev *sd, size_t n,
				      struct v4l2_subdev_io_pin_config *pincfg);
	int (*init)(struct v4l2_subdev *sd, u32 val);
	int (*load_fw)(struct v4l2_subdev *sd);
	int (*reset)(struct v4l2_subdev *sd, u32 val);
	int (*s_gpio)(struct v4l2_subdev *sd, u32 val);
	int (*queryctrl)(struct v4l2_subdev *sd, struct v4l2_queryctrl *qc);
	int (*g_ctrl)(struct v4l2_subdev *sd, struct v4l2_control *ctrl);
	int (*s_ctrl)(struct v4l2_subdev *sd, struct v4l2_control *ctrl);
	int (*g_ext_ctrls)(struct v4l2_subdev *sd, struct v4l2_ext_controls *ctrls);
	int (*s_ext_ctrls)(struct v4l2_subdev *sd, struct v4l2_ext_controls *ctrls);
	int (*try_ext_ctrls)(struct v4l2_subdev *sd, struct v4l2_ext_controls *ctrls);
	int (*querymenu)(struct v4l2_subdev *sd, struct v4l2_querymenu *qm);
	int (*s_std)(struct v4l2_subdev *sd, v4l2_std_id norm);
	long (*ioctl)(struct v4l2_subdev *sd, unsigned int cmd, void *arg);
#ifdef CONFIG_VIDEO_ADV_DEBUG
	int (*g_register)(struct v4l2_subdev *sd, struct v4l2_dbg_register *reg);
	int (*s_register)(struct v4l2_subdev *sd, const struct v4l2_dbg_register *reg);
#endif
	int (*s_power)(struct v4l2_subdev *sd, int on);
	int (*interrupt_service_routine)(struct v4l2_subdev *sd,
						u32 status, bool *handled);
	int (*subscribe_event)(struct v4l2_subdev *sd, struct v4l2_fh *fh,
			       struct v4l2_event_subscription *sub);
	int (*unsubscribe_event)(struct v4l2_subdev *sd, struct v4l2_fh *fh,
				 struct v4l2_event_subscription *sub);
};

struct v4l2_subdev_tuner_ops {
	int (*s_radio)(struct v4l2_subdev *sd);
	int (*s_frequency)(struct v4l2_subdev *sd, const struct v4l2_frequency *freq);
	int (*g_frequency)(struct v4l2_subdev *sd, struct v4l2_frequency *freq);
	int (*g_tuner)(struct v4l2_subdev *sd, struct v4l2_tuner *vt);
	int (*s_tuner)(struct v4l2_subdev *sd, const struct v4l2_tuner *vt);
	int (*g_modulator)(struct v4l2_subdev *sd, struct v4l2_modulator *vm);
	int (*s_modulator)(struct v4l2_subdev *sd, const struct v4l2_modulator *vm);
	int (*s_type_addr)(struct v4l2_subdev *sd, struct tuner_setup *type);
	int (*s_config)(struct v4l2_subdev *sd, const struct v4l2_priv_tun_config *config);
};

struct v4l2_subdev_audio_ops {
	int (*s_clock_freq)(struct v4l2_subdev *sd, u32 freq);
	int (*s_i2s_clock_freq)(struct v4l2_subdev *sd, u32 freq);
	int (*s_routing)(struct v4l2_subdev *sd, u32 input, u32 output, u32 config);
	int (*s_stream)(struct v4l2_subdev *sd, int enable);
};

<<<<<<< HEAD
=======
/* Indicates the @length field specifies maximum data length. */
#define V4L2_MBUS_FRAME_DESC_FL_LEN_MAX		(1U << 0)
/* Indicates user defined data format, i.e. non standard frame format. */
#define V4L2_MBUS_FRAME_DESC_FL_BLOB		(1U << 1)

/**
 * struct v4l2_mbus_frame_desc_entry - media bus frame description structure
 * @flags: V4L2_MBUS_FRAME_DESC_FL_* flags
 * @pixelcode: media bus pixel code, valid if FRAME_DESC_FL_BLOB is not set
 * @length: number of octets per frame, valid for compressed or unspecified
 *          formats
 */
struct v4l2_mbus_frame_desc_entry {
	u16 flags;
	u32 pixelcode;
	u32 length;
};

#define V4L2_FRAME_DESC_ENTRY_MAX	4

/**
 * struct v4l2_mbus_frame_desc - media bus data frame description
 * @entry: frame descriptors array
 * @num_entries: number of entries in @entry array
 */
struct v4l2_mbus_frame_desc {
	struct v4l2_mbus_frame_desc_entry entry[V4L2_FRAME_DESC_ENTRY_MAX];
	unsigned short num_entries;
};

/*
   s_std_output: set v4l2_std_id for video OUTPUT devices. This is ignored by
	video input devices.

   g_std_output: get current standard for video OUTPUT devices. This is ignored
	by video input devices.

   g_tvnorms_output: get v4l2_std_id with all standards supported by video
	OUTPUT device. This is ignored by video input devices.

   s_crystal_freq: sets the frequency of the crystal used to generate the
	clocks in Hz. An extra flags field allows device specific configuration
	regarding clock frequency dividers, etc. If not used, then set flags
	to 0. If the frequency is not supported, then -EINVAL is returned.

   g_input_status: get input status. Same as the status field in the v4l2_input
	struct.

   s_routing: see s_routing in audio_ops, except this version is for video
	devices.

   s_dv_timings(): Set custom dv timings in the sub device. This is used
	when sub device is capable of setting detailed timing information
	in the hardware to generate/detect the video signal.

   g_dv_timings(): Get custom dv timings in the sub device.

   enum_mbus_fmt: enumerate pixel formats, provided by a video data source

   g_mbus_fmt: get the current pixel format, provided by a video data source

   try_mbus_fmt: try to set a pixel format on a video data source

   s_mbus_fmt: set a pixel format on a video data source

   g_mbus_config: get supported mediabus configurations

   s_mbus_config: set a certain mediabus configuration. This operation is added
	for compatibility with soc-camera drivers and should not be used by new
	software.

   s_rx_buffer: set a host allocated memory buffer for the subdev. The subdev
	can adjust @size to a lower value and must not write more data to the
	buffer starting at @data than the original value of @size.
 */
>>>>>>> common/android-3.10.y
struct v4l2_subdev_video_ops {
	int (*s_routing)(struct v4l2_subdev *sd, u32 input, u32 output, u32 config);
	int (*s_crystal_freq)(struct v4l2_subdev *sd, u32 freq, u32 flags);
	int (*s_std_output)(struct v4l2_subdev *sd, v4l2_std_id std);
	int (*querystd)(struct v4l2_subdev *sd, v4l2_std_id *std);
	int (*g_input_status)(struct v4l2_subdev *sd, u32 *status);
	int (*s_stream)(struct v4l2_subdev *sd, int enable);
	int (*cropcap)(struct v4l2_subdev *sd, struct v4l2_cropcap *cc);
	int (*g_crop)(struct v4l2_subdev *sd, struct v4l2_crop *crop);
	int (*s_crop)(struct v4l2_subdev *sd, const struct v4l2_crop *crop);
	int (*g_parm)(struct v4l2_subdev *sd, struct v4l2_streamparm *param);
	int (*s_parm)(struct v4l2_subdev *sd, struct v4l2_streamparm *param);
	int (*g_frame_interval)(struct v4l2_subdev *sd,
				struct v4l2_subdev_frame_interval *interval);
	int (*s_frame_interval)(struct v4l2_subdev *sd,
				struct v4l2_subdev_frame_interval *interval);
	int (*enum_framesizes)(struct v4l2_subdev *sd, struct v4l2_frmsizeenum *fsize);
	int (*enum_frameintervals)(struct v4l2_subdev *sd, struct v4l2_frmivalenum *fival);
<<<<<<< HEAD
	int (*enum_dv_presets) (struct v4l2_subdev *sd,
			struct v4l2_dv_enum_preset *preset);
	int (*s_dv_preset)(struct v4l2_subdev *sd,
			struct v4l2_dv_preset *preset);
	int (*query_dv_preset)(struct v4l2_subdev *sd,
			struct v4l2_dv_preset *preset);
=======
>>>>>>> common/android-3.10.y
	int (*s_dv_timings)(struct v4l2_subdev *sd,
			struct v4l2_dv_timings *timings);
	int (*g_dv_timings)(struct v4l2_subdev *sd,
			struct v4l2_dv_timings *timings);
	int (*enum_dv_timings)(struct v4l2_subdev *sd,
			struct v4l2_enum_dv_timings *timings);
	int (*query_dv_timings)(struct v4l2_subdev *sd,
			struct v4l2_dv_timings *timings);
	int (*dv_timings_cap)(struct v4l2_subdev *sd,
			struct v4l2_dv_timings_cap *cap);
	int (*enum_mbus_fmt)(struct v4l2_subdev *sd, unsigned int index,
			     enum v4l2_mbus_pixelcode *code);
	int (*enum_mbus_fsizes)(struct v4l2_subdev *sd,
			     struct v4l2_frmsizeenum *fsize);
	int (*g_mbus_fmt)(struct v4l2_subdev *sd,
			  struct v4l2_mbus_framefmt *fmt);
	int (*try_mbus_fmt)(struct v4l2_subdev *sd,
			    struct v4l2_mbus_framefmt *fmt);
	int (*s_mbus_fmt)(struct v4l2_subdev *sd,
			  struct v4l2_mbus_framefmt *fmt);
<<<<<<< HEAD
=======
	int (*g_mbus_config)(struct v4l2_subdev *sd,
			     struct v4l2_mbus_config *cfg);
	int (*s_mbus_config)(struct v4l2_subdev *sd,
			     const struct v4l2_mbus_config *cfg);
	int (*s_rx_buffer)(struct v4l2_subdev *sd, void *buf,
			   unsigned int *size);
>>>>>>> common/android-3.10.y
};

struct v4l2_subdev_vbi_ops {
	int (*decode_vbi_line)(struct v4l2_subdev *sd, struct v4l2_decode_vbi_line *vbi_line);
	int (*s_vbi_data)(struct v4l2_subdev *sd, const struct v4l2_sliced_vbi_data *vbi_data);
	int (*g_vbi_data)(struct v4l2_subdev *sd, struct v4l2_sliced_vbi_data *vbi_data);
	int (*g_sliced_vbi_cap)(struct v4l2_subdev *sd, struct v4l2_sliced_vbi_cap *cap);
	int (*s_raw_fmt)(struct v4l2_subdev *sd, struct v4l2_vbi_format *fmt);
	int (*g_sliced_fmt)(struct v4l2_subdev *sd, struct v4l2_sliced_vbi_format *fmt);
	int (*s_sliced_fmt)(struct v4l2_subdev *sd, struct v4l2_sliced_vbi_format *fmt);
};

struct v4l2_subdev_sensor_ops {
	int (*g_skip_top_lines)(struct v4l2_subdev *sd, u32 *lines);
	int (*g_skip_frames)(struct v4l2_subdev *sd, u32 *frames);
};


enum v4l2_subdev_ir_mode {
	V4L2_SUBDEV_IR_MODE_PULSE_WIDTH, 
};

struct v4l2_subdev_ir_parameters {
	
	unsigned int bytes_per_data_element; 
	enum v4l2_subdev_ir_mode mode;

	bool enable;
	bool interrupt_enable;
	bool shutdown; 

	bool modulation;           
	u32 max_pulse_width;       
	unsigned int carrier_freq; 
	unsigned int duty_cycle;   
	bool invert_level;	   

	
	bool invert_carrier_sense; 

	
	u32 noise_filter_min_width;       
	unsigned int carrier_range_lower; 
	unsigned int carrier_range_upper; 
	u32 resolution;                   
};

struct v4l2_subdev_ir_ops {
	
	int (*rx_read)(struct v4l2_subdev *sd, u8 *buf, size_t count,
				ssize_t *num);

	int (*rx_g_parameters)(struct v4l2_subdev *sd,
				struct v4l2_subdev_ir_parameters *params);
	int (*rx_s_parameters)(struct v4l2_subdev *sd,
				struct v4l2_subdev_ir_parameters *params);

	
	int (*tx_write)(struct v4l2_subdev *sd, u8 *buf, size_t count,
				ssize_t *num);

	int (*tx_g_parameters)(struct v4l2_subdev *sd,
				struct v4l2_subdev_ir_parameters *params);
	int (*tx_s_parameters)(struct v4l2_subdev *sd,
				struct v4l2_subdev_ir_parameters *params);
};

/**
 * struct v4l2_subdev_pad_ops - v4l2-subdev pad level operations
 * @get_frame_desc: get the current low level media bus frame parameters.
 * @get_frame_desc: set the low level media bus frame parameters, @fd array
 *                  may be adjusted by the subdev driver to device capabilities.
 */
struct v4l2_subdev_pad_ops {
	int (*enum_mbus_code)(struct v4l2_subdev *sd, struct v4l2_subdev_fh *fh,
			      struct v4l2_subdev_mbus_code_enum *code);
	int (*enum_frame_size)(struct v4l2_subdev *sd,
			       struct v4l2_subdev_fh *fh,
			       struct v4l2_subdev_frame_size_enum *fse);
	int (*enum_frame_interval)(struct v4l2_subdev *sd,
				   struct v4l2_subdev_fh *fh,
				   struct v4l2_subdev_frame_interval_enum *fie);
	int (*get_fmt)(struct v4l2_subdev *sd, struct v4l2_subdev_fh *fh,
		       struct v4l2_subdev_format *format);
	int (*set_fmt)(struct v4l2_subdev *sd, struct v4l2_subdev_fh *fh,
		       struct v4l2_subdev_format *format);
	int (*set_crop)(struct v4l2_subdev *sd, struct v4l2_subdev_fh *fh,
		       struct v4l2_subdev_crop *crop);
	int (*get_crop)(struct v4l2_subdev *sd, struct v4l2_subdev_fh *fh,
		       struct v4l2_subdev_crop *crop);
	int (*get_selection)(struct v4l2_subdev *sd, struct v4l2_subdev_fh *fh,
			     struct v4l2_subdev_selection *sel);
	int (*set_selection)(struct v4l2_subdev *sd, struct v4l2_subdev_fh *fh,
			     struct v4l2_subdev_selection *sel);
	int (*get_edid)(struct v4l2_subdev *sd, struct v4l2_subdev_edid *edid);
	int (*set_edid)(struct v4l2_subdev *sd, struct v4l2_subdev_edid *edid);
#ifdef CONFIG_MEDIA_CONTROLLER
	int (*link_validate)(struct v4l2_subdev *sd, struct media_link *link,
			     struct v4l2_subdev_format *source_fmt,
			     struct v4l2_subdev_format *sink_fmt);
#endif /* CONFIG_MEDIA_CONTROLLER */
	int (*get_frame_desc)(struct v4l2_subdev *sd, unsigned int pad,
			      struct v4l2_mbus_frame_desc *fd);
	int (*set_frame_desc)(struct v4l2_subdev *sd, unsigned int pad,
			      struct v4l2_mbus_frame_desc *fd);
};

struct v4l2_subdev_ops {
	const struct v4l2_subdev_core_ops	*core;
	const struct v4l2_subdev_tuner_ops	*tuner;
	const struct v4l2_subdev_audio_ops	*audio;
	const struct v4l2_subdev_video_ops	*video;
	const struct v4l2_subdev_vbi_ops	*vbi;
	const struct v4l2_subdev_ir_ops		*ir;
	const struct v4l2_subdev_sensor_ops	*sensor;
	const struct v4l2_subdev_pad_ops	*pad;
};

struct v4l2_subdev_internal_ops {
	int (*registered)(struct v4l2_subdev *sd);
	void (*unregistered)(struct v4l2_subdev *sd);
	int (*open)(struct v4l2_subdev *sd, struct v4l2_subdev_fh *fh);
	int (*close)(struct v4l2_subdev *sd, struct v4l2_subdev_fh *fh);
};

#define V4L2_SUBDEV_NAME_SIZE 32

#define V4L2_SUBDEV_FL_IS_I2C			(1U << 0)
#define V4L2_SUBDEV_FL_IS_SPI			(1U << 1)
#define V4L2_SUBDEV_FL_HAS_DEVNODE		(1U << 2)
#define V4L2_SUBDEV_FL_HAS_EVENTS		(1U << 3)

struct v4l2_subdev {
#if defined(CONFIG_MEDIA_CONTROLLER)
	struct media_entity entity;
#endif
	struct list_head list;
	struct module *owner;
	u32 flags;
	struct v4l2_device *v4l2_dev;
	const struct v4l2_subdev_ops *ops;
	
	const struct v4l2_subdev_internal_ops *internal_ops;
	
	struct v4l2_ctrl_handler *ctrl_handler;
	
	char name[V4L2_SUBDEV_NAME_SIZE];
	
	u32 grp_id;
	
	void *dev_priv;
	void *host_priv;
	
	struct video_device devnode;
	
	unsigned int nevents;
};

#define media_entity_to_v4l2_subdev(ent) \
	container_of(ent, struct v4l2_subdev, entity)
#define vdev_to_v4l2_subdev(vdev) \
<<<<<<< HEAD
	container_of(vdev, struct v4l2_subdev, devnode)
=======
	((struct v4l2_subdev *)video_get_drvdata(vdev))
>>>>>>> common/android-3.10.y

struct v4l2_subdev_fh {
	struct v4l2_fh vfh;
#if defined(CONFIG_VIDEO_V4L2_SUBDEV_API)
	struct {
		struct v4l2_mbus_framefmt try_fmt;
		struct v4l2_rect try_crop;
		struct v4l2_rect try_compose;
	} *pad;
#endif
};

#define to_v4l2_subdev_fh(fh)	\
	container_of(fh, struct v4l2_subdev_fh, vfh)

#if defined(CONFIG_VIDEO_V4L2_SUBDEV_API)
#define __V4L2_SUBDEV_MK_GET_TRY(rtype, fun_name, field_name)		\
	static inline struct rtype *					\
	v4l2_subdev_get_try_##fun_name(struct v4l2_subdev_fh *fh,	\
				       unsigned int pad)		\
	{								\
		BUG_ON(unlikely(pad >= vdev_to_v4l2_subdev(		\
					fh->vfh.vdev)->entity.num_pads)); \
		return &fh->pad[pad].field_name;			\
	}

__V4L2_SUBDEV_MK_GET_TRY(v4l2_mbus_framefmt, format, try_fmt)
__V4L2_SUBDEV_MK_GET_TRY(v4l2_rect, crop, try_compose)
__V4L2_SUBDEV_MK_GET_TRY(v4l2_rect, compose, try_compose)
#endif

extern const struct v4l2_file_operations v4l2_subdev_fops;

static inline void v4l2_set_subdevdata(struct v4l2_subdev *sd, void *p)
{
	sd->dev_priv = p;
}

static inline void *v4l2_get_subdevdata(const struct v4l2_subdev *sd)
{
	return sd->dev_priv;
}

static inline void v4l2_set_subdev_hostdata(struct v4l2_subdev *sd, void *p)
{
	sd->host_priv = p;
}

static inline void *v4l2_get_subdev_hostdata(const struct v4l2_subdev *sd)
{
	return sd->host_priv;
}

#ifdef CONFIG_MEDIA_CONTROLLER
int v4l2_subdev_link_validate_default(struct v4l2_subdev *sd,
				      struct media_link *link,
				      struct v4l2_subdev_format *source_fmt,
				      struct v4l2_subdev_format *sink_fmt);
int v4l2_subdev_link_validate(struct media_link *link);
#endif /* CONFIG_MEDIA_CONTROLLER */
void v4l2_subdev_init(struct v4l2_subdev *sd,
		      const struct v4l2_subdev_ops *ops);

#define v4l2_subdev_call(sd, o, f, args...)				\
	(!(sd) ? -ENODEV : (((sd)->ops && (sd)->ops->o && (sd)->ops->o->f) ?	\
		(sd)->ops->o->f((sd) , ##args) : -ENOIOCTLCMD))

#define v4l2_subdev_notify(sd, notification, arg)			   \
	((!(sd) || !(sd)->v4l2_dev || !(sd)->v4l2_dev->notify) ? -ENODEV : \
	 (sd)->v4l2_dev->notify((sd), (notification), (arg)))

#define v4l2_subdev_has_op(sd, o, f) \
	((sd)->ops->o && (sd)->ops->o->f)

#endif
