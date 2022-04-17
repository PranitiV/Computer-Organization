/* This files provides address values that exist in the system */

#define BOARD "DE1-SoC"

/* Memory */
#define DDR_BASE 0x00000000
#define DDR_END 0x3FFFFFFF
#define A9_ONCHIP_BASE 0xFFFF0000
#define A9_ONCHIP_END 0xFFFFFFFF
#define SDRAM_BASE 0xC0000000
#define SDRAM_END 0xC3FFFFFF
#define FPGA_ONCHIP_BASE 0xC8000000
#define FPGA_ONCHIP_END 0xC803FFFF
#define FPGA_CHAR_BASE 0xC9000000
#define FPGA_CHAR_END 0xC9001FFF

/* Cyclone V FPGA devices */
#define LEDR_BASE 0xFF200000
#define HEX3_HEX0_BASE 0xFF200020
#define HEX5_HEX4_BASE 0xFF200030
#define SW_BASE 0xFF200040
#define KEY_BASE 0xFF200050
#define JP1_BASE 0xFF200060
#define JP2_BASE 0xFF200070
#define PS2_BASE 0xFF200100
#define PS2_DUAL_BASE 0xFF200108
#define JTAG_UART_BASE 0xFF201000
#define JTAG_UART_2_BASE 0xFF201008
#define IrDA_BASE 0xFF201020
#define TIMER_BASE 0xFF202000
#define AV_CONFIG_BASE 0xFF203000
#define PIXEL_BUF_CTRL_BASE 0xFF203020
#define CHAR_BUF_CTRL_BASE 0xFF203030
#define AUDIO_BASE 0xFF203040
#define VIDEO_IN_BASE 0xFF203060
#define ADC_BASE 0xFF204000

/* Cyclone V HPS devices */
#define HPS_GPIO1_BASE 0xFF709000
#define HPS_TIMER0_BASE 0xFFC08000
#define HPS_TIMER1_BASE 0xFFC09000
#define HPS_TIMER2_BASE 0xFFD00000
#define HPS_TIMER3_BASE 0xFFD01000
#define FPGA_BRIDGE 0xFFD0501C

/* ARM A9 MPCORE devices */
#define PERIPH_BASE 0xFFFEC000       // base address of peripheral devices
#define MPCORE_PRIV_TIMER 0xFFFEC600 // PERIPH_BASE + 0x0600

/* Interrupt controller (GIC) CPU interface(s) */
#define MPCORE_GIC_CPUIF 0xFFFEC100 // PERIPH_BASE + 0x100
#define ICCICR 0x00                 // offset to CPU interface control reg
#define ICCPMR 0x04                 // offset to interrupt priority mask reg
#define ICCIAR 0x0C                 // offset to interrupt acknowledge reg
#define ICCEOIR 0x10                // offset to end of interrupt reg
/* Interrupt controller (GIC) distributor interface(s) */
#define MPCORE_GIC_DIST 0xFFFED000 // PERIPH_BASE + 0x1000
#define ICDDCR 0x00                // offset to distributor control reg
#define ICDISER 0x100              // offset to interrupt set-enable regs
#define ICDICER 0x180              // offset to interrupt clear-enable regs
#define ICDIPTR 0x800              // offset to interrupt processor targets regs
#define ICDICFR 0xC00              // offset to interrupt configuration regs

/* VGA colors */
#define WHITE 0xFFFF
#define YELLOW 0xFFE0
#define RED 0xF800
#define GREEN 0x07E0
#define BLUE 0x001F
#define CYAN 0x07FF
#define MAGENTA 0xF81F
#define GREY 0xC618
#define PINK 0xFC18
#define ORANGE 0xFC00

#define ABS(x) (((x) > 0) ? (x) : -(x))

/* Screen size */

#define RESOLUTION_X 320
#define RESOLUTION_Y 240

/* GIC Modes */

#define USER_MODE 0b10000
#define FIQ_MODE 0b10001
#define IRQ_MODE 0b10010
#define SVC_MODE 0b10011
#define ABORT_MODE 0b10111
#define UNDEF_MODE 0b11011
#define SYS_MODE 0b111111

/* Other definitions */
#define ENABLE 0b1
#define INT_ENABLE 0b01000000
#define INT_DISABLE 0b11000000
#define A9_TIMER_IRQ 29
#define KEYS_IRQ 73

void set_A9_IRQ_stack(void);
void enable_A9_interrupts(void);
void config_GIC(void);
void config_KEYS(void);
void pushbutton_ISR(void);
void wait_for_vsync();
int first_digit(int n);
int last_digit(int n);
void display_score(int score);

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

int balloon_image[1140] = {65535, 65535, 65535, 65535, 65535, 65535, 65535, 65535, 65535, 65535, 65535, 65535, 65535, 65535, 65535, 65535, 65535, 65535, 65535, 65535, 65535, 65535, 65535, 65535, 65535, 65535, 65535, 65535, 65535, 65535, 65535, 65535, 65535, 65535, 65535, 65535, 65535, 65535, 65535,
                             65535, 65535, 65535, 65469, 65339, 63225, 63225, 63258, 65437,
                             65535, 65535, 65535, 65535, 65535, 65535, 65535, 65535, 65535,
                             65535, 65535, 65535, 65535, 65535, 65535, 65535, 65535, 65535, 65535, 65535, 65535,
                             65436, 62898, 60523, 60359, 60293, 60261, 60261, 60261, 60327,
                             60490, 62865, 63323, 65535, 65535, 65535, 65535, 65535, 65535, 65535, 65535,
                             65535, 65535, 65535, 65535, 65535, 65535, 65535, 65535, 65535, 62832, 60360, 62539, 62539, 62374,
                             62341, 62374, 62374, 62342, 62341, 60261, 60228, 60261, 60719, 65470, 65535, 65535, 65535, 65535, 65535,
                             65535, 65535, 65535, 65535, 65535, 65535, 65535, 65535, 63355, 60424, 62474, 65111, 65210, 62670, 62374,
                             62374, 62374, 62374, 62374, 62374, 62374, 62374, 62342, 60195, 60359, 63225, 65535, 65535, 65535, 65535, 65535,
                             65535, 65535, 65535, 65535, 65535, 65535, 63291, 60294, 62670, 65308, 65144, 62572, 60293, 62374, 62374, 62374, 62374,
                             62374, 62374, 62374, 62374, 62374, 62374, 62308, 60261, 63160, 65535, 65535, 65535, 65535, 65535, 65535, 65535,
                             65535, 65535, 65470, 60327, 62703, 65373, 65112, 62506, 60261, 62374, 62374, 62374, 62374, 62374, 62374, 62374, 62374, 62374,
                             62374, 62374, 62374, 62276, 60260, 63291, 65535, 65535, 65535, 65535, 65535, 65535, 65535, 65535, 60555, 62539,
                             65340, 65242, 62571, 60261, 62374, 62374, 62374, 62374, 62374, 62374, 62374, 62374, 62374, 62374, 62374, 62374, 60326, 62506, 62407,
                             60392, 65535, 65535, 65535, 65535, 65535, 65535, 65535, 63094, 60261, 65046, 65275, 62736, 62342, 62374,
                             62374, 62374, 62374, 62374, 62374, 62374, 62374, 62374, 62374, 62374, 62374, 62374, 62341, 62572, 64915,
                             60228, 62866, 65535, 65535, 65535, 65535, 65535, 65535, 60457, 62539, 62868, 62736, 62440, 62342, 62374, 62374, 62374, 62374, 62374, 62374, 62374, 62374, 62374, 62374, 62374, 62374, 62374, 62374, 60294, 62802, 62670, 60294, 65470, 65535, 65535, 65535, 65535,
                             63193, 60261, 62670, 62638, 62473, 62374, 62374, 62374, 62374, 62374, 62374, 62374, 62374, 62374, 62374, 62374, 62374, 62374, 62374, 62374, 62374, 60293, 62539, 64948, 60261, 62964, 65535, 65535, 65535, 65535, 62800, 62342,
                             62637, 62506, 62374, 62374, 62374, 62374, 62374, 62374, 62374, 62374, 62375, 62375, 62374, 62374, 62374, 62374, 62374, 62374, 62374, 62374, 62375, 62834, 62539, 60523, 65535, 65535, 65535,
                             65535, 60457, 62440, 62604, 62439, 62374, 62374, 62374, 62374, 62374, 62374, 62375, 62407, 62407, 62407, 62407, 62375, 62374, 62374, 62374, 62374, 62374, 62374, 60293, 62703, 62703, 60326, 65469, 65535,
                             65535, 65437, 60326, 62407, 62440, 62374, 62374, 62374, 62374, 62374, 62374, 62374, 62407, 62440, 62472, 62473, 62440, 62407, 62375, 62374, 62374, 62374, 62374, 62374, 60293, 62572, 62769, 60261, 63225, 65535,
                             65535, 63258, 60261, 62375, 62375, 62374, 62374, 62374, 62374, 62374, 62374, 62375, 62407, 62472, 62506, 62506, 62473, 62408, 62375, 62374, 62374, 62374, 62374, 62374, 62342, 62506, 62801, 60294, 63061, 65535,
                             65535, 63160, 60261, 62408, 62407, 62374, 62374, 62374, 62374, 62374, 62374, 62375, 62407, 62473, 62506, 62506, 62473, 62440, 62407, 62374, 62374, 62374, 62374, 62374, 62342, 62473, 62768, 60294, 62931, 65535,
                             65535, 63127, 60261, 62440, 62407, 62374, 62374, 62374, 62374, 62374, 62374, 62375, 62407, 62440, 62473, 62473, 62440, 62407, 62375, 62374, 62374, 62374, 62374, 62374, 62342, 62473, 62736, 60294, 62898, 65535,
                             65535, 63159, 60261, 62440, 62408, 62374, 62374, 62374, 62374, 62374, 62374, 62374, 62407, 62407, 62440, 62440, 62407, 62407, 62375, 62374, 62374, 62374, 62374, 62374, 60294, 62506, 62703, 60261, 62930, 65535,
                             65535, 63257, 60261, 62440, 62440, 62374, 62374, 62374, 62374, 62374, 62374, 62374, 62374, 62375, 62375, 62407, 62375, 62374, 62374, 62374, 62374, 62374, 62374, 62374, 60326, 62539, 62637, 60228, 63029, 65535,
                             65535, 65437, 60327, 62407, 62473, 62374, 62374, 62374, 62374, 62374, 62374, 62374, 62374, 62374, 62374, 62374, 62374, 62374, 62374, 62374, 62374, 62374, 62374, 62374, 60326, 62572, 62571, 60228, 63225, 65535,
                             65535, 65535, 60523, 62342, 62505, 62407, 62374, 62374, 62374, 62374, 62374, 62374, 62374, 62374, 62374, 62374, 62374, 62374, 62374, 62374, 62374, 62374, 62374, 62374, 62407, 62604, 62440, 60359, 65502, 65535,
                             65535, 65535, 62898, 60228, 62473, 62440, 62374, 62374, 62374, 62374, 62374, 62374, 62374, 62374, 62374, 62374, 62374, 62374, 62374, 62374, 62374, 62374, 62374, 62374, 62440, 62570, 62309, 60653, 65535, 65535,
                             65535, 65535, 63323, 60261, 62407, 62440, 62374, 62374, 62374, 62374, 62374, 62374, 62374, 62374, 62374, 62374, 62374, 62374, 62374, 62374, 62374, 62374, 62374, 62374, 62472, 62440, 60196, 63127, 65535, 65535,
                             65535, 65535, 65535, 60621, 62276, 62440, 62407, 62374, 62374, 62374, 62374, 62374, 62374, 62374, 62374, 62374, 62374, 62374, 62374, 62374, 62374, 62374, 62374, 62375, 62472, 62309, 60425, 65535, 65535, 65535,
                             65535, 65535, 65535, 63258, 60228, 62374, 62407, 62374, 62374, 62374, 62374, 62374, 62374, 62374, 62374, 62374, 62374, 62374, 62374, 62374, 62374, 62374, 62374, 62407, 62407, 60196, 63029, 65535, 65535, 65535,
                             65535, 65535, 65535, 65535, 60686, 60196, 62375, 62374, 62374, 62374, 62374, 62374, 62374, 62374, 62374, 62374, 62374, 62374, 62374, 62374, 62374, 62374, 62374, 62375, 60228, 60490, 65535, 65535, 65535, 65535,
                             65535, 65535, 65535, 65535, 65469, 60327, 62309, 62374, 62374, 62374, 62374, 62374, 62374, 62374, 62374, 62374, 62374, 62374, 62374, 62374, 62374, 62374, 62374, 62341, 60228, 63258, 65535, 65535, 65535, 65535,
                             65535, 65535, 65535, 65535, 65535, 63127, 60196, 62342, 62374, 62374, 62374, 62374, 62374, 62374, 62374, 62374, 62374, 62374, 62374, 62374, 62374, 62374, 62374, 60195, 62898, 65535, 65535, 65535, 65535, 65535,
                             65535, 65535, 65535, 65535, 65535, 65535, 62865, 60195, 62374, 62374, 62374, 62374, 62374, 62374, 62374, 62374, 62374, 62374, 62374, 62374, 62374, 62374, 60195, 60621, 65535, 65535, 65535, 65535, 65535, 65535,
                             65535, 65535, 65535, 65535, 65535, 65535, 65535, 60686, 60195, 62374, 62374, 62374, 62374, 62374, 62374, 62374, 62374, 62374, 62374, 62374, 62374, 60195, 60523, 65535, 65535, 65535, 65535, 65535, 65535, 65535,
                             65535, 65535, 65535, 65535, 65535, 65535, 65535, 65535, 60686, 60195, 62342, 62374, 62374, 62374, 62374, 62374, 62374, 62374, 62374, 62374, 60195, 60523, 65535, 65535, 65535, 65535, 65535, 65535, 65535, 65535,
                             65535, 65535, 65535, 65535, 65535, 65535, 65535, 65535, 65535, 62866, 60228, 62308, 62374, 62374, 62374, 62374, 62374, 62374, 62309, 60195, 60686, 65535, 65535, 65535, 65535, 65535, 65535, 65535, 65535, 65535,
                             65535, 65535, 65535, 65535, 65535, 65535, 65535, 65535, 65535, 65535, 63192, 60424, 60196, 60260, 62342, 62374, 62309, 60196, 60327, 63029, 65535, 65535, 65535, 65535, 65535, 65535, 65535, 65535, 65535, 65535,
                             65535, 65535, 65535, 65535, 65535, 65535, 65535, 65535, 65535, 65535, 65535, 65535, 62996, 60523, 60359, 60359, 60490, 62898, 65469, 65535, 65535, 65535, 65535, 65535, 65535, 65535, 65535, 65535, 65535, 65535,
                             65535, 65535, 65535, 65535, 65535, 65535, 65535, 65535, 65535, 65535, 65535, 65535, 65535, 65535, 62931, 60719, 65535, 65535, 65535, 65535, 65535, 65535, 65535, 65535, 65535, 65535, 65535, 65535, 65535, 65535,
                             65535, 65535, 65535, 65535, 65535, 65535, 65535, 65535, 65535, 65535, 65535, 65535, 65535, 65535, 60588, 60392, 65535, 65535, 65535, 65535, 65535, 65535, 65535, 65535, 65535, 65535, 65535, 65535, 65535, 65535,
                             65535, 65535, 65535, 65535, 65535, 65535, 65535, 65535, 65535, 65535, 65535, 65535, 65535, 65535, 63258, 63225, 65502, 65535, 65535, 65535, 65535, 65535, 65535, 65535, 65535, 65535, 65535, 65535, 65535, 65535,
                             65535, 65535, 65535, 65535, 65535, 65535, 65535, 65535, 65535, 65535, 65535, 65535, 65535, 65535, 65535, 65535, 65535, 65535, 65535, 65535, 65535, 65535, 65535, 65535, 65535, 65535, 65535, 65535, 65535, 65535};
//function declarations
void clear_screen();
void plot_pixel(int x, int y, short int line_color);
void plot_image(int initialX, int initialY, int imageArray[], unsigned width, unsigned height);
void VGA_text(int x, int y, char *text_ptr);
void VGA_text_clean();
void initial_setup();
void plot_image(int initialX, int initialY, int imageArray[], unsigned width, unsigned height);
int pick_lane();
void clearBalloonEnd(int initialX, int initialY, unsigned width, unsigned height);
void loadTimer(int);
bool timerDone();
int getSpeed();
void doDelay();
void clear_screen_initial();

volatile int *LEDR_ptr = (int *)0xFF200000;
volatile int *SW_ptr = (int *)0xFF200040;
volatile int *KEY_EDGE_ptr = (int *)0xFF20005C;
volatile char *character_buffer = (char *)0xC9000000;
volatile int *pixel_ctrl_ptr = (int *)0xFF203020;
volatile int pixel_buffer_start;
volatile int *timer_A9_ptr = (int *)0xFFFEC600;
volatile int *push_ptr = (int *)0xFF200050;

volatile int key_dir;
volatile int key_val;
char seg7[] = {0x3f, 0x06, 0x5b, 0x4f, 0x66, 0x6d, 0x7d, 0x07, 0x7f, 0x67};

