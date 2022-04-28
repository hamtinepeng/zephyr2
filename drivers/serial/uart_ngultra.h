#ifndef __UART_NX_NGULTRA_H__
#define __UART_NX_NGULTRA_H__

#define UART_DATA          0x0

#define UART_STATUS        0x4
#define UART_STATUS_DR     0x0 /* Data Ready */
#define UART_STATUS_RE     0x7 /* Receiver buffer empty */
#define UART_STATUS_TF     0x8 /* Transmitter buffer full */

#define UART_CTRL          0x8
#define UART_CTRL_RXEN     0x0 /* Receiver enable */
#define UART_CTRL_TXEN     0x1 /* Transmiter enable */
#define UART_CTRL_PS       0x4 /* Parity Select */
#define UART_CTRL_PE       0x5 /* Parity enable */
#define UART_CTRL_FL       0x6 /* Transmiter enable */
#define UART_CTRL_LB       0x7 /* internal LoopBack */
#define UART_CTRL_SB       0x8 /* Stop Bit */

#define UART_SCALER                 0xC
#define UART_SCALER_RELOAD_VAL_MASK 0xFFF

#endif /* __UART_NX_NGULTRA_H__ */
