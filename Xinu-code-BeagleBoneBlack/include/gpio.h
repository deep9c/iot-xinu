/* gpio.h */

/* GPIO memory-mapped I/O address */
#define GPIO_SETDATAOUT 0x194
#define GPIO_CLEARDATAOUT 0x190
//#define GPIO_BASE (0xB8000060+0x194)
#define GPIO_CONTROL_REG 0x44E10000
#define GPIO_BASE 0x4804C000

unsigned long *	m_controlModule;
unsigned long * m_gpio[4];

static enum
{
	kINPUT	,
	kOUTPUT 
} Beagle_GPIO_Direction;

typedef enum
{
	kFail 	= 0,
	kSuccess = 1
} Beagle_GPIO_Status;

struct gpioblk {
	unsigned long gpio_base_address;
	uint32 initialized;
	uint32 pin_num;
	uint32 direction;
	uint32 is_interrupt_enabled;
	struct gpioblk *next;
}*gpioblk;

extern struct gpioblk *gpio_blk_input;
extern struct gpioblk *gpio_blk_output;

struct gpiopinconfig{
  //uint32 _pin;
  uint32 _pinindex;
  uint32 direction;
  uint32 isInterruptEnabled;

} ;

extern struct gpiopinconfig gpioinitblk;
extern struct gpiopinconfig gpiotab[];

// GPIO Pins
static enum
	{
		P8_1,  P8_2,  P8_3,  P8_4,  P8_5,
		P8_6,  P8_7,  P8_8,  P8_9,  P8_10,
		P8_11, P8_12, P8_13, P8_14, P8_15,
		P8_16, P8_17, P8_18, P8_19, P8_20,
		P8_21, P8_22, P8_23, P8_24, P8_25,
		P8_26, P8_27, P8_28, P8_29, P8_30,
		P8_31, P8_32, P8_33, P8_34, P8_35,
		P8_36, P8_37, P8_38, P8_39, P8_40,
		P8_41, P8_42, P8_43, P8_44, P8_45,
		P8_46,
		P9_1,  P9_2,  P9_3,  P9_4,  P9_5,
		P9_6,  P9_7,  P9_8,  P9_9,  P9_10,
		P9_11, P9_12, P9_13, P9_14, P9_15,
		P9_16, P9_17, P9_18, P9_19, P9_20,
		P9_21, P9_22, P9_23, P9_24, P9_25,
		P9_26, P9_27, P9_28, P9_29, P9_30,
		P9_31, P9_32, P9_33, P9_34, P9_35,
		P9_36, P9_37, P9_38, P9_39, P9_40,
		P9_41, P9_42, P9_43, P9_44, P9_45,
		P9_46
	} GPIO_Pins;

static const int32 GPIO_Pin_Bank[] = 
{
	-1, -1,  1,  1,  1,	// P8_1  -> P8_5
	 1,  2,  2,  2,  2,	// P8_6  -> P8_10
	 1,  1,  0,  0,  1, 	// P8_11 -> P8_15
	 1,  0,  2,  0,  1, 	// P8_16 -> P8_20
	 1,  1,  1,  1,  1, 	// P8_21 -> P8_25
	 1,  2,  2,  2,  2, 	// P8_26 -> P8_30
	 0,  0,  0,  2,  0, 	// P8_31 -> P9_35
	 2,  2,  2,  2,  2, 	// P8_36 -> P8_40
	 2,  2,  2,  2,  2, 	// P8_41 -> P8_45
	 2,			// P8_46
	-1, -1, -1, -1, -1, 	// P9_1  -> P9_5
	-1, -1, -1, -1, -1, 	// P9_6  -> P9_10
	 0,  1,  0,  1,  1, 	// P9_11 -> P9_15
	 1,  0,  0,  0,  0,	// P9_16 -> P9_20
	 0,  0,  1,  0,  3, 	// P9_21 -> P9_25
	 0,  3,  3,  3,  3, 	// P9_26 -> P9_30
	 3, -1, -1, -1, -1, 	// P9_31 -> P9_35
	-1, -1, -1, -1, -1, 	// P9_36 -> P9_40
	 0,  0, -1, -1, -1, 	// P9_41 -> P9_45
	-1			// P9_46
};

static  int32 GPIO_Pin_Id[] = 
{
	-1, -1,  6,  7,  2,	// P8_1  -> P8_5
	 3,  2,  3,  5,  4,	// P8_6  -> P8_10
	13, 12, 23, 26, 15, 	// P8_11 -> P8_15
	14, 27,  1, 22, 31, 	// P8_16 -> P8_20
	30,  5,  4,  1,  0, 	// P8_21 -> P8_25
	29, 22, 24, 23, 25, 	// P8_26 -> P8_30
	10, 11,  9, 17,  8, 	// P8_31 -> P9_35
	16, 14, 15, 12, 13, 	// P8_36 -> P8_40
	10, 11,  8,  9,  6, 	// P8_41 -> P8_45
	 7,			// P8_46
	-1, -1, -1, -1, -1, 	// P9_1  -> P9_5
	-1, -1, -1, -1, -1,	// P9_6  -> P9_10
	30, 28, 31, 18, 16, 	// P9_11 -> P9_15
	19,  5,  4, 13, 12, 	// P9_16 -> P9_20
	 3,  2, 17, 15, 21, 	// P9_21 -> P9_25
	14, 19, 17, 15, 16, 	// P9_26 -> P9_30
	14, -1, -1, -1, -1, 	// P9_31 -> P9_35
	-1, -1, -1, -1, -1, 	// P9_36 -> P9_40
	20,  7, -1, -1, -1, 	// P9_41 -> P9_45
	-1			// P9_46
};

//=======================================================
//=======================================================

