# SPDX-License-Identifier: GPL-2.0

subdir-ccflags-y += -I$(srctree)/drivers/misc/mediatek/typec/inc

ifeq ($(CONFIG_AGATE_CHARGER),y)
obj-$(CONFIG_TCPC_CLASS) += tcpc_agate/
else
obj-$(CONFIG_TCPC_CLASS) += tcpc/
endif
obj-$(CONFIG_MTK_USB_TYPEC_U3_MUX) += switch/
obj-$(CONFIG_OCP96011_I2C) += switch/
obj-$(CONFIG_MTK_USB_TYPEC_DP_MUX) += dp/
obj-$(CONFIG_MTK_USB_TYPEC_DP_MUX_V2) += dp/