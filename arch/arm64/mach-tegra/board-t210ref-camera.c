/*
 * arch/arm/mach-tegra/board-t210ref-sensors.c
 *
 * Copyright (c) 2013-2015, NVIDIA CORPORATION.  All rights reserved.
 *
 * This program is free software; you can redistribute it and/or modify it
 * under the terms and conditions of the GNU General Public License,
 * version 2, as published by the Free Software Foundation.
 *
 * This program is distributed in the hope it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
 * FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License for
 * more details.
 */

#include <linux/i2c.h>
#include <linux/platform_device.h>
#include <media/imx214.h>
#include <media/ov5693.h>
#include <media/ov5647.h>
#include <media/soc_camera.h>
#include <media/soc_camera_platform.h>
#include <media/tegra_v4l2_camera.h>
#include <media/camera_common.h>
#include "board-t210.h"
/*
 * Soc Camera platform driver for testing
 */
#if IS_ENABLED(CONFIG_SOC_CAMERA_PLATFORM)
static struct platform_device *t210ref_pdev;
static int t210ref_soc_camera_add(struct soc_camera_device *icd);
static void t210ref_soc_camera_del(struct soc_camera_device *icd);

static int t210ref_soc_camera_set_capture(struct soc_camera_platform_info *info,
		int enable)
{
	/* TODO: probably add clk opertaion here */
	return 0; /* camera sensor always enabled */
}

static struct soc_camera_platform_info t210ref_soc_camera_info = {
	.format_name = "RGB4",
	.format_depth = 32,
	.format = {
		.code = V4L2_MBUS_FMT_RGBA8888_4X8_LE,
		.colorspace = V4L2_COLORSPACE_SRGB,
		.field = V4L2_FIELD_NONE,
		.width = 1280,
		.height = 720,
	},
	.set_capture = t210ref_soc_camera_set_capture,
};

static struct tegra_camera_platform_data t210ref_camera_platform_data = {
	.port                   = TEGRA_CAMERA_PORT_CSI_A,
	.lanes                  = 4,
};

static struct soc_camera_link t210ref_soc_camera_link = {
	.bus_id         = 0, /* This must match the .id of tegra_vi01_device */
	.add_device     = t210ref_soc_camera_add,
	.del_device     = t210ref_soc_camera_del,
	.module_name    = "soc_camera_platform",
	.priv		= &t210ref_camera_platform_data,
	.dev_priv	= &t210ref_soc_camera_info,
};

static void t210ref_soc_camera_release(struct device *dev)
{
	soc_camera_platform_release(&t210ref_pdev);
}

static int t210ref_soc_camera_add(struct soc_camera_device *icd)
{
	return soc_camera_platform_add(icd, &t210ref_pdev,
			&t210ref_soc_camera_link,
			t210ref_soc_camera_release, 0);
}

static void t210ref_soc_camera_del(struct soc_camera_device *icd)
{
	soc_camera_platform_del(icd, t210ref_pdev, &t210ref_soc_camera_link);
}

static struct platform_device t210ref_soc_camera_device = {
	.name   = "soc-camera-pdrv",
	.dev    = {
		.platform_data = &t210ref_soc_camera_link,
	},
};
#endif

#if IS_ENABLED(CONFIG_SOC_CAMERA_IMX214)
static int t210ref_imx214_power(struct device *dev, int enable)
{
	return 0;
}

static struct camera_common_pdata t210ref_imx214_data;

static struct i2c_board_info t210ref_imx214_camera_i2c_device = {
	I2C_BOARD_INFO("imx214_v4l2", 0x1a),
};

static struct tegra_camera_platform_data t210ref_imx214_camera_platform_data = {
	.flip_v			= 0,
	.flip_h			= 0,
	.port			= TEGRA_CAMERA_PORT_CSI_A,
	.lanes			= 4,
	.continuous_clk		= 0,
};

static struct soc_camera_link imx214_iclink = {
	.bus_id		= 0, /* This must match the .id of tegra_vi01_device */
	.board_info	= &t210ref_imx214_camera_i2c_device,
	.module_name	= "imx214_v4l2",
	.i2c_adapter_id	= 6, /* VI2 I2C controller */
	.power		= t210ref_imx214_power,
	.priv		= &t210ref_imx214_camera_platform_data,
	.dev_priv	= &t210ref_imx214_data,
};