bool gameNotOver = true;
int score = 0;

int whereToPop;

char *title = "BALLOON POP";
char *lane1 = "Lane 1";
char *lane2 = "Lane 2";
char *lane3 = "Lane 3";
char *you_win_1 = "CONGRATULATIONS!!!";
char *you_win_2 = "You Win :)"; 
char *game_over = "GAME OVER :( ";

int main()
{

    set_A9_IRQ_stack();
    enable_A9_interrupts();
    config_GIC();
    config_KEYS();

    volatile int *pixel_ctrl_ptr = (int *)0xFF203020;

    /* set front pixel buffer to start of FPGA On-chip memory */
    *(pixel_ctrl_ptr + 1) = 0xC8000000; // first store the address in the
                                        // back buffer
    /* now, swap the front/back buffers, to set the front buffer location */
    wait_for_vsync();
    /* initialize a pointer to the pixel buffer, used by drawing functions */
    pixel_buffer_start = *pixel_ctrl_ptr;
    clear_screen_initial(); // pixel_buffer_start points to the pixel buffer
    /* set back pixel buffer to start of SDRAM memory */
    *(pixel_ctrl_ptr + 1) = 0xC0000000;
    pixel_buffer_start = *(pixel_ctrl_ptr + 1); // we draw on the back buffer
    clear_screen();                             // pixel_buffer_start points to the pixel buffer
    pixel_buffer_start = *pixel_ctrl_ptr;

    initial_setup();

    while (gameNotOver && score!= 99)
    {
        display_score(score);
        int laneNumber = pick_lane();
      
		     int i;
            int j;
        if (laneNumber == 0)
        {

            for (i = 35; i < 200; i++)
            {
                plot_image(35, i, balloon_image, 30, 38);
                for (j = 35; j < 65; j++)
                {
                    plot_pixel(j, i, WHITE);
                    if (i == 199)
                    {
                        clearBalloonEnd(j, i, 30, 38);
                        gameNotOver = false;
                    }
                }
                //pressed_key = (*push_ptr & 0xF);
                if (key_val == 4)
                {
                    for (i = 35; i < 240; i++)
                    {
                        for (j = 35; j < 65; j++)
                        {
                            plot_pixel(j, i, WHITE);
                        }
                    }
                    key_val = 0;
                    score++;

                    break;
                }

                doDelay();
            }
        }
        else if (laneNumber == 1)
        {
            for (i = 35; i < 200; i++)
            {
                plot_image(145, i, balloon_image, 30, 38);
                for (j = 145; j < 175; j++)
                {
                    plot_pixel(j, i, WHITE);
                    if (i == 199)
                    {
                        gameNotOver = false;
                        clearBalloonEnd(j, i, 30, 38);
                    }
                }

                //pressed_key = (*push_ptr & 0xF);
                if (key_val == 2)
                {
                    for (i = 35; i < 240; i++)
                    {
                        for (j = 145; j < 175; j++)
                        {
                            plot_pixel(j, i, WHITE);
                        }
                    }
                    key_val = 0;
                    score++;
                    break;
                }
                doDelay();
            }
        }
        else if (laneNumber == 2)
        {
            for (i = 35; i < 200; i++)
            {
                plot_image(245, i, balloon_image, 30, 38);
                for (j = 245; j < 275; j++)
                {
                    plot_pixel(j, i, WHITE);
                    if (i == 199)
                    {
                        gameNotOver = false;
                        clearBalloonEnd(j, i, 30, 38);
                    }
                }
                //pressed_key = (*push_ptr & 0xF);
                if (key_val == 1)
                {
                    for (i = 35; i < 240; i++)
                    {
                        for (j = 245; j < 275; j++)
                        {
                            plot_pixel(j, i, WHITE);
                        }
                    }
                    key_val = 0;
                    score++;
                    break;
                }
                doDelay();
            }
        }
    }
    wait_for_vsync();                           // swap front and back buffers on VGA vertical sync
    pixel_buffer_start = *(pixel_ctrl_ptr + 1); // new back buffer

    if (!gameNotOver)
    {
        clear_screen();
        VGA_text_clean();
        VGA_text(35, 30, game_over);
    }
	if (score == 99){
		clear_screen();
        VGA_text_clean();
        VGA_text(35, 30, you_win_1);
		VGA_text(35, 35, you_win_2);
	}
    //return 0;
}

