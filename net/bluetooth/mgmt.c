/*
   BlueZ - Bluetooth protocol stack for Linux
   Copyright (C) 2010  Nokia Corporation
   Copyright (c) 2011-2012 The Linux Foundation.  All rights reserved.

   This program is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License version 2 as
   published by the Free Software Foundation;

   THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS
   OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
   FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT OF THIRD PARTY RIGHTS.
   IN NO EVENT SHALL THE COPYRIGHT HOLDER(S) AND AUTHOR(S) BE LIABLE FOR ANY
   CLAIM, OR ANY SPECIAL INDIRECT OR CONSEQUENTIAL DAMAGES, OR ANY DAMAGES
   WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS, WHETHER IN AN
   ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT OF
   OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.

   ALL LIABILITY, INCLUDING LIABILITY FOR INFRINGEMENT OF ANY PATENTS,
   COPYRIGHTS, TRADEMARKS OR OTHER RIGHTS, RELATING TO USE OF THIS
   SOFTWARE IS DISCLAIMED.
*/

/* Bluetooth HCI Management interface */

<<<<<<< HEAD
#include <linux/uaccess.h>
#include <linux/interrupt.h>
=======
>>>>>>> common/android-3.10.y
#include <linux/module.h>
#include <asm/unaligned.h>

#include <net/bluetooth/bluetooth.h>
#include <net/bluetooth/hci_core.h>
#include <net/bluetooth/l2cap.h>
#include <net/bluetooth/mgmt.h>
#include <net/bluetooth/smp.h>

<<<<<<< HEAD
#define MGMT_VERSION	0
#define MGMT_REVISION	1
=======
bool enable_hs;

#define MGMT_VERSION	1
#define MGMT_REVISION	3

static const u16 mgmt_commands[] = {
	MGMT_OP_READ_INDEX_LIST,
	MGMT_OP_READ_INFO,
	MGMT_OP_SET_POWERED,
	MGMT_OP_SET_DISCOVERABLE,
	MGMT_OP_SET_CONNECTABLE,
	MGMT_OP_SET_FAST_CONNECTABLE,
	MGMT_OP_SET_PAIRABLE,
	MGMT_OP_SET_LINK_SECURITY,
	MGMT_OP_SET_SSP,
	MGMT_OP_SET_HS,
	MGMT_OP_SET_LE,
	MGMT_OP_SET_DEV_CLASS,
	MGMT_OP_SET_LOCAL_NAME,
	MGMT_OP_ADD_UUID,
	MGMT_OP_REMOVE_UUID,
	MGMT_OP_LOAD_LINK_KEYS,
	MGMT_OP_LOAD_LONG_TERM_KEYS,
	MGMT_OP_DISCONNECT,
	MGMT_OP_GET_CONNECTIONS,
	MGMT_OP_PIN_CODE_REPLY,
	MGMT_OP_PIN_CODE_NEG_REPLY,
	MGMT_OP_SET_IO_CAPABILITY,
	MGMT_OP_PAIR_DEVICE,
	MGMT_OP_CANCEL_PAIR_DEVICE,
	MGMT_OP_UNPAIR_DEVICE,
	MGMT_OP_USER_CONFIRM_REPLY,
	MGMT_OP_USER_CONFIRM_NEG_REPLY,
	MGMT_OP_USER_PASSKEY_REPLY,
	MGMT_OP_USER_PASSKEY_NEG_REPLY,
	MGMT_OP_READ_LOCAL_OOB_DATA,
	MGMT_OP_ADD_REMOTE_OOB_DATA,
	MGMT_OP_REMOVE_REMOTE_OOB_DATA,
	MGMT_OP_START_DISCOVERY,
	MGMT_OP_STOP_DISCOVERY,
	MGMT_OP_CONFIRM_NAME,
	MGMT_OP_BLOCK_DEVICE,
	MGMT_OP_UNBLOCK_DEVICE,
	MGMT_OP_SET_DEVICE_ID,
};

static const u16 mgmt_events[] = {
	MGMT_EV_CONTROLLER_ERROR,
	MGMT_EV_INDEX_ADDED,
	MGMT_EV_INDEX_REMOVED,
	MGMT_EV_NEW_SETTINGS,
	MGMT_EV_CLASS_OF_DEV_CHANGED,
	MGMT_EV_LOCAL_NAME_CHANGED,
	MGMT_EV_NEW_LINK_KEY,
	MGMT_EV_NEW_LONG_TERM_KEY,
	MGMT_EV_DEVICE_CONNECTED,
	MGMT_EV_DEVICE_DISCONNECTED,
	MGMT_EV_CONNECT_FAILED,
	MGMT_EV_PIN_CODE_REQUEST,
	MGMT_EV_USER_CONFIRM_REQUEST,
	MGMT_EV_USER_PASSKEY_REQUEST,
	MGMT_EV_AUTH_FAILED,
	MGMT_EV_DEVICE_FOUND,
	MGMT_EV_DISCOVERING,
	MGMT_EV_DEVICE_BLOCKED,
	MGMT_EV_DEVICE_UNBLOCKED,
	MGMT_EV_DEVICE_UNPAIRED,
	MGMT_EV_PASSKEY_NOTIFY,
};

/*
 * These LE scan and inquiry parameters were chosen according to LE General
 * Discovery Procedure specification.
 */
#define LE_SCAN_WIN			0x12
#define LE_SCAN_INT			0x12
#define LE_SCAN_TIMEOUT_LE_ONLY		msecs_to_jiffies(10240)
#define LE_SCAN_TIMEOUT_BREDR_LE	msecs_to_jiffies(5120)

#define INQUIRY_LEN_BREDR		0x08	/* TGAP(100) */
#define INQUIRY_LEN_BREDR_LE		0x04	/* TGAP(100)/2 */

#define CACHE_TIMEOUT	msecs_to_jiffies(2 * 1000)
>>>>>>> common/android-3.10.y

#define SCAN_IDLE	0x00
#define SCAN_LE		0x01
#define SCAN_BR		0x02

struct pending_cmd {
	struct list_head list;
	__u16 opcode;
	int index;
	void *param;
	struct sock *sk;
	void *user_data;
};

struct mgmt_pending_free_work {
	struct work_struct work;
	struct sock *sk;
};

<<<<<<< HEAD
LIST_HEAD(cmd_list);
=======
bool mgmt_valid_hdev(struct hci_dev *hdev)
{
	return hdev->dev_type == HCI_BREDR;
}

static u8 mgmt_status(u8 hci_status)
{
	if (hci_status < ARRAY_SIZE(mgmt_status_table))
		return mgmt_status_table[hci_status];

	return MGMT_STATUS_FAILED;
}
>>>>>>> common/android-3.10.y

static int cmd_status(struct sock *sk, u16 index, u16 cmd, u8 status)
{
	struct sk_buff *skb;
	struct mgmt_hdr *hdr;
	struct mgmt_ev_cmd_status *ev;

	BT_DBG("sock %p, index %u, cmd %u, status %u", sk, index, cmd, status);

	skb = alloc_skb(sizeof(*hdr) + sizeof(*ev), GFP_KERNEL);
	if (!skb)
		return -ENOMEM;

	hdr = (void *) skb_put(skb, sizeof(*hdr));

	hdr->opcode = __constant_cpu_to_le16(MGMT_EV_CMD_STATUS);
	hdr->index = cpu_to_le16(index);
	hdr->len = cpu_to_le16(sizeof(*ev));

	ev = (void *) skb_put(skb, sizeof(*ev));
	ev->status = status;
	ev->opcode = cpu_to_le16(cmd);

	if (sock_queue_rcv_skb(sk, skb) < 0)
		kfree_skb(skb);

	return 0;
}

static int cmd_complete(struct sock *sk, u16 index, u16 cmd, void *rp,
								size_t rp_len)
{
	struct sk_buff *skb;
	struct mgmt_hdr *hdr;
	struct mgmt_ev_cmd_complete *ev;

	BT_DBG("sock %p", sk);

	skb = alloc_skb(sizeof(*hdr) + sizeof(*ev) + rp_len, GFP_KERNEL);
	if (!skb)
		return -ENOMEM;

	hdr = (void *) skb_put(skb, sizeof(*hdr));

	hdr->opcode = __constant_cpu_to_le16(MGMT_EV_CMD_COMPLETE);
	hdr->index = cpu_to_le16(index);
	hdr->len = cpu_to_le16(sizeof(*ev) + rp_len);

	ev = (void *) skb_put(skb, sizeof(*ev) + rp_len);
<<<<<<< HEAD
	put_unaligned_le16(cmd, &ev->opcode);
=======
	ev->opcode = cpu_to_le16(cmd);
	ev->status = status;
>>>>>>> common/android-3.10.y

	if (rp)
		memcpy(ev->data, rp, rp_len);

	if (sock_queue_rcv_skb(sk, skb) < 0)
		kfree_skb(skb);

	return 0;
}

static int read_version(struct sock *sk)
{
	struct mgmt_rp_read_version rp;

	BT_DBG("sock %p", sk);

	rp.version = MGMT_VERSION;
	rp.revision = __constant_cpu_to_le16(MGMT_REVISION);

<<<<<<< HEAD
	return cmd_complete(sk, MGMT_INDEX_NONE, MGMT_OP_READ_VERSION, &rp,
								sizeof(rp));
=======
	return cmd_complete(sk, MGMT_INDEX_NONE, MGMT_OP_READ_VERSION, 0, &rp,
			    sizeof(rp));
}

static int read_commands(struct sock *sk, struct hci_dev *hdev, void *data,
			 u16 data_len)
{
	struct mgmt_rp_read_commands *rp;
	const u16 num_commands = ARRAY_SIZE(mgmt_commands);
	const u16 num_events = ARRAY_SIZE(mgmt_events);
	__le16 *opcode;
	size_t rp_size;
	int i, err;

	BT_DBG("sock %p", sk);

	rp_size = sizeof(*rp) + ((num_commands + num_events) * sizeof(u16));

	rp = kmalloc(rp_size, GFP_KERNEL);
	if (!rp)
		return -ENOMEM;

	rp->num_commands = __constant_cpu_to_le16(num_commands);
	rp->num_events = __constant_cpu_to_le16(num_events);

	for (i = 0, opcode = rp->opcodes; i < num_commands; i++, opcode++)
		put_unaligned_le16(mgmt_commands[i], opcode);

	for (i = 0; i < num_events; i++, opcode++)
		put_unaligned_le16(mgmt_events[i], opcode);

	err = cmd_complete(sk, MGMT_INDEX_NONE, MGMT_OP_READ_COMMANDS, 0, rp,
			   rp_size);
	kfree(rp);

	return err;
>>>>>>> common/android-3.10.y
}

