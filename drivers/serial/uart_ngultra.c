/*
 * Copyright (c) 2022 NanoXplore
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#define DT_DRV_COMPAT nanoxplore_ngultra_uart

/**
 * @brief Driver for UART on NanoXplore processors
 */

#include <drivers/uart.h>
#include "uart_ngultra.h"

struct ngultra_uart_config {
	mm_reg_t base;
	uint32_t baud_rate;
	uint32_t freq;
	uint8_t modem_mode;
};

/**
 * @brief Initialize the UART channel
 *
 * Configure the UART depending on the Clock SLOW frequency.
 *
 * @param dev UART device struct
 *
 * @return 0
 */
static int uart_nx_ngultra_init(const struct device *dev)
{
	const struct ngultra_uart_config *cfg = dev->config;
	uint32_t ctrl = 0;
	uint32_t scaler = 0;

	ctrl = sys_read32(cfg->base + UART_CTRL);
	ctrl |= (BIT(UART_CTRL_RXEN) | BIT(UART_CTRL_SB));
	ctrl &= ~(BIT(UART_CTRL_PE) | BIT(UART_CTRL_FL) | BIT(UART_CTRL_LB));
	sys_write32(ctrl, cfg->base + UART_CTRL);

	scaler = (cfg->freq / (cfg->baud_rate * 8)) - 1;
	scaler &= UART_SCALER_RELOAD_VAL_MASK;
	sys_write32(scaler, cfg->base + UART_SCALER);

	return 0;
}

static void uart_nx_ngultra_poll_out(const struct device *dev, unsigned char c)
{
	const struct ngultra_uart_config *cfg = dev->config;
	uint32_t status;

	sys_set_bit(cfg->base + UART_CTRL, UART_CTRL_TXEN);

	do {
		status = sys_read32(cfg->base + UART_STATUS);
	} while (status & BIT(UART_STATUS_TF));

	sys_write8(c, cfg->base + UART_DATA);
}

static const struct uart_driver_api uart_nx_ngultra_driver_api = {
	.poll_out = uart_nx_ngultra_poll_out,
};

#define UART_NX_NGULTRA_DECLARE_CFG(n)					\

#define UART_NX_NGULTRA_INIT_CFG(n)					\
	static const struct ngultra_uart_config ngultra_uart_##n##_config = {	\
		.base = DT_INST_REG_ADDR(n),		\
		.baud_rate = DT_INST_PROP(n, current_speed),		\
		.freq = DT_INST_PROP(n, clock_frequency)		\
	}

#define UART_NX_NGULTRA_INIT(n)						\
	static const struct ngultra_uart_config ngultra_uart_##n##_config;	\
									\
	DEVICE_DT_INST_DEFINE(n, &uart_nx_ngultra_init, NULL,		\
			NULL, &ngultra_uart_##n##_config,		\
			PRE_KERNEL_1,					\
			CONFIG_SERIAL_INIT_PRIORITY,			\
			&uart_nx_ngultra_driver_api);			\
									\
	UART_NX_NGULTRA_INIT_CFG(n);

DT_INST_FOREACH_STATUS_OKAY(UART_NX_NGULTRA_INIT)