void plot_pixel(int x, int y, short int line_color)
{
    *(short *)(pixel_buffer_start + (y << 10) + (x << 1)) = line_color;
}

void clear_screen()
{
    int y;
    int x;
    for (y = 0; y < 240; y++)
    {
        for (x = 0; x < 320; x++)
        {
            plot_pixel(x, y, 0);
        }
    }
}

void clear_screen_initial()
{
    int y;
    int x;
    for (y = 0; y < 29; y++)
    {
        for (x = 0; x < 320; x++)
        {
            plot_pixel(x, y, BLUE);
        }
    }
	for (y = 29; y < 240; y++)
    {
        for (x = 0; x < 320; x++)
        {
            plot_pixel(x, y, WHITE);
        }
    }
}


void VGA_text(int x, int y, char *text_ptr)
{
    int offset = (y << 7) + x;

    while (*(text_ptr))
    {
        *(character_buffer + offset) = *(text_ptr);
        ++text_ptr;
        ++offset;
    }
}
void VGA_text_clean()
{
    for (int y = 0; y < 60; y++)
    {
        for (int x = 0; x < 80; x++)
        {
            int offset = (y << 7) + x;
            *(character_buffer + offset) = 0;
            ++offset;
        }
    }
}

void initial_setup()
{
    VGA_text_clean();
    VGA_text(33, 2, title);
    VGA_text(10, 5, lane1);
    VGA_text(35, 5, lane2);
    VGA_text(60, 5, lane3);

    int y;
    int x;
    for (y = 16; y < 240; y++)
    {
        plot_pixel(106, y, 0);
        plot_pixel(214, y, 0);
    }

    for (x = 0; x < 320; x++)
    {
        plot_pixel(x, 15, 0);
        plot_pixel(x, 28, 0);
    }
}