static int read_index_list(struct sock *sk)
{
	struct mgmt_rp_read_index_list *rp;
<<<<<<< HEAD
	struct list_head *p;
=======
	struct hci_dev *d;
>>>>>>> common/android-3.10.y
	size_t rp_len;
	u16 count;
	int err;

	BT_DBG("sock %p", sk);

	read_lock(&hci_dev_list_lock);

	count = 0;
<<<<<<< HEAD
	list_for_each(p, &hci_dev_list) {
		struct hci_dev *d = list_entry(p, struct hci_dev, list);
		if (d->dev_type != HCI_BREDR)
			continue;
=======
	list_for_each_entry(d, &hci_dev_list, list) {
		if (!mgmt_valid_hdev(d))
			continue;

>>>>>>> common/android-3.10.y
		count++;
	}

	rp_len = sizeof(*rp) + (2 * count);
	rp = kmalloc(rp_len, GFP_ATOMIC);
	if (!rp) {
		read_unlock(&hci_dev_list_lock);
		return -ENOMEM;
	}

<<<<<<< HEAD
	put_unaligned_le16(0, &rp->num_controllers);

	i = 0;
	list_for_each(p, &hci_dev_list) {
		struct hci_dev *d = list_entry(p, struct hci_dev, list);

		hci_del_off_timer(d);

		if (d->dev_type != HCI_BREDR)
			continue;

		set_bit(HCI_MGMT, &d->flags);

		if (test_bit(HCI_SETUP, &d->flags))
			continue;

		put_unaligned_le16(d->id, &rp->index[i++]);
		put_unaligned_le16((u16)i, &rp->num_controllers);
=======
	count = 0;
	list_for_each_entry(d, &hci_dev_list, list) {
		if (test_bit(HCI_SETUP, &d->dev_flags))
			continue;

		if (!mgmt_valid_hdev(d))
			continue;

		rp->index[count++] = cpu_to_le16(d->id);
>>>>>>> common/android-3.10.y
		BT_DBG("Added hci%u", d->id);
	}

	rp->num_controllers = cpu_to_le16(count);
	rp_len = sizeof(*rp) + (2 * count);

	read_unlock(&hci_dev_list_lock);

	err = cmd_complete(sk, MGMT_INDEX_NONE, MGMT_OP_READ_INDEX_LIST, rp,
									rp_len);

	kfree(rp);

	return err;
}

static int read_controller_info(struct sock *sk, u16 index)
{
	struct mgmt_rp_read_info rp;
	struct hci_dev *hdev;

<<<<<<< HEAD
	BT_DBG("sock %p hci%u", sk, index);

	hdev = hci_dev_get(index);
	if (!hdev)
		return cmd_status(sk, index, MGMT_OP_READ_INFO, ENODEV);

	hci_del_off_timer(hdev);
=======
	settings |= MGMT_SETTING_POWERED;
	settings |= MGMT_SETTING_PAIRABLE;

	if (lmp_ssp_capable(hdev))
		settings |= MGMT_SETTING_SSP;

	if (lmp_bredr_capable(hdev)) {
		settings |= MGMT_SETTING_CONNECTABLE;
		if (hdev->hci_ver >= BLUETOOTH_VER_1_2)
			settings |= MGMT_SETTING_FAST_CONNECTABLE;
		settings |= MGMT_SETTING_DISCOVERABLE;
		settings |= MGMT_SETTING_BREDR;
		settings |= MGMT_SETTING_LINK_SECURITY;
	}
>>>>>>> common/android-3.10.y

	hci_dev_lock_bh(hdev);

<<<<<<< HEAD
	set_bit(HCI_MGMT, &hdev->flags);
=======
	if (lmp_le_capable(hdev))
		settings |= MGMT_SETTING_LE;
>>>>>>> common/android-3.10.y

	memset(&rp, 0, sizeof(rp));

	rp.type = hdev->dev_type;

	rp.powered = test_bit(HCI_UP, &hdev->flags);
	rp.connectable = test_bit(HCI_PSCAN, &hdev->flags);
	rp.discoverable = test_bit(HCI_ISCAN, &hdev->flags);
	rp.pairable = test_bit(HCI_PSCAN, &hdev->flags);

	if (test_bit(HCI_AUTH, &hdev->flags))
		rp.sec_mode = 3;
	else if (hdev->ssp_mode > 0)
		rp.sec_mode = 4;
	else
		rp.sec_mode = 2;

<<<<<<< HEAD
	bacpy(&rp.bdaddr, &hdev->bdaddr);
	memcpy(rp.features, hdev->features, 8);
	memcpy(rp.dev_class, hdev->dev_class, 3);
	put_unaligned_le16(hdev->manufacturer, &rp.manufacturer);
	rp.hci_ver = hdev->hci_ver;
	put_unaligned_le16(hdev->hci_rev, &rp.hci_rev);
=======
	if (test_bit(HCI_FAST_CONNECTABLE, &hdev->dev_flags))
		settings |= MGMT_SETTING_FAST_CONNECTABLE;

	if (test_bit(HCI_DISCOVERABLE, &hdev->dev_flags))
		settings |= MGMT_SETTING_DISCOVERABLE;
>>>>>>> common/android-3.10.y

	memcpy(rp.name, hdev->dev_name, sizeof(hdev->dev_name));

<<<<<<< HEAD
	rp.le_white_list_size = hdev->le_white_list_size;
=======
	if (lmp_bredr_capable(hdev))
		settings |= MGMT_SETTING_BREDR;
>>>>>>> common/android-3.10.y

	hci_dev_unlock_bh(hdev);
	hci_dev_put(hdev);

	return cmd_complete(sk, index, MGMT_OP_READ_INFO, &rp, sizeof(rp));
}

static void mgmt_pending_free_worker(struct work_struct *work)
{
	struct mgmt_pending_free_work *free_work =
		container_of(work, struct mgmt_pending_free_work, work);

	BT_DBG("sk %p", free_work->sk);

	sock_put(free_work->sk);
	kfree(free_work);
}

static void mgmt_pending_free(struct pending_cmd *cmd)
{
	struct mgmt_pending_free_work *free_work;
	struct sock *sk = cmd->sk;

<<<<<<< HEAD
	BT_DBG("opcode %d, sk %p", cmd->opcode, sk);

	kfree(cmd->param);
	kfree(cmd);

	free_work = kzalloc(sizeof(*free_work), GFP_ATOMIC);
	if (free_work) {
		INIT_WORK(&free_work->work, mgmt_pending_free_worker);
		free_work->sk = sk;

		if (!schedule_work(&free_work->work))
			kfree(free_work);
	}
}

static struct pending_cmd *mgmt_pending_add(struct sock *sk, u16 opcode,
						u16 index, void *data, u16 len)
{
	struct pending_cmd *cmd;

	BT_DBG("%d", opcode);

	cmd = kmalloc(sizeof(*cmd), GFP_ATOMIC);
	if (!cmd)
		return NULL;

	cmd->opcode = opcode;
	cmd->index = index;

	cmd->param = kmalloc(len, GFP_ATOMIC);
	if (!cmd->param) {
		kfree(cmd);
		return NULL;
	}

	if (data)
		memcpy(cmd->param, data, len);

	cmd->sk = sk;
	sock_hold(sk);

	list_add(&cmd->list, &cmd_list);

	return cmd;
=======
static u8 *create_uuid16_list(struct hci_dev *hdev, u8 *data, ptrdiff_t len)
{
	u8 *ptr = data, *uuids_start = NULL;
	struct bt_uuid *uuid;

	if (len < 4)
		return ptr;

	list_for_each_entry(uuid, &hdev->uuids, list) {
		u16 uuid16;

		if (uuid->size != 16)
			continue;

		uuid16 = get_unaligned_le16(&uuid->uuid[12]);
		if (uuid16 < 0x1100)
			continue;

		if (uuid16 == PNP_INFO_SVCLASS_ID)
			continue;

		if (!uuids_start) {
			uuids_start = ptr;
			uuids_start[0] = 1;
			uuids_start[1] = EIR_UUID16_ALL;
			ptr += 2;
		}

		/* Stop if not enough space to put next UUID */
		if ((ptr - data) + sizeof(u16) > len) {
			uuids_start[1] = EIR_UUID16_SOME;
			break;
		}

		*ptr++ = (uuid16 & 0x00ff);
		*ptr++ = (uuid16 & 0xff00) >> 8;
		uuids_start[0] += sizeof(uuid16);
	}

	return ptr;
}

static u8 *create_uuid32_list(struct hci_dev *hdev, u8 *data, ptrdiff_t len)
{
	u8 *ptr = data, *uuids_start = NULL;
	struct bt_uuid *uuid;

	if (len < 6)
		return ptr;

	list_for_each_entry(uuid, &hdev->uuids, list) {
		if (uuid->size != 32)
			continue;

		if (!uuids_start) {
			uuids_start = ptr;
			uuids_start[0] = 1;
			uuids_start[1] = EIR_UUID32_ALL;
			ptr += 2;
		}

		/* Stop if not enough space to put next UUID */
		if ((ptr - data) + sizeof(u32) > len) {
			uuids_start[1] = EIR_UUID32_SOME;
			break;
		}

		memcpy(ptr, &uuid->uuid[12], sizeof(u32));
		ptr += sizeof(u32);
		uuids_start[0] += sizeof(u32);
	}

	return ptr;
}

static u8 *create_uuid128_list(struct hci_dev *hdev, u8 *data, ptrdiff_t len)
{
	u8 *ptr = data, *uuids_start = NULL;
	struct bt_uuid *uuid;

	if (len < 18)
		return ptr;

	list_for_each_entry(uuid, &hdev->uuids, list) {
		if (uuid->size != 128)
			continue;

		if (!uuids_start) {
			uuids_start = ptr;
			uuids_start[0] = 1;
			uuids_start[1] = EIR_UUID128_ALL;
			ptr += 2;
		}

		/* Stop if not enough space to put next UUID */
		if ((ptr - data) + 16 > len) {
			uuids_start[1] = EIR_UUID128_SOME;
			break;
		}

		memcpy(ptr, uuid->uuid, 16);
		ptr += 16;
		uuids_start[0] += 16;
	}

	return ptr;
>>>>>>> common/android-3.10.y
}

static void mgmt_pending_foreach(u16 opcode, int index,
				void (*cb)(struct pending_cmd *cmd, void *data),
				void *data)
{
<<<<<<< HEAD
	struct list_head *p, *n;
=======
	u8 *ptr = data;
	size_t name_len;
>>>>>>> common/android-3.10.y

	BT_DBG(" %d", opcode);

	list_for_each_safe(p, n, &cmd_list) {
		struct pending_cmd *cmd;

		cmd = list_entry(p, struct pending_cmd, list);

		if (opcode > 0 && cmd->opcode != opcode)
			continue;

<<<<<<< HEAD
		if (index >= 0 && cmd->index != index)
			continue;

		cb(cmd, data);
=======
		ptr += (name_len + 2);
>>>>>>> common/android-3.10.y
	}
}

<<<<<<< HEAD
static struct pending_cmd *mgmt_pending_find(u16 opcode, int index)
{
	struct list_head *p;

	BT_DBG(" %d", opcode);

	list_for_each(p, &cmd_list) {
		struct pending_cmd *cmd;

		cmd = list_entry(p, struct pending_cmd, list);

		if (cmd->opcode != opcode)
			continue;

		if (index >= 0 && cmd->index != index)
			continue;

		return cmd;
	}

	return NULL;
}

static void mgmt_pending_remove(struct pending_cmd *cmd)
{
	BT_DBG(" %d", cmd->opcode);

	list_del(&cmd->list);
	mgmt_pending_free(cmd);
}

static int set_powered(struct sock *sk, u16 index, unsigned char *data, u16 len)
{
	struct mgmt_mode *cp;
	struct hci_dev *hdev;
	struct pending_cmd *cmd;
	int err, up;

	cp = (void *) data;

	BT_DBG("request for hci%u", index);

	if (len != sizeof(*cp))
		return cmd_status(sk, index, MGMT_OP_SET_POWERED, EINVAL);

	hdev = hci_dev_get(index);
	if (!hdev)
		return cmd_status(sk, index, MGMT_OP_SET_POWERED, ENODEV);

	hci_dev_lock_bh(hdev);

	up = test_bit(HCI_UP, &hdev->flags);
	if ((cp->val && up) || (!cp->val && !up)) {
		err = cmd_status(sk, index, MGMT_OP_SET_POWERED, EALREADY);
		goto failed;
	}

	if (mgmt_pending_find(MGMT_OP_SET_POWERED, index)) {
		err = cmd_status(sk, index, MGMT_OP_SET_POWERED, EBUSY);
		goto failed;
	}

	cmd = mgmt_pending_add(sk, MGMT_OP_SET_POWERED, index, data, len);
	if (!cmd) {
		err = -ENOMEM;
		goto failed;
	}
=======
	if (hdev->inq_tx_power != HCI_TX_POWER_INVALID) {
		ptr[0] = 2;
		ptr[1] = EIR_TX_POWER;
		ptr[2] = (u8) hdev->inq_tx_power;

		ptr += 3;
	}

	if (hdev->devid_source > 0) {
		ptr[0] = 9;
		ptr[1] = EIR_DEVICE_ID;

		put_unaligned_le16(hdev->devid_source, ptr + 2);
		put_unaligned_le16(hdev->devid_vendor, ptr + 4);
		put_unaligned_le16(hdev->devid_product, ptr + 6);
		put_unaligned_le16(hdev->devid_version, ptr + 8);

		ptr += 10;
	}

	ptr = create_uuid16_list(hdev, ptr, HCI_MAX_EIR_LENGTH - (ptr - data));
	ptr = create_uuid32_list(hdev, ptr, HCI_MAX_EIR_LENGTH - (ptr - data));
	ptr = create_uuid128_list(hdev, ptr, HCI_MAX_EIR_LENGTH - (ptr - data));
}

static void update_eir(struct hci_request *req)
{
	struct hci_dev *hdev = req->hdev;
	struct hci_cp_write_eir cp;

	if (!hdev_is_powered(hdev))
		return;

	if (!lmp_ext_inq_capable(hdev))
		return;

	if (!test_bit(HCI_SSP_ENABLED, &hdev->dev_flags))
		return;

	if (test_bit(HCI_SERVICE_CACHE, &hdev->dev_flags))
		return;
>>>>>>> common/android-3.10.y

	hci_dev_unlock_bh(hdev);

	if (cp->val)
		queue_work(hdev->workqueue, &hdev->power_on);
	else
		queue_work(hdev->workqueue, &hdev->power_off);

<<<<<<< HEAD
	err = 0;
	hci_dev_put(hdev);
=======
	if (memcmp(cp.data, hdev->eir, sizeof(cp.data)) == 0)
		return;
>>>>>>> common/android-3.10.y

	return err;

<<<<<<< HEAD
failed:
	hci_dev_unlock_bh(hdev);
	hci_dev_put(hdev);
	return err;
=======
	hci_req_add(req, HCI_OP_WRITE_EIR, sizeof(cp), &cp);
>>>>>>> common/android-3.10.y
}

static u8 get_service_classes(struct hci_dev *hdev)
{
	struct list_head *p;
	u8 val = 0;

	list_for_each(p, &hdev->uuids) {
		struct bt_uuid *uuid = list_entry(p, struct bt_uuid, list);

		val |= uuid->svc_hint;
	}

	return val;
}

static void update_class(struct hci_request *req)
{
	struct hci_dev *hdev = req->hdev;
	u8 cod[3];
<<<<<<< HEAD
	int err = 0;

	BT_DBG("%s", hdev->name);

	if (test_bit(HCI_SERVICE_CACHE, &hdev->flags))
		return 0;
=======

	BT_DBG("%s", hdev->name);

	if (!hdev_is_powered(hdev))
		return;

	if (test_bit(HCI_SERVICE_CACHE, &hdev->dev_flags))
		return;
>>>>>>> common/android-3.10.y

	cod[0] = hdev->minor_class;
	cod[1] = hdev->major_class;
	cod[2] = get_service_classes(hdev);

	if (memcmp(cod, hdev->dev_class, 3) == 0)
<<<<<<< HEAD
		return 0;

	err =  hci_send_cmd(hdev, HCI_OP_WRITE_CLASS_OF_DEV, sizeof(cod), cod);

	if (err == 0)
		memcpy(hdev->dev_class, cod, 3);
=======
		return;
>>>>>>> common/android-3.10.y

	hci_req_add(req, HCI_OP_WRITE_CLASS_OF_DEV, sizeof(cod), cod);
}

static int set_limited_discoverable(struct sock *sk, u16 index,
						unsigned char *data, u16 len)
{
<<<<<<< HEAD
	struct mgmt_mode *cp;
	struct hci_dev *hdev;
	struct pending_cmd *cmd;
	struct hci_cp_write_current_iac_lap dcp;
	int update_cod;
	int err = 0;
	/* General Inquiry LAP: 0x9E8B33, Limited Inquiry LAP: 0x9E8B00 */
	u8 lap[] = { 0x33, 0x8b, 0x9e, 0x00, 0x8b, 0x9e };

	cp = (void *) data;

	BT_DBG("hci%u discoverable: %d", index, cp->val);

	if (!cp || len != sizeof(*cp))
		return cmd_status(sk, index, MGMT_OP_SET_LIMIT_DISCOVERABLE,
									EINVAL);
=======
	struct hci_dev *hdev = container_of(work, struct hci_dev,
					    service_cache.work);
	struct hci_request req;

	if (!test_and_clear_bit(HCI_SERVICE_CACHE, &hdev->dev_flags))
		return;

	hci_req_init(&req, hdev);

	hci_dev_lock(hdev);

	update_eir(&req);
	update_class(&req);

	hci_dev_unlock(hdev);

	hci_req_run(&req, NULL);
}
>>>>>>> common/android-3.10.y

	hdev = hci_dev_get(index);
	if (!hdev)
		return cmd_status(sk, index, MGMT_OP_SET_LIMIT_DISCOVERABLE,
									ENODEV);

	hci_dev_lock_bh(hdev);

	if (!test_bit(HCI_UP, &hdev->flags)) {
		err = cmd_status(sk, index, MGMT_OP_SET_LIMIT_DISCOVERABLE,
								ENETDOWN);
		goto failed;
	}

	if (mgmt_pending_find(MGMT_OP_SET_LIMIT_DISCOVERABLE, index)) {
		err = cmd_status(sk, index, MGMT_OP_SET_LIMIT_DISCOVERABLE,
									EBUSY);
		goto failed;
	}

	if (cp->val == test_bit(HCI_ISCAN, &hdev->flags) &&
					test_bit(HCI_PSCAN, &hdev->flags)) {
		err = cmd_status(sk, index, MGMT_OP_SET_LIMIT_DISCOVERABLE,
								EALREADY);
		goto failed;
	}

	cmd = mgmt_pending_add(sk, MGMT_OP_SET_LIMIT_DISCOVERABLE, index, data,
									len);
	if (!cmd) {
		err = -ENOMEM;
		goto failed;
	}

	memset(&dcp, 0, sizeof(dcp));
	dcp.num_current_iac = cp->val ? 2 : 1;
	memcpy(&dcp.lap, lap, dcp.num_current_iac * 3);
	update_cod = 1;

	if (cp->val) {
		if (hdev->major_class & MGMT_MAJOR_CLASS_LIMITED)
			update_cod = 0;
		hdev->major_class |= MGMT_MAJOR_CLASS_LIMITED;
	} else {
		if (!(hdev->major_class & MGMT_MAJOR_CLASS_LIMITED))
			update_cod = 0;
		hdev->major_class &= ~MGMT_MAJOR_CLASS_LIMITED;
	}

<<<<<<< HEAD
	if (update_cod)
		err = update_class(hdev);

	if (err >= 0)
		err = hci_send_cmd(hdev, HCI_OP_WRITE_CURRENT_IAC_LAP,
							sizeof(dcp), &dcp);
=======
	rp.version = hdev->hci_ver;
	rp.manufacturer = cpu_to_le16(hdev->manufacturer);
>>>>>>> common/android-3.10.y

	if (err < 0)
		mgmt_pending_remove(cmd);

failed:
	hci_dev_unlock_bh(hdev);
	hci_dev_put(hdev);

	return err;
}

<<<<<<< HEAD
static int set_discoverable(struct sock *sk, u16 index, unsigned char *data,
									u16 len)
{
	struct mgmt_mode *cp;
	struct hci_dev *hdev;
=======
	hci_dev_unlock(hdev);

	return cmd_complete(sk, hdev->id, MGMT_OP_READ_INFO, 0, &rp,
			    sizeof(rp));
}

static void mgmt_pending_free(struct pending_cmd *cmd)
{
	sock_put(cmd->sk);
	kfree(cmd->param);
	kfree(cmd);
}

static struct pending_cmd *mgmt_pending_add(struct sock *sk, u16 opcode,
					    struct hci_dev *hdev, void *data,
					    u16 len)
{
	struct pending_cmd *cmd;

	cmd = kmalloc(sizeof(*cmd), GFP_KERNEL);
	if (!cmd)
		return NULL;

	cmd->opcode = opcode;
	cmd->index = hdev->id;

	cmd->param = kmalloc(len, GFP_KERNEL);
	if (!cmd->param) {
		kfree(cmd);
		return NULL;
	}

	if (data)
		memcpy(cmd->param, data, len);

	cmd->sk = sk;
	sock_hold(sk);

	list_add(&cmd->list, &hdev->mgmt_pending);

	return cmd;
}

static void mgmt_pending_foreach(u16 opcode, struct hci_dev *hdev,
				 void (*cb)(struct pending_cmd *cmd,
					    void *data),
				 void *data)
{
	struct pending_cmd *cmd, *tmp;

	list_for_each_entry_safe(cmd, tmp, &hdev->mgmt_pending, list) {
		if (opcode > 0 && cmd->opcode != opcode)
			continue;

		cb(cmd, data);
	}
}

static struct pending_cmd *mgmt_pending_find(u16 opcode, struct hci_dev *hdev)
{
	struct pending_cmd *cmd;

	list_for_each_entry(cmd, &hdev->mgmt_pending, list) {
		if (cmd->opcode == opcode)
			return cmd;
	}

	return NULL;
}

static void mgmt_pending_remove(struct pending_cmd *cmd)
{
	list_del(&cmd->list);
	mgmt_pending_free(cmd);
}

static int send_settings_rsp(struct sock *sk, u16 opcode, struct hci_dev *hdev)
{
	__le32 settings = cpu_to_le32(get_current_settings(hdev));

	return cmd_complete(sk, hdev->id, opcode, 0, &settings,
			    sizeof(settings));
}

static int set_powered(struct sock *sk, struct hci_dev *hdev, void *data,
		       u16 len)
{
	struct mgmt_mode *cp = data;
	struct pending_cmd *cmd;
	int err;

	BT_DBG("request for %s", hdev->name);

	if (cp->val != 0x00 && cp->val != 0x01)
		return cmd_status(sk, hdev->id, MGMT_OP_SET_POWERED,
				  MGMT_STATUS_INVALID_PARAMS);

	hci_dev_lock(hdev);

	if (test_and_clear_bit(HCI_AUTO_OFF, &hdev->dev_flags)) {
		cancel_delayed_work(&hdev->power_off);

		if (cp->val) {
			mgmt_pending_add(sk, MGMT_OP_SET_POWERED, hdev,
					 data, len);
			err = mgmt_powered(hdev, 1);
			goto failed;
		}
	}

	if (!!cp->val == hdev_is_powered(hdev)) {
		err = send_settings_rsp(sk, MGMT_OP_SET_POWERED, hdev);
		goto failed;
	}

	if (mgmt_pending_find(MGMT_OP_SET_POWERED, hdev)) {
		err = cmd_status(sk, hdev->id, MGMT_OP_SET_POWERED,
				 MGMT_STATUS_BUSY);
		goto failed;
	}

	cmd = mgmt_pending_add(sk, MGMT_OP_SET_POWERED, hdev, data, len);
	if (!cmd) {
		err = -ENOMEM;
		goto failed;
	}

	if (cp->val)
		queue_work(hdev->req_workqueue, &hdev->power_on);
	else
		queue_work(hdev->req_workqueue, &hdev->power_off.work);

	err = 0;

failed:
	hci_dev_unlock(hdev);
	return err;
}

static int mgmt_event(u16 event, struct hci_dev *hdev, void *data, u16 data_len,
		      struct sock *skip_sk)
{
	struct sk_buff *skb;
	struct mgmt_hdr *hdr;

	skb = alloc_skb(sizeof(*hdr) + data_len, GFP_KERNEL);
	if (!skb)
		return -ENOMEM;

	hdr = (void *) skb_put(skb, sizeof(*hdr));
	hdr->opcode = cpu_to_le16(event);
	if (hdev)
		hdr->index = cpu_to_le16(hdev->id);
	else
		hdr->index = __constant_cpu_to_le16(MGMT_INDEX_NONE);
	hdr->len = cpu_to_le16(data_len);

	if (data)
		memcpy(skb_put(skb, data_len), data, data_len);

	/* Time stamp */
	__net_timestamp(skb);

	hci_send_to_control(skb, skip_sk);
	kfree_skb(skb);

	return 0;
}

static int new_settings(struct hci_dev *hdev, struct sock *skip)
{
	__le32 ev;

	ev = cpu_to_le32(get_current_settings(hdev));

	return mgmt_event(MGMT_EV_NEW_SETTINGS, hdev, &ev, sizeof(ev), skip);
}

static int set_discoverable(struct sock *sk, struct hci_dev *hdev, void *data,
			    u16 len)
{
	struct mgmt_cp_set_discoverable *cp = data;
>>>>>>> common/android-3.10.y
	struct pending_cmd *cmd;
	u8 scan;
	int err;

	cp = (void *) data;

<<<<<<< HEAD
	BT_DBG("request for hci%u", index);
=======
	if (!lmp_bredr_capable(hdev))
		return cmd_status(sk, hdev->id, MGMT_OP_SET_DISCOVERABLE,
				 MGMT_STATUS_NOT_SUPPORTED);

	if (cp->val != 0x00 && cp->val != 0x01)
		return cmd_status(sk, hdev->id, MGMT_OP_SET_DISCOVERABLE,
				  MGMT_STATUS_INVALID_PARAMS);

	timeout = __le16_to_cpu(cp->timeout);
	if (!cp->val && timeout > 0)
		return cmd_status(sk, hdev->id, MGMT_OP_SET_DISCOVERABLE,
				  MGMT_STATUS_INVALID_PARAMS);
>>>>>>> common/android-3.10.y

	if (len != sizeof(*cp))
		return cmd_status(sk, index, MGMT_OP_SET_DISCOVERABLE, EINVAL);

	hdev = hci_dev_get(index);
	if (!hdev)
		return cmd_status(sk, index, MGMT_OP_SET_DISCOVERABLE, ENODEV);

<<<<<<< HEAD
	hci_dev_lock_bh(hdev);
=======
	if (mgmt_pending_find(MGMT_OP_SET_DISCOVERABLE, hdev) ||
	    mgmt_pending_find(MGMT_OP_SET_CONNECTABLE, hdev)) {
		err = cmd_status(sk, hdev->id, MGMT_OP_SET_DISCOVERABLE,
				 MGMT_STATUS_BUSY);
		goto failed;
	}
>>>>>>> common/android-3.10.y

	if (!test_bit(HCI_UP, &hdev->flags)) {
		err = cmd_status(sk, index, MGMT_OP_SET_DISCOVERABLE, ENETDOWN);
		goto failed;
	}

	if (mgmt_pending_find(MGMT_OP_SET_DISCOVERABLE, index) ||
			mgmt_pending_find(MGMT_OP_SET_CONNECTABLE, index)) {
		err = cmd_status(sk, index, MGMT_OP_SET_DISCOVERABLE, EBUSY);
		goto failed;
	}

	if (cp->val == test_bit(HCI_ISCAN, &hdev->flags) &&
					test_bit(HCI_PSCAN, &hdev->flags)) {
		err = cmd_status(sk, index, MGMT_OP_SET_DISCOVERABLE, EALREADY);
		goto failed;
	}

	cmd = mgmt_pending_add(sk, MGMT_OP_SET_DISCOVERABLE, index, data, len);
	if (!cmd) {
		err = -ENOMEM;
		goto failed;
	}

	scan = SCAN_PAGE;

	if (cp->val)
		scan |= SCAN_INQUIRY;

	err = hci_send_cmd(hdev, HCI_OP_WRITE_SCAN_ENABLE, 1, &scan);
	if (err < 0)
		mgmt_pending_remove(cmd);

failed:
	hci_dev_unlock_bh(hdev);
	hci_dev_put(hdev);

	return err;
}

<<<<<<< HEAD
static int set_connectable(struct sock *sk, u16 index, unsigned char *data,
									u16 len)
=======
static void write_fast_connectable(struct hci_request *req, bool enable)
{
	struct hci_dev *hdev = req->hdev;
	struct hci_cp_write_page_scan_activity acp;
	u8 type;

	if (hdev->hci_ver < BLUETOOTH_VER_1_2)
		return;

	if (enable) {
		type = PAGE_SCAN_TYPE_INTERLACED;

		/* 160 msec page scan interval */
		acp.interval = __constant_cpu_to_le16(0x0100);
	} else {
		type = PAGE_SCAN_TYPE_STANDARD;	/* default */

		/* default 1.28 sec page scan */
		acp.interval = __constant_cpu_to_le16(0x0800);
	}

	acp.window = __constant_cpu_to_le16(0x0012);

	if (__cpu_to_le16(hdev->page_scan_interval) != acp.interval ||
	    __cpu_to_le16(hdev->page_scan_window) != acp.window)
		hci_req_add(req, HCI_OP_WRITE_PAGE_SCAN_ACTIVITY,
			    sizeof(acp), &acp);

	if (hdev->page_scan_type != type)
		hci_req_add(req, HCI_OP_WRITE_PAGE_SCAN_TYPE, 1, &type);
}

static void set_connectable_complete(struct hci_dev *hdev, u8 status)
{
	struct pending_cmd *cmd;

	BT_DBG("status 0x%02x", status);

	hci_dev_lock(hdev);

	cmd = mgmt_pending_find(MGMT_OP_SET_CONNECTABLE, hdev);
	if (!cmd)
		goto unlock;

	send_settings_rsp(cmd->sk, MGMT_OP_SET_CONNECTABLE, hdev);

	mgmt_pending_remove(cmd);

unlock:
	hci_dev_unlock(hdev);
}

static int set_connectable(struct sock *sk, struct hci_dev *hdev, void *data,
			   u16 len)
>>>>>>> common/android-3.10.y
{
	struct mgmt_mode *cp;
	struct hci_dev *hdev;
	struct pending_cmd *cmd;
	struct hci_request req;
	u8 scan;
	int err;

<<<<<<< HEAD
	cp = (void *) data;
=======
	BT_DBG("request for %s", hdev->name);

	if (!lmp_bredr_capable(hdev))
		return cmd_status(sk, hdev->id, MGMT_OP_SET_CONNECTABLE,
				  MGMT_STATUS_NOT_SUPPORTED);

	if (cp->val != 0x00 && cp->val != 0x01)
		return cmd_status(sk, hdev->id, MGMT_OP_SET_CONNECTABLE,
				  MGMT_STATUS_INVALID_PARAMS);

	hci_dev_lock(hdev);
>>>>>>> common/android-3.10.y

	BT_DBG("request for hci%u", index);

	if (len != sizeof(*cp))
		return cmd_status(sk, index, MGMT_OP_SET_CONNECTABLE, EINVAL);

	hdev = hci_dev_get(index);
	if (!hdev)
		return cmd_status(sk, index, MGMT_OP_SET_CONNECTABLE, ENODEV);

	hci_dev_lock_bh(hdev);

	if (!test_bit(HCI_UP, &hdev->flags)) {
		err = cmd_status(sk, index, MGMT_OP_SET_CONNECTABLE, ENETDOWN);
		goto failed;
	}

<<<<<<< HEAD
	if (mgmt_pending_find(MGMT_OP_SET_DISCOVERABLE, index) ||
			mgmt_pending_find(MGMT_OP_SET_CONNECTABLE, index)) {
		err = cmd_status(sk, index, MGMT_OP_SET_CONNECTABLE, EBUSY);
=======
	if (mgmt_pending_find(MGMT_OP_SET_DISCOVERABLE, hdev) ||
	    mgmt_pending_find(MGMT_OP_SET_CONNECTABLE, hdev)) {
		err = cmd_status(sk, hdev->id, MGMT_OP_SET_CONNECTABLE,
				 MGMT_STATUS_BUSY);
>>>>>>> common/android-3.10.y
		goto failed;
	}

	if (cp->val == test_bit(HCI_PSCAN, &hdev->flags)) {
		err = cmd_status(sk, index, MGMT_OP_SET_CONNECTABLE, EALREADY);
		goto failed;
	}

	cmd = mgmt_pending_add(sk, MGMT_OP_SET_CONNECTABLE, index, data, len);
	if (!cmd) {
		err = -ENOMEM;
		goto failed;
	}

	if (cp->val)
		scan = SCAN_PAGE;
	else
		scan = 0;

<<<<<<< HEAD
	err = hci_send_cmd(hdev, HCI_OP_WRITE_SCAN_ENABLE, 1, &scan);
=======
		if (test_bit(HCI_ISCAN, &hdev->flags) &&
		    hdev->discov_timeout > 0)
			cancel_delayed_work(&hdev->discov_off);
	}

	hci_req_init(&req, hdev);

	hci_req_add(&req, HCI_OP_WRITE_SCAN_ENABLE, 1, &scan);

	/* If we're going from non-connectable to connectable or
	 * vice-versa when fast connectable is enabled ensure that fast
	 * connectable gets disabled. write_fast_connectable won't do
	 * anything if the page scan parameters are already what they
	 * should be.
	 */
	if (cp->val || test_bit(HCI_FAST_CONNECTABLE, &hdev->dev_flags))
		write_fast_connectable(&req, false);

	err = hci_req_run(&req, set_connectable_complete);
>>>>>>> common/android-3.10.y
	if (err < 0)
		mgmt_pending_remove(cmd);

failed:
	hci_dev_unlock_bh(hdev);
	hci_dev_put(hdev);

	return err;
}

static int mgmt_event(u16 event, u16 index, void *data, u16 data_len,
							struct sock *skip_sk)
{
	struct sk_buff *skb;
	struct mgmt_hdr *hdr;

	BT_DBG("hci%d %d", index, event);

<<<<<<< HEAD
	skb = alloc_skb(sizeof(*hdr) + data_len, GFP_ATOMIC);
	if (!skb)
		return -ENOMEM;
=======
	if (cp->val != 0x00 && cp->val != 0x01)
		return cmd_status(sk, hdev->id, MGMT_OP_SET_PAIRABLE,
				  MGMT_STATUS_INVALID_PARAMS);

	hci_dev_lock(hdev);
>>>>>>> common/android-3.10.y

	bt_cb(skb)->channel = HCI_CHANNEL_CONTROL;

	hdr = (void *) skb_put(skb, sizeof(*hdr));
	hdr->opcode = cpu_to_le16(event);
	hdr->index = cpu_to_le16(index);
	hdr->len = cpu_to_le16(data_len);

	if (data)
		memcpy(skb_put(skb, data_len), data, data_len);

	hci_send_to_sock(NULL, skb, skip_sk);
	kfree_skb(skb);

	return 0;
}

static int send_mode_rsp(struct sock *sk, u16 opcode, u16 index, u8 val)
{
	struct mgmt_mode rp;

	rp.val = val;

<<<<<<< HEAD
	return cmd_complete(sk, index, opcode, &rp, sizeof(rp));
}
=======
	if (!lmp_bredr_capable(hdev))
		return cmd_status(sk, hdev->id, MGMT_OP_SET_LINK_SECURITY,
				  MGMT_STATUS_NOT_SUPPORTED);

	if (cp->val != 0x00 && cp->val != 0x01)
		return cmd_status(sk, hdev->id, MGMT_OP_SET_LINK_SECURITY,
				  MGMT_STATUS_INVALID_PARAMS);

	hci_dev_lock(hdev);
>>>>>>> common/android-3.10.y

static int set_pairable(struct sock *sk, u16 index, unsigned char *data,
									u16 len)
{
	struct mgmt_mode *cp, ev;
	struct hci_dev *hdev;
	int err;

<<<<<<< HEAD
	cp = (void *) data;
=======
		if (!!cp->val != test_bit(HCI_LINK_SECURITY,
					  &hdev->dev_flags)) {
			change_bit(HCI_LINK_SECURITY, &hdev->dev_flags);
			changed = true;
		}
>>>>>>> common/android-3.10.y

	BT_DBG("request for hci%u", index);

	if (len != sizeof(*cp))
		return cmd_status(sk, index, MGMT_OP_SET_PAIRABLE, EINVAL);

	hdev = hci_dev_get(index);
	if (!hdev)
		return cmd_status(sk, index, MGMT_OP_SET_PAIRABLE, ENODEV);

	hci_dev_lock_bh(hdev);

	if (cp->val)
		set_bit(HCI_PAIRABLE, &hdev->flags);
	else
		clear_bit(HCI_PAIRABLE, &hdev->flags);

	err = send_mode_rsp(sk, MGMT_OP_SET_PAIRABLE, index, cp->val);
	if (err < 0)
		goto failed;

	ev.val = cp->val;

	err = mgmt_event(MGMT_EV_PAIRABLE, index, &ev, sizeof(ev), sk);

failed:
	hci_dev_unlock_bh(hdev);
	hci_dev_put(hdev);

	return err;
}

#define EIR_FLAGS		0x01 /* flags */
#define EIR_UUID16_SOME		0x02 /* 16-bit UUID, more available */
#define EIR_UUID16_ALL		0x03 /* 16-bit UUID, all listed */
#define EIR_UUID32_SOME		0x04 /* 32-bit UUID, more available */
#define EIR_UUID32_ALL		0x05 /* 32-bit UUID, all listed */
#define EIR_UUID128_SOME	0x06 /* 128-bit UUID, more available */
#define EIR_UUID128_ALL		0x07 /* 128-bit UUID, all listed */
#define EIR_NAME_SHORT		0x08 /* shortened local name */
#define EIR_NAME_COMPLETE	0x09 /* complete local name */
#define EIR_TX_POWER		0x0A /* transmit power level */
#define EIR_DEVICE_ID		0x10 /* device ID */

#define PNP_INFO_SVCLASS_ID		0x1200

<<<<<<< HEAD
static u8 bluetooth_base_uuid[] = {
			0xFB, 0x34, 0x9B, 0x5F, 0x80, 0x00, 0x00, 0x80,
			0x00, 0x10, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
};

static u16 get_uuid16(u8 *uuid128)
{
	u32 val;
	int i;
=======
	if (!lmp_ssp_capable(hdev))
		return cmd_status(sk, hdev->id, MGMT_OP_SET_SSP,
				  MGMT_STATUS_NOT_SUPPORTED);

	if (cp->val != 0x00 && cp->val != 0x01)
		return cmd_status(sk, hdev->id, MGMT_OP_SET_SSP,
				  MGMT_STATUS_INVALID_PARAMS);

	hci_dev_lock(hdev);
>>>>>>> common/android-3.10.y

	for (i = 0; i < 12; i++) {
		if (bluetooth_base_uuid[i] != uuid128[i])
			return 0;
	}

	memcpy(&val, &uuid128[12], 4);

	val = le32_to_cpu(val);
	if (val > 0xffff)
		return 0;

	return (u16) val;
}

static void create_eir(struct hci_dev *hdev, u8 *data)
{
	u8 *ptr = data;
	u16 eir_len = 0;
	u16 uuid16_list[HCI_MAX_EIR_LENGTH / sizeof(u16)];
	int i, truncated = 0;
	struct list_head *p;
	size_t name_len;

	name_len = strnlen(hdev->dev_name, HCI_MAX_EIR_LENGTH);

<<<<<<< HEAD
	if (name_len > 0) {
		/* EIR Data type */
		if (name_len > 48) {
			name_len = 48;
			ptr[1] = EIR_NAME_SHORT;
		} else
			ptr[1] = EIR_NAME_COMPLETE;
=======
	if (mgmt_pending_find(MGMT_OP_SET_SSP, hdev)) {
		err = cmd_status(sk, hdev->id, MGMT_OP_SET_SSP,
				 MGMT_STATUS_BUSY);
		goto failed;
	}
>>>>>>> common/android-3.10.y

		/* EIR Data length */
		ptr[0] = name_len + 1;

		memcpy(ptr + 2, hdev->dev_name, name_len);

		eir_len += (name_len + 2);
		ptr += (name_len + 2);
	}

	memset(uuid16_list, 0, sizeof(uuid16_list));

	/* Group all UUID16 types */
	list_for_each(p, &hdev->uuids) {
		struct bt_uuid *uuid = list_entry(p, struct bt_uuid, list);
		u16 uuid16;

<<<<<<< HEAD
		uuid16 = get_uuid16(uuid->uuid);
		if (uuid16 == 0)
			return;
=======
	if (cp->val != 0x00 && cp->val != 0x01)
		return cmd_status(sk, hdev->id, MGMT_OP_SET_HS,
				  MGMT_STATUS_INVALID_PARAMS);

	if (cp->val)
		set_bit(HCI_HS_ENABLED, &hdev->dev_flags);
	else
		clear_bit(HCI_HS_ENABLED, &hdev->dev_flags);
>>>>>>> common/android-3.10.y

		if (uuid16 < 0x1100)
			continue;

		if (uuid16 == PNP_INFO_SVCLASS_ID)
			continue;

		/* Stop if not enough space to put next UUID */
		if (eir_len + 2 + sizeof(u16) > HCI_MAX_EIR_LENGTH) {
			truncated = 1;
			break;
		}

<<<<<<< HEAD
		/* Check for duplicates */
		for (i = 0; uuid16_list[i] != 0; i++)
			if (uuid16_list[i] == uuid16)
				break;

		if (uuid16_list[i] == 0) {
			uuid16_list[i] = uuid16;
			eir_len += sizeof(u16);
		}
	}

	if (uuid16_list[0] != 0) {
		u8 *length = ptr;

		/* EIR Data type */
		ptr[1] = truncated ? EIR_UUID16_SOME : EIR_UUID16_ALL;
=======
	if (!lmp_le_capable(hdev))
		return cmd_status(sk, hdev->id, MGMT_OP_SET_LE,
				  MGMT_STATUS_NOT_SUPPORTED);

	if (cp->val != 0x00 && cp->val != 0x01)
		return cmd_status(sk, hdev->id, MGMT_OP_SET_LE,
				  MGMT_STATUS_INVALID_PARAMS);

	/* LE-only devices do not allow toggling LE on/off */
	if (!lmp_bredr_capable(hdev))
		return cmd_status(sk, hdev->id, MGMT_OP_SET_LE,
				  MGMT_STATUS_REJECTED);

	hci_dev_lock(hdev);

	val = !!cp->val;
	enabled = lmp_host_le_capable(hdev);
>>>>>>> common/android-3.10.y

		ptr += 2;
		eir_len += 2;

		for (i = 0; uuid16_list[i] != 0; i++) {
			*ptr++ = (uuid16_list[i] & 0x00ff);
			*ptr++ = (uuid16_list[i] & 0xff00) >> 8;
		}

		/* EIR Data length */
		*length = (i * sizeof(u16)) + 1;
	}
}

static int update_eir(struct hci_dev *hdev)
{
	struct hci_cp_write_eir cp;

	if (!(hdev->features[6] & LMP_EXT_INQ))
		return 0;

	if (hdev->ssp_mode == 0)
		return 0;

	if (test_bit(HCI_SERVICE_CACHE, &hdev->flags))
		return 0;

	memset(&cp, 0, sizeof(cp));

<<<<<<< HEAD
	create_eir(hdev, cp.data);

	if (memcmp(cp.data, hdev->eir, sizeof(cp.data)) == 0)
		return 0;
=======
	if (val) {
		hci_cp.le = val;
		hci_cp.simul = lmp_le_br_capable(hdev);
	}

	err = hci_send_cmd(hdev, HCI_OP_WRITE_LE_HOST_SUPPORTED, sizeof(hci_cp),
			   &hci_cp);
	if (err < 0)
		mgmt_pending_remove(cmd);
>>>>>>> common/android-3.10.y

	memcpy(hdev->eir, cp.data, sizeof(cp.data));

	return hci_send_cmd(hdev, HCI_OP_WRITE_EIR, sizeof(cp), &cp);
}

<<<<<<< HEAD
static int add_uuid(struct sock *sk, u16 index, unsigned char *data, u16 len)
{
	struct mgmt_cp_add_uuid *cp;
	struct hci_dev *hdev;
=======
/* This is a helper function to test for pending mgmt commands that can
 * cause CoD or EIR HCI commands. We can only allow one such pending
 * mgmt command at a time since otherwise we cannot easily track what
 * the current values are, will be, and based on that calculate if a new
 * HCI command needs to be sent and if yes with what value.
 */
static bool pending_eir_or_class(struct hci_dev *hdev)
{
	struct pending_cmd *cmd;

	list_for_each_entry(cmd, &hdev->mgmt_pending, list) {
		switch (cmd->opcode) {
		case MGMT_OP_ADD_UUID:
		case MGMT_OP_REMOVE_UUID:
		case MGMT_OP_SET_DEV_CLASS:
		case MGMT_OP_SET_POWERED:
			return true;
		}
	}

	return false;
}

static const u8 bluetooth_base_uuid[] = {
			0xfb, 0x34, 0x9b, 0x5f, 0x80, 0x00, 0x00, 0x80,
			0x00, 0x10, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
};

static u8 get_uuid_size(const u8 *uuid)
{
	u32 val;

	if (memcmp(uuid, bluetooth_base_uuid, 12))
		return 128;

	val = get_unaligned_le32(&uuid[12]);
	if (val > 0xffff)
		return 32;

	return 16;
}

static void mgmt_class_complete(struct hci_dev *hdev, u16 mgmt_op, u8 status)
{
	struct pending_cmd *cmd;

	hci_dev_lock(hdev);

	cmd = mgmt_pending_find(mgmt_op, hdev);
	if (!cmd)
		goto unlock;

	cmd_complete(cmd->sk, cmd->index, cmd->opcode, mgmt_status(status),
		     hdev->dev_class, 3);

	mgmt_pending_remove(cmd);

unlock:
	hci_dev_unlock(hdev);
}

static void add_uuid_complete(struct hci_dev *hdev, u8 status)
{
	BT_DBG("status 0x%02x", status);

	mgmt_class_complete(hdev, MGMT_OP_ADD_UUID, status);
}

static int add_uuid(struct sock *sk, struct hci_dev *hdev, void *data, u16 len)
{
	struct mgmt_cp_add_uuid *cp = data;
	struct pending_cmd *cmd;
	struct hci_request req;
>>>>>>> common/android-3.10.y
	struct bt_uuid *uuid;
	int err;

	cp = (void *) data;

	BT_DBG("request for hci%u", index);

<<<<<<< HEAD
	if (len != sizeof(*cp))
		return cmd_status(sk, index, MGMT_OP_ADD_UUID, EINVAL);

	hdev = hci_dev_get(index);
	if (!hdev)
		return cmd_status(sk, index, MGMT_OP_ADD_UUID, ENODEV);

	hci_dev_lock_bh(hdev);
=======
	if (pending_eir_or_class(hdev)) {
		err = cmd_status(sk, hdev->id, MGMT_OP_ADD_UUID,
				 MGMT_STATUS_BUSY);
		goto failed;
	}
>>>>>>> common/android-3.10.y

	uuid = kmalloc(sizeof(*uuid), GFP_KERNEL);
	if (!uuid) {
		err = -ENOMEM;
		goto failed;
	}

	memcpy(uuid->uuid, cp->uuid, 16);
	uuid->svc_hint = cp->svc_hint;
	uuid->size = get_uuid_size(cp->uuid);

	list_add_tail(&uuid->list, &hdev->uuids);

<<<<<<< HEAD
	if (test_bit(HCI_UP, &hdev->flags)) {

		err = update_class(hdev);
		if (err < 0)
			goto failed;

		err = update_eir(hdev);
		if (err < 0)
			goto failed;
	} else
		err = 0;
=======
	hci_req_init(&req, hdev);

	update_class(&req);
	update_eir(&req);

	err = hci_req_run(&req, add_uuid_complete);
	if (err < 0) {
		if (err != -ENODATA)
			goto failed;

		err = cmd_complete(sk, hdev->id, MGMT_OP_ADD_UUID, 0,
				   hdev->dev_class, 3);
		goto failed;
	}
>>>>>>> common/android-3.10.y

	err = cmd_complete(sk, index, MGMT_OP_ADD_UUID, NULL, 0);

	err = 0;

failed:
	hci_dev_unlock_bh(hdev);
	hci_dev_put(hdev);

<<<<<<< HEAD
	return err;
}

static int remove_uuid(struct sock *sk, u16 index, unsigned char *data, u16 len)
{
	struct list_head *p, *n;
	struct mgmt_cp_remove_uuid *cp;
	struct hci_dev *hdev;
=======
	if (!test_and_set_bit(HCI_SERVICE_CACHE, &hdev->dev_flags)) {
		queue_delayed_work(hdev->workqueue, &hdev->service_cache,
				   CACHE_TIMEOUT);
		return true;
	}

	return false;
}

static void remove_uuid_complete(struct hci_dev *hdev, u8 status)
{
	BT_DBG("status 0x%02x", status);

	mgmt_class_complete(hdev, MGMT_OP_REMOVE_UUID, status);
}

static int remove_uuid(struct sock *sk, struct hci_dev *hdev, void *data,
		       u16 len)
{
	struct mgmt_cp_remove_uuid *cp = data;
	struct pending_cmd *cmd;
	struct bt_uuid *match, *tmp;
>>>>>>> common/android-3.10.y
	u8 bt_uuid_any[] = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
	struct hci_request req;
	int err, found;

	cp = (void *) data;

	BT_DBG("request for hci%u", index);

<<<<<<< HEAD
	if (len != sizeof(*cp))
		return cmd_status(sk, index, MGMT_OP_REMOVE_UUID, EINVAL);
=======
	if (pending_eir_or_class(hdev)) {
		err = cmd_status(sk, hdev->id, MGMT_OP_REMOVE_UUID,
				 MGMT_STATUS_BUSY);
		goto unlock;
	}
>>>>>>> common/android-3.10.y

	hdev = hci_dev_get(index);
	if (!hdev)
		return cmd_status(sk, index, MGMT_OP_REMOVE_UUID, ENODEV);

	hci_dev_lock_bh(hdev);

	if (memcmp(cp->uuid, bt_uuid_any, 16) == 0) {
		err = hci_uuids_clear(hdev);
		goto unlock;
	}

	found = 0;

	list_for_each_entry_safe(match, tmp, &hdev->uuids, list) {
		if (memcmp(match->uuid, cp->uuid, 16) != 0)
			continue;

		list_del(&match->list);
		kfree(match);
		found++;
	}

	if (found == 0) {
		err = cmd_status(sk, index, MGMT_OP_REMOVE_UUID, ENOENT);
		goto unlock;
	}

<<<<<<< HEAD
	if (test_bit(HCI_UP, &hdev->flags)) {
		err = update_class(hdev);
		if (err < 0)
			goto unlock;

		err = update_eir(hdev);
		if (err < 0)
			goto unlock;
	} else
		err = 0;
=======
update_class:
	hci_req_init(&req, hdev);

	update_class(&req);
	update_eir(&req);

	err = hci_req_run(&req, remove_uuid_complete);
	if (err < 0) {
		if (err != -ENODATA)
			goto unlock;

		err = cmd_complete(sk, hdev->id, MGMT_OP_REMOVE_UUID, 0,
				   hdev->dev_class, 3);
		goto unlock;
	}
>>>>>>> common/android-3.10.y

	err = cmd_complete(sk, index, MGMT_OP_REMOVE_UUID, NULL, 0);

	err = 0;

unlock:
	hci_dev_unlock_bh(hdev);
	hci_dev_put(hdev);

	return err;
}

<<<<<<< HEAD
static int set_dev_class(struct sock *sk, u16 index, unsigned char *data,
									u16 len)
{
	struct hci_dev *hdev;
	struct mgmt_cp_set_dev_class *cp;
=======
static void set_class_complete(struct hci_dev *hdev, u8 status)
{
	BT_DBG("status 0x%02x", status);

	mgmt_class_complete(hdev, MGMT_OP_SET_DEV_CLASS, status);
}

static int set_dev_class(struct sock *sk, struct hci_dev *hdev, void *data,
			 u16 len)
{
	struct mgmt_cp_set_dev_class *cp = data;
	struct pending_cmd *cmd;
	struct hci_request req;
>>>>>>> common/android-3.10.y
	int err;

	cp = (void *) data;

<<<<<<< HEAD
	BT_DBG("request for hci%u", index);

	if (len != sizeof(*cp))
		return cmd_status(sk, index, MGMT_OP_SET_DEV_CLASS, EINVAL);

	hdev = hci_dev_get(index);
	if (!hdev)
		return cmd_status(sk, index, MGMT_OP_SET_DEV_CLASS, ENODEV);

	hci_dev_lock_bh(hdev);

	hdev->major_class &= ~MGMT_MAJOR_CLASS_MASK;
	hdev->major_class |= cp->major & MGMT_MAJOR_CLASS_MASK;
=======
	if (!lmp_bredr_capable(hdev))
		return cmd_status(sk, hdev->id, MGMT_OP_SET_DEV_CLASS,
				  MGMT_STATUS_NOT_SUPPORTED);

	hci_dev_lock(hdev);

	if (pending_eir_or_class(hdev)) {
		err = cmd_status(sk, hdev->id, MGMT_OP_SET_DEV_CLASS,
				 MGMT_STATUS_BUSY);
		goto unlock;
	}

	if ((cp->minor & 0x03) != 0 || (cp->major & 0xe0) != 0) {
		err = cmd_status(sk, hdev->id, MGMT_OP_SET_DEV_CLASS,
				 MGMT_STATUS_INVALID_PARAMS);
		goto unlock;
	}

	hdev->major_class = cp->major;
>>>>>>> common/android-3.10.y
	hdev->minor_class = cp->minor;

	if (test_bit(HCI_UP, &hdev->flags)) {
		err = update_class(hdev);
		if (err == 0)
			err = cmd_complete(sk, index,
		MGMT_OP_SET_DEV_CLASS, hdev->dev_class, sizeof(u8)*3);
	} else
		err = cmd_complete(sk, index, MGMT_OP_SET_DEV_CLASS, NULL, 0);

<<<<<<< HEAD
	hci_dev_unlock_bh(hdev);
	hci_dev_put(hdev);

	return err;
}

static int set_service_cache(struct sock *sk, u16 index,  unsigned char *data,
									u16 len)
{
	struct hci_dev *hdev;
	struct mgmt_cp_set_service_cache *cp;
	int err;
=======
	hci_req_init(&req, hdev);

	if (test_and_clear_bit(HCI_SERVICE_CACHE, &hdev->dev_flags)) {
		hci_dev_unlock(hdev);
		cancel_delayed_work_sync(&hdev->service_cache);
		hci_dev_lock(hdev);
		update_eir(&req);
	}

	update_class(&req);

	err = hci_req_run(&req, set_class_complete);
	if (err < 0) {
		if (err != -ENODATA)
			goto unlock;

		err = cmd_complete(sk, hdev->id, MGMT_OP_SET_DEV_CLASS, 0,
				   hdev->dev_class, 3);
		goto unlock;
	}
>>>>>>> common/android-3.10.y

	cp = (void *) data;

	if (len != sizeof(*cp))
		return cmd_status(sk, index, MGMT_OP_SET_SERVICE_CACHE, EINVAL);

	hdev = hci_dev_get(index);
	if (!hdev)
		return cmd_status(sk, index, MGMT_OP_SET_SERVICE_CACHE, ENODEV);

	hci_dev_lock_bh(hdev);

	BT_DBG("hci%u enable %d", index, cp->enable);

	if (cp->enable) {
		set_bit(HCI_SERVICE_CACHE, &hdev->flags);
		err = 0;
	} else {
		clear_bit(HCI_SERVICE_CACHE, &hdev->flags);
		if (test_bit(HCI_UP, &hdev->flags)) {
			err = update_class(hdev);
			if (err == 0)
				err = update_eir(hdev);
		} else
			err = 0;
	}

<<<<<<< HEAD
	if (err == 0)
		err = cmd_complete(sk, index, MGMT_OP_SET_SERVICE_CACHE, NULL,
									0);

	hci_dev_unlock_bh(hdev);
	hci_dev_put(hdev);

	return err;
}

static int load_keys(struct sock *sk, u16 index, unsigned char *data, u16 len)
=======
	err = 0;

unlock:
	hci_dev_unlock(hdev);
	return err;
}

static int load_link_keys(struct sock *sk, struct hci_dev *hdev, void *data,
			  u16 len)
>>>>>>> common/android-3.10.y
{
	struct hci_dev *hdev;
	struct mgmt_cp_load_keys *cp;
	u16 key_count, expected_len;
	int i, err;

	cp = (void *) data;

	if (len < sizeof(*cp))
		return -EINVAL;

	key_count = __le16_to_cpu(cp->key_count);

<<<<<<< HEAD
	expected_len = sizeof(*cp) + key_count * sizeof(struct mgmt_key_info);
	if (expected_len > len) {
		BT_ERR("load_keys: expected at least %u bytes, got %u bytes",
							expected_len, len);
		return -EINVAL;
	}

	hdev = hci_dev_get(index);
	if (!hdev)
		return cmd_status(sk, index, MGMT_OP_LOAD_KEYS, ENODEV);

	BT_DBG("hci%u debug_keys %u key_count %u", index, cp->debug_keys,
								key_count);
=======
	expected_len = sizeof(*cp) + key_count *
					sizeof(struct mgmt_link_key_info);
	if (expected_len != len) {
		BT_ERR("load_link_keys: expected %u bytes, got %u bytes",
		       len, expected_len);
		return cmd_status(sk, hdev->id, MGMT_OP_LOAD_LINK_KEYS,
				  MGMT_STATUS_INVALID_PARAMS);
	}

	if (cp->debug_keys != 0x00 && cp->debug_keys != 0x01)
		return cmd_status(sk, hdev->id, MGMT_OP_LOAD_LINK_KEYS,
				  MGMT_STATUS_INVALID_PARAMS);

	BT_DBG("%s debug_keys %u key_count %u", hdev->name, cp->debug_keys,
	       key_count);

	for (i = 0; i < key_count; i++) {
		struct mgmt_link_key_info *key = &cp->keys[i];

		if (key->addr.type != BDADDR_BREDR)
			return cmd_status(sk, hdev->id, MGMT_OP_LOAD_LINK_KEYS,
					  MGMT_STATUS_INVALID_PARAMS);
	}
>>>>>>> common/android-3.10.y

	hci_dev_lock_bh(hdev);

	hci_link_keys_clear(hdev);

	set_bit(HCI_LINK_KEYS, &hdev->flags);

	if (cp->debug_keys)
		set_bit(HCI_DEBUG_KEYS, &hdev->flags);
	else
		clear_bit(HCI_DEBUG_KEYS, &hdev->flags);

	len -= sizeof(*cp);
	i = 0;

	while (i < len) {
		struct mgmt_key_info *key = (void *) cp->keys + i;

		i += sizeof(*key);

		if (key->key_type == KEY_TYPE_LTK) {
			struct key_master_id *id = (void *) key->data;

			if (key->dlen != sizeof(struct key_master_id))
				continue;

			hci_add_ltk(hdev, 0, &key->bdaddr, key->addr_type,
					key->pin_len, key->auth, id->ediv,
					id->rand, key->val);

			continue;
		}

		hci_add_link_key(hdev, 0, &key->bdaddr, key->val, key->key_type,
								key->pin_len);
	}

	err = cmd_complete(sk, index, MGMT_OP_LOAD_KEYS, NULL, 0);

	hci_dev_unlock_bh(hdev);
	hci_dev_put(hdev);

	return err;
}

static int remove_key(struct sock *sk, u16 index, unsigned char *data, u16 len)
{
	struct hci_dev *hdev;
	struct mgmt_cp_remove_key *cp;
	struct hci_conn *conn;
	int err;

	cp = (void *) data;

	if (len != sizeof(*cp))
		return cmd_status(sk, index, MGMT_OP_REMOVE_KEY, EINVAL);

	hdev = hci_dev_get(index);
	if (!hdev)
		return cmd_status(sk, index, MGMT_OP_REMOVE_KEY, ENODEV);

	hci_dev_lock_bh(hdev);

	err = hci_remove_link_key(hdev, &cp->bdaddr);
	if (err < 0) {
		err = cmd_status(sk, index, MGMT_OP_REMOVE_KEY, -err);
		goto unlock;
	}

	err = 0;

	if (!test_bit(HCI_UP, &hdev->flags) || !cp->disconnect)
		goto unlock;

	conn = hci_conn_hash_lookup_ba(hdev, ACL_LINK, &cp->bdaddr);
	if (conn) {
		struct hci_cp_disconnect dc;

		put_unaligned_le16(conn->handle, &dc.handle);
		dc.reason = 0x13; /* Remote User Terminated Connection */
		err = hci_send_cmd(hdev, HCI_OP_DISCONNECT, 0, NULL);
	}

unlock:
	hci_dev_unlock_bh(hdev);
	hci_dev_put(hdev);

	return err;
}

static int disconnect(struct sock *sk, u16 index, unsigned char *data, u16 len)
{
	struct hci_dev *hdev;
	struct mgmt_cp_disconnect *cp;
	struct hci_cp_disconnect dc;
	struct pending_cmd *cmd;
	struct hci_conn *conn;
	int err;

<<<<<<< HEAD
	BT_DBG("");

	cp = (void *) data;

	if (len != sizeof(*cp))
		return cmd_status(sk, index, MGMT_OP_DISCONNECT, EINVAL);

	hdev = hci_dev_get(index);
	if (!hdev)
		return cmd_status(sk, index, MGMT_OP_DISCONNECT, ENODEV);
=======
	memset(&rp, 0, sizeof(rp));
	bacpy(&rp.addr.bdaddr, &cp->addr.bdaddr);
	rp.addr.type = cp->addr.type;

	if (!bdaddr_type_is_valid(cp->addr.type))
		return cmd_complete(sk, hdev->id, MGMT_OP_UNPAIR_DEVICE,
				    MGMT_STATUS_INVALID_PARAMS,
				    &rp, sizeof(rp));

	if (cp->disconnect != 0x00 && cp->disconnect != 0x01)
		return cmd_complete(sk, hdev->id, MGMT_OP_UNPAIR_DEVICE,
				    MGMT_STATUS_INVALID_PARAMS,
				    &rp, sizeof(rp));

	hci_dev_lock(hdev);

	if (!hdev_is_powered(hdev)) {
		err = cmd_complete(sk, hdev->id, MGMT_OP_UNPAIR_DEVICE,
				   MGMT_STATUS_NOT_POWERED, &rp, sizeof(rp));
		goto unlock;
	}

	if (cp->addr.type == BDADDR_BREDR)
		err = hci_remove_link_key(hdev, &cp->addr.bdaddr);
	else
		err = hci_remove_ltk(hdev, &cp->addr.bdaddr);
>>>>>>> common/android-3.10.y

	hci_dev_lock_bh(hdev);

	if (!test_bit(HCI_UP, &hdev->flags)) {
		err = cmd_status(sk, index, MGMT_OP_DISCONNECT, ENETDOWN);
		goto failed;
	}

<<<<<<< HEAD
	if (mgmt_pending_find(MGMT_OP_DISCONNECT, index)) {
		err = cmd_status(sk, index, MGMT_OP_DISCONNECT, EBUSY);
		goto failed;
=======
	if (cp->disconnect) {
		if (cp->addr.type == BDADDR_BREDR)
			conn = hci_conn_hash_lookup_ba(hdev, ACL_LINK,
						       &cp->addr.bdaddr);
		else
			conn = hci_conn_hash_lookup_ba(hdev, LE_LINK,
						       &cp->addr.bdaddr);
	} else {
		conn = NULL;
>>>>>>> common/android-3.10.y
	}

	conn = hci_conn_hash_lookup_ba(hdev, ACL_LINK, &cp->bdaddr);
	if (!conn) {
		conn = hci_conn_hash_lookup_ba(hdev, LE_LINK, &cp->bdaddr);
		if (!conn) {
			err = cmd_status(sk, index, MGMT_OP_DISCONNECT,
							ENOTCONN);
			goto failed;
		}
	}

	cmd = mgmt_pending_add(sk, MGMT_OP_DISCONNECT, index, data, len);
	if (!cmd) {
		err = -ENOMEM;
		goto failed;
	}

	dc.handle = cpu_to_le16(conn->handle);
	dc.reason = 0x13; /* Remote User Terminated Connection */

	err = hci_send_cmd(hdev, HCI_OP_DISCONNECT, sizeof(dc), &dc);
	if (err < 0)
		mgmt_pending_remove(cmd);

failed:
	hci_dev_unlock_bh(hdev);
	hci_dev_put(hdev);

	return err;
}

static int get_connections(struct sock *sk, u16 index)
{
	struct mgmt_rp_get_connections *rp;
	struct hci_dev *hdev;
	struct list_head *p;
	size_t rp_len;
	u16 count;
	int i, err;

	BT_DBG("");

	hdev = hci_dev_get(index);
	if (!hdev)
		return cmd_status(sk, index, MGMT_OP_GET_CONNECTIONS, ENODEV);

	hci_dev_lock_bh(hdev);

	count = 0;
	list_for_each(p, &hdev->conn_hash.list) {
		count++;
	}

	rp_len = sizeof(*rp) + (count * sizeof(bdaddr_t));
	rp = kmalloc(rp_len, GFP_ATOMIC);
	if (!rp) {
		err = -ENOMEM;
		goto unlock;
	}

	put_unaligned_le16(count, &rp->conn_count);

	read_lock(&hci_dev_list_lock);

	i = 0;
	list_for_each(p, &hdev->conn_hash.list) {
		struct hci_conn *c = list_entry(p, struct hci_conn, list);

		bacpy(&rp->conn[i++], &c->dst);
	}

	read_unlock(&hci_dev_list_lock);

	err = cmd_complete(sk, index, MGMT_OP_GET_CONNECTIONS, rp, rp_len);

unlock:
	kfree(rp);
	hci_dev_unlock_bh(hdev);
	hci_dev_put(hdev);
	return err;
}

static int pin_code_reply(struct sock *sk, u16 index, unsigned char *data,
									u16 len)
{
<<<<<<< HEAD
	struct hci_dev *hdev;
	struct mgmt_cp_pin_code_reply *cp;
	struct hci_cp_pin_code_reply reply;
=======
	struct mgmt_cp_disconnect *cp = data;
	struct mgmt_rp_disconnect rp;
	struct hci_cp_disconnect dc;
>>>>>>> common/android-3.10.y
	struct pending_cmd *cmd;
	int err;

	BT_DBG("");

<<<<<<< HEAD
	cp = (void *) data;

	if (len != sizeof(*cp))
		return cmd_status(sk, index, MGMT_OP_PIN_CODE_REPLY, EINVAL);

	hdev = hci_dev_get(index);
	if (!hdev)
		return cmd_status(sk, index, MGMT_OP_PIN_CODE_REPLY, ENODEV);

	hci_dev_lock_bh(hdev);

	if (!test_bit(HCI_UP, &hdev->flags)) {
		err = cmd_status(sk, index, MGMT_OP_PIN_CODE_REPLY, ENETDOWN);
=======
	memset(&rp, 0, sizeof(rp));
	bacpy(&rp.addr.bdaddr, &cp->addr.bdaddr);
	rp.addr.type = cp->addr.type;

	if (!bdaddr_type_is_valid(cp->addr.type))
		return cmd_complete(sk, hdev->id, MGMT_OP_DISCONNECT,
				    MGMT_STATUS_INVALID_PARAMS,
				    &rp, sizeof(rp));

	hci_dev_lock(hdev);

	if (!test_bit(HCI_UP, &hdev->flags)) {
		err = cmd_complete(sk, hdev->id, MGMT_OP_DISCONNECT,
				   MGMT_STATUS_NOT_POWERED, &rp, sizeof(rp));
		goto failed;
	}

	if (mgmt_pending_find(MGMT_OP_DISCONNECT, hdev)) {
		err = cmd_complete(sk, hdev->id, MGMT_OP_DISCONNECT,
				   MGMT_STATUS_BUSY, &rp, sizeof(rp));
		goto failed;
	}

	if (cp->addr.type == BDADDR_BREDR)
		conn = hci_conn_hash_lookup_ba(hdev, ACL_LINK,
					       &cp->addr.bdaddr);
	else
		conn = hci_conn_hash_lookup_ba(hdev, LE_LINK, &cp->addr.bdaddr);

	if (!conn || conn->state == BT_OPEN || conn->state == BT_CLOSED) {
		err = cmd_complete(sk, hdev->id, MGMT_OP_DISCONNECT,
				   MGMT_STATUS_NOT_CONNECTED, &rp, sizeof(rp));
>>>>>>> common/android-3.10.y
		goto failed;
	}

	cmd = mgmt_pending_add(sk, MGMT_OP_PIN_CODE_REPLY, index, data, len);
	if (!cmd) {
		err = -ENOMEM;
		goto failed;
	}

<<<<<<< HEAD
	bacpy(&reply.bdaddr, &cp->bdaddr);
	reply.pin_len = cp->pin_len;
	memcpy(reply.pin_code, cp->pin_code, 16);
=======
	dc.handle = cpu_to_le16(conn->handle);
	dc.reason = HCI_ERROR_REMOTE_USER_TERM;
>>>>>>> common/android-3.10.y

	err = hci_send_cmd(hdev, HCI_OP_PIN_CODE_REPLY, sizeof(reply), &reply);
	if (err < 0)
		mgmt_pending_remove(cmd);

failed:
	hci_dev_unlock_bh(hdev);
	hci_dev_put(hdev);

	return err;
}

<<<<<<< HEAD
static int encrypt_link(struct sock *sk, u16 index, unsigned char *data,
									u16 len)
{
	struct hci_dev *hdev;
	struct mgmt_cp_encrypt_link *cp;
	struct hci_cp_set_conn_encrypt enc;
	struct hci_conn *conn;
	int err = 0;

	BT_DBG("");

	cp = (void *) data;

	if (len != sizeof(*cp))
		return cmd_status(sk, index, MGMT_OP_ENCRYPT_LINK, EINVAL);

	hdev = hci_dev_get(index);
	if (!hdev)
		return cmd_status(sk, index, MGMT_OP_ENCRYPT_LINK, ENODEV);

	hci_dev_lock_bh(hdev);

	if (!test_bit(HCI_UP, &hdev->flags)) {
		err = cmd_status(sk, index, MGMT_OP_ENCRYPT_LINK, ENETDOWN);
		goto done;
	}

	conn = hci_conn_hash_lookup_ba(hdev, ACL_LINK, &cp->bdaddr);
	if (!conn) {
		err = cmd_status(sk, index, MGMT_OP_ENCRYPT_LINK, ENOTCONN);
		goto done;
	}

	if (test_and_set_bit(HCI_CONN_ENCRYPT_PEND, &conn->pend)) {
		err = cmd_status(sk, index, MGMT_OP_ENCRYPT_LINK, EINPROGRESS);
		goto done;
	}

	if (conn->link_mode & HCI_LM_AUTH) {
		enc.handle = cpu_to_le16(conn->handle);
		enc.encrypt = cp->enable;
		err = hci_send_cmd(hdev,
				HCI_OP_SET_CONN_ENCRYPT, sizeof(enc), &enc);
	} else {
		conn->auth_initiator = 1;
		if (!test_and_set_bit(HCI_CONN_AUTH_PEND, &conn->pend)) {
			struct hci_cp_auth_requested cp;
			cp.handle = cpu_to_le16(conn->handle);
			err = hci_send_cmd(conn->hdev,
				HCI_OP_AUTH_REQUESTED, sizeof(cp), &cp);
		}
=======
static u8 link_to_bdaddr(u8 link_type, u8 addr_type)
{
	switch (link_type) {
	case LE_LINK:
		switch (addr_type) {
		case ADDR_LE_DEV_PUBLIC:
			return BDADDR_LE_PUBLIC;

		default:
			/* Fallback to LE Random address type */
			return BDADDR_LE_RANDOM;
		}

	default:
		/* Fallback to BR/EDR type */
		return BDADDR_BREDR;
>>>>>>> common/android-3.10.y
	}

done:
	hci_dev_unlock_bh(hdev);
	hci_dev_put(hdev);

	return err;
}


static int pin_code_neg_reply(struct sock *sk, u16 index, unsigned char *data,
									u16 len)
{
	struct hci_dev *hdev;
	struct mgmt_cp_pin_code_neg_reply *cp;
	struct pending_cmd *cmd;
	int err;

	BT_DBG("");

	cp = (void *) data;

	if (len != sizeof(*cp))
		return cmd_status(sk, index, MGMT_OP_PIN_CODE_NEG_REPLY,
									EINVAL);

	hdev = hci_dev_get(index);
	if (!hdev)
		return cmd_status(sk, index, MGMT_OP_PIN_CODE_NEG_REPLY,
									ENODEV);

	hci_dev_lock_bh(hdev);

	if (!test_bit(HCI_UP, &hdev->flags)) {
		err = cmd_status(sk, index, MGMT_OP_PIN_CODE_NEG_REPLY,
								ENETDOWN);
		goto failed;
	}

<<<<<<< HEAD
	cmd = mgmt_pending_add(sk, MGMT_OP_PIN_CODE_NEG_REPLY, index,
								data, len);
	if (!cmd) {
=======
	rp_len = sizeof(*rp) + (i * sizeof(struct mgmt_addr_info));
	rp = kmalloc(rp_len, GFP_KERNEL);
	if (!rp) {
>>>>>>> common/android-3.10.y
		err = -ENOMEM;
		goto failed;
	}

<<<<<<< HEAD
	err = hci_send_cmd(hdev, HCI_OP_PIN_CODE_NEG_REPLY, sizeof(cp->bdaddr),
								&cp->bdaddr);
	if (err < 0)
		mgmt_pending_remove(cmd);

failed:
	hci_dev_unlock_bh(hdev);
	hci_dev_put(hdev);
=======
	i = 0;
	list_for_each_entry(c, &hdev->conn_hash.list, list) {
		if (!test_bit(HCI_CONN_MGMT_CONNECTED, &c->flags))
			continue;
		bacpy(&rp->addr[i].bdaddr, &c->dst);
		rp->addr[i].type = link_to_bdaddr(c->type, c->dst_type);
		if (c->type == SCO_LINK || c->type == ESCO_LINK)
			continue;
		i++;
	}

	rp->conn_count = cpu_to_le16(i);
>>>>>>> common/android-3.10.y

	return err;
}

static int le_add_dev_white_list(struct sock *sk, u16 index,
					unsigned char *data, u16 len)
{
	struct hci_dev *hdev;
	struct mgmt_cp_le_add_dev_white_list *cp;
	int err = 0;

	BT_DBG("");

	cp = (void *) data;

	if (len != sizeof(*cp))
		return cmd_status(sk, index, MGMT_OP_LE_ADD_DEV_WHITE_LIST,
									EINVAL);

	hdev = hci_dev_get(index);
	if (!hdev)
		return cmd_status(sk, index, MGMT_OP_LE_ADD_DEV_WHITE_LIST,
									ENODEV);

	hci_dev_lock_bh(hdev);

	if (!test_bit(HCI_UP, &hdev->flags)) {
		err = cmd_status(sk, index, MGMT_OP_LE_ADD_DEV_WHITE_LIST,
								ENETDOWN);
		goto failed;
	}

	hci_le_add_dev_white_list(hdev, &cp->bdaddr);

failed:
	hci_dev_unlock_bh(hdev);
	hci_dev_put(hdev);

	return err;
}

static int le_remove_dev_white_list(struct sock *sk, u16 index,
					unsigned char *data, u16 len)
{
	struct hci_dev *hdev;
	struct mgmt_cp_le_remove_dev_white_list *cp;
	int err = 0;

	BT_DBG("");

	cp = (void *) data;

	if (len != sizeof(*cp))
		return cmd_status(sk, index, MGMT_OP_LE_REMOVE_DEV_WHITE_LIST,
									EINVAL);

	hdev = hci_dev_get(index);
	if (!hdev)
		return cmd_status(sk, index, MGMT_OP_LE_REMOVE_DEV_WHITE_LIST,
									ENODEV);

	hci_dev_lock_bh(hdev);

	if (!test_bit(HCI_UP, &hdev->flags)) {
		err = cmd_status(sk, index, MGMT_OP_LE_REMOVE_DEV_WHITE_LIST,
								ENETDOWN);
		goto failed;
	}

	hci_le_remove_dev_white_list(hdev, &cp->bdaddr);

failed:
	hci_dev_unlock_bh(hdev);
	hci_dev_put(hdev);

	return err;
}

static int le_create_conn_white_list(struct sock *sk, u16 index)
{
	struct hci_dev *hdev;
	struct hci_conn *conn;
	u8 sec_level, auth_type;
	struct pending_cmd *cmd;
	bdaddr_t bdaddr;
	int err = 0;

	BT_DBG("");

	hdev = hci_dev_get(index);
	if (!hdev)
		return cmd_status(sk, index, MGMT_OP_LE_CREATE_CONN_WHITE_LIST,
									ENODEV);

	hci_dev_lock_bh(hdev);

	if (!test_bit(HCI_UP, &hdev->flags)) {
		err = cmd_status(sk, index, MGMT_OP_LE_CREATE_CONN_WHITE_LIST,
								ENETDOWN);
		goto failed;
	}

	cmd = mgmt_pending_add(sk, MGMT_OP_LE_CREATE_CONN_WHITE_LIST, index,
								NULL, 0);
	if (!cmd) {
		err = -ENOMEM;
		goto failed;
	}

	sec_level = BT_SECURITY_MEDIUM;
	auth_type = HCI_AT_GENERAL_BONDING;
	memset(&bdaddr, 0, sizeof(bdaddr));
	conn = hci_le_connect(hdev, 0, BDADDR_ANY, sec_level, auth_type, NULL);
	if (IS_ERR(conn)) {
		err = PTR_ERR(conn);
		mgmt_pending_remove(cmd);
	}

failed:
	hci_dev_unlock_bh(hdev);
	hci_dev_put(hdev);

	return err;
}

static int le_cancel_create_conn_white_list(struct sock *sk, u16 index)
{
	struct hci_dev *hdev;
	int err = 0;

	BT_DBG("");

	hdev = hci_dev_get(index);
	if (!hdev)
		return cmd_status(sk, index,
			MGMT_OP_LE_CANCEL_CREATE_CONN_WHITE_LIST, ENODEV);

	hci_dev_lock_bh(hdev);

	if (!test_bit(HCI_UP, &hdev->flags)) {
		err = cmd_status(sk, index,
			MGMT_OP_LE_CANCEL_CREATE_CONN_WHITE_LIST, ENETDOWN);
		goto failed;
	}

	hci_le_cancel_create_connect(hdev, BDADDR_ANY);

failed:
	hci_dev_unlock_bh(hdev);
	hci_dev_put(hdev);

	return err;
}

<<<<<<< HEAD
static int le_clear_white_list(struct sock *sk, u16 index)
{
	struct hci_dev *hdev;
	int err;

	BT_DBG("");

	hdev = hci_dev_get(index);
	if (!hdev)
		return cmd_status(sk, index,
			MGMT_OP_LE_CLEAR_WHITE_LIST, ENODEV);

	hci_dev_lock_bh(hdev);

	if (!test_bit(HCI_UP, &hdev->flags)) {
		err = cmd_status(sk, index,
			MGMT_OP_LE_CLEAR_WHITE_LIST, ENETDOWN);
		goto failed;
	}

	err = hci_send_cmd(hdev, HCI_OP_LE_CLEAR_WHITE_LIST, 0, NULL);

failed:
	hci_dev_unlock_bh(hdev);
	hci_dev_put(hdev);

	return err;
}

static int set_io_capability(struct sock *sk, u16 index, unsigned char *data,
									u16 len)
=======
static int set_io_capability(struct sock *sk, struct hci_dev *hdev, void *data,
			     u16 len)
>>>>>>> common/android-3.10.y
{
	struct hci_dev *hdev;
	struct mgmt_cp_set_io_capability *cp;

	BT_DBG("");

	cp = (void *) data;

	if (len != sizeof(*cp))
		return cmd_status(sk, index, MGMT_OP_SET_IO_CAPABILITY, EINVAL);

	hdev = hci_dev_get(index);
	if (!hdev)
		return cmd_status(sk, index, MGMT_OP_SET_IO_CAPABILITY, ENODEV);

	hci_dev_lock_bh(hdev);

	hdev->io_capability = cp->io_capability;

	BT_DBG("%s IO capability set to 0x%02x", hdev->name,
	       hdev->io_capability);

	hci_dev_unlock_bh(hdev);
	hci_dev_put(hdev);

	return cmd_complete(sk, index, MGMT_OP_SET_IO_CAPABILITY, NULL, 0);
}

static struct pending_cmd *find_pairing(struct hci_conn *conn)
{
	struct hci_dev *hdev = conn->hdev;
	struct list_head *p;

	list_for_each(p, &cmd_list) {
		struct pending_cmd *cmd;

		cmd = list_entry(p, struct pending_cmd, list);

		if (cmd->opcode != MGMT_OP_PAIR_DEVICE)
			continue;

		if (cmd->index != hdev->id)
			continue;

		if (cmd->user_data != conn)
			continue;

		return cmd;
	}

	return NULL;
}

static void pairing_complete(struct pending_cmd *cmd, u8 status)
{
	struct mgmt_rp_pair_device rp;
	struct hci_conn *conn = cmd->user_data;

<<<<<<< HEAD
	BT_DBG(" %u", status);

	bacpy(&rp.bdaddr, &conn->dst);
	rp.status = status;
=======
	bacpy(&rp.addr.bdaddr, &conn->dst);
	rp.addr.type = link_to_bdaddr(conn->type, conn->dst_type);
>>>>>>> common/android-3.10.y

	cmd_complete(cmd->sk, cmd->index, MGMT_OP_PAIR_DEVICE, &rp, sizeof(rp));

	/* So we don't get further callbacks for this connection */
	conn->connect_cfm_cb = NULL;
	conn->security_cfm_cb = NULL;
	conn->disconn_cfm_cb = NULL;

<<<<<<< HEAD
=======
	hci_conn_drop(conn);

>>>>>>> common/android-3.10.y
	mgmt_pending_remove(cmd);
}

static void pairing_complete_cb(struct hci_conn *conn, u8 status)
{
	struct pending_cmd *cmd;

	BT_DBG(" %u", status);

	cmd = find_pairing(conn);
	if (!cmd) {
		BT_DBG("Unable to find a pending command");
		return;
	}

	pairing_complete(cmd, status);
	hci_conn_put(conn);
}

static void pairing_security_complete_cb(struct hci_conn *conn, u8 status)
{
	struct pending_cmd *cmd;

	BT_DBG(" %u", status);

	cmd = find_pairing(conn);
	if (!cmd) {
		BT_DBG("Unable to find a pending command");
		return;
	}

	if (conn->type == LE_LINK)
		smp_link_encrypt_cmplt(conn->l2cap_data, status,
				status ? 0 : 1);
	else
		pairing_complete(cmd, status);
}

<<<<<<< HEAD
static void pairing_connect_complete_cb(struct hci_conn *conn, u8 status)
=======
static void le_connect_complete_cb(struct hci_conn *conn, u8 status)
{
	struct pending_cmd *cmd;

	BT_DBG("status %u", status);

	if (!status)
		return;

	cmd = find_pairing(conn);
	if (!cmd)
		BT_DBG("Unable to find a pending command");
	else
		pairing_complete(cmd, mgmt_status(status));
}

static int pair_device(struct sock *sk, struct hci_dev *hdev, void *data,
		       u16 len)
>>>>>>> common/android-3.10.y
{
	struct pending_cmd *cmd;

	BT_DBG("conn: %p %u", conn, status);

	cmd = find_pairing(conn);
	if (!cmd) {
		BT_DBG("Unable to find a pending command");
		return;
	}

	if (status || conn->pending_sec_level < BT_SECURITY_MEDIUM)
		pairing_complete(cmd, status);

	hci_conn_put(conn);
}

static void discovery_terminated(struct pending_cmd *cmd, void *data)
{
	struct hci_dev *hdev;
	struct mgmt_mode ev = {0};

	BT_DBG("");
	hdev = hci_dev_get(cmd->index);
	if (!hdev)
		goto not_found;

	del_timer(&hdev->disco_le_timer);
	del_timer(&hdev->disco_timer);
	hci_dev_put(hdev);

not_found:
	mgmt_event(MGMT_EV_DISCOVERING, cmd->index, &ev, sizeof(ev), NULL);

	list_del(&cmd->list);

	mgmt_pending_free(cmd);
}

static int pair_device(struct sock *sk, u16 index, unsigned char *data, u16 len)
{
	struct hci_dev *hdev;
	struct mgmt_cp_pair_device *cp;
	struct pending_cmd *cmd;
	u8 sec_level, auth_type, io_cap;
	struct hci_conn *conn;
	struct adv_entry *entry;
	int err;

	BT_DBG("");

<<<<<<< HEAD
	cp = (void *) data;

	if (len != sizeof(*cp))
		return cmd_status(sk, index, MGMT_OP_PAIR_DEVICE, EINVAL);
=======
	memset(&rp, 0, sizeof(rp));
	bacpy(&rp.addr.bdaddr, &cp->addr.bdaddr);
	rp.addr.type = cp->addr.type;

	if (!bdaddr_type_is_valid(cp->addr.type))
		return cmd_complete(sk, hdev->id, MGMT_OP_PAIR_DEVICE,
				    MGMT_STATUS_INVALID_PARAMS,
				    &rp, sizeof(rp));

	hci_dev_lock(hdev);

	if (!hdev_is_powered(hdev)) {
		err = cmd_complete(sk, hdev->id, MGMT_OP_PAIR_DEVICE,
				   MGMT_STATUS_NOT_POWERED, &rp, sizeof(rp));
		goto unlock;
	}
>>>>>>> common/android-3.10.y

	hdev = hci_dev_get(index);

<<<<<<< HEAD
	if (!hdev)
		return cmd_status(sk, index, MGMT_OP_PAIR_DEVICE, ENODEV);

	hci_dev_lock_bh(hdev);

	io_cap = cp->io_cap;

	sec_level = BT_SECURITY_MEDIUM;
	auth_type = HCI_AT_DEDICATED_BONDING;

	entry = hci_find_adv_entry(hdev, &cp->bdaddr);
	if (entry && entry->flags & 0x04) {
		conn = hci_le_connect(hdev, 0, &cp->bdaddr, sec_level,
							auth_type, NULL);
	} else {
		/* ACL-SSP does not support io_cap 0x04 (KeyboadDisplay) */
		if (io_cap == 0x04)
			io_cap = 0x01;
		conn = hci_connect(hdev, ACL_LINK, 0, &cp->bdaddr, sec_level,
								auth_type);
		conn->auth_initiator = 1;
	}

	if (IS_ERR(conn)) {
		err = PTR_ERR(conn);
=======
	if (cp->addr.type == BDADDR_BREDR)
		conn = hci_connect(hdev, ACL_LINK, 0, &cp->addr.bdaddr,
				   cp->addr.type, sec_level, auth_type);
	else
		conn = hci_connect(hdev, LE_LINK, 0, &cp->addr.bdaddr,
				   cp->addr.type, sec_level, auth_type);

	if (IS_ERR(conn)) {
		int status;

		if (PTR_ERR(conn) == -EBUSY)
			status = MGMT_STATUS_BUSY;
		else
			status = MGMT_STATUS_CONNECT_FAILED;

		err = cmd_complete(sk, hdev->id, MGMT_OP_PAIR_DEVICE,
				   status, &rp,
				   sizeof(rp));
>>>>>>> common/android-3.10.y
		goto unlock;
	}

	if (conn->connect_cfm_cb) {
<<<<<<< HEAD
		hci_conn_put(conn);
		err = cmd_status(sk, index, MGMT_OP_PAIR_DEVICE, EBUSY);
=======
		hci_conn_drop(conn);
		err = cmd_complete(sk, hdev->id, MGMT_OP_PAIR_DEVICE,
				   MGMT_STATUS_BUSY, &rp, sizeof(rp));
>>>>>>> common/android-3.10.y
		goto unlock;
	}

	cmd = mgmt_pending_add(sk, MGMT_OP_PAIR_DEVICE, index, data, len);
	if (!cmd) {
		err = -ENOMEM;
		hci_conn_drop(conn);
		goto unlock;
	}

<<<<<<< HEAD
	conn->connect_cfm_cb = pairing_connect_complete_cb;
	conn->security_cfm_cb = pairing_security_complete_cb;
=======
	/* For LE, just connecting isn't a proof that the pairing finished */
	if (cp->addr.type == BDADDR_BREDR)
		conn->connect_cfm_cb = pairing_complete_cb;
	else
		conn->connect_cfm_cb = le_connect_complete_cb;

	conn->security_cfm_cb = pairing_complete_cb;
>>>>>>> common/android-3.10.y
	conn->disconn_cfm_cb = pairing_complete_cb;
	conn->io_capability = io_cap;
	cmd->user_data = conn;

	if (conn->state == BT_CONNECTED &&
	    hci_conn_security(conn, sec_level, auth_type))
		pairing_complete(cmd, 0);

	err = 0;

unlock:
	hci_dev_unlock_bh(hdev);
	hci_dev_put(hdev);

	return err;
}

static int user_confirm_reply(struct sock *sk, u16 index, unsigned char *data,
							u16 len, u16 opcode)
{
	struct mgmt_cp_user_confirm_reply *cp = (void *) data;
	u16 mgmt_op = opcode, hci_op;
	struct pending_cmd *cmd;
	struct hci_dev *hdev;
	struct hci_conn *le_conn;
	int err;

	BT_DBG("%d", mgmt_op);

	if (mgmt_op == MGMT_OP_USER_CONFIRM_NEG_REPLY)
		hci_op = HCI_OP_USER_CONFIRM_NEG_REPLY;
	else
		hci_op = HCI_OP_USER_CONFIRM_REPLY;

	if (len < sizeof(*cp))
		return cmd_status(sk, index, mgmt_op, EINVAL);

	hdev = hci_dev_get(index);
	if (!hdev)
		return cmd_status(sk, index, mgmt_op, ENODEV);

	hci_dev_lock_bh(hdev);

	if (!test_bit(HCI_UP, &hdev->flags)) {
		err = cmd_status(sk, index, mgmt_op, ENETDOWN);
		goto done;
	}

	le_conn = hci_conn_hash_lookup_ba(hdev, LE_LINK, &cp->bdaddr);
	if (le_conn) {
		err = le_user_confirm_reply(le_conn, mgmt_op, (void *) cp);
		goto done;
	}
	BT_DBG("BR/EDR: %s", mgmt_op == MGMT_OP_USER_CONFIRM_NEG_REPLY ?
							"Reject" : "Accept");

	cmd = mgmt_pending_add(sk, mgmt_op, index, data, len);
	if (!cmd) {
		err = -ENOMEM;
		goto done;
	}

	err = hci_send_cmd(hdev, hci_op, sizeof(cp->bdaddr), &cp->bdaddr);
	if (err < 0)
		mgmt_pending_remove(cmd);

done:
	hci_dev_unlock_bh(hdev);
	hci_dev_put(hdev);

	return err;
}

<<<<<<< HEAD
static int resolve_name(struct sock *sk, u16 index, unsigned char *data,
								u16 len)
=======
static int user_pairing_resp(struct sock *sk, struct hci_dev *hdev,
			     struct mgmt_addr_info *addr, u16 mgmt_op,
			     u16 hci_op, __le32 passkey)
>>>>>>> common/android-3.10.y
{
	struct mgmt_cp_resolve_name *mgmt_cp = (void *) data;
	struct hci_cp_remote_name_req hci_cp;
	struct hci_dev *hdev;
	struct pending_cmd *cmd;
	int err;

<<<<<<< HEAD
	BT_DBG("");

	if (len != sizeof(*mgmt_cp))
		return cmd_status(sk, index, MGMT_OP_RESOLVE_NAME, EINVAL);

	hdev = hci_dev_get(index);
	if (!hdev)
		return cmd_status(sk, index, MGMT_OP_RESOLVE_NAME, ENODEV);
=======
	hci_dev_lock(hdev);

	if (!hdev_is_powered(hdev)) {
		err = cmd_complete(sk, hdev->id, mgmt_op,
				   MGMT_STATUS_NOT_POWERED, addr,
				   sizeof(*addr));
		goto done;
	}

	if (addr->type == BDADDR_BREDR)
		conn = hci_conn_hash_lookup_ba(hdev, ACL_LINK, &addr->bdaddr);
	else
		conn = hci_conn_hash_lookup_ba(hdev, LE_LINK, &addr->bdaddr);

	if (!conn) {
		err = cmd_complete(sk, hdev->id, mgmt_op,
				   MGMT_STATUS_NOT_CONNECTED, addr,
				   sizeof(*addr));
		goto done;
	}

	if (addr->type == BDADDR_LE_PUBLIC || addr->type == BDADDR_LE_RANDOM) {
		/* Continue with pairing via SMP. The hdev lock must be
		 * released as SMP may try to recquire it for crypto
		 * purposes.
		 */
		hci_dev_unlock(hdev);
		err = smp_user_confirm_reply(conn, mgmt_op, passkey);
		hci_dev_lock(hdev);

		if (!err)
			err = cmd_complete(sk, hdev->id, mgmt_op,
					   MGMT_STATUS_SUCCESS, addr,
					   sizeof(*addr));
		else
			err = cmd_complete(sk, hdev->id, mgmt_op,
					   MGMT_STATUS_FAILED, addr,
					   sizeof(*addr));
>>>>>>> common/android-3.10.y

	hci_dev_lock_bh(hdev);

<<<<<<< HEAD
	cmd = mgmt_pending_add(sk, MGMT_OP_RESOLVE_NAME, index, data, len);
=======
	cmd = mgmt_pending_add(sk, mgmt_op, hdev, addr, sizeof(*addr));
>>>>>>> common/android-3.10.y
	if (!cmd) {
		err = -ENOMEM;
		goto failed;
	}

<<<<<<< HEAD
	memset(&hci_cp, 0, sizeof(hci_cp));
	bacpy(&hci_cp.bdaddr, &mgmt_cp->bdaddr);
	err = hci_send_cmd(hdev, HCI_OP_REMOTE_NAME_REQ, sizeof(hci_cp),
								&hci_cp);
=======
	/* Continue with pairing via HCI */
	if (hci_op == HCI_OP_USER_PASSKEY_REPLY) {
		struct hci_cp_user_passkey_reply cp;

		bacpy(&cp.bdaddr, &addr->bdaddr);
		cp.passkey = passkey;
		err = hci_send_cmd(hdev, hci_op, sizeof(cp), &cp);
	} else
		err = hci_send_cmd(hdev, hci_op, sizeof(addr->bdaddr),
				   &addr->bdaddr);

>>>>>>> common/android-3.10.y
	if (err < 0)
		mgmt_pending_remove(cmd);

failed:
	hci_dev_unlock_bh(hdev);
	hci_dev_put(hdev);

	return err;
}

<<<<<<< HEAD
static int cancel_resolve_name(struct sock *sk, u16 index, unsigned char *data,
								u16 len)
=======
static int pin_code_neg_reply(struct sock *sk, struct hci_dev *hdev,
			      void *data, u16 len)
{
	struct mgmt_cp_pin_code_neg_reply *cp = data;

	BT_DBG("");

	return user_pairing_resp(sk, hdev, &cp->addr,
				MGMT_OP_PIN_CODE_NEG_REPLY,
				HCI_OP_PIN_CODE_NEG_REPLY, 0);
}

static int user_confirm_reply(struct sock *sk, struct hci_dev *hdev, void *data,
			      u16 len)
>>>>>>> common/android-3.10.y
{
	struct mgmt_cp_cancel_resolve_name *mgmt_cp = (void *) data;
	struct hci_cp_remote_name_req_cancel hci_cp;
	struct hci_dev *hdev;
	int err;

	BT_DBG("");

	if (len != sizeof(*mgmt_cp))
		return cmd_status(sk, index, MGMT_OP_CANCEL_RESOLVE_NAME,
								EINVAL);

	hdev = hci_dev_get(index);
	if (!hdev)
		return cmd_status(sk, index, MGMT_OP_CANCEL_RESOLVE_NAME,
								ENODEV);

	hci_dev_lock_bh(hdev);

	memset(&hci_cp, 0, sizeof(hci_cp));
	bacpy(&hci_cp.bdaddr, &mgmt_cp->bdaddr);
	err = hci_send_cmd(hdev, HCI_OP_REMOTE_NAME_REQ_CANCEL, sizeof(hci_cp),
								&hci_cp);

<<<<<<< HEAD
	hci_dev_unlock_bh(hdev);
	hci_dev_put(hdev);

	return err;
=======
	return user_pairing_resp(sk, hdev, &cp->addr,
				 MGMT_OP_USER_CONFIRM_REPLY,
				 HCI_OP_USER_CONFIRM_REPLY, 0);
>>>>>>> common/android-3.10.y
}

static int set_connection_params(struct sock *sk, u16 index,
				unsigned char *data, u16 len)
{
	struct mgmt_cp_set_connection_params *cp = (void *) data;
	struct hci_dev *hdev;
	struct hci_conn *conn;
	int err;

	BT_DBG("");

<<<<<<< HEAD
	if (len != sizeof(*cp))
		return cmd_status(sk, index, MGMT_OP_SET_CONNECTION_PARAMS,
									EINVAL);

	hdev = hci_dev_get(index);
	if (!hdev)
		return cmd_status(sk, index, MGMT_OP_SET_CONNECTION_PARAMS,
									ENODEV);

	hci_dev_lock_bh(hdev);

	conn = hci_conn_hash_lookup_ba(hdev, LE_LINK, &cp->bdaddr);
	if (!conn) {
		err = cmd_status(sk, index, MGMT_OP_SET_CONNECTION_PARAMS,
								ENOTCONN);
		goto failed;
	}

	hci_le_conn_update(conn, le16_to_cpu(cp->interval_min),
				le16_to_cpu(cp->interval_max),
				le16_to_cpu(cp->slave_latency),
				le16_to_cpu(cp->timeout_multiplier));

	err = cmd_status(sk, index, MGMT_OP_SET_CONNECTION_PARAMS, 0);

failed:
	hci_dev_unlock_bh(hdev);
	hci_dev_put(hdev);

	return err;
=======
	return user_pairing_resp(sk, hdev, &cp->addr,
				 MGMT_OP_USER_CONFIRM_NEG_REPLY,
				 HCI_OP_USER_CONFIRM_NEG_REPLY, 0);
>>>>>>> common/android-3.10.y
}

static int set_rssi_reporter(struct sock *sk, u16 index,
				unsigned char *data, u16 len)
{
	struct mgmt_cp_set_rssi_reporter *cp = (void *) data;
	struct hci_dev *hdev;
	struct hci_conn *conn;
	int err = 0;

	if (len != sizeof(*cp))
		return cmd_status(sk, index, MGMT_OP_SET_RSSI_REPORTER,
								EINVAL);

	hdev = hci_dev_get(index);
	if (!hdev)
		return cmd_status(sk, index, MGMT_OP_SET_RSSI_REPORTER,
							ENODEV);

<<<<<<< HEAD
	hci_dev_lock_bh(hdev);

	conn = hci_conn_hash_lookup_ba(hdev, LE_LINK, &cp->bdaddr);

	if (!conn) {
		err = cmd_status(sk, index, MGMT_OP_SET_RSSI_REPORTER,
						ENOTCONN);
		goto failed;
	}

	BT_DBG("updateOnThreshExceed %d ", cp->updateOnThreshExceed);
	hci_conn_set_rssi_reporter(conn, cp->rssi_threshold,
			__le16_to_cpu(cp->interval), cp->updateOnThreshExceed);

failed:
	hci_dev_unlock_bh(hdev);
	hci_dev_put(hdev);

	return err;
=======
	return user_pairing_resp(sk, hdev, &cp->addr,
				 MGMT_OP_USER_PASSKEY_REPLY,
				 HCI_OP_USER_PASSKEY_REPLY, cp->passkey);
>>>>>>> common/android-3.10.y
}

static int unset_rssi_reporter(struct sock *sk, u16 index,
			unsigned char *data, u16 len)
{
	struct mgmt_cp_unset_rssi_reporter *cp = (void *) data;
	struct hci_dev *hdev;
	struct hci_conn *conn;
	int err = 0;

	if (len != sizeof(*cp))
		return cmd_status(sk, index, MGMT_OP_UNSET_RSSI_REPORTER,
					EINVAL);

	hdev = hci_dev_get(index);

<<<<<<< HEAD
	if (!hdev)
		return cmd_status(sk, index, MGMT_OP_UNSET_RSSI_REPORTER,
					ENODEV);

	hci_dev_lock_bh(hdev);

	conn = hci_conn_hash_lookup_ba(hdev, LE_LINK, &cp->bdaddr);

	if (!conn) {
		err = cmd_status(sk, index, MGMT_OP_UNSET_RSSI_REPORTER,
					ENOTCONN);
		goto failed;
	}

	hci_conn_unset_rssi_reporter(conn);

failed:
	hci_dev_unlock_bh(hdev);
	hci_dev_put(hdev);

	return err;
}

static int le_cancel_create_conn(struct sock *sk, u16 index,
	unsigned char *data, u16 len)
{
	struct mgmt_cp_le_cancel_create_conn *cp = (void *) data;
	struct hci_dev *hdev;
	int err = 0;

	if (len != sizeof(*cp))
		return cmd_status(sk, index, MGMT_OP_LE_CANCEL_CREATE_CONN,
							EINVAL);

	hdev = hci_dev_get(index);

	if (!hdev)
		return cmd_status(sk, index, MGMT_OP_LE_CANCEL_CREATE_CONN,
							ENODEV);

	hci_dev_lock_bh(hdev);

	if (!test_bit(HCI_UP, &hdev->flags)) {
		err = cmd_status(sk, index, MGMT_OP_LE_CANCEL_CREATE_CONN,
						ENETDOWN);
		goto failed;
	}

	hci_le_cancel_create_connect(hdev, &cp->bdaddr);

failed:
	hci_dev_unlock_bh(hdev);
	hci_dev_put(hdev);

return err;
=======
	return user_pairing_resp(sk, hdev, &cp->addr,
				 MGMT_OP_USER_PASSKEY_NEG_REPLY,
				 HCI_OP_USER_PASSKEY_NEG_REPLY, 0);
}

static void update_name(struct hci_request *req)
{
	struct hci_dev *hdev = req->hdev;
	struct hci_cp_write_local_name cp;

	memcpy(cp.name, hdev->dev_name, sizeof(cp.name));

	hci_req_add(req, HCI_OP_WRITE_LOCAL_NAME, sizeof(cp), &cp);
}

static void set_name_complete(struct hci_dev *hdev, u8 status)
{
	struct mgmt_cp_set_local_name *cp;
	struct pending_cmd *cmd;

	BT_DBG("status 0x%02x", status);

	hci_dev_lock(hdev);

	cmd = mgmt_pending_find(MGMT_OP_SET_LOCAL_NAME, hdev);
	if (!cmd)
		goto unlock;

	cp = cmd->param;

	if (status)
		cmd_status(cmd->sk, hdev->id, MGMT_OP_SET_LOCAL_NAME,
			   mgmt_status(status));
	else
		cmd_complete(cmd->sk, hdev->id, MGMT_OP_SET_LOCAL_NAME, 0,
			     cp, sizeof(*cp));

	mgmt_pending_remove(cmd);

unlock:
	hci_dev_unlock(hdev);
>>>>>>> common/android-3.10.y
}

static int set_local_name(struct sock *sk, u16 index, unsigned char *data,
								u16 len)
{
	struct mgmt_cp_set_local_name *mgmt_cp = (void *) data;
	struct hci_cp_write_local_name hci_cp;
	struct hci_dev *hdev;
	struct pending_cmd *cmd;
	struct hci_request req;
	int err;

	BT_DBG("");

<<<<<<< HEAD
	if (len != sizeof(*mgmt_cp))
		return cmd_status(sk, index, MGMT_OP_SET_LOCAL_NAME, EINVAL);
=======
	hci_dev_lock(hdev);

	/* If the old values are the same as the new ones just return a
	 * direct command complete event.
	 */
	if (!memcmp(hdev->dev_name, cp->name, sizeof(hdev->dev_name)) &&
	    !memcmp(hdev->short_name, cp->short_name,
		    sizeof(hdev->short_name))) {
		err = cmd_complete(sk, hdev->id, MGMT_OP_SET_LOCAL_NAME, 0,
				   data, len);
		goto failed;
	}

	memcpy(hdev->short_name, cp->short_name, sizeof(hdev->short_name));

	if (!hdev_is_powered(hdev)) {
		memcpy(hdev->dev_name, cp->name, sizeof(hdev->dev_name));

		err = cmd_complete(sk, hdev->id, MGMT_OP_SET_LOCAL_NAME, 0,
				   data, len);
		if (err < 0)
			goto failed;

		err = mgmt_event(MGMT_EV_LOCAL_NAME_CHANGED, hdev, data, len,
				 sk);
>>>>>>> common/android-3.10.y

	hdev = hci_dev_get(index);
	if (!hdev)
		return cmd_status(sk, index, MGMT_OP_SET_LOCAL_NAME, ENODEV);

	hci_dev_lock_bh(hdev);

	cmd = mgmt_pending_add(sk, MGMT_OP_SET_LOCAL_NAME, index, data, len);
	if (!cmd) {
		err = -ENOMEM;
		goto failed;
	}

<<<<<<< HEAD
	memcpy(hci_cp.name, mgmt_cp->name, sizeof(hci_cp.name));
	err = hci_send_cmd(hdev, HCI_OP_WRITE_LOCAL_NAME, sizeof(hci_cp),
								&hci_cp);
=======
	memcpy(hdev->dev_name, cp->name, sizeof(hdev->dev_name));

	hci_req_init(&req, hdev);

	if (lmp_bredr_capable(hdev)) {
		update_name(&req);
		update_eir(&req);
	}

	if (lmp_le_capable(hdev))
		hci_update_ad(&req);

	err = hci_req_run(&req, set_name_complete);
>>>>>>> common/android-3.10.y
	if (err < 0)
		mgmt_pending_remove(cmd);

failed:
	hci_dev_unlock_bh(hdev);
	hci_dev_put(hdev);

	return err;
}

static void discovery_rsp(struct pending_cmd *cmd, void *data)
{
	struct mgmt_mode ev;

<<<<<<< HEAD
	BT_DBG("");
	if (cmd->opcode == MGMT_OP_START_DISCOVERY) {
		ev.val = 1;
		cmd_status(cmd->sk, cmd->index, MGMT_OP_START_DISCOVERY, 0);
	} else {
		ev.val = 0;
		cmd_complete(cmd->sk, cmd->index, MGMT_OP_STOP_DISCOVERY,
								NULL, 0);
		if (cmd->opcode == MGMT_OP_STOP_DISCOVERY) {
			struct hci_dev *hdev = hci_dev_get(cmd->index);
			if (hdev) {
				del_timer(&hdev->disco_le_timer);
				del_timer(&hdev->disco_timer);
				hci_dev_put(hdev);
			}
		}
=======
	hci_dev_lock(hdev);

	if (!hdev_is_powered(hdev)) {
		err = cmd_status(sk, hdev->id, MGMT_OP_READ_LOCAL_OOB_DATA,
				 MGMT_STATUS_NOT_POWERED);
		goto unlock;
	}

	if (!lmp_ssp_capable(hdev)) {
		err = cmd_status(sk, hdev->id, MGMT_OP_READ_LOCAL_OOB_DATA,
				 MGMT_STATUS_NOT_SUPPORTED);
		goto unlock;
>>>>>>> common/android-3.10.y
	}

	mgmt_event(MGMT_EV_DISCOVERING, cmd->index, &ev, sizeof(ev), NULL);

	list_del(&cmd->list);

	mgmt_pending_free(cmd);
}

void mgmt_inquiry_started(u16 index)
{
	BT_DBG("");
	mgmt_pending_foreach(MGMT_OP_START_DISCOVERY, index,
						discovery_rsp, NULL);
}

void mgmt_inquiry_complete_evt(u16 index, u8 status)
{
	struct hci_dev *hdev;
	struct hci_cp_le_set_scan_enable le_cp = {1, 0};
	struct mgmt_mode cp = {0};
	int err = -1;

	hdev = hci_dev_get(index);

	if (hdev)
		BT_DBG("disco_state: %d", hdev->disco_state);

	if (!hdev || !lmp_le_capable(hdev)) {

<<<<<<< HEAD
		mgmt_pending_foreach(MGMT_OP_STOP_DISCOVERY, index,
						discovery_terminated, NULL);

		mgmt_event(MGMT_EV_DISCOVERING, index, &cp, sizeof(cp), NULL);

		hdev->disco_state = SCAN_IDLE;

		if (hdev)
			goto done;
		else
			return;
	}

	if (hdev->disco_state != SCAN_IDLE) {
		err = hci_send_cmd(hdev, HCI_OP_LE_SET_SCAN_ENABLE,
						sizeof(le_cp), &le_cp);
		if (err >= 0) {
			mod_timer(&hdev->disco_le_timer, jiffies +
				msecs_to_jiffies(hdev->disco_int_phase * 1000));
			hdev->disco_state = SCAN_LE;
		} else
			hdev->disco_state = SCAN_IDLE;
	}
=======
	err = hci_add_remote_oob_data(hdev, &cp->addr.bdaddr, cp->hash,
				      cp->randomizer);
	if (err < 0)
		status = MGMT_STATUS_FAILED;
	else
		status = MGMT_STATUS_SUCCESS;
>>>>>>> common/android-3.10.y

	if (hdev->disco_state == SCAN_IDLE)
		mgmt_event(MGMT_EV_DISCOVERING, index, &cp, sizeof(cp), NULL);

<<<<<<< HEAD
	if (err < 0)
		mgmt_pending_foreach(MGMT_OP_STOP_DISCOVERY, index,
						discovery_terminated, NULL);

done:
	hci_dev_put(hdev);
}

void mgmt_disco_timeout(unsigned long data)
=======
	hci_dev_unlock(hdev);
	return err;
}

static int remove_remote_oob_data(struct sock *sk, struct hci_dev *hdev,
				  void *data, u16 len)
>>>>>>> common/android-3.10.y
{
	struct hci_dev *hdev = (void *) data;
	struct pending_cmd *cmd;
	struct mgmt_mode cp = {0};

	BT_DBG("hci%d", hdev->id);

	hdev = hci_dev_get(hdev->id);

<<<<<<< HEAD
	if (!hdev)
		return;

	hci_dev_lock_bh(hdev);
	del_timer(&hdev->disco_le_timer);

	if (hdev->disco_state != SCAN_IDLE) {
		struct hci_cp_le_set_scan_enable le_cp = {0, 0};

		if (test_bit(HCI_UP, &hdev->flags)) {
			if (hdev->disco_state == SCAN_LE)
				hci_send_cmd(hdev, HCI_OP_LE_SET_SCAN_ENABLE,
							sizeof(le_cp), &le_cp);
			else
				hci_send_cmd(hdev, HCI_OP_INQUIRY_CANCEL, 0,
									 NULL);
		}
		hdev->disco_state = SCAN_IDLE;
	}

	mgmt_event(MGMT_EV_DISCOVERING, hdev->id, &cp, sizeof(cp), NULL);
=======
	err = hci_remove_remote_oob_data(hdev, &cp->addr.bdaddr);
	if (err < 0)
		status = MGMT_STATUS_INVALID_PARAMS;
	else
		status = MGMT_STATUS_SUCCESS;
>>>>>>> common/android-3.10.y

	cmd = mgmt_pending_find(MGMT_OP_STOP_DISCOVERY, hdev->id);
	if (cmd)
		mgmt_pending_remove(cmd);

<<<<<<< HEAD
	hci_dev_unlock_bh(hdev);
	hci_dev_put(hdev);
=======
	hci_dev_unlock(hdev);
	return err;
>>>>>>> common/android-3.10.y
}

void mgmt_disco_le_timeout(unsigned long data)
{
	struct hci_dev *hdev = (void *)data;
	struct hci_cp_le_set_scan_enable le_cp = {0, 0};

	BT_DBG("hci%d", hdev->id);

	hdev = hci_dev_get(hdev->id);

	if (!hdev)
		return;

	hci_dev_lock_bh(hdev);

	if (test_bit(HCI_UP, &hdev->flags)) {
		if (hdev->disco_state == SCAN_LE)
			hci_send_cmd(hdev, HCI_OP_LE_SET_SCAN_ENABLE,
					sizeof(le_cp), &le_cp);

	/* re-start BR scan */
		if (hdev->disco_state != SCAN_IDLE) {
			struct hci_cp_inquiry cp = {{0x33, 0x8b, 0x9e}, 4, 0};
			hdev->disco_int_phase *= 2;
			hdev->disco_int_count = 0;
			cp.num_rsp = (u8) hdev->disco_int_phase;
			hci_send_cmd(hdev, HCI_OP_INQUIRY, sizeof(cp), &cp);
			hdev->disco_state = SCAN_BR;
		}
	}

	hci_dev_unlock_bh(hdev);
	hci_dev_put(hdev);
}

static int start_discovery(struct sock *sk, u16 index)
{
	struct hci_cp_inquiry cp = {{0x33, 0x8b, 0x9e}, 8, 0};
	struct hci_dev *hdev;
	struct pending_cmd *cmd;
	int err;

	BT_DBG("");

	hdev = hci_dev_get(index);
	if (!hdev)
		return cmd_status(sk, index, MGMT_OP_START_DISCOVERY, ENODEV);

	BT_DBG("disco_state: %d", hdev->disco_state);
	hci_dev_lock_bh(hdev);

<<<<<<< HEAD
	if (hdev->disco_state && timer_pending(&hdev->disco_timer)) {
		err = -EBUSY;
=======
	if (test_bit(HCI_PERIODIC_INQ, &hdev->dev_flags)) {
		err = cmd_status(sk, hdev->id, MGMT_OP_START_DISCOVERY,
				 MGMT_STATUS_BUSY);
		goto failed;
	}

	if (hdev->discovery.state != DISCOVERY_STOPPED) {
		err = cmd_status(sk, hdev->id, MGMT_OP_START_DISCOVERY,
				 MGMT_STATUS_BUSY);
>>>>>>> common/android-3.10.y
		goto failed;
	}

	cmd = mgmt_pending_add(sk, MGMT_OP_START_DISCOVERY, index, NULL, 0);
	if (!cmd) {
		err = -ENOMEM;
		goto failed;
	}

	/* If LE Capable, we will alternate between BR/EDR and LE */
	if (lmp_le_capable(hdev)) {
		struct hci_cp_le_set_scan_parameters le_cp;

<<<<<<< HEAD
		/* Shorten BR scan params */
		cp.num_rsp = 1;
		cp.length /= 2;

		/* Setup LE scan params */
		memset(&le_cp, 0, sizeof(le_cp));
		le_cp.type = 0x01;		/* Active scanning */
		/* The recommended value for scan interval and window is
		 * 11.25 msec. It is calculated by: time = n * 0.625 msec */
		le_cp.interval = cpu_to_le16(0x0012);
		le_cp.window = cpu_to_le16(0x0012);
		le_cp.own_bdaddr_type = 0;	/* Public address */
		le_cp.filter = 0;		/* Accept all adv packets */

		hci_send_cmd(hdev, HCI_OP_LE_SET_SCAN_PARAMETERS,
						sizeof(le_cp), &le_cp);
=======
	switch (hdev->discovery.type) {
	case DISCOV_TYPE_BREDR:
		if (!lmp_bredr_capable(hdev)) {
			err = cmd_status(sk, hdev->id, MGMT_OP_START_DISCOVERY,
					 MGMT_STATUS_NOT_SUPPORTED);
			mgmt_pending_remove(cmd);
			goto failed;
		}

		err = hci_do_inquiry(hdev, INQUIRY_LEN_BREDR);
		break;

	case DISCOV_TYPE_LE:
		if (!test_bit(HCI_LE_ENABLED, &hdev->dev_flags)) {
			err = cmd_status(sk, hdev->id, MGMT_OP_START_DISCOVERY,
					 MGMT_STATUS_NOT_SUPPORTED);
			mgmt_pending_remove(cmd);
			goto failed;
		}

		err = hci_le_scan(hdev, LE_SCAN_ACTIVE, LE_SCAN_INT,
				  LE_SCAN_WIN, LE_SCAN_TIMEOUT_LE_ONLY);
		break;

	case DISCOV_TYPE_INTERLEAVED:
		if (!lmp_host_le_capable(hdev) || !lmp_bredr_capable(hdev)) {
			err = cmd_status(sk, hdev->id, MGMT_OP_START_DISCOVERY,
					 MGMT_STATUS_NOT_SUPPORTED);
			mgmt_pending_remove(cmd);
			goto failed;
		}

		err = hci_le_scan(hdev, LE_SCAN_ACTIVE, LE_SCAN_INT,
				  LE_SCAN_WIN, LE_SCAN_TIMEOUT_BREDR_LE);
		break;

	default:
		err = cmd_status(sk, hdev->id, MGMT_OP_START_DISCOVERY,
				 MGMT_STATUS_INVALID_PARAMS);
		mgmt_pending_remove(cmd);
		goto failed;
>>>>>>> common/android-3.10.y
	}

	err = hci_send_cmd(hdev, HCI_OP_INQUIRY, sizeof(cp), &cp);

	if (err < 0) {
		mgmt_pending_remove(cmd);
		hdev->disco_state = SCAN_IDLE;
	} else if (lmp_le_capable(hdev)) {
		cmd = mgmt_pending_find(MGMT_OP_STOP_DISCOVERY, index);
		if (!cmd)
			mgmt_pending_add(sk, MGMT_OP_STOP_DISCOVERY, index,
								NULL, 0);
		hdev->disco_int_phase = 1;
		hdev->disco_int_count = 0;
		hdev->disco_state = SCAN_BR;
		del_timer(&hdev->disco_le_timer);
		del_timer(&hdev->disco_timer);
		mod_timer(&hdev->disco_timer,
				jiffies + msecs_to_jiffies(20000));
	} else
		hdev->disco_state = SCAN_BR;

failed:
	hci_dev_unlock_bh(hdev);
	hci_dev_put(hdev);

	if (err < 0)
		return cmd_status(sk, index, MGMT_OP_START_DISCOVERY, -err);

	return err;
}

static int stop_discovery(struct sock *sk, u16 index)
{
	struct hci_cp_le_set_scan_enable le_cp = {0, 0};
	struct mgmt_mode mode_cp = {0};
	struct hci_dev *hdev;
	struct pending_cmd *cmd = NULL;
	int err = -EPERM;
	u8 state;

	BT_DBG("");

	hdev = hci_dev_get(index);
	if (!hdev)
		return cmd_status(sk, index, MGMT_OP_STOP_DISCOVERY, ENODEV);

	BT_DBG("disco_state: %d", hdev->disco_state);

	hci_dev_lock_bh(hdev);

	state = hdev->disco_state;
	hdev->disco_state = SCAN_IDLE;
	del_timer(&hdev->disco_le_timer);
	del_timer(&hdev->disco_timer);

<<<<<<< HEAD
	if (state == SCAN_LE) {
		err = hci_send_cmd(hdev, HCI_OP_LE_SET_SCAN_ENABLE,
							sizeof(le_cp), &le_cp);
		if (err >= 0) {
			mgmt_pending_foreach(MGMT_OP_STOP_DISCOVERY, index,
						discovery_terminated, NULL);

			err = cmd_complete(sk, index, MGMT_OP_STOP_DISCOVERY,
								NULL, 0);
		}
	} else if (state == SCAN_BR)
		err = hci_send_cmd(hdev, HCI_OP_INQUIRY_CANCEL, 0, NULL);

	cmd = mgmt_pending_find(MGMT_OP_STOP_DISCOVERY, index);
	if (err < 0 && cmd)
		mgmt_pending_remove(cmd);

	mgmt_event(MGMT_EV_DISCOVERING, index, &mode_cp, sizeof(mode_cp), NULL);

	hci_dev_unlock_bh(hdev);
	hci_dev_put(hdev);

=======
	switch (hdev->discovery.state) {
	case DISCOVERY_FINDING:
		if (test_bit(HCI_INQUIRY, &hdev->flags))
			err = hci_cancel_inquiry(hdev);
		else
			err = hci_cancel_le_scan(hdev);

		break;

	case DISCOVERY_RESOLVING:
		e = hci_inquiry_cache_lookup_resolve(hdev, BDADDR_ANY,
						     NAME_PENDING);
		if (!e) {
			mgmt_pending_remove(cmd);
			err = cmd_complete(sk, hdev->id,
					   MGMT_OP_STOP_DISCOVERY, 0,
					   &mgmt_cp->type,
					   sizeof(mgmt_cp->type));
			hci_discovery_set_state(hdev, DISCOVERY_STOPPED);
			goto unlock;
		}

		bacpy(&cp.bdaddr, &e->data.bdaddr);
		err = hci_send_cmd(hdev, HCI_OP_REMOTE_NAME_REQ_CANCEL,
				   sizeof(cp), &cp);

		break;

	default:
		BT_DBG("unknown discovery state %u", hdev->discovery.state);
		err = -EFAULT;
	}

>>>>>>> common/android-3.10.y
	if (err < 0)
		return cmd_status(sk, index, MGMT_OP_STOP_DISCOVERY, -err);
	else
		return err;
}

static int read_local_oob_data(struct sock *sk, u16 index)
{
	struct hci_dev *hdev;
	struct pending_cmd *cmd;
	int err;

	BT_DBG("hci%u", index);

	hdev = hci_dev_get(index);
	if (!hdev)
		return cmd_status(sk, index, MGMT_OP_READ_LOCAL_OOB_DATA,
									ENODEV);

	hci_dev_lock_bh(hdev);

	if (!test_bit(HCI_UP, &hdev->flags)) {
		err = cmd_status(sk, index, MGMT_OP_READ_LOCAL_OOB_DATA,
								ENETDOWN);
		goto unlock;
	}

	if (!(hdev->features[6] & LMP_SIMPLE_PAIR)) {
		err = cmd_status(sk, index, MGMT_OP_READ_LOCAL_OOB_DATA,
								EOPNOTSUPP);
		goto unlock;
	}

<<<<<<< HEAD
	if (mgmt_pending_find(MGMT_OP_READ_LOCAL_OOB_DATA, index)) {
		err = cmd_status(sk, index, MGMT_OP_READ_LOCAL_OOB_DATA, EBUSY);
		goto unlock;
	}

	cmd = mgmt_pending_add(sk, MGMT_OP_READ_LOCAL_OOB_DATA, index, NULL, 0);
	if (!cmd) {
		err = -ENOMEM;
		goto unlock;
	}
=======
	err = cmd_complete(sk, hdev->id, MGMT_OP_CONFIRM_NAME, 0, &cp->addr,
			   sizeof(cp->addr));

failed:
	hci_dev_unlock(hdev);
	return err;
}

static int block_device(struct sock *sk, struct hci_dev *hdev, void *data,
			u16 len)
{
	struct mgmt_cp_block_device *cp = data;
	u8 status;
	int err;

	BT_DBG("%s", hdev->name);

	if (!bdaddr_type_is_valid(cp->addr.type))
		return cmd_complete(sk, hdev->id, MGMT_OP_BLOCK_DEVICE,
				    MGMT_STATUS_INVALID_PARAMS,
				    &cp->addr, sizeof(cp->addr));

	hci_dev_lock(hdev);
>>>>>>> common/android-3.10.y

	err = hci_send_cmd(hdev, HCI_OP_READ_LOCAL_OOB_DATA, 0, NULL);
	if (err < 0)
<<<<<<< HEAD
		mgmt_pending_remove(cmd);
=======
		status = MGMT_STATUS_FAILED;
	else
		status = MGMT_STATUS_SUCCESS;

	err = cmd_complete(sk, hdev->id, MGMT_OP_BLOCK_DEVICE, status,
			   &cp->addr, sizeof(cp->addr));
>>>>>>> common/android-3.10.y

unlock:
	hci_dev_unlock_bh(hdev);
	hci_dev_put(hdev);

	return err;
}

static int add_remote_oob_data(struct sock *sk, u16 index, unsigned char *data,
									u16 len)
{
	struct hci_dev *hdev;
	struct mgmt_cp_add_remote_oob_data *cp = (void *) data;
	int err;

	BT_DBG("hci%u ", index);

	if (len != sizeof(*cp))
		return cmd_status(sk, index, MGMT_OP_ADD_REMOTE_OOB_DATA,
									EINVAL);

	hdev = hci_dev_get(index);
	if (!hdev)
		return cmd_status(sk, index, MGMT_OP_ADD_REMOTE_OOB_DATA,
									ENODEV);

<<<<<<< HEAD
	hci_dev_lock_bh(hdev);
=======
	if (!bdaddr_type_is_valid(cp->addr.type))
		return cmd_complete(sk, hdev->id, MGMT_OP_UNBLOCK_DEVICE,
				    MGMT_STATUS_INVALID_PARAMS,
				    &cp->addr, sizeof(cp->addr));

	hci_dev_lock(hdev);
>>>>>>> common/android-3.10.y

	err = hci_add_remote_oob_data(hdev, &cp->bdaddr, cp->hash,
								cp->randomizer);
	if (err < 0)
		err = cmd_status(sk, index, MGMT_OP_ADD_REMOTE_OOB_DATA, -err);
	else
<<<<<<< HEAD
		err = cmd_complete(sk, index, MGMT_OP_ADD_REMOTE_OOB_DATA, NULL,
									0);
=======
		status = MGMT_STATUS_SUCCESS;
>>>>>>> common/android-3.10.y

	hci_dev_unlock_bh(hdev);
	hci_dev_put(hdev);

	return err;
}

<<<<<<< HEAD
static int remove_remote_oob_data(struct sock *sk, u16 index,
						unsigned char *data, u16 len)
{
	struct hci_dev *hdev;
	struct mgmt_cp_remove_remote_oob_data *cp = (void *) data;
	int err;

	BT_DBG("hci%u ", index);

	if (len != sizeof(*cp))
		return cmd_status(sk, index, MGMT_OP_REMOVE_REMOTE_OOB_DATA,
									EINVAL);

	hdev = hci_dev_get(index);
	if (!hdev)
		return cmd_status(sk, index, MGMT_OP_REMOVE_REMOTE_OOB_DATA,
									ENODEV);

	hci_dev_lock_bh(hdev);

	err = hci_remove_remote_oob_data(hdev, &cp->bdaddr);
	if (err < 0)
		err = cmd_status(sk, index, MGMT_OP_REMOVE_REMOTE_OOB_DATA,
									-err);
	else
		err = cmd_complete(sk, index, MGMT_OP_REMOVE_REMOTE_OOB_DATA,
								NULL, 0);

	hci_dev_unlock_bh(hdev);
	hci_dev_put(hdev);

	return err;
}

=======
static int set_device_id(struct sock *sk, struct hci_dev *hdev, void *data,
			 u16 len)
{
	struct mgmt_cp_set_device_id *cp = data;
	struct hci_request req;
	int err;
	__u16 source;

	BT_DBG("%s", hdev->name);

	source = __le16_to_cpu(cp->source);

	if (source > 0x0002)
		return cmd_status(sk, hdev->id, MGMT_OP_SET_DEVICE_ID,
				  MGMT_STATUS_INVALID_PARAMS);

	hci_dev_lock(hdev);

	hdev->devid_source = source;
	hdev->devid_vendor = __le16_to_cpu(cp->vendor);
	hdev->devid_product = __le16_to_cpu(cp->product);
	hdev->devid_version = __le16_to_cpu(cp->version);

	err = cmd_complete(sk, hdev->id, MGMT_OP_SET_DEVICE_ID, 0, NULL, 0);

	hci_req_init(&req, hdev);
	update_eir(&req);
	hci_req_run(&req, NULL);

	hci_dev_unlock(hdev);

	return err;
}

static void fast_connectable_complete(struct hci_dev *hdev, u8 status)
{
	struct pending_cmd *cmd;

	BT_DBG("status 0x%02x", status);

	hci_dev_lock(hdev);

	cmd = mgmt_pending_find(MGMT_OP_SET_FAST_CONNECTABLE, hdev);
	if (!cmd)
		goto unlock;

	if (status) {
		cmd_status(cmd->sk, hdev->id, MGMT_OP_SET_FAST_CONNECTABLE,
			   mgmt_status(status));
	} else {
		struct mgmt_mode *cp = cmd->param;

		if (cp->val)
			set_bit(HCI_FAST_CONNECTABLE, &hdev->dev_flags);
		else
			clear_bit(HCI_FAST_CONNECTABLE, &hdev->dev_flags);

		send_settings_rsp(cmd->sk, MGMT_OP_SET_FAST_CONNECTABLE, hdev);
		new_settings(hdev, cmd->sk);
	}

	mgmt_pending_remove(cmd);

unlock:
	hci_dev_unlock(hdev);
}

static int set_fast_connectable(struct sock *sk, struct hci_dev *hdev,
				void *data, u16 len)
{
	struct mgmt_mode *cp = data;
	struct pending_cmd *cmd;
	struct hci_request req;
	int err;

	BT_DBG("%s", hdev->name);

	if (!lmp_bredr_capable(hdev) || hdev->hci_ver < BLUETOOTH_VER_1_2)
		return cmd_status(sk, hdev->id, MGMT_OP_SET_FAST_CONNECTABLE,
				  MGMT_STATUS_NOT_SUPPORTED);

	if (cp->val != 0x00 && cp->val != 0x01)
		return cmd_status(sk, hdev->id, MGMT_OP_SET_FAST_CONNECTABLE,
				  MGMT_STATUS_INVALID_PARAMS);

	if (!hdev_is_powered(hdev))
		return cmd_status(sk, hdev->id, MGMT_OP_SET_FAST_CONNECTABLE,
				  MGMT_STATUS_NOT_POWERED);

	if (!test_bit(HCI_CONNECTABLE, &hdev->dev_flags))
		return cmd_status(sk, hdev->id, MGMT_OP_SET_FAST_CONNECTABLE,
				  MGMT_STATUS_REJECTED);

	hci_dev_lock(hdev);

	if (mgmt_pending_find(MGMT_OP_SET_FAST_CONNECTABLE, hdev)) {
		err = cmd_status(sk, hdev->id, MGMT_OP_SET_FAST_CONNECTABLE,
				 MGMT_STATUS_BUSY);
		goto unlock;
	}

	if (!!cp->val == test_bit(HCI_FAST_CONNECTABLE, &hdev->dev_flags)) {
		err = send_settings_rsp(sk, MGMT_OP_SET_FAST_CONNECTABLE,
					hdev);
		goto unlock;
	}

	cmd = mgmt_pending_add(sk, MGMT_OP_SET_FAST_CONNECTABLE, hdev,
			       data, len);
	if (!cmd) {
		err = -ENOMEM;
		goto unlock;
	}

	hci_req_init(&req, hdev);

	write_fast_connectable(&req, cp->val);

	err = hci_req_run(&req, fast_connectable_complete);
	if (err < 0) {
		err = cmd_status(sk, hdev->id, MGMT_OP_SET_FAST_CONNECTABLE,
				 MGMT_STATUS_FAILED);
		mgmt_pending_remove(cmd);
	}

unlock:
	hci_dev_unlock(hdev);

	return err;
}

static bool ltk_is_valid(struct mgmt_ltk_info *key)
{
	if (key->authenticated != 0x00 && key->authenticated != 0x01)
		return false;
	if (key->master != 0x00 && key->master != 0x01)
		return false;
	if (!bdaddr_type_is_le(key->addr.type))
		return false;
	return true;
}

static int load_long_term_keys(struct sock *sk, struct hci_dev *hdev,
			       void *cp_data, u16 len)
{
	struct mgmt_cp_load_long_term_keys *cp = cp_data;
	u16 key_count, expected_len;
	int i, err;

	key_count = __le16_to_cpu(cp->key_count);

	expected_len = sizeof(*cp) + key_count *
					sizeof(struct mgmt_ltk_info);
	if (expected_len != len) {
		BT_ERR("load_keys: expected %u bytes, got %u bytes",
		       len, expected_len);
		return cmd_status(sk, hdev->id, MGMT_OP_LOAD_LONG_TERM_KEYS,
				  MGMT_STATUS_INVALID_PARAMS);
	}

	BT_DBG("%s key_count %u", hdev->name, key_count);

	for (i = 0; i < key_count; i++) {
		struct mgmt_ltk_info *key = &cp->keys[i];

		if (!ltk_is_valid(key))
			return cmd_status(sk, hdev->id,
					  MGMT_OP_LOAD_LONG_TERM_KEYS,
					  MGMT_STATUS_INVALID_PARAMS);
	}

	hci_dev_lock(hdev);

	hci_smp_ltks_clear(hdev);

	for (i = 0; i < key_count; i++) {
		struct mgmt_ltk_info *key = &cp->keys[i];
		u8 type;

		if (key->master)
			type = HCI_SMP_LTK;
		else
			type = HCI_SMP_LTK_SLAVE;

		hci_add_ltk(hdev, &key->addr.bdaddr,
			    bdaddr_to_le(key->addr.type),
			    type, 0, key->authenticated, key->val,
			    key->enc_size, key->ediv, key->rand);
	}

	err = cmd_complete(sk, hdev->id, MGMT_OP_LOAD_LONG_TERM_KEYS, 0,
			   NULL, 0);

	hci_dev_unlock(hdev);

	return err;
}

static const struct mgmt_handler {
	int (*func) (struct sock *sk, struct hci_dev *hdev, void *data,
		     u16 data_len);
	bool var_len;
	size_t data_len;
} mgmt_handlers[] = {
	{ NULL }, /* 0x0000 (no command) */
	{ read_version,           false, MGMT_READ_VERSION_SIZE },
	{ read_commands,          false, MGMT_READ_COMMANDS_SIZE },
	{ read_index_list,        false, MGMT_READ_INDEX_LIST_SIZE },
	{ read_controller_info,   false, MGMT_READ_INFO_SIZE },
	{ set_powered,            false, MGMT_SETTING_SIZE },
	{ set_discoverable,       false, MGMT_SET_DISCOVERABLE_SIZE },
	{ set_connectable,        false, MGMT_SETTING_SIZE },
	{ set_fast_connectable,   false, MGMT_SETTING_SIZE },
	{ set_pairable,           false, MGMT_SETTING_SIZE },
	{ set_link_security,      false, MGMT_SETTING_SIZE },
	{ set_ssp,                false, MGMT_SETTING_SIZE },
	{ set_hs,                 false, MGMT_SETTING_SIZE },
	{ set_le,                 false, MGMT_SETTING_SIZE },
	{ set_dev_class,          false, MGMT_SET_DEV_CLASS_SIZE },
	{ set_local_name,         false, MGMT_SET_LOCAL_NAME_SIZE },
	{ add_uuid,               false, MGMT_ADD_UUID_SIZE },
	{ remove_uuid,            false, MGMT_REMOVE_UUID_SIZE },
	{ load_link_keys,         true,  MGMT_LOAD_LINK_KEYS_SIZE },
	{ load_long_term_keys,    true,  MGMT_LOAD_LONG_TERM_KEYS_SIZE },
	{ disconnect,             false, MGMT_DISCONNECT_SIZE },
	{ get_connections,        false, MGMT_GET_CONNECTIONS_SIZE },
	{ pin_code_reply,         false, MGMT_PIN_CODE_REPLY_SIZE },
	{ pin_code_neg_reply,     false, MGMT_PIN_CODE_NEG_REPLY_SIZE },
	{ set_io_capability,      false, MGMT_SET_IO_CAPABILITY_SIZE },
	{ pair_device,            false, MGMT_PAIR_DEVICE_SIZE },
	{ cancel_pair_device,     false, MGMT_CANCEL_PAIR_DEVICE_SIZE },
	{ unpair_device,          false, MGMT_UNPAIR_DEVICE_SIZE },
	{ user_confirm_reply,     false, MGMT_USER_CONFIRM_REPLY_SIZE },
	{ user_confirm_neg_reply, false, MGMT_USER_CONFIRM_NEG_REPLY_SIZE },
	{ user_passkey_reply,     false, MGMT_USER_PASSKEY_REPLY_SIZE },
	{ user_passkey_neg_reply, false, MGMT_USER_PASSKEY_NEG_REPLY_SIZE },
	{ read_local_oob_data,    false, MGMT_READ_LOCAL_OOB_DATA_SIZE },
	{ add_remote_oob_data,    false, MGMT_ADD_REMOTE_OOB_DATA_SIZE },
	{ remove_remote_oob_data, false, MGMT_REMOVE_REMOTE_OOB_DATA_SIZE },
	{ start_discovery,        false, MGMT_START_DISCOVERY_SIZE },
	{ stop_discovery,         false, MGMT_STOP_DISCOVERY_SIZE },
	{ confirm_name,           false, MGMT_CONFIRM_NAME_SIZE },
	{ block_device,           false, MGMT_BLOCK_DEVICE_SIZE },
	{ unblock_device,         false, MGMT_UNBLOCK_DEVICE_SIZE },
	{ set_device_id,          false, MGMT_SET_DEVICE_ID_SIZE },
};


>>>>>>> common/android-3.10.y
int mgmt_control(struct sock *sk, struct msghdr *msg, size_t msglen)
{
	unsigned char *buf;
	struct mgmt_hdr *hdr;
	u16 opcode, index, len;
<<<<<<< HEAD
=======
	struct hci_dev *hdev = NULL;
	const struct mgmt_handler *handler;
>>>>>>> common/android-3.10.y
	int err;

	BT_DBG("got %zu bytes", msglen);

	if (msglen < sizeof(*hdr))
		return -EINVAL;

	buf = kmalloc(msglen, GFP_KERNEL);
	if (!buf)
		return -ENOMEM;

	if (memcpy_fromiovec(buf, msg->msg_iov, msglen)) {
		err = -EFAULT;
		goto done;
	}

<<<<<<< HEAD
	hdr = (struct mgmt_hdr *) buf;
	opcode = get_unaligned_le16(&hdr->opcode);
	index = get_unaligned_le16(&hdr->index);
	len = get_unaligned_le16(&hdr->len);
=======
	hdr = buf;
	opcode = __le16_to_cpu(hdr->opcode);
	index = __le16_to_cpu(hdr->index);
	len = __le16_to_cpu(hdr->len);
>>>>>>> common/android-3.10.y

	if (len != msglen - sizeof(*hdr)) {
		err = -EINVAL;
		goto done;
	}

<<<<<<< HEAD
	BT_DBG("got opcode %x", opcode);
	switch (opcode) {
	case MGMT_OP_READ_VERSION:
		err = read_version(sk);
		break;
	case MGMT_OP_READ_INDEX_LIST:
		err = read_index_list(sk);
		break;
	case MGMT_OP_READ_INFO:
		err = read_controller_info(sk, index);
		break;
	case MGMT_OP_SET_POWERED:
		err = set_powered(sk, index, buf + sizeof(*hdr), len);
		break;
	case MGMT_OP_SET_DISCOVERABLE:
		err = set_discoverable(sk, index, buf + sizeof(*hdr), len);
		break;
	case MGMT_OP_SET_LIMIT_DISCOVERABLE:
		err = set_limited_discoverable(sk, index, buf + sizeof(*hdr),
									len);
		break;
	case MGMT_OP_SET_CONNECTABLE:
		err = set_connectable(sk, index, buf + sizeof(*hdr), len);
		break;
	case MGMT_OP_SET_PAIRABLE:
		err = set_pairable(sk, index, buf + sizeof(*hdr), len);
		break;
	case MGMT_OP_ADD_UUID:
		err = add_uuid(sk, index, buf + sizeof(*hdr), len);
		break;
	case MGMT_OP_REMOVE_UUID:
		err = remove_uuid(sk, index, buf + sizeof(*hdr), len);
		break;
	case MGMT_OP_SET_DEV_CLASS:
		err = set_dev_class(sk, index, buf + sizeof(*hdr), len);
		break;
	case MGMT_OP_SET_SERVICE_CACHE:
		err = set_service_cache(sk, index, buf + sizeof(*hdr), len);
		break;
	case MGMT_OP_LOAD_KEYS:
		err = load_keys(sk, index, buf + sizeof(*hdr), len);
		break;
	case MGMT_OP_REMOVE_KEY:
		err = remove_key(sk, index, buf + sizeof(*hdr), len);
		break;
	case MGMT_OP_DISCONNECT:
		err = disconnect(sk, index, buf + sizeof(*hdr), len);
		break;
	case MGMT_OP_GET_CONNECTIONS:
		err = get_connections(sk, index);
		break;
	case MGMT_OP_PIN_CODE_REPLY:
		err = pin_code_reply(sk, index, buf + sizeof(*hdr), len);
		break;
	case MGMT_OP_PIN_CODE_NEG_REPLY:
		err = pin_code_neg_reply(sk, index, buf + sizeof(*hdr), len);
		break;
	case MGMT_OP_SET_IO_CAPABILITY:
		err = set_io_capability(sk, index, buf + sizeof(*hdr), len);
		break;
	case MGMT_OP_PAIR_DEVICE:
		err = pair_device(sk, index, buf + sizeof(*hdr), len);
		break;
	case MGMT_OP_USER_CONFIRM_REPLY:
	case MGMT_OP_USER_PASSKEY_REPLY:
	case MGMT_OP_USER_CONFIRM_NEG_REPLY:
		err = user_confirm_reply(sk, index, buf + sizeof(*hdr),
								len, opcode);
		break;
	case MGMT_OP_SET_LOCAL_NAME:
		err = set_local_name(sk, index, buf + sizeof(*hdr), len);
		break;
	case MGMT_OP_START_DISCOVERY:
		err = start_discovery(sk, index);
		break;
	case MGMT_OP_STOP_DISCOVERY:
		err = stop_discovery(sk, index);
		break;
	case MGMT_OP_RESOLVE_NAME:
		err = resolve_name(sk, index, buf + sizeof(*hdr), len);
		break;
	case MGMT_OP_CANCEL_RESOLVE_NAME:
		err = cancel_resolve_name(sk, index, buf + sizeof(*hdr), len);
		break;
	case MGMT_OP_SET_CONNECTION_PARAMS:
		err = set_connection_params(sk, index, buf + sizeof(*hdr), len);
		break;
	case MGMT_OP_SET_RSSI_REPORTER:
		err = set_rssi_reporter(sk, index, buf + sizeof(*hdr), len);
		break;
	case MGMT_OP_UNSET_RSSI_REPORTER:
		err = unset_rssi_reporter(sk, index, buf + sizeof(*hdr), len);
		break;
	case MGMT_OP_READ_LOCAL_OOB_DATA:
		err = read_local_oob_data(sk, index);
		break;
	case MGMT_OP_ADD_REMOTE_OOB_DATA:
		err = add_remote_oob_data(sk, index, buf + sizeof(*hdr), len);
		break;
	case MGMT_OP_REMOVE_REMOTE_OOB_DATA:
		err = remove_remote_oob_data(sk, index, buf + sizeof(*hdr),
									len);
		break;
	case MGMT_OP_ENCRYPT_LINK:
		err = encrypt_link(sk, index, buf + sizeof(*hdr), len);
		break;
	case MGMT_OP_LE_ADD_DEV_WHITE_LIST:
		err = le_add_dev_white_list(sk, index, buf + sizeof(*hdr),
									len);
		break;
	case MGMT_OP_LE_REMOVE_DEV_WHITE_LIST:
		err = le_remove_dev_white_list(sk, index, buf + sizeof(*hdr),
									len);
		break;
	case MGMT_OP_LE_CLEAR_WHITE_LIST:
		err = le_clear_white_list(sk, index);
		break;
	case MGMT_OP_LE_CREATE_CONN_WHITE_LIST:
		err = le_create_conn_white_list(sk, index);
		break;
	case MGMT_OP_LE_CANCEL_CREATE_CONN_WHITE_LIST:
		err = le_cancel_create_conn_white_list(sk, index);
		break;
	case MGMT_OP_LE_CANCEL_CREATE_CONN:
		err = le_cancel_create_conn(sk, index, buf + sizeof(*hdr), len);
		break;
	default:
		BT_DBG("Unknown op %u", opcode);
		err = cmd_status(sk, index, opcode, 0x01);
		break;
=======
	if (index != MGMT_INDEX_NONE) {
		hdev = hci_dev_get(index);
		if (!hdev) {
			err = cmd_status(sk, index, opcode,
					 MGMT_STATUS_INVALID_INDEX);
			goto done;
		}
	}

	if (opcode >= ARRAY_SIZE(mgmt_handlers) ||
	    mgmt_handlers[opcode].func == NULL) {
		BT_DBG("Unknown op %u", opcode);
		err = cmd_status(sk, index, opcode,
				 MGMT_STATUS_UNKNOWN_COMMAND);
		goto done;
	}

	if ((hdev && opcode < MGMT_OP_READ_INFO) ||
	    (!hdev && opcode >= MGMT_OP_READ_INFO)) {
		err = cmd_status(sk, index, opcode,
				 MGMT_STATUS_INVALID_INDEX);
		goto done;
	}

	handler = &mgmt_handlers[opcode];

	if ((handler->var_len && len < handler->data_len) ||
	    (!handler->var_len && len != handler->data_len)) {
		err = cmd_status(sk, index, opcode,
				 MGMT_STATUS_INVALID_PARAMS);
		goto done;
>>>>>>> common/android-3.10.y
	}

	if (err < 0)
		goto done;

	err = msglen;

done:
	kfree(buf);
	return err;
}

static void cmd_status_rsp(struct pending_cmd *cmd, void *data)
{
	u8 *status = data;

	cmd_status(cmd->sk, cmd->index, cmd->opcode, *status);
	mgmt_pending_remove(cmd);
}

int mgmt_index_added(u16 index)
{
<<<<<<< HEAD
	BT_DBG("%d", index);
	return mgmt_event(MGMT_EV_INDEX_ADDED, index, NULL, 0, NULL);
=======
	if (!mgmt_valid_hdev(hdev))
		return -ENOTSUPP;

	return mgmt_event(MGMT_EV_INDEX_ADDED, hdev, NULL, 0, NULL);
>>>>>>> common/android-3.10.y
}

int mgmt_index_removed(u16 index)
{
	u8 status = ENODEV;

<<<<<<< HEAD
	BT_DBG("%d", index);
=======
	if (!mgmt_valid_hdev(hdev))
		return -ENOTSUPP;

	mgmt_pending_foreach(0, hdev, cmd_status_rsp, &status);
>>>>>>> common/android-3.10.y

	mgmt_pending_foreach(0, index, cmd_status_rsp, &status);

	return mgmt_event(MGMT_EV_INDEX_REMOVED, index, NULL, 0, NULL);
}

struct cmd_lookup {
	u8 val;
	struct sock *sk;
};

static void mode_rsp(struct pending_cmd *cmd, void *data)
{
	struct mgmt_mode *cp = cmd->param;
	struct cmd_lookup *match = data;

	if (cp->val != match->val)
		return;

	send_mode_rsp(cmd->sk, cmd->opcode, cmd->index, cp->val);

	list_del(&cmd->list);

	if (match->sk == NULL) {
		match->sk = cmd->sk;
		sock_hold(match->sk);
	}

	mgmt_pending_free(cmd);
}

<<<<<<< HEAD
int mgmt_powered(u16 index, u8 powered)
{
	struct mgmt_mode ev;
	struct cmd_lookup match = { powered, NULL };
	int ret;

	BT_DBG("hci%u %d", index, powered);

	mgmt_pending_foreach(MGMT_OP_SET_POWERED, index, mode_rsp, &match);

	if (!powered) {
		u8 status = ENETDOWN;
		mgmt_pending_foreach(0, index, cmd_status_rsp, &status);
	}

	ev.val = powered;

	ret = mgmt_event(MGMT_EV_POWERED, index, &ev, sizeof(ev), match.sk);
=======
static void set_bredr_scan(struct hci_request *req)
{
	struct hci_dev *hdev = req->hdev;
	u8 scan = 0;

	/* Ensure that fast connectable is disabled. This function will
	 * not do anything if the page scan parameters are already what
	 * they should be.
	 */
	write_fast_connectable(req, false);

	if (test_bit(HCI_CONNECTABLE, &hdev->dev_flags))
		scan |= SCAN_PAGE;
	if (test_bit(HCI_DISCOVERABLE, &hdev->dev_flags))
		scan |= SCAN_INQUIRY;

	if (scan)
		hci_req_add(req, HCI_OP_WRITE_SCAN_ENABLE, 1, &scan);
}

static void powered_complete(struct hci_dev *hdev, u8 status)
{
	struct cmd_lookup match = { NULL, hdev };

	BT_DBG("status 0x%02x", status);

	hci_dev_lock(hdev);

	mgmt_pending_foreach(MGMT_OP_SET_POWERED, hdev, settings_rsp, &match);

	new_settings(hdev, match.sk);

	hci_dev_unlock(hdev);

	if (match.sk)
		sock_put(match.sk);
}

static int powered_update_hci(struct hci_dev *hdev)
{
	struct hci_request req;
	u8 link_sec;

	hci_req_init(&req, hdev);

	if (test_bit(HCI_SSP_ENABLED, &hdev->dev_flags) &&
	    !lmp_host_ssp_capable(hdev)) {
		u8 ssp = 1;

		hci_req_add(&req, HCI_OP_WRITE_SSP_MODE, 1, &ssp);
	}

	if (test_bit(HCI_LE_ENABLED, &hdev->dev_flags) &&
	    lmp_bredr_capable(hdev)) {
		struct hci_cp_write_le_host_supported cp;

		cp.le = 1;
		cp.simul = lmp_le_br_capable(hdev);

		/* Check first if we already have the right
		 * host state (host features set)
		 */
		if (cp.le != lmp_host_le_capable(hdev) ||
		    cp.simul != lmp_host_le_br_capable(hdev))
			hci_req_add(&req, HCI_OP_WRITE_LE_HOST_SUPPORTED,
				    sizeof(cp), &cp);
	}

	link_sec = test_bit(HCI_LINK_SECURITY, &hdev->dev_flags);
	if (link_sec != test_bit(HCI_AUTH, &hdev->flags))
		hci_req_add(&req, HCI_OP_WRITE_AUTH_ENABLE,
			    sizeof(link_sec), &link_sec);

	if (lmp_bredr_capable(hdev)) {
		set_bredr_scan(&req);
		update_class(&req);
		update_name(&req);
		update_eir(&req);
	}

	return hci_req_run(&req, powered_complete);
}

int mgmt_powered(struct hci_dev *hdev, u8 powered)
{
	struct cmd_lookup match = { NULL, hdev };
	u8 status_not_powered = MGMT_STATUS_NOT_POWERED;
	u8 zero_cod[] = { 0, 0, 0 };
	int err;

	if (!test_bit(HCI_MGMT, &hdev->dev_flags))
		return 0;

	if (powered) {
		if (powered_update_hci(hdev) == 0)
			return 0;

		mgmt_pending_foreach(MGMT_OP_SET_POWERED, hdev, settings_rsp,
				     &match);
		goto new_settings;
	}

	mgmt_pending_foreach(MGMT_OP_SET_POWERED, hdev, settings_rsp, &match);
	mgmt_pending_foreach(0, hdev, cmd_status_rsp, &status_not_powered);

	if (memcmp(hdev->dev_class, zero_cod, sizeof(zero_cod)) != 0)
		mgmt_event(MGMT_EV_CLASS_OF_DEV_CHANGED, hdev,
			   zero_cod, sizeof(zero_cod), NULL);

new_settings:
	err = new_settings(hdev, match.sk);
>>>>>>> common/android-3.10.y

	if (match.sk)
		sock_put(match.sk);

	return ret;
}

<<<<<<< HEAD
int mgmt_discoverable(u16 index, u8 discoverable)
=======
int mgmt_set_powered_failed(struct hci_dev *hdev, int err)
{
	struct pending_cmd *cmd;
	u8 status;

	cmd = mgmt_pending_find(MGMT_OP_SET_POWERED, hdev);
	if (!cmd)
		return -ENOENT;

	if (err == -ERFKILL)
		status = MGMT_STATUS_RFKILLED;
	else
		status = MGMT_STATUS_FAILED;

	err = cmd_status(cmd->sk, hdev->id, MGMT_OP_SET_POWERED, status);

	mgmt_pending_remove(cmd);

	return err;
}

int mgmt_discoverable(struct hci_dev *hdev, u8 discoverable)
>>>>>>> common/android-3.10.y
{
	struct mgmt_mode ev;
	struct cmd_lookup match = { discoverable, NULL };
	int ret;

	mgmt_pending_foreach(MGMT_OP_SET_DISCOVERABLE, index, mode_rsp, &match);

	ev.val = discoverable;

	ret = mgmt_event(MGMT_EV_DISCOVERABLE, index, &ev, sizeof(ev),
								match.sk);

	if (match.sk)
		sock_put(match.sk);

	return ret;
}

int mgmt_connectable(u16 index, u8 connectable)
{
<<<<<<< HEAD
	struct mgmt_mode ev;
	struct cmd_lookup match = { connectable, NULL };
	int ret;
=======
	struct pending_cmd *cmd;
	bool changed = false;
	int err = 0;
>>>>>>> common/android-3.10.y

	mgmt_pending_foreach(MGMT_OP_SET_CONNECTABLE, index, mode_rsp, &match);

<<<<<<< HEAD
	ev.val = connectable;

	ret = mgmt_event(MGMT_EV_CONNECTABLE, index, &ev, sizeof(ev), match.sk);

	if (match.sk)
		sock_put(match.sk);
=======
	cmd = mgmt_pending_find(MGMT_OP_SET_CONNECTABLE, hdev);

	if (changed)
		err = new_settings(hdev, cmd ? cmd->sk : NULL);
>>>>>>> common/android-3.10.y

	return ret;
}

int mgmt_new_key(u16 index, struct link_key *key, u8 bonded)
{
	struct mgmt_ev_new_key *ev;
	int err, total;

	total = sizeof(struct mgmt_ev_new_key) + key->dlen;
	ev = kzalloc(total, GFP_ATOMIC);
	if (!ev)
		return -ENOMEM;

	bacpy(&ev->key.bdaddr, &key->bdaddr);
	ev->key.addr_type = key->addr_type;
	ev->key.key_type = key->key_type;
	memcpy(ev->key.val, key->val, 16);
	ev->key.pin_len = key->pin_len;
	ev->key.auth = key->auth;
	ev->store_hint = bonded;
	ev->key.dlen = key->dlen;

<<<<<<< HEAD
	memcpy(ev->key.data, key->data, key->dlen);
=======
int mgmt_new_link_key(struct hci_dev *hdev, struct link_key *key,
		      bool persistent)
{
	struct mgmt_ev_new_link_key ev;
>>>>>>> common/android-3.10.y

	err = mgmt_event(MGMT_EV_NEW_KEY, index, ev, total, NULL);

<<<<<<< HEAD
	kfree(ev);
=======
	ev.store_hint = persistent;
	bacpy(&ev.key.addr.bdaddr, &key->bdaddr);
	ev.key.addr.type = BDADDR_BREDR;
	ev.key.type = key->type;
	memcpy(ev.key.val, key->val, HCI_LINK_KEY_SIZE);
	ev.key.pin_len = key->pin_len;
>>>>>>> common/android-3.10.y

	return err;
}

int mgmt_connected(u16 index, bdaddr_t *bdaddr, u8 le)
{
	struct mgmt_ev_connected ev;
	struct pending_cmd *cmd;
	struct hci_dev *hdev;

	BT_DBG("hci%u", index);

<<<<<<< HEAD
	hdev = hci_dev_get(index);
=======
	ev.store_hint = persistent;
	bacpy(&ev.key.addr.bdaddr, &key->bdaddr);
	ev.key.addr.type = link_to_bdaddr(LE_LINK, key->bdaddr_type);
	ev.key.authenticated = key->authenticated;
	ev.key.enc_size = key->enc_size;
	ev.key.ediv = key->ediv;
>>>>>>> common/android-3.10.y

	if (!hdev)
		return -ENODEV;

	bacpy(&ev.bdaddr, bdaddr);
	ev.le = le;

	cmd = mgmt_pending_find(MGMT_OP_LE_CREATE_CONN_WHITE_LIST, index);
	if (cmd) {
		BT_ERR("mgmt_connected remove mgmt pending white_list");
		mgmt_pending_remove(cmd);
	}

	return mgmt_event(MGMT_EV_CONNECTED, index, &ev, sizeof(ev), NULL);
}

int mgmt_le_conn_params(u16 index, bdaddr_t *bdaddr, u16 interval,
						u16 latency, u16 timeout)
{
<<<<<<< HEAD
	struct mgmt_ev_le_conn_params ev;

	bacpy(&ev.bdaddr, bdaddr);
	ev.interval = interval;
	ev.latency = latency;
	ev.timeout = timeout;
=======
	char buf[512];
	struct mgmt_ev_device_connected *ev = (void *) buf;
	u16 eir_len = 0;

	bacpy(&ev->addr.bdaddr, bdaddr);
	ev->addr.type = link_to_bdaddr(link_type, addr_type);

	ev->flags = __cpu_to_le32(flags);

	if (name_len > 0)
		eir_len = eir_append_data(ev->eir, 0, EIR_NAME_COMPLETE,
					  name, name_len);

	if (dev_class && memcmp(dev_class, "\0\0\0", 3) != 0)
		eir_len = eir_append_data(ev->eir, eir_len,
					  EIR_CLASS_OF_DEV, dev_class, 3);

	ev->eir_len = cpu_to_le16(eir_len);
>>>>>>> common/android-3.10.y

	return mgmt_event(MGMT_EV_LE_CONN_PARAMS, index, &ev, sizeof(ev),
									NULL);
}

static void disconnect_rsp(struct pending_cmd *cmd, void *data)
{
	struct mgmt_cp_disconnect *cp = cmd->param;
	struct sock **sk = data;
	struct mgmt_rp_disconnect rp;

	bacpy(&rp.bdaddr, &cp->bdaddr);

	cmd_complete(cmd->sk, cmd->index, MGMT_OP_DISCONNECT, &rp, sizeof(rp));

	*sk = cmd->sk;
	sock_hold(*sk);

	mgmt_pending_remove(cmd);
}

<<<<<<< HEAD
int mgmt_disconnected(u16 index, bdaddr_t *bdaddr, u8 reason)
{
	struct mgmt_ev_disconnected ev;
	struct sock *sk = NULL;
	int err;

	bacpy(&ev.bdaddr, bdaddr);
=======
static void unpair_device_rsp(struct pending_cmd *cmd, void *data)
{
	struct hci_dev *hdev = data;
	struct mgmt_cp_unpair_device *cp = cmd->param;
	struct mgmt_rp_unpair_device rp;

	memset(&rp, 0, sizeof(rp));
	bacpy(&rp.addr.bdaddr, &cp->addr.bdaddr);
	rp.addr.type = cp->addr.type;

	device_unpaired(hdev, &cp->addr.bdaddr, cp->addr.type, cmd->sk);

	cmd_complete(cmd->sk, cmd->index, cmd->opcode, 0, &rp, sizeof(rp));

	mgmt_pending_remove(cmd);
}

int mgmt_device_disconnected(struct hci_dev *hdev, bdaddr_t *bdaddr,
			     u8 link_type, u8 addr_type, u8 reason)
{
	struct mgmt_ev_device_disconnected ev;
	struct sock *sk = NULL;
	int err;

	mgmt_pending_foreach(MGMT_OP_DISCONNECT, hdev, disconnect_rsp, &sk);

	bacpy(&ev.addr.bdaddr, bdaddr);
	ev.addr.type = link_to_bdaddr(link_type, addr_type);
>>>>>>> common/android-3.10.y
	ev.reason = reason;

	err = mgmt_event(MGMT_EV_DISCONNECTED, index, &ev, sizeof(ev), sk);

	if (sk)
		sock_put(sk);

	mgmt_pending_foreach(MGMT_OP_DISCONNECT, index, disconnect_rsp, &sk);

	return err;
}

int mgmt_disconnect_failed(u16 index)
{
	struct pending_cmd *cmd;
	int err;

<<<<<<< HEAD
	cmd = mgmt_pending_find(MGMT_OP_DISCONNECT, index);
	if (!cmd)
		return -ENOENT;

	err = cmd_status(cmd->sk, index, MGMT_OP_DISCONNECT, EIO);
=======
	mgmt_pending_foreach(MGMT_OP_UNPAIR_DEVICE, hdev, unpair_device_rsp,
			     hdev);

	cmd = mgmt_pending_find(MGMT_OP_DISCONNECT, hdev);
	if (!cmd)
		return -ENOENT;

	bacpy(&rp.addr.bdaddr, bdaddr);
	rp.addr.type = link_to_bdaddr(link_type, addr_type);

	err = cmd_complete(cmd->sk, cmd->index, MGMT_OP_DISCONNECT,
			   mgmt_status(status), &rp, sizeof(rp));
>>>>>>> common/android-3.10.y

	mgmt_pending_remove(cmd);

	return err;
}

int mgmt_connect_failed(u16 index, bdaddr_t *bdaddr, u8 status)
{
	struct mgmt_ev_connect_failed ev;

<<<<<<< HEAD
	bacpy(&ev.bdaddr, bdaddr);
	ev.status = status;
=======
	bacpy(&ev.addr.bdaddr, bdaddr);
	ev.addr.type = link_to_bdaddr(link_type, addr_type);
	ev.status = mgmt_status(status);
>>>>>>> common/android-3.10.y

	return mgmt_event(MGMT_EV_CONNECT_FAILED, index, &ev, sizeof(ev), NULL);
}

int mgmt_pin_code_request(u16 index, bdaddr_t *bdaddr)
{
	struct mgmt_ev_pin_code_request ev;

<<<<<<< HEAD
	BT_DBG("hci%u", index);

	bacpy(&ev.bdaddr, bdaddr);
	ev.secure = 0;
=======
	bacpy(&ev.addr.bdaddr, bdaddr);
	ev.addr.type = BDADDR_BREDR;
	ev.secure = secure;
>>>>>>> common/android-3.10.y

	return mgmt_event(MGMT_EV_PIN_CODE_REQUEST, index, &ev, sizeof(ev),
									NULL);
}

int mgmt_pin_code_reply_complete(u16 index, bdaddr_t *bdaddr, u8 status)
{
	struct pending_cmd *cmd;
	struct mgmt_rp_pin_code_reply rp;
	int err;

	cmd = mgmt_pending_find(MGMT_OP_PIN_CODE_REPLY, index);
	if (!cmd)
		return -ENOENT;

<<<<<<< HEAD
	bacpy(&rp.bdaddr, bdaddr);
	rp.status = status;
=======
	bacpy(&rp.addr.bdaddr, bdaddr);
	rp.addr.type = BDADDR_BREDR;
>>>>>>> common/android-3.10.y

	err = cmd_complete(cmd->sk, index, MGMT_OP_PIN_CODE_REPLY, &rp,
								sizeof(rp));

	mgmt_pending_remove(cmd);

	return err;
}

int mgmt_pin_code_neg_reply_complete(u16 index, bdaddr_t *bdaddr, u8 status)
{
	struct pending_cmd *cmd;
	struct mgmt_rp_pin_code_reply rp;
	int err;

	cmd = mgmt_pending_find(MGMT_OP_PIN_CODE_NEG_REPLY, index);
	if (!cmd)
		return -ENOENT;

<<<<<<< HEAD
	bacpy(&rp.bdaddr, bdaddr);
	rp.status = status;
=======
	bacpy(&rp.addr.bdaddr, bdaddr);
	rp.addr.type = BDADDR_BREDR;
>>>>>>> common/android-3.10.y

	err = cmd_complete(cmd->sk, index, MGMT_OP_PIN_CODE_NEG_REPLY, &rp,
								sizeof(rp));

	mgmt_pending_remove(cmd);

	return err;
}

int mgmt_user_confirm_request(u16 index, u8 event,
					bdaddr_t *bdaddr, __le32 value)
{
	struct mgmt_ev_user_confirm_request ev;
	struct hci_conn *conn = NULL;
	struct hci_dev *hdev;
	u8 loc_cap, rem_cap, loc_mitm, rem_mitm;

<<<<<<< HEAD
	BT_DBG("hci%u", index);
=======
	BT_DBG("%s", hdev->name);

	bacpy(&ev.addr.bdaddr, bdaddr);
	ev.addr.type = link_to_bdaddr(link_type, addr_type);
	ev.confirm_hint = confirm_hint;
	ev.value = value;

	return mgmt_event(MGMT_EV_USER_CONFIRM_REQUEST, hdev, &ev, sizeof(ev),
			  NULL);
}

int mgmt_user_passkey_request(struct hci_dev *hdev, bdaddr_t *bdaddr,
			      u8 link_type, u8 addr_type)
{
	struct mgmt_ev_user_passkey_request ev;

	BT_DBG("%s", hdev->name);

	bacpy(&ev.addr.bdaddr, bdaddr);
	ev.addr.type = link_to_bdaddr(link_type, addr_type);

	return mgmt_event(MGMT_EV_USER_PASSKEY_REQUEST, hdev, &ev, sizeof(ev),
			  NULL);
}

static int user_pairing_resp_complete(struct hci_dev *hdev, bdaddr_t *bdaddr,
				      u8 link_type, u8 addr_type, u8 status,
				      u8 opcode)
{
	struct pending_cmd *cmd;
	struct mgmt_rp_user_confirm_reply rp;
	int err;

	cmd = mgmt_pending_find(opcode, hdev);
	if (!cmd)
		return -ENOENT;

	bacpy(&rp.addr.bdaddr, bdaddr);
	rp.addr.type = link_to_bdaddr(link_type, addr_type);
	err = cmd_complete(cmd->sk, hdev->id, opcode, mgmt_status(status),
			   &rp, sizeof(rp));

	mgmt_pending_remove(cmd);

	return err;
}

int mgmt_user_confirm_reply_complete(struct hci_dev *hdev, bdaddr_t *bdaddr,
				     u8 link_type, u8 addr_type, u8 status)
{
	return user_pairing_resp_complete(hdev, bdaddr, link_type, addr_type,
					  status, MGMT_OP_USER_CONFIRM_REPLY);
}

int mgmt_user_confirm_neg_reply_complete(struct hci_dev *hdev, bdaddr_t *bdaddr,
					 u8 link_type, u8 addr_type, u8 status)
{
	return user_pairing_resp_complete(hdev, bdaddr, link_type, addr_type,
					  status,
					  MGMT_OP_USER_CONFIRM_NEG_REPLY);
}
>>>>>>> common/android-3.10.y

	hdev = hci_dev_get(index);

<<<<<<< HEAD
	if (!hdev)
		return -ENODEV;
=======
int mgmt_user_passkey_neg_reply_complete(struct hci_dev *hdev, bdaddr_t *bdaddr,
					 u8 link_type, u8 addr_type, u8 status)
{
	return user_pairing_resp_complete(hdev, bdaddr, link_type, addr_type,
					  status,
					  MGMT_OP_USER_PASSKEY_NEG_REPLY);
}

int mgmt_user_passkey_notify(struct hci_dev *hdev, bdaddr_t *bdaddr,
			     u8 link_type, u8 addr_type, u32 passkey,
			     u8 entered)
{
	struct mgmt_ev_passkey_notify ev;

	BT_DBG("%s", hdev->name);

	bacpy(&ev.addr.bdaddr, bdaddr);
	ev.addr.type = link_to_bdaddr(link_type, addr_type);
	ev.passkey = __cpu_to_le32(passkey);
	ev.entered = entered;

	return mgmt_event(MGMT_EV_PASSKEY_NOTIFY, hdev, &ev, sizeof(ev), NULL);
}
>>>>>>> common/android-3.10.y

	conn = hci_conn_hash_lookup_ba(hdev, ACL_LINK, bdaddr);

<<<<<<< HEAD
	ev.auto_confirm = 0;
=======
	bacpy(&ev.addr.bdaddr, bdaddr);
	ev.addr.type = link_to_bdaddr(link_type, addr_type);
	ev.status = mgmt_status(status);
>>>>>>> common/android-3.10.y

	if (!conn || event != HCI_EV_USER_CONFIRM_REQUEST)
		goto no_auto_confirm;

	loc_cap = (conn->io_capability == 0x04) ? 0x01 : conn->io_capability;
	rem_cap = conn->remote_cap;
	loc_mitm = conn->auth_type & 0x01;
	rem_mitm = conn->remote_auth & 0x01;

	if ((conn->auth_type & HCI_AT_DEDICATED_BONDING) &&
			conn->auth_initiator && rem_cap == 0x03)
		ev.auto_confirm = 1;
	else if (loc_cap == 0x01 && (rem_cap == 0x00 || rem_cap == 0x03)) {
		if (!loc_mitm && !rem_mitm)
			value = 0;
		goto no_auto_confirm;
	}

	/* Show bonding dialog if neither side requires no bonding */
	if ((conn->auth_type > 0x01) && (conn->remote_auth > 0x01)) {
		if (!loc_mitm && !rem_mitm)
			value = 0;
		goto no_auto_confirm;
	}

	if ((!loc_mitm || rem_cap == 0x03) && (!rem_mitm || loc_cap == 0x03))
		ev.auto_confirm = 1;

no_auto_confirm:
	bacpy(&ev.bdaddr, bdaddr);
	ev.event = event;
	put_unaligned_le32(value, &ev.value);

	hci_dev_put(hdev);

	return mgmt_event(MGMT_EV_USER_CONFIRM_REQUEST, index, &ev, sizeof(ev),
									NULL);
}

<<<<<<< HEAD
int mgmt_user_passkey_request(u16 index, bdaddr_t *bdaddr)
{
	struct mgmt_ev_user_passkey_request ev;
=======
static void clear_eir(struct hci_request *req)
{
	struct hci_dev *hdev = req->hdev;
	struct hci_cp_write_eir cp;

	if (!lmp_ext_inq_capable(hdev))
		return;
>>>>>>> common/android-3.10.y

	BT_DBG("hci%u", index);

	bacpy(&ev.bdaddr, bdaddr);

<<<<<<< HEAD
	return mgmt_event(MGMT_EV_USER_PASSKEY_REQUEST, index, &ev, sizeof(ev),
									NULL);
=======
	hci_req_add(req, HCI_OP_WRITE_EIR, sizeof(cp), &cp);
>>>>>>> common/android-3.10.y
}

static int confirm_reply_complete(u16 index, bdaddr_t *bdaddr, u8 status,
								u8 opcode)
{
<<<<<<< HEAD
	struct pending_cmd *cmd;
	struct mgmt_rp_user_confirm_reply rp;
	int err;
=======
	struct cmd_lookup match = { NULL, hdev };
	struct hci_request req;
	bool changed = false;
	int err = 0;

	if (status) {
		u8 mgmt_err = mgmt_status(status);

		if (enable && test_and_clear_bit(HCI_SSP_ENABLED,
						 &hdev->dev_flags))
			err = new_settings(hdev, NULL);

		mgmt_pending_foreach(MGMT_OP_SET_SSP, hdev, cmd_status_rsp,
				     &mgmt_err);

		return err;
	}

	if (enable) {
		if (!test_and_set_bit(HCI_SSP_ENABLED, &hdev->dev_flags))
			changed = true;
	} else {
		if (test_and_clear_bit(HCI_SSP_ENABLED, &hdev->dev_flags))
			changed = true;
	}

	mgmt_pending_foreach(MGMT_OP_SET_SSP, hdev, settings_rsp, &match);
>>>>>>> common/android-3.10.y

	cmd = mgmt_pending_find(opcode, index);
	if (!cmd)
		return -ENOENT;

	bacpy(&rp.bdaddr, bdaddr);
	rp.status = status;
	err = cmd_complete(cmd->sk, index, opcode, &rp, sizeof(rp));

<<<<<<< HEAD
	mgmt_pending_remove(cmd);
=======
	hci_req_init(&req, hdev);

	if (test_bit(HCI_SSP_ENABLED, &hdev->dev_flags))
		update_eir(&req);
	else
		clear_eir(&req);

	hci_req_run(&req, NULL);
>>>>>>> common/android-3.10.y

	return err;
}

<<<<<<< HEAD
int mgmt_user_confirm_reply_complete(u16 index, bdaddr_t *bdaddr, u8 status)
{
	return confirm_reply_complete(index, bdaddr, status,
						MGMT_OP_USER_CONFIRM_REPLY);
=======
static void sk_lookup(struct pending_cmd *cmd, void *data)
{
	struct cmd_lookup *match = data;

	if (match->sk == NULL) {
		match->sk = cmd->sk;
		sock_hold(match->sk);
	}
>>>>>>> common/android-3.10.y
}

int mgmt_user_confirm_neg_reply_complete(u16 index, bdaddr_t *bdaddr, u8 status)
{
<<<<<<< HEAD
	return confirm_reply_complete(index, bdaddr, status,
					MGMT_OP_USER_CONFIRM_NEG_REPLY);
}
=======
	struct cmd_lookup match = { NULL, hdev, mgmt_status(status) };
	int err = 0;

	mgmt_pending_foreach(MGMT_OP_SET_DEV_CLASS, hdev, sk_lookup, &match);
	mgmt_pending_foreach(MGMT_OP_ADD_UUID, hdev, sk_lookup, &match);
	mgmt_pending_foreach(MGMT_OP_REMOVE_UUID, hdev, sk_lookup, &match);
>>>>>>> common/android-3.10.y

int mgmt_auth_failed(u16 index, bdaddr_t *bdaddr, u8 status)
{
	struct mgmt_ev_auth_failed ev;

	bacpy(&ev.bdaddr, bdaddr);
	ev.status = status;

	return mgmt_event(MGMT_EV_AUTH_FAILED, index, &ev, sizeof(ev), NULL);
}

int mgmt_set_local_name_complete(u16 index, u8 *name, u8 status)
{
<<<<<<< HEAD
	struct pending_cmd *cmd;
	struct hci_dev *hdev;
	struct mgmt_cp_set_local_name ev;
	int err;
=======
	struct mgmt_cp_set_local_name ev;
	struct pending_cmd *cmd;

	if (status)
		return 0;
>>>>>>> common/android-3.10.y

	memset(&ev, 0, sizeof(ev));
	memcpy(ev.name, name, HCI_MAX_NAME_LENGTH);

<<<<<<< HEAD
	cmd = mgmt_pending_find(MGMT_OP_SET_LOCAL_NAME, index);
	if (!cmd)
		goto send_event;

	if (status) {
		err = cmd_status(cmd->sk, index, MGMT_OP_SET_LOCAL_NAME, EIO);
		goto failed;
	}

	hdev = hci_dev_get(index);
	if (hdev) {
		update_eir(hdev);
		hci_dev_put(hdev);
	}

	err = cmd_complete(cmd->sk, index, MGMT_OP_SET_LOCAL_NAME, &ev,
								sizeof(ev));
	if (err < 0)
		goto failed;

send_event:
	err = mgmt_event(MGMT_EV_LOCAL_NAME_CHANGED, index, &ev, sizeof(ev),
							cmd ? cmd->sk : NULL);

failed:
	if (cmd)
		mgmt_pending_remove(cmd);
	return err;
=======
	cmd = mgmt_pending_find(MGMT_OP_SET_LOCAL_NAME, hdev);
	if (!cmd) {
		memcpy(hdev->dev_name, name, sizeof(hdev->dev_name));

		/* If this is a HCI command related to powering on the
		 * HCI dev don't send any mgmt signals.
		 */
		if (mgmt_pending_find(MGMT_OP_SET_POWERED, hdev))
			return 0;
	}

	return mgmt_event(MGMT_EV_LOCAL_NAME_CHANGED, hdev, &ev, sizeof(ev),
			  cmd ? cmd->sk : NULL);
>>>>>>> common/android-3.10.y
}

int mgmt_read_local_oob_data_reply_complete(u16 index, u8 *hash, u8 *randomizer,
								u8 status)
{
	struct pending_cmd *cmd;
	int err;

	BT_DBG("hci%u status %u", index, status);

	cmd = mgmt_pending_find(MGMT_OP_READ_LOCAL_OOB_DATA, index);
	if (!cmd)
		return -ENOENT;

	if (status) {
		err = cmd_status(cmd->sk, index, MGMT_OP_READ_LOCAL_OOB_DATA,
									EIO);
	} else {
		struct mgmt_rp_read_local_oob_data rp;

		memcpy(rp.hash, hash, sizeof(rp.hash));
		memcpy(rp.randomizer, randomizer, sizeof(rp.randomizer));

		err = cmd_complete(cmd->sk, index, MGMT_OP_READ_LOCAL_OOB_DATA,
							&rp, sizeof(rp));
	}

	mgmt_pending_remove(cmd);

	return err;
}

void mgmt_read_rssi_complete(u16 index, s8 rssi, bdaddr_t *bdaddr,
		u16 handle, u8 status)
{
	struct mgmt_ev_rssi_update ev;
	struct hci_conn *conn;
	struct hci_dev *hdev;

<<<<<<< HEAD
	if (status)
		return;

	hdev = hci_dev_get(index);
	conn = hci_conn_hash_lookup_handle(hdev, handle);
=======
		if (enable && test_and_clear_bit(HCI_LE_ENABLED,
						 &hdev->dev_flags))
			err = new_settings(hdev, NULL);

		mgmt_pending_foreach(MGMT_OP_SET_LE, hdev, cmd_status_rsp,
				     &mgmt_err);
>>>>>>> common/android-3.10.y

	if (!conn)
		return;

	BT_DBG("rssi_update_thresh_exceed : %d ",
		   conn->rssi_update_thresh_exceed);
	BT_DBG("RSSI Threshold : %d , recvd RSSI : %d ",
			conn->rssi_threshold, rssi);

	if (conn->rssi_update_thresh_exceed == 1) {
		BT_DBG("rssi_update_thresh_exceed == 1");
		if (rssi > conn->rssi_threshold) {
			memset(&ev, 0, sizeof(ev));
			bacpy(&ev.bdaddr, bdaddr);
			ev.rssi = rssi;
			mgmt_event(MGMT_EV_RSSI_UPDATE, index, &ev,
				sizeof(ev), NULL);
		} else {
			hci_conn_set_rssi_reporter(conn, conn->rssi_threshold,
				conn->rssi_update_interval,
				conn->rssi_update_thresh_exceed);
		}
	} else {
		BT_DBG("rssi_update_thresh_exceed == 0");
		if (rssi < conn->rssi_threshold) {
			memset(&ev, 0, sizeof(ev));
			bacpy(&ev.bdaddr, bdaddr);
			ev.rssi = rssi;
			mgmt_event(MGMT_EV_RSSI_UPDATE, index, &ev,
				sizeof(ev), NULL);
		} else {
			hci_conn_set_rssi_reporter(conn, conn->rssi_threshold,
				conn->rssi_update_interval,
				conn->rssi_update_thresh_exceed);
		}
	}
}


<<<<<<< HEAD
int mgmt_device_found(u16 index, bdaddr_t *bdaddr, u8 type, u8 le,
			u8 *dev_class, s8 rssi, u8 eir_len, u8 *eir)
{
	struct mgmt_ev_device_found ev;
	struct hci_dev *hdev;
	int err;
=======
	bacpy(&ev->addr.bdaddr, bdaddr);
	ev->addr.type = link_to_bdaddr(link_type, addr_type);
	ev->rssi = rssi;
	if (cfm_name)
		ev->flags |= __constant_cpu_to_le32(MGMT_DEV_FOUND_CONFIRM_NAME);
	if (!ssp)
		ev->flags |= __constant_cpu_to_le32(MGMT_DEV_FOUND_LEGACY_PAIRING);
>>>>>>> common/android-3.10.y

	BT_DBG("le: %d", le);

	memset(&ev, 0, sizeof(ev));

<<<<<<< HEAD
	bacpy(&ev.bdaddr, bdaddr);
	ev.rssi = rssi;
	ev.type = type;
	ev.le = le;

	if (dev_class)
		memcpy(ev.dev_class, dev_class, sizeof(ev.dev_class));
=======
	ev->eir_len = cpu_to_le16(eir_len);
	ev_size = sizeof(*ev) + eir_len;
>>>>>>> common/android-3.10.y

	if (eir && eir_len)
		memcpy(ev.eir, eir, eir_len);

	err = mgmt_event(MGMT_EV_DEVICE_FOUND, index, &ev, sizeof(ev), NULL);

	if (err < 0)
		return err;

	hdev = hci_dev_get(index);

<<<<<<< HEAD
	if (!hdev)
		return 0;
=======
	bacpy(&ev->addr.bdaddr, bdaddr);
	ev->addr.type = link_to_bdaddr(link_type, addr_type);
	ev->rssi = rssi;
>>>>>>> common/android-3.10.y

	if (hdev->disco_state == SCAN_IDLE)
		goto done;

<<<<<<< HEAD
	hdev->disco_int_count++;

	if (hdev->disco_int_count >= hdev->disco_int_phase) {
		/* Inquiry scan for General Discovery LAP */
		struct hci_cp_inquiry cp = {{0x33, 0x8b, 0x9e}, 4, 0};
		struct hci_cp_le_set_scan_enable le_cp = {0, 0};

		hdev->disco_int_phase *= 2;
		hdev->disco_int_count = 0;
		if (hdev->disco_state == SCAN_LE) {
			/* cancel LE scan */
			hci_send_cmd(hdev, HCI_OP_LE_SET_SCAN_ENABLE,
					sizeof(le_cp), &le_cp);
			/* start BR scan */
			cp.num_rsp = (u8) hdev->disco_int_phase;
			hci_send_cmd(hdev, HCI_OP_INQUIRY,
					sizeof(cp), &cp);
			hdev->disco_state = SCAN_BR;
			del_timer_sync(&hdev->disco_le_timer);
		}
	}
=======
	ev->eir_len = cpu_to_le16(eir_len);
>>>>>>> common/android-3.10.y

done:
	hci_dev_put(hdev);
	return 0;
}


int mgmt_remote_name(u16 index, bdaddr_t *bdaddr, u8 status, u8 *name)
{
	struct mgmt_ev_remote_name ev;

	memset(&ev, 0, sizeof(ev));

	bacpy(&ev.bdaddr, bdaddr);
	ev.status = status;
	memcpy(ev.name, name, HCI_MAX_NAME_LENGTH);

	return mgmt_event(MGMT_EV_REMOTE_NAME, index, &ev, sizeof(ev), NULL);
}

int mgmt_encrypt_change(u16 index, bdaddr_t *bdaddr, u8 status)
{
	struct mgmt_ev_encrypt_change ev;

	BT_DBG("hci%u", index);

	bacpy(&ev.bdaddr, bdaddr);
	ev.status = status;

	return mgmt_event(MGMT_EV_ENCRYPT_CHANGE, index, &ev, sizeof(ev),
									NULL);
}

int mgmt_remote_class(u16 index, bdaddr_t *bdaddr, u8 dev_class[3])
{
	struct mgmt_ev_remote_class ev;

	memset(&ev, 0, sizeof(ev));

	bacpy(&ev.bdaddr, bdaddr);
	memcpy(ev.dev_class, dev_class, 3);

	return mgmt_event(MGMT_EV_REMOTE_CLASS, index, &ev, sizeof(ev), NULL);
}

int mgmt_remote_version(u16 index, bdaddr_t *bdaddr, u8 ver, u16 mnf,
							u16 sub_ver)
{
	struct mgmt_ev_remote_version ev;

	memset(&ev, 0, sizeof(ev));

	bacpy(&ev.bdaddr, bdaddr);
	ev.lmp_ver = ver;
	ev.manufacturer = mnf;
	ev.lmp_subver = sub_ver;

	return mgmt_event(MGMT_EV_REMOTE_VERSION, index, &ev, sizeof(ev), NULL);
}

int mgmt_remote_features(u16 index, bdaddr_t *bdaddr, u8 features[8])
{
	struct mgmt_ev_remote_features ev;

	memset(&ev, 0, sizeof(ev));

	bacpy(&ev.bdaddr, bdaddr);
	memcpy(ev.features, features, sizeof(ev.features));

	return mgmt_event(MGMT_EV_REMOTE_FEATURES, index, &ev, sizeof(ev),
									NULL);
}
<<<<<<< HEAD
=======

module_param(enable_hs, bool, 0644);
MODULE_PARM_DESC(enable_hs, "Enable High Speed support");
>>>>>>> common/android-3.10.y