// Pad Control Register
static uint32 GPIO_Pad_Control[] =
{
	0x0000, 0x0000, 0x0818, 0x081C, 0x0808,	// P8_1  -> P8_5
	0x080C, 0x0890, 0x0894, 0x089C, 0x0898,	// P8_6  -> P8_10
	0x0834, 0x0830, 0x0824, 0x0828, 0x083C,	// P8_11 -> P8_15
	0x0838, 0x082C, 0x088C, 0x0820, 0x0884,	// P8_16 -> P8_20
	0x0880, 0x0814, 0x0810, 0x0804, 0x0800,	// P8_21 -> P8_25
	0x087C, 0x08E0, 0x08E8, 0x08E4, 0x08EC,	// P8_26 -> P8_30
	0x08D8, 0x08DC, 0x08D4, 0x08CC, 0x08D0,	// P8_31 -> P8_35
	0x08C8, 0x08C0, 0x08C4, 0x08B8, 0x08BC,	// P8_36 -> P8_40
	0x08B0, 0x08B4, 0x08A8, 0x08AC, 0x08A0,	// P8_41 -> P8_45
	0x08A4,					// P8_46
	0x0000, 0x0000, 0x0000, 0x0000, 0x0000,	// P9_1  -> P9_5
	0x0000, 0x0000, 0x0000, 0x0000, 0x0000,	// P9_6  -> P9_10
	0x0870, 0x0878, 0x0874, 0x0848, 0x0840,	// P9_11 -> P9_15
	0x084C, 0x095C, 0x0958, 0x097C, 0x0978,	// P9_16 -> P9_20
	0x0954, 0x0950, 0x0844, 0x0984, 0x09AC,	// P9_21 -> P9_25
	0x0980, 0x09A4, 0x099C, 0x0994, 0x0998,	// P9_26 -> P9_30
	0x0990, 0x0000, 0x0000, 0x0000, 0x0000,	// P9_31 -> P9_35
	0x0000, 0x0000, 0x0000, 0x0000, 0x0000,	// P9_36 -> P9_40
	0x09B4, 0x0964, 0x0000, 0x0000, 0x0000,	// P9_41 -> P9_45
	0x0000					// P9_46
};



static uint32 GPIO_Base[] = 
{
	0x44E07000,	// GPIO0
	0x4804C000,	// GPIO1
	0x481AC000,	// GPIO2
	0x481AE000	// GPIO3
};

static	enum
	{
		kREVISION		= 0x0,
		kSYSCONFIG		= 0x10,
		kIRQSTATUS_RAW_0	= 0x24,
		kIRQSTATUS_RAW_1	= 0x28,
		kIRQSTATUS_0		= 0x2C,
		kIRQSTATUS_1		= 0x30,
		kIRQSTATUS_SET_0	= 0x34,
		kIRQSTATUS_SET_1	= 0x38,
		kIRQSTATUS_CLR_0	= 0x3C,
		kIRQSTATUS_CLR_1	= 0x40,
		kIRQWAKEN_0		= 0x44,
		kIRQWAKEN_1		= 0x48,
		kSYSSTATUS		= 0x114,
		kCTRL			= 0x130,
		kOE			= 0x134,
		kDATAIN			= 0x138,
		kDATAOUT		= 0x13C,
		kLEVELDETECT0		= 0x140,
		kLEVELDETECT1		= 0x144,
		kRISINGDETECT		= 0x148,
		kFALLINGDETECT		= 0x14C,
		kDEBOUNCEENABLE		= 0x150,
		kDEBOUNCINGTIME		= 0x154,
		kCLEARDATAOUT		= 0x190,
		kSETDATAOUT		= 0x194	
	} Beagle_GPIO_Registers;



/* GPIO bits for pin direction */
#define GPIO_DIR_IN        0x00 /**< set pin for input                  */
#define GPIO_DIR_OUT       0x01 /**< set pin for output                 */

/* GPIO bit flags for pins */
#define GPIO0              0x01
#define GPIO1              0x02
#define GPIO2              0x04
#define GPIO3              0x08
#define GPIO4              0x10
#define GPIO5              0x20
#define GPIO6              0x40
#define GPIO7              0x80

#define GPIO_PIN_COUNT     8

/* Buttons */
#define GPIO_BUT_CISCO    GPIO4 /**< Front Cisco button                 */
#define GPIO_BUT_RESET    GPIO6 /**< Back reset button                  */

/* LEDs */
#define GPIO_LED_WLAN     GPIO0 /**< WLAN LED                           */
#define GPIO_LED_POWER    GPIO1 /**< Power LED (hardware controlled)    */
#define GPIO_LED_CISCOWHT GPIO2 /**< White Cisco LED                    */
#define GPIO_LED_CISCOONG GPIO3 /**< Orange Cisco LED                   */
#define GPIO_LED_DMZ      GPIO7 /**< DMZ LED                            */

/**
 * Control and status registers for the GPIO.
 */
struct gpio_csreg
{
    volatile uint32 gp_input;       /**< input                              */
    volatile uint32 gp_output;      /**< output                             */
    volatile uint32 gp_enable;      /**< direction                          */
    volatile uint32 gp_control;     /**< usage unkmown                      */
};


/* LED control functions */

/**
 * Turns an LED on
 * @param n GPIO bit for LED (use GPIO_LED_* constants)
 */
#define gpioLEDOn(n) ((struct gpio_csreg *)GPIO_BASE)->gp_enable |= (n); \
		((struct gpio_csreg *)GPIO_BASE)->gp_output &= ~(n)

/**
 * Turns an LED off
 * @param n GPIO bit for LED (use GPIO_LED_* constants)
 */
#define gpioLEDOff(n) ((struct gpio_csreg *)GPIO_BASE)->gp_enable &= ~(n)