void plot_image(int initialX, int initialY, int balloon[], unsigned width, unsigned height)
{

    int i = 0;
    unsigned y, x;

    for (y = 0; y < height; y++)
    {
        for (x = 0; x < width; x++)
        {

            if (initialX + x >= 0 && initialY + y >= 0 && x < 30 && y < 38)
                plot_pixel(initialX + x, initialY + y, balloon[i]);

            i++;
        }
    }
}

int pick_lane()
{
    return rand() % 3;
}

void clearBalloonEnd(int initialX, int initialY, unsigned width, unsigned height)
{
    unsigned y;
    unsigned x;
    for (y = 0; y < height; y++)
    {
        for (x = 0; x < width; x++)
        {

            if (initialX + x >= 0 && initialY + y >= 0 && x < 30 && y < 38)
                plot_pixel(initialX + x, initialY + y, WHITE);
        }
    }
}

bool timerDone()
{
    short interruptBit = *(timer_A9_ptr + 3);

    if (interruptBit == 0)
    {
        return false;
    }
    else
    {
        *(timer_A9_ptr + 3) = 0b1;
        return true;
    }
}

void loadTimer(int seconds)
{
#define A9_RATE 500000

    *(timer_A9_ptr) = A9_RATE * seconds;
    *(timer_A9_ptr + 2) = 0b0001;

    return;
}

int getSpeed()
{

    unsigned SW_value = (unsigned int)*SW_ptr;

    if (SW_value == 0b0000000100)
    {
        return 3;
    }
    else if (SW_value == 0b0000000010)
    {
        return 2;
    }
    else if (SW_value == 0b0000000001)
    {
        return 1;
    }
    else
        return 0;
}

void doDelay()
{

    int speed = getSpeed();

    if (speed == 3)
    {
        return;
    }
    else if (speed == 2 || score >= 30)
    {
        loadTimer(5);
    }
    else if (speed == 1 || score>= 15)
    {
        loadTimer(12);
    }
    else if (speed == 0 || score >= 5 )
    {
        loadTimer(40);
    }

    while (!timerDone())
    {
    }

    return;
}

/*
 * Initialize the banked stack pointer register for IRQ mode
*/