static struct platform_device t210ref_imx214_soc_camera_device = {
	.name	= "soc-camera-pdrv",
	.id	= 0,
	.dev	= {
		.platform_data = &imx214_iclink,
	},
};
#endif

#if IS_ENABLED(CONFIG_VIDEO_IMX219)
static int t210ref_imx219_power(struct device *dev, int enable)
{
	return 0;
}

static struct camera_common_pdata t210ref_imx219_data;

static struct i2c_board_info t210ref_imx219_camera_i2c_device = {
	I2C_BOARD_INFO("imx219", 0x10),
};

//Bottom
static struct tegra_camera_platform_data t210ref_imx219_camera_bottom_platform_data = {
	.flip_v			      = 0,
	.flip_h			      = 0,
	.port			        = TEGRA_CAMERA_PORT_CSI_A,
	.lanes			      = 2,
	.continuous_clk		= 0,
};

static struct soc_camera_link imx219_bottom_iclink = {
	.bus_id		      = 0, /* This must match the .id of tegra_vi01_device */
	.board_info	    = &t210ref_imx219_camera_i2c_device,
	.module_name	  = "imx219",
	.i2c_adapter_id	= 7, /* VI2 I2C controller */
	.power		      = t210ref_imx219_power,
	.priv		        = &t210ref_imx219_camera_bottom_platform_data,
	.dev_priv	      = &t210ref_imx219_data,
};

static struct platform_device t210ref_imx219_bottom_soc_camera_device = {
	.name	= "soc-camera-pdrv",
	.id	= 0,
	.dev	= {
		.platform_data = &imx219_bottom_iclink,
	},
};

//Front Left Camera
static struct tegra_camera_platform_data t210ref_imx219_camera_front_right_platform_data = {
	.flip_v			      = 0,
	.flip_h			      = 0,
	.port			        = TEGRA_CAMERA_PORT_CSI_C,
	.lanes			      = 2,
	.continuous_clk		= 0,
};

static struct soc_camera_link imx219_front_right_iclink = {
	.bus_id		      = 1, /* This must match the .id of tegra_vi01_device */
	.board_info	    = &t210ref_imx219_camera_i2c_device,
	.module_name	  = "imx219",
	.i2c_adapter_id	= 8, /* VI2 I2C controller */
	.power		      = t210ref_imx219_power,
	.priv		        = &t210ref_imx219_camera_front_right_platform_data,
	.dev_priv	      = &t210ref_imx219_data,
};

static struct platform_device t210ref_imx219_front_right_soc_camera_device = {
	.name	= "soc-camera-pdrv",
	.id	= 1,
	.dev	= {
		.platform_data = &imx219_front_right_iclink,
	},
};

//Front Right Camera
static struct tegra_camera_platform_data t210ref_imx219_camera_front_left_platform_data = {
	.flip_v			      = 0,
	.flip_h			      = 0,
	.port			        = TEGRA_CAMERA_PORT_CSI_E,
	.lanes			      = 2,
	.continuous_clk		= 0,
};

static struct soc_camera_link imx219_front_left_iclink = {
	.bus_id		      = 2, /* This must match the .id of tegra_vi01_device */
	.board_info	    = &t210ref_imx219_camera_i2c_device,
	.module_name	  = "imx219",
	.i2c_adapter_id	= 9, /* VI2 I2C controller */
	.power		      = t210ref_imx219_power,
	.priv		        = &t210ref_imx219_camera_front_left_platform_data,
	.dev_priv	      = &t210ref_imx219_data,
};

static struct platform_device t210ref_imx219_front_left_soc_camera_device = {
	.name	= "soc-camera-pdrv",
	.id	= 2,
	.dev	= {
		.platform_data = &imx219_front_left_iclink,
	},
};





#endif

#if IS_ENABLED(CONFIG_VIDEO_OV5647)
static int t210ref_ov5647_power(struct device *dev, int enable)
{
	return 0;
}

static struct camera_common_pdata t210ref_ov5647_data;

static struct i2c_board_info t210ref_ov5647_camera_i2c_device = {
	I2C_BOARD_INFO("ov5647", 0x36),
};

static struct tegra_camera_platform_data t210ref_ov5647_camera_platform_data = {
	.flip_v			      = 0,
	.flip_h			      = 0,
	.port			        = TEGRA_CAMERA_PORT_CSI_A,
	.lanes			      = 2,
	.continuous_clk		= 0,
};

