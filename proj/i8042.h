#ifndef I8042_H
#define I8042_H

//Macros and constants for programming the i8042

//#define BIT(n)		(0x01 <<(n))

#define KBD_IRQ   1
#define KBD_HOOK_BIT	1
#define MOUSE_IRQ   12
#define MOUSE_HOOK_BIT    2

#define DELAY_US	20000
#define TIMEOUT		3
#define MAX_UINT_16	0xFFFF
#define KBD_ERROR	0xFFFF
#define ENABLED		0x01
#define DISABLED	0X00


//I8042 Ports

#define STATUS_PORT	0x64 	//for reading
#define KBD_OUT_BUF	0x60
#define KBD_IN_BUF 0x60
#define KBD_CMD_BUF	0x64	//for writing
#define KBD_DATA_BUF	0x60


//Status register byte

#define OUT_BUF_STATUS	BIT(0)
#define	IN_BUF_STATUS	BIT(1)
#define SYS_FLAG	BIT(2)
#define CMD_DATA	BIT(3)
#define TIME_OUT_ERROR	BIT(6)
#define PARITY_ERROR	BIT(7)


//Controller Commands (write to port 0x64)

#define	FIRST_PS2_ENABLE	0xAE
#define FIRST_PS2_DISABLE	0xAD
#define KBC_READ_CMD		0x20
#define KBC_WRITE_CMD		0x60


//Keyboard Commands (write to port 0x60)

#define	KBD_ENABLE		0xF4
#define	KBD_DISABLE		0xF5

//PS/2 Mouse Commands (write to port 0x60)
#define ENABLE_DATA			0xF4
#define DISABLE_DATA		0xF5
#define STATUS_REQUEST		0xE9
#define STREAM_MODE			0xEA
#define REMOTE_MODE			0xF0
#define READ_DATA			0xEB
#define KBC_MOUSE_COM 0xD4

//PS/2 Mouse Packets
#define SYNC_BIT			BIT(3)
#define MOUSE_LB			BIT(0)
#define MOUSE_RB			BIT(1)
#define MOUSE_MB			BIT(2)
#define MOUSE_XS			BIT(4)
#define MOUSE_YS			BIT(5)
#define MOUSE_XOV			BIT(6)
#define MOUSE_YOV			BIT(7)

//Keyboard Responses

#define	KBD_ACK			0xFA
#define MOUSE_ACK   0xFA
#define KBD_RESEND		0XFE


//Scan Code Set 1


#define	ESC_BREAK		0x81
#define TWOB_CODE		0xE0	//Prefix of 2 Byte Scancodes

#define W_MAKE        0x11
#define W_BREAK       0x91
#define A_MAKE        0x1E
#define A_BREAK       0x9E
#define S_MAKE        0x1F
#define S_BREAK       0x9F
#define D_MAKE        0x20
#define D_BREAK       0xA0



#endif	//I8042_H