void set_A9_IRQ_stack(void)
{
    int stack, mode;
    stack = A9_ONCHIP_END - 7; // top of A9 onchip memory, aligned to 8 bytes /* change processor to IRQ mode with interrupts disabled */
    mode = INT_DISABLE | IRQ_MODE;
    __asm__("msr cpsr, %[ps]"
            :
            : [ps] "r"(mode));
    /* set banked stack pointer */
    __asm__("mov sp, %[ps]"
            :
            : [ps] "r"(stack));
    /* go back to SVC mode before executing subroutine return! */
    mode = INT_DISABLE | SVC_MODE;
    __asm__("msr cpsr, %[ps]"
            :
            : [ps] "r"(mode));
}

/*
 * Turn on interrupts in the ARM processor
*/

void enable_A9_interrupts(void)
{
    int status = SVC_MODE | INT_ENABLE;
    __asm__("msr cpsr, %[ps]"
            :
            : [ps] "r"(status));
}

/*
 * Configure the Generic Interrupt Controller (GIC)
*/

void config_GIC(void)
{
    int address; // used to calculate register addresses
    /* configure the FPGA interval timer and KEYs interrupts */
    *((int *)0xFFFED848) = 0x00000101;
    *((int *)0xFFFED108) = 0x00000300;
    // Set Interrupt Priority Mask Register (ICCPMR). Enable interrupts of all
    // priorities
    address = MPCORE_GIC_CPUIF + ICCPMR;
    *((int *)address) = 0xFFFF;
    // Set CPU Interface Control Register (ICCICR). Enable signaling of
    // interrupts
    address = MPCORE_GIC_CPUIF + ICCICR;
    *((int *)address) = ENABLE;
    // Configure the Distributor Control Register (ICDDCR) to send pending
    // interrupts to CPUs
    address = MPCORE_GIC_DIST + ICDDCR;
    *((int *)address) = ENABLE;
}

// Define the IRQ exception handler

void __attribute__((interrupt)) __cs3_isr_irq(void)
{
    // Read the ICCIAR from the processor interface
    int address = MPCORE_GIC_CPUIF + ICCIAR;
    int int_ID = *((int *)address);
    if (int_ID == KEYS_IRQ) // check if interrupt is from the KEYs
        pushbutton_ISR();
    else
        while (1)
            ; // if unexpected, then stay here
    // Write to the End of Interrupt Register (ICCEOIR)
    address = MPCORE_GIC_CPUIF + ICCEOIR;
    *((int *)address) = int_ID;
    return;
}

// Define the remaining exception handlers

void __attribute__((interrupt)) __cs3_reset(void)
{
    while (1)
        ;
}

void __attribute__((interrupt)) __cs3_isr_undef(void)
{
    while (1)
        ;
}

void __attribute__((interrupt)) __cs3_isr_swi(void)
{
    while (1)
        ;
}

void __attribute__((interrupt)) __cs3_isr_pabort(void)
{
    while (1)
        ;
}

void __attribute__((interrupt)) __cs3_isr_dabort(void)
{
    while (1)
        ;
}

void __attribute__((interrupt)) __cs3_isr_fiq(void)
{
    while (1)
        ;
}

void config_KEYS(void)
{
    volatile int *KEY_ptr = (int *)KEY_BASE; // pushbutton KEY address
    *(KEY_ptr + 2) = 0xF;                    // enable interrupts for all KEYs
}

void pushbutton_ISR(void)
{
    volatile int *KEY_ptr = (int *)KEY_BASE;
    int press;
    press = *(KEY_ptr + 3); // read the pushbutton interrupt register
    *(KEY_ptr + 3) = press; // Clear the interrupt
    key_val = press;
    key_dir = !key_dir;
    return;
}

void wait_for_vsync()
{
    volatile int *pixel_ctrl_ptr = (int *)0xFF203020;
    int status;

    *pixel_ctrl_ptr = 1;
    status = *(pixel_ctrl_ptr + 3);

    while ((status & 0x01) != 0)
    {
        status = *(pixel_ctrl_ptr + 3);
    }
}

int first_digit(int n)
{
    while (n >= 10)
    {
        n = n / 10;
    }
    return n;
}

int last_digit(int n)
{
    n = n % 10;
    return n;
}

void display_score(int score)
{
    volatile int *HEX3_1_ptr = (int *)0xFF200020;
    if (score < 10)
    {
        *HEX3_1_ptr = seg7[score];
    }
    else
    {
        int first = first_digit(score);
        int last = last_digit(score);
        *HEX3_1_ptr = (seg7[first] << 8) | seg7[last];
    }
}