static struct soc_camera_link ov5647_iclink = {
	.bus_id		      = 0, /* This must match the .id of tegra_vi01_device */
	.board_info	    = &t210ref_ov5647_camera_i2c_device,
	.module_name	  = "ov5647",
	.i2c_adapter_id	= 6, /* VI2 I2C controller */
	.power		      = t210ref_ov5647_power,
	.priv		        = &t210ref_ov5647_camera_platform_data,
	.dev_priv	      = &t210ref_ov5647_data,
};

static struct platform_device t210ref_ov5647_bottom_soc_camera_device = {
	.name	= "soc-camera-pdrv",
	.id	= 0,
	.dev	= {
		.platform_data = &ov5647_iclink,
	},
};

#endif








#if IS_ENABLED(CONFIG_SOC_CAMERA_OV5693)
static int t210ref_ov5693_power(struct device *dev, int enable)
{
	return 0;
}

/* Jetson Erista camera board E2149 with dual OV5693 */
static struct camera_common_pdata t210ref_ov5693_rear_data = {
	.regulators = {
		.avdd = "vana",
		.iovdd = "vif",
	},
	.pwdn_gpio = 151, /* TEGRA_GPIO_PS7 */
};

static struct i2c_board_info t210ref_ov5693_rear_camera_i2c_device = {
	I2C_BOARD_INFO("ov5693_v4l2", 0x10),
};

static struct tegra_camera_platform_data
t210ref_ov5693_rear_camera_platform_data = {
	.flip_v			= 0,
	.flip_h			= 0,
	.port			= TEGRA_CAMERA_PORT_CSI_A,
	.lanes			= 2,
	.continuous_clk		= 0,
};

static struct soc_camera_link ov5693_rear_iclink = {
	.bus_id		= 0, /* This must match the .id of tegra_vi01_device */
	.board_info	= &t210ref_ov5693_rear_camera_i2c_device,
	.module_name	= "ov5693_v4l2",
	.i2c_adapter_id	= 6, /* VI2 I2C controller */
	.power		= t210ref_ov5693_power,
	.priv		= &t210ref_ov5693_rear_camera_platform_data,
	.dev_priv	= &t210ref_ov5693_rear_data,
};

static struct platform_device t210ref_ov5693_rear_soc_camera_device = {
	.name	= "soc-camera-pdrv",
	.id	= 0,
	.dev	= {
		.platform_data = &ov5693_rear_iclink,
	},
};

static struct camera_common_pdata t210ref_ov5693_front_data = {
	.regulators = {
		.avdd = "vana",
		.iovdd = "vif",
	},
	.pwdn_gpio = 152, /* TEGRA_GPIO_PT0 */
};

static struct i2c_board_info t210ref_ov5693_front_camera_i2c_device = {
	I2C_BOARD_INFO("ov5693_v4l2", 0x36),
};

static struct tegra_camera_platform_data
t210ref_ov5693_front_camera_platform_data = {
	.flip_v			= 0,
	.flip_h			= 0,
	.port			= TEGRA_CAMERA_PORT_CSI_C,
	.lanes			= 2,
	.continuous_clk		= 0,
};

static struct soc_camera_link ov5693_front_iclink = {
	.bus_id		= 0, /* This must match the .id of tegra_vi01_device */
	.board_info	= &t210ref_ov5693_front_camera_i2c_device,
	.module_name	= "ov5693_v4l2",
	.i2c_adapter_id	= 6, /* VI2 I2C controller */
	.power		= t210ref_ov5693_power,
	.priv		= &t210ref_ov5693_front_camera_platform_data,
	.dev_priv	= &t210ref_ov5693_front_data,
};

static struct platform_device t210ref_ov5693_front_soc_camera_device = {
	.name	= "soc-camera-pdrv",
	.id	= 1,
	.dev	= {
		.platform_data = &ov5693_front_iclink,
	},
};

/* T210 ERS (E2220-1170) with six camera sensor board E2128 */
static struct i2c_board_info t210ref_ov5693_camera_i2c_device = {
	I2C_BOARD_INFO("ov5693_v4l2", 0x36),
};

static struct camera_common_pdata t210ref_ov5693_ab_data = {
	.regulators = {
		.avdd = "vana",
		.iovdd = "vif",
	},
	.use_cam_gpio = 1,
	.pwdn_gpio = 151, /* TEGRA_GPIO_PS7 */
};

static struct camera_common_pdata t210ref_ov5693_cd_data = {
	.regulators = {
		.avdd = "vana",
		.iovdd = "vif",
	},
	.use_cam_gpio = 1,
	.pwdn_gpio = 152, /* TEGRA_GPIO_PT0 */
};

/* OV5693 on CSI A */
static struct tegra_camera_platform_data
t210ref_ov5693_a_camera_platform_data = {
	.flip_v			= 0,
	.flip_h			= 0,
	.port			= TEGRA_CAMERA_PORT_CSI_A,
	.lanes			= 2,
	.continuous_clk		= 0,
};

static struct soc_camera_link ov5693_a_iclink = {
	.bus_id		= 0, /* This must match the .id of tegra_vi01_device */
	.board_info	= &t210ref_ov5693_camera_i2c_device,
	.module_name	= "ov5693_v4l2",
	.i2c_adapter_id	= 33, /* VI2 I2C controller */
	.power		= t210ref_ov5693_power,
	.priv		= &t210ref_ov5693_a_camera_platform_data,
	.dev_priv	= &t210ref_ov5693_ab_data,
};

static struct platform_device t210ref_ov5693_a_soc_camera_device = {
	.name	= "soc-camera-pdrv",
	.id	= 0,
	.dev	= {
		.platform_data = &ov5693_a_iclink,
	},
};

/* OV5693 on CSI B */
static struct tegra_camera_platform_data
t210ref_ov5693_b_camera_platform_data = {
	.flip_v			= 0,
	.flip_h			= 0,
	.port			= TEGRA_CAMERA_PORT_CSI_B,
	.lanes			= 2,
	.continuous_clk		= 0,
};

static struct soc_camera_link ov5693_b_iclink = {
	.bus_id		= 0, /* This must match the .id of tegra_vi01_device */
	.board_info	= &t210ref_ov5693_camera_i2c_device,
	.module_name	= "ov5693_v4l2",
	.i2c_adapter_id	= 31, /* VI2 I2C controller */
	.power		= t210ref_ov5693_power,
	.priv		= &t210ref_ov5693_b_camera_platform_data,
	.dev_priv	= &t210ref_ov5693_ab_data,
};

static struct platform_device t210ref_ov5693_b_soc_camera_device = {
	.name	= "soc-camera-pdrv",
	.id	= 1,
	.dev	= {
		.platform_data = &ov5693_b_iclink,
	},
};

/* OV5693 on CSI C */
static struct tegra_camera_platform_data
t210ref_ov5693_c_camera_platform_data = {
	.flip_v			= 0,
	.flip_h			= 0,
	.port			= TEGRA_CAMERA_PORT_CSI_C,
	.lanes			= 2,
	.continuous_clk		= 0,
};

static struct soc_camera_link ov5693_c_iclink = {
	.bus_id		= 0, /* This must match the .id of tegra_vi01_device */
	.board_info	= &t210ref_ov5693_camera_i2c_device,
	.module_name	= "ov5693_v4l2",
	.i2c_adapter_id	= 32, /* VI2 I2C controller */
	.power		= t210ref_ov5693_power,
	.priv		= &t210ref_ov5693_c_camera_platform_data,
	.dev_priv	= &t210ref_ov5693_cd_data,
};

static struct platform_device t210ref_ov5693_c_soc_camera_device = {
	.name	= "soc-camera-pdrv",
	.id	= 2,
	.dev	= {
		.platform_data = &ov5693_c_iclink,
	},
};

/* OV5693 on CSI D */
static struct tegra_camera_platform_data
t210ref_ov5693_d_camera_platform_data = {
	.flip_v			= 0,
	.flip_h			= 0,
	.port			= TEGRA_CAMERA_PORT_CSI_D,
	.lanes			= 2,
	.continuous_clk		= 0,
};

static struct soc_camera_link ov5693_d_iclink = {
	.bus_id		= 0, /* This must match the .id of tegra_vi01_device */
	.board_info	= &t210ref_ov5693_camera_i2c_device,
	.module_name	= "ov5693_v4l2",
	.i2c_adapter_id	= 30, /* VI2 I2C controller */
	.power		= t210ref_ov5693_power,
	.priv		= &t210ref_ov5693_d_camera_platform_data,
	.dev_priv	= &t210ref_ov5693_cd_data,
};

static struct platform_device t210ref_ov5693_d_soc_camera_device = {
	.name	= "soc-camera-pdrv",
	.id	= 3,
	.dev	= {
		.platform_data = &ov5693_d_iclink,
	},
};

/* OV5693 on CSI E */
static struct camera_common_pdata t210ref_ov5693_e_data = {
	.mclk_name = "cam_mclk3",
	.regulators = {
		.avdd = "vana",
		.iovdd = "vif",
	},
	.pwdn_gpio = 203, /* TEGRA_GPIO_PZ3 */
};

static struct tegra_camera_platform_data
t210ref_ov5693_e_camera_platform_data = {
	.flip_v			= 0,
	.flip_h			= 0,
	.port			= TEGRA_CAMERA_PORT_CSI_E,
	.lanes			= 2,
	.continuous_clk		= 0,
};

static struct soc_camera_link ov5693_e_iclink = {
	.bus_id		= 0, /* This must match the .id of tegra_vi01_device */
	.board_info	= &t210ref_ov5693_camera_i2c_device,
	.module_name	= "ov5693_v4l2",
	.i2c_adapter_id	= 35, /* VI2 I2C controller */
	.power		= t210ref_ov5693_power,
	.priv		= &t210ref_ov5693_e_camera_platform_data,
	.dev_priv	= &t210ref_ov5693_e_data,
};

static struct platform_device t210ref_ov5693_e_soc_camera_device = {
	.name	= "soc-camera-pdrv",
	.id	= 4,
	.dev	= {
		.platform_data = &ov5693_e_iclink,
	},
};

/* OV5693 on CSI F */
static struct camera_common_pdata t210ref_ov5693_f_data = {
	.mclk_name = "cam_mclk3",
	.regulators = {
		.avdd = "vana",
		.iovdd = "vif",
	},
	.pwdn_gpio = 153, /* TEGRA_GPIO_PT1 */
};

static struct tegra_camera_platform_data
t210ref_ov5693_f_camera_platform_data = {
	.flip_v			= 0,
	.flip_h			= 0,
	.port			= TEGRA_CAMERA_PORT_CSI_F,
	.lanes			= 2,
	.continuous_clk		= 0,
};

static struct soc_camera_link ov5693_f_iclink = {
	.bus_id		= 0, /* This must match the .id of tegra_vi01_device */
	.board_info	= &t210ref_ov5693_camera_i2c_device,
	.module_name	= "ov5693_v4l2",
	.i2c_adapter_id	= 34, /* VI2 I2C controller */
	.power		= t210ref_ov5693_power,
	.priv		= &t210ref_ov5693_f_camera_platform_data,
	.dev_priv	= &t210ref_ov5693_f_data,
};

static struct platform_device t210ref_ov5693_f_soc_camera_device = {
	.name	= "soc-camera-pdrv",
	.id	= 5,
	.dev	= {
		.platform_data = &ov5693_f_iclink,
	},
};

/* OV5693 on CSI C of E3326 */
static struct camera_common_pdata t210ref_ov5693_e3326_data = {
	.regulators = {
		.avdd = "vana",
		.iovdd = "vif",
	},
	.reset_gpio = 148, /* TEGRA_GPIO_PS4 */
	.pwdn_gpio = 151, /* TEGRA_GPIO_PS7 */
};

static struct i2c_board_info t210ref_ov5693_e3326_camera_i2c_device = {
	I2C_BOARD_INFO("ov5693_v4l2", 0x36),
};

static struct tegra_camera_platform_data
t210ref_ov5693_e3326_camera_platform_data = {
	.flip_v			= 0,
	.flip_h			= 0,
	.port			= TEGRA_CAMERA_PORT_CSI_C,
	.lanes			= 2,
	.continuous_clk		= 0,
};

static struct soc_camera_link ov5693_e3326_iclink = {
	.bus_id		= 0, /* This must match the .id of tegra_vi01_device */
	.board_info	= &t210ref_ov5693_e3326_camera_i2c_device,
	.module_name	= "ov5693_v4l2",
	.i2c_adapter_id	= 6, /* VI2 I2C controller */
	.power		= t210ref_ov5693_power,
	.priv		= &t210ref_ov5693_e3326_camera_platform_data,
	.dev_priv	= &t210ref_ov5693_e3326_data,
};

static struct platform_device t210ref_ov5693_e3326_soc_camera_device = {
	.name	= "soc-camera-pdrv",
	.id	= 0,
	.dev	= {
		.platform_data = &ov5693_e3326_iclink,
	},
};
#endif

int t210ref_camera_init(void)
{
	pr_debug("%s: ++\n", __func__);
	//pr_info("%s: Entered\n", __func__);
#if IS_ENABLED(CONFIG_SOC_CAMERA_PLATFORM)
	platform_device_register(&t210ref_soc_camera_device);
#endif

#if IS_ENABLED(CONFIG_VIDEO_IMX219)
  if (of_machine_is_compatible("nvidia,mit-uav-reva") || 
      of_machine_is_compatible("nvidia,mit-uav-revb")) {
    pr_info("%s: MIT UAV REVA/B Detected\n", __func__);

		//Register all camera
    if (of_find_node_by_name(NULL, "imx219_a"))
    {
      pr_info("%s: Detected IMX219 A in Device Tree\n", __func__);
		  platform_device_register(&t210ref_imx219_bottom_soc_camera_device);
    }
    else
      pr_warning("%s: Did not find imx219_a in device tree\n", __func__);


   if (of_find_node_by_name(NULL, "imx219_c"))
    {
      pr_info("%s: Detected imx219_c in Device Tree\n", __func__);
		  platform_device_register(&t210ref_imx219_front_right_soc_camera_device);
    }
    else
      pr_warning("%s: Did not find imx219_c in device tree\n", __func__);


   if (of_find_node_by_name(NULL, "imx219_e"))
    {
      pr_info("%s: Detected imx219_e in Device Tree\n", __func__);
		  platform_device_register(&t210ref_imx219_front_left_soc_camera_device);
    }
    else
      pr_warning("%s: Did not find imx219_e in device tree\n", __func__);
	}

  if (of_machine_is_compatible("nvidia,jetson-cv")){
    if (of_find_node_by_name(NULL, "imx219_a"))
    {
      pr_info("%s: Detected IMX219 A in Device Tree\n", __func__);
		  platform_device_register(&t210ref_imx219_bottom_soc_camera_device);
    }
    else
      pr_warning("%s: Did not find imx219_a in device tree\n", __func__);


  }

#endif
#if IS_ENABLED(CONFIG_VIDEO_OV5647)
  if (of_machine_is_compatible("nvidia,mit-uav-reva") ||
      of_machine_is_compatible("nvidia,mit-uav-revb")) {
    //pr_info("%s: MIT UAV REVA Detected\n", __func__);
    pr_info("%s: MIT UAV REVA/B Detected\n", __func__);

		//Register all camera

		//Register Front Left Camera
		//platform_device_register(&t210ref_ov5647_front_right_soc_camera_device);

		//Register Front Right Camera
		//platform_device_register(&t210ref_ov5647_front_left_soc_camera_device);

		//Register Bottom Camera
    if (of_find_node_by_name(NULL, "ov5647_a"))
    {
      pr_info("%s: Detected OV5647 in Device Tree\n", __func__);
		  platform_device_register(&t210ref_ov5647_bottom_soc_camera_device);
    }
    else
      pr_warning("%s: Did not find ov5647_a in device tree\n", __func__);
	}

#endif




#if IS_ENABLED(CONFIG_SOC_CAMERA_IMX214)
	platform_device_register(&t210ref_imx214_soc_camera_device);
#endif


#if IS_ENABLED(CONFIG_SOC_CAMERA_OV5693)
	if (of_machine_is_compatible("nvidia,jetson-e")) {
		platform_device_register(
				&t210ref_ov5693_rear_soc_camera_device);
		platform_device_register(
				&t210ref_ov5693_front_soc_camera_device);
	} else if (of_machine_is_compatible("nvidia,jetson-cv")) {
		platform_device_register(
				&t210ref_ov5693_e3326_soc_camera_device);
	}
	else if (of_machine_is_compatible("nvidia,e2220")) {
		platform_device_register(&t210ref_ov5693_a_soc_camera_device);
		platform_device_register(&t210ref_ov5693_b_soc_camera_device);
		platform_device_register(&t210ref_ov5693_c_soc_camera_device);
		platform_device_register(&t210ref_ov5693_d_soc_camera_device);
		platform_device_register(&t210ref_ov5693_e_soc_camera_device);
		platform_device_register(&t210ref_ov5693_f_soc_camera_device);
	}
#endif

	return 0;
}
