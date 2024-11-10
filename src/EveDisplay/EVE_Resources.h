//------------------------------------------------------------------------------
// FTDI EVE2 - Data types & constants
//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
#ifndef __EVE_DATA_TYPES_H
#define __EVE_DATA_TYPES_H
//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
//************************** FTDI EVE2 Registers *******************************
//------------------------------------------------------------------------------
#define REG_CHIPID	0x0C0000		// ROM_CHIPID from FT800

#define	REG_ID 		0x302000
#define	REG_FRAMES 	0x302004
#define	REG_CLOCK 	0x302008 
#define	REG_FREQUENCY 	0x30200C 
#define	REG_RENDERMODE 	0x302010 
#define	REG_SNAPY 	0x302014 
#define	REG_SNAPSHOT 	0x302018 
#define	REG_SNAPFORMAT 	0x30201C 
#define	REG_CPURESET 	0x302020 
#define	REG_TAP_CRC 	0x302024 

#define	REG_TAP_MASK 	0x302028 
#define	REG_HCYCLE 	0x30202C 
#define	REG_HOFFSET 	0x302030 
#define	REG_HSIZE 	0x302034 
#define	REG_HSYNC0 	0x302038 
#define	REG_HSYNC1 	0x30203C 
#define	REG_VCYCLE 	0x302040 
#define	REG_VOFFSET 	0x302044 
#define	REG_VSIZE 	0x302048 
#define	REG_VSYNC0 	0x30204C 
#define	REG_VSYNC1 	0x302050 
#define	REG_DLSWAP 	0x302054 
#define	REG_ROTATE 	0x302058 
#define	REG_OUTBITS 	0x30205C 
#define	REG_DITHER 	0x302060 
#define	REG_SWIZZLE 	0x302064 
#define	REG_CSPREAD 	0x302068 
#define	REG_PCLK_POL 	0x30206C 
#define	REG_PCLK 	0x302070 
#define	REG_TAG_X 	0x302074 
#define	REG_TAG_Y 	0x302078 
#define	REG_TAG 	0x30207C 
#define	REG_VOL_PB 	0x302080 
#define	REG_VOL_SOUND 	0x302084 

#define	REG_SOUND 		0x302088 
#define	REG_PLAY 		0x30208C 
#define	REG_GPIO_DIR 		0x302090 
#define	REG_GPIO 		0x302094 
#define	REG_GPIOX_DIR 		0x302098 
#define	REG_GPIOX 		0x30209C 
#define	Reserved1 		0x3020A0 - 0x3020A4 
#define	REG_INT_FLAGS 		0x3020A8 
#define	REG_INT_EN 		0x3020Ac 
#define	REG_INT_MASK 		0x3020B0 
#define	REG_PLAYBACK_START 	0x3020B4 
#define	REG_PLAYBACK_LENGTH 	0x3020B8 
#define	REG_PLAYBACK_READPTR 	0x3020BC 
#define	REG_PLAYBACK_FREQ 	0x3020C0 
#define	REG_PLAYBACK_FORMAT 	0x3020C4 
#define	REG_PLAYBACK_LOOP 	0x3020C8 
#define	REG_PLAYBACK_PLAY 	0x3020CC 
#define	REG_PWM_HZ 		0x3020D0 
#define	REG_PWM_DUTY 		0x3020D4 
#define	REG_MACRO_0 		0x3020D8 
#define	REG_MACRO_1 		0x3020DC 
#define	Reserved2 		0x3020E0 – 0x3020F4 
#define	REG_CMD_READ 		0x3020F8 

#define	REG_CMD_WRITE 		0x3020FC 
#define	REG_CMD_DL 		0x302100 
#define	REG_TOUCH_MODE 		0x302104 
#define	REG_TOUCH_ADC_MODE		0x302108  
#define	REG_CTOUCH_EXTENDED 		0x302108 
#define	REG_TOUCH_CHARGE 	0x30210C 
#define	REG_TOUCH_SETTLE 	0x302110 
#define	REG_TOUCH_OVERSAMPLE 	0x302114 
#define	REG_TOUCH_RZTHRESH 	0x302118 
#define	REG_TOUCH_RAW_XY		0x30211C
#define REG_CTOUCH_TOUCH1_XY 		0x30211C
#define	REG_TOUCH_RZ			0x302120
#define REG_CTOUCH_TOUCH4_Y 		0x302120
#define	REG_TOUCH_SCREEN_XY		0x302124
#define	REG_CTOUCH_TOUCH0_XY 		0x302124
#define	REG_TOUCH_TAG_XY 	0x302128
#define	REG_TOUCH_TAG		0x30212C
#define	REG_TOUCH_TAG1_XY 	0x302130
#define	REG_TOUCH_TAG1		0x302134
#define	REG_TOUCH_TAG2_XY 	0x302138 
#define	REG_TOUCH_TAG2		0x30213C

#define	REG_TOUCH_TAG3_XY	0x302140 
#define	REG_TOUCH_TAG3		0x302144 
#define	REG_TOUCH_TAG4_XY	0x302148 
#define	REG_TOUCH_TAG4		0x30214C 
#define	REG_TOUCH_TRANSFORM_A 	0x302150 
#define	REG_TOUCH_TRANSFORM_B 	0x302154 
#define	REG_TOUCH_TRANSFORM_C 	0x302158 
#define	REG_TOUCH_TRANSFORM_D 	0x30215C 
#define	REG_TOUCH_TRANSFORM_E 	0x302160 
#define	REG_TOUCH_TRANSFORM_F 	0x302164 
#define	REG_TOUCH_CONFIG 	0x302168 
#define	REG_CTOUCH_TOUCH4_X 	0x30216C 
#define	Reserved3	 	0x302170 
#define	REG_BIST_EN		0x302174 

#define	Reserved4		0x302178 
#define	Reserved5		0x30217C 
#define	REG_TRIM		0x302180 
#define	REG_ANA_COMP		0x302184 
#define	REG_SPI_WIDTH		0x302188 
#define	REG_TOUCH_DIRECT_XY		0x30218C
#define	REG_CTOUCH_TOUCH2_XY		0x30218C
#define	REG_TOUCH_DIRECT_Z1Z2		0x302190
#define	REG_CTOUCH_TOUCH3_XY		0x302190
#define	Reserved6		0x302194 - 302560 
#define	REG_DATESTAMP		0x302564 
#define	REG_CMDB_SPACE		0x302574 
#define	REG_CMDB_WRITE		0x302578

//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
//********************* FTDI EVE2 Coprocessor defines  *************************
//------------------------------------------------------------------------------
// From:	FTDI SampleCode/.../Hdr/FT_Gpu.h
// Status:	НЕ РАЗБИРАЛОСЬ и НЕ ТЕСТИРОВАЛОСЬ!!!
//------------------------------------------------------------------------------
#define ADC_DIFFERENTIAL     1UL
#define ADC_SINGLE_ENDED     0UL
#define ADPCM_SAMPLES        2UL
#define ALWAYS               7UL
#define ARGB1555             0UL
#define ARGB2                5UL
#define ARGB4                6UL
#define BARGRAPH             11UL
#define BILINEAR             1UL
#define BITMAPS              1UL
#define BORDER               0UL

#define CMDBUF_SIZE          4096UL

#define CMD_APPEND                      0xFFFFFF1E
#define CMD_BGCOLOR          4294967049UL
#define CMD_BITMAP_TRANSFORM 4294967073UL
#define CMD_BUTTON                      0xFFFFFF0D
#define CMD_CALIBRATE                   0xFFFFFF15
#define CMD_CLOCK                       0xFFFFFF14
#define CMD_COLDSTART        4294967090UL
#define CMD_CRC              4294967043UL
#define CMD_CSKETCH          4294967093UL
#define CMD_DIAL             4294967085UL
#define CMD_DLSTART                     0xFFFFFF00
#define CMD_EXECUTE          4294967047UL
#define CMD_FGCOLOR          4294967050UL
#define CMD_GAUGE            4294967059UL
#define CMD_GETMATRIX        4294967091UL
#define CMD_GETPOINT         4294967048UL
#define CMD_GETPROPS         4294967077UL
#define CMD_GETPTR           4294967075UL
#define CMD_GRADCOLOR        4294967092UL
#define CMD_GRADIENT         4294967051UL
#define CMD_HAMMERAUX        4294967044UL
#define CMD_IDCT_DELETED     4294967046UL
#define CMD_INFLATE          4294967074UL
#define CMD_INTERRUPT        4294967042UL
#define CMD_INT_RAMSHARED    4294967101UL
#define CMD_INT_SWLOADIMAGE  4294967102UL
#define CMD_KEYS                        0xFFFFFF0E
#define CMD_LOADIDENTITY                0xFFFFFF26
#define CMD_LOADIMAGE        4294967076UL
#define CMD_LOGO             4294967089UL       // 0xFFFFFF31
#define CMD_MARCH            4294967045UL
#define CMD_MEDIAFIFO        4294967097UL
#define CMD_MEMCPY           4294967069UL
#define CMD_MEMCRC           4294967064UL
#define CMD_MEMSET           4294967067UL
#define CMD_MEMWRITE         4294967066UL
#define CMD_MEMZERO          4294967068UL
#define CMD_NUMBER                      0xFFFFFF2E
#define CMD_PLAYVIDEO        4294967098UL
#define CMD_PROGRESS         4294967055UL	// 0xFFFFFF0F
#define CMD_REGREAD          4294967065UL
#define CMD_ROMFONT          4294967103UL
#define CMD_ROTATE                      0xFFFFFF29
#define CMD_SCALE            4294967080UL
#define CMD_SCREENSAVER      4294967087UL
#define CMD_SCROLLBAR        4294967057UL
#define CMD_SETBASE          4294967096UL
#define CMD_SETBITMAP                   0xFFFFFF43      //4294967107UL
#define CMD_SETFONT          4294967083UL
#define CMD_SETFONT2                    0xFFFFFF3B
#define CMD_SETMATRIX        4294967082UL
#define CMD_SETROTATE        4294967094UL
#define CMD_SETSCRATCH       4294967100UL
#define CMD_SKETCH           4294967088UL
#define CMD_SLIDER           4294967056UL
#define CMD_SNAPSHOT         4294967071UL
#define CMD_SNAPSHOT2        4294967095UL
#define CMD_SPINNER          4294967062UL
#define CMD_STOP             4294967063UL
#define CMD_SWAP                        0xFFFFFF01
#define CMD_SYNC             4294967106UL
#define CMD_TEXT                        0xFFFFFF0C
#define CMD_TOGGLE           4294967058UL	// 0xFFFFFF12
#define CMD_TOUCH_TRANSFORM  4294967072UL
#define CMD_TRACK            4294967084UL
#define CMD_TRANSLATE        4294967079UL
#define CMD_VIDEOFRAME       4294967105UL
#define CMD_VIDEOSTART       4294967104UL

#define DECR                 4UL
#define DLSWAP_DONE          0UL
#define DLSWAP_FRAME         2UL
#define DLSWAP_LINE          1UL
#define DST_ALPHA            3UL
#define EDGE_STRIP_A         7UL
#define EDGE_STRIP_B         8UL
#define EDGE_STRIP_L         6UL
#define EDGE_STRIP_R         5UL
#define EQUAL                5UL
#define GEQUAL               4UL
#define GREATER              3UL
#define INCR                 3UL

#define INT_CMDEMPTY         32UL
#define INT_CMDFLAG          64UL
#define INT_CONVCOMPLETE     128UL
#define INT_G8               18UL
#define INT_L8C              12UL
#define INT_PLAYBACK         16UL
#define INT_SOUND            8UL
#define INT_SWAP             1UL
#define INT_TAG              4UL
#define INT_TOUCH            2UL
#define INT_VGA              13UL
#define INVERT               5UL

#define KEEP                 1UL
#define L1                   1UL
#define L2                   17UL
#define L4                   2UL
#define L8                   3UL
#define LEQUAL               2UL
#define LESS                 1UL
#define LINEAR_SAMPLES       0UL
#define LINES                3UL
#define LINE_STRIP           4UL
#define NEAREST              0UL
#define NEVER                0UL
#define NOTEQUAL             6UL

#define ONE                  1UL
#define ONE_MINUS_DST_ALPHA  5UL
#define ONE_MINUS_SRC_ALPHA  4UL
#define OPT_CENTER           1536UL
#define OPT_CENTERX          512UL
#define OPT_CENTERY          1024UL
#define OPT_FLAT             256UL
#define OPT_FULLSCREEN       8UL
#define OPT_MEDIAFIFO        16UL
#define OPT_MONO             1UL
#define OPT_NOBACK           4096UL
#define OPT_NODL             2UL
#define OPT_NOHANDS          49152UL
#define OPT_NOHM             16384UL
#define OPT_NOPOINTER        16384UL
#define OPT_NOSECS           32768UL
#define OPT_NOTEAR           4UL
#define OPT_NOTICKS          8192UL
#define OPT_RIGHTX           2048UL
#define OPT_SIGNED           256UL
#define OPT_SOUND            32UL

#define PALETTED             8UL
#define PALETTED4444         15UL
#define PALETTED565          14UL
#define PALETTED8            16UL
#define FTPOINTS             2UL

/*
#define RAM_CMD              3178496UL		0x308000
#define RAM_DL               3145728UL		0x300000
#define RAM_G                0UL		0x000000
#define RAM_REG              3153920UL		0x302000
#define RAM_ROMSUB           3186688UL		0x30A000
*/

#define RECTS                9UL
#define ROMFONT_TABLEADDRESS 3145724UL
/*
#define REG_ANALOG           3154284UL
#define REG_ANA_COMP         3154308UL
#define REG_BIST_EN          3154292UL
#define REG_BUSYBITS         3154152UL
#define REG_CLOCK            3153928UL
#define REG_CMDB_SPACE       3155316UL
#define REG_CMDB_WRITE       3155320UL
#define REG_CMD_DL           3154176UL
#define REG_CMD_READ         3154168UL
#define REG_CMD_WRITE        3154172UL
#define REG_CPURESET         3153952UL
#define REG_CRC              3154296UL
#define REG_CSPREAD          3154024UL
#define REG_CTOUCH_EXTENDED  3154184UL
//modified below to reflect the same as datasheet
#define REG_CTOUCH_TOUCH0_XY 3154212UL
#define REG_CTOUCH_TOUCH4_X  3154284UL
#define REG_CTOUCH_TOUCH4_Y  3154208UL
#define REG_CTOUCH_TOUCH1_XY 3154204UL
#define REG_CTOUCH_TOUCH2_XY 3154316UL
#define REG_CTOUCH_TOUCH3_XY 3154320UL
#define REG_CYA_TOUCH        3154280UL
#define REG_DATESTAMP        3155300UL
#define REG_DITHER           3154016UL
#define REG_DLSWAP           3154004UL
#define REG_FRAMES           3153924UL
#define REG_FREQUENCY        3153932UL
#define REG_GPIO             3154068UL
#define REG_GPIOX            3154076UL
#define REG_GPIOX_DIR        3154072UL
#define REG_GPIO_DIR         3154064UL
#define REG_HCYCLE           3153964UL
#define REG_HOFFSET          3153968UL
#define REG_HSIZE            3153972UL
#define REG_HSYNC0           3153976UL
#define REG_HSYNC1           3153980UL
#define REG_ID               3153920UL
#define REG_INT_EN           3154092UL
#define REG_INT_FLAGS        3154088UL
#define REG_INT_MASK         3154096UL
#define REG_MACRO_0          3154136UL
#define REG_MACRO_1          3154140UL
#define REG_MEDIAFIFO_READ   3182612UL
#define REG_MEDIAFIFO_WRITE  3182616UL
#define REG_OUTBITS          3154012UL
#define REG_PATCHED_ANALOG   3154288UL
#define REG_PATCHED_TOUCH_FAULT 3154284UL
#define REG_PCLK             3154032UL
#define REG_PCLK_POL         3154028UL
#define REG_PLAY             3154060UL
#define REG_PLAYBACK_FORMAT  3154116UL
#define REG_PLAYBACK_FREQ    3154112UL
#define REG_PLAYBACK_LENGTH  3154104UL
#define REG_PLAYBACK_LOOP    3154120UL
#define REG_PLAYBACK_PLAY    3154124UL
#define REG_PLAYBACK_READPTR 3154108UL
#define REG_PLAYBACK_START   3154100UL
#define REG_PWM_DUTY         3154132UL
#define REG_PWM_HZ           3154128UL
#define REG_RENDERMODE       3153936UL
#define REG_ROMSUB_SEL       3154160UL
#define REG_ROTATE           3154008UL
#define REG_SNAPFORMAT       3153948UL
#define REG_SNAPSHOT         3153944UL
#define REG_SNAPY            3153940UL
#define REG_SOUND            3154056UL
#define REG_SPI_EARLY_TX     3154300UL
#define REG_SPI_WIDTH        3154312UL
#define REG_SWIZZLE          3154020UL
#define REG_TAG              3154044UL
#define REG_TAG_X            3154036UL
#define REG_TAG_Y            3154040UL
#define REG_TAP_CRC          3153956UL
#define REG_TAP_MASK         3153960UL
#define REG_TOUCH_ADC_MODE   3154184UL
#define REG_TOUCH_CHARGE     3154188UL
#define REG_TOUCH_DIRECT_XY  3154316UL
#define REG_TOUCH_DIRECT_Z1Z2 3154320UL
#define REG_TOUCH_FAULT      3154288UL
#define REG_TOUCH_MODE       3154180UL
#define REG_TOUCH_OVERSAMPLE 3154196UL
#define REG_TOUCH_RAW_XY     3154204UL
#define REG_TOUCH_RZ         3154208UL
#define REG_TOUCH_RZTHRESH   3154200UL
#define REG_TOUCH_SCREEN_XY  3154212UL
#define REG_TOUCH_SETTLE     3154192UL
#define REG_TOUCH_TAG        3154220UL
#define REG_TOUCH_TAG1       3154228UL
#define REG_TOUCH_TAG1_XY    3154224UL
#define REG_TOUCH_TAG2       3154236UL
#define REG_TOUCH_TAG2_XY    3154232UL
#define REG_TOUCH_TAG3       3154244UL
#define REG_TOUCH_TAG3_XY    3154240UL
#define REG_TOUCH_TAG4       3154252UL
#define REG_TOUCH_TAG4_XY    3154248UL
#define REG_TOUCH_TAG_XY     3154216UL
#define REG_TOUCH_TRANSFORM_A 3154256UL
#define REG_TOUCH_TRANSFORM_B 3154260UL
#define REG_TOUCH_TRANSFORM_C 3154264UL
#define REG_TOUCH_TRANSFORM_D 3154268UL
#define REG_TOUCH_TRANSFORM_E 3154272UL
#define REG_TOUCH_TRANSFORM_F 3154276UL
*/

#define REG_TRACKER          3182592UL

/*
#define REG_TRACKER_1        3182596UL
#define REG_TRACKER_2        3182600UL
#define REG_TRACKER_3        3182604UL
#define REG_TRACKER_4        3182608UL
#define REG_TRIM             3154304UL
#define REG_VCYCLE           3153984UL
#define REG_VOFFSET          3153988UL
#define REG_VOL_PB           3154048UL
#define REG_VOL_SOUND        3154052UL
#define REG_VSIZE            3153992UL
#define REG_VSYNC0           3153996UL
#define REG_VSYNC1           3154000UL
*/


#define REPEAT               1UL
#define REPLACE              2UL
#define RGB332               4UL
#define RGB565               7UL
#define SRC_ALPHA            2UL
#define TEXT8X8              9UL
#define TEXTVGA              10UL
#define TOUCHMODE_CONTINUOUS 3UL
#define TOUCHMODE_FRAME      2UL
#define TOUCHMODE_OFF        0UL
#define TOUCHMODE_ONESHOT    1UL
#define ULAW_SAMPLES         1UL
#define ZERO                 0UL



//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
//********************** FTDI GPU Primitives macro *****************************
//------------------------------------------------------------------------------

#define VERTEX2F(x,y) ((1UL<<30)|(((x)&32767UL)<<15)|(((y)&32767UL)<<0))
#define VERTEX2II(x,y,handle,cell) ((2UL<<30)|(((x)&511UL)<<21)|(((y)&511UL)<<12)|(((handle)&31UL)<<7)|(((cell)&127UL)<<0))
#define BITMAP_SOURCE(addr) ((1UL<<24)|(((addr)&4194303UL)<<0))
#define CLEAR_COLOR_RGB(red,green,blue) ((2UL<<24)|(((red)&255UL)<<16)|(((green)&255UL)<<8)|(((blue)&255UL)<<0))

//#define TAG(s) ((3UL<<24)|(((s)&255UL)<<0))

#define TAG(s) (0x03000000UL | ((s) & 0xFF))

#define COLOR_RGB(red,green,blue) ((4UL<<24)|(((red)&255UL)<<16)|(((green)&255UL)<<8)|(((blue)&255UL)<<0))
//#define COLOR_RGB_NEW(color)      (0x04000000UL | ((color) & 0x00FFFFFFUL))

//#define COLOR_RGB(r,g,b) (0x02000000UL | (((r) & 0xFF) << 16L) | (((g) & 0xFF) << 8L) | ((b) & 0xFF))

#define BITMAP_HANDLE(handle) ((5UL<<24)|(((handle)&31UL)<<0))
#define CELL(cell) ((6UL<<24)|(((cell)&127UL)<<0))
#define BITMAP_LAYOUT(format,linestride,height) ((7UL<<24)|(((format)&31UL)<<19)|(((linestride)&1023UL)<<9)|(((height)&511UL)<<0))
#define BITMAP_SIZE(filter,wrapx,wrapy,width,height) ((8UL<<24)|(((filter)&1UL)<<20)|(((wrapx)&1UL)<<19)|(((wrapy)&1UL)<<18)|(((width)&511UL)<<9)|(((height)&511UL)<<0))
#define ALPHA_FUNC(func,ref) ((9UL<<24)|(((func)&7UL)<<8)|(((ref)&255UL)<<0))
#define STENCIL_FUNC(func,ref,mask) ((10UL<<24)|(((func)&7UL)<<16)|(((ref)&255UL)<<8)|(((mask)&255UL)<<0))
#define BLEND_FUNC(src,dst) ((11UL<<24)|(((src)&7UL)<<3)|(((dst)&7UL)<<0))
#define STENCIL_OP(sfail,spass) ((12UL<<24)|(((sfail)&7UL)<<3)|(((spass)&7UL)<<0))
#define POINT_SIZE(size) ((13UL<<24)|(((size)&8191UL)<<0))
#define LINE_WIDTH(width) ((14UL<<24)|(((width)&4095UL)<<0))
#define CLEAR_COLOR_A(alpha) ((15UL<<24)|(((alpha)&255UL)<<0))
#define COLOR_A(alpha) ((16UL<<24)|(((alpha)&255UL)<<0))
#define CLEAR_STENCIL(s) ((17UL<<24)|(((s)&255UL)<<0))
#define CLEAR_TAG(s) ((18UL<<24)|(((s)&255UL)<<0))
#define STENCIL_MASK(mask) ((19UL<<24)|(((mask)&255UL)<<0))
#define TAG_MASK(mask) ((20UL<<24)|(((mask)&1UL)<<0))
#define BITMAP_TRANSFORM_A(a) ((21UL<<24)|(((a)&131071UL)<<0))
#define BITMAP_TRANSFORM_B(b) ((22UL<<24)|(((b)&131071UL)<<0))
#define BITMAP_TRANSFORM_C(c) ((23UL<<24)|(((c)&16777215UL)<<0))
#define BITMAP_TRANSFORM_D(d) ((24UL<<24)|(((d)&131071UL)<<0))
#define BITMAP_TRANSFORM_E(e) ((25UL<<24)|(((e)&131071UL)<<0))
#define BITMAP_TRANSFORM_F(f) ((26UL<<24)|(((f)&16777215UL)<<0))
#define SCISSOR_XY(x,y) ((27UL<<24)|(((x)&2047UL)<<11)|(((y)&2047UL)<<0))
#define SCISSOR_SIZE(width,height) ((28UL<<24)|(((width)&4095UL)<<12)|(((height)&4095UL)<<0))
#define CALL(dest) ((29UL<<24)|(((dest)&65535UL)<<0))
#define JUMP(dest) ((30UL<<24)|(((dest)&65535UL)<<0))
#define BEGIN(prim) ((31UL<<24)|(((prim)&15UL)<<0))
#define COLOR_MASK(r,g,b,a) ((32UL<<24)|(((r)&1UL)<<3)|(((g)&1UL)<<2)|(((b)&1UL)<<1)|(((a)&1UL)<<0))
#define CLEAR(c,s,t) ((38UL<<24)|(((c)&1UL)<<2)|(((s)&1UL)<<1)|(((t)&1UL)<<0))
#define VERTEX_FORMAT(frac) ((39UL<<24)|(((frac)&7UL)<<0))
#define BITMAP_LAYOUT_H(linestride,height) ((40UL<<24)|(((linestride)&3UL)<<2)|(((height)&3UL)<<0))
#define BITMAP_SIZE_H(width,height) ((41UL<<24)|(((width)&3UL)<<2)|(((height)&3UL)<<0))
#define PALETTE_SOURCE(addr) ((42UL<<24)|(((addr)&4194303UL)<<0))

/*
#define VERTEX_TRANSLATE_X(x) ((43UL<<24)|(((x)&131071UL)<<0))
#define VERTEX_TRANSLATE_Y(y) ((44UL<<24)|(((y)&131071UL)<<0))
*/


#define VERTEX_TRANSLATE_X(x)   (0x2B000000UL | ((x) & 0x0001FFFFUL))
#define VERTEX_TRANSLATE_Y(y)   (0x2C000000UL | ((y) & 0x0001FFFFUL))

#define COLOR_RGB_NEW(color)    (0x04000000UL | ((color) & 0x00FFFFFFUL))
//------------------------------------------------------------------------------
//------------------------------------------------------------------------------


#define NOP()                   0x2D000000L             // ((45UL<<24))	
#define END()           	0x21000000L     	// ((33UL<<24))
#define SAVE_CONTEXT()          0x22000000L             // ((34UL<<24))
#define RESTORE_CONTEXT()       0x23000000L             // ((35UL<<24))
#define RETURN()                0x24000000L             // ((36UL<<24))
#define DISPLAY()               0x00000000L     	// ((0UL<<24))

#define MACRO(m) ((37UL<<24)|(((m)&1UL)<<0))
//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
//------------------------------------------------------------------------------



#define FT_GPU_NUMCHAR_PERFONT (128)
#define FT_GPU_FONT_TABLE_SIZE (148)

struct FontMetricBlock
{
 unsigned char CharWidth[128];

 unsigned long BitmapFormat;
 unsigned long LineStride;
 unsigned long MaxWidth;
 unsigned long MaxHeight;
 
 unsigned long BitmapAddress;
};

// FT81x and FT80x font table structure
// Font table address in ROM can be found by reading the address from 0xFFFFC location
// 16 font tables are present at the address read from location 0xFFFFC 

/*
typedef struct FT_Gpu_Fonts
{
 ft_uint8_t FontWidth[FT_GPU_NUMCHAR_PERFONT];  // Width of each character font from 0 to 127

 ft_uint32_t	FontBitmapFormat;               // Bitmap format of font wrt bitmap formats supported by FT800 - L1, L4, L8
 ft_uint32_t	FontLineStride;                 // Font line stride in FT800 ROM
 ft_uint32_t	FontWidthInPixels;              // Font width in pixels
 ft_uint32_t	FontHeightInPixels;	        // Font height in pixels
 ft_uint32_t	PointerToFontGraphicsData;      // Pointer to font graphics raw data
}FT_Gpu_Fonts_t;
*/
//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
//********************* FTDI EVE2 Coprocessor macros ***************************
//------------------------------------------------------------------------------
#define PACK(a,b)  ((((unsigned long)((a) & 0xFFFF)) << 16L) | ((b) & 0xFFFF))
#define POSXY(x,y) ((((unsigned long)((y) & 0xFFFF)) << 16L) | ((x) & 0xFFFF))

#define TXTPACK(a,b,c,d) ( (((unsigned long)((d) & 0xFF)) << 24L) | \
			   (((unsigned long)((c) & 0xFF)) << 16L) | \
			   (((unsigned long)((b) & 0xFF)) <<  8L) | \
			   (((unsigned long)((a) & 0xFF)) <<  0L) )
//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
#define OPT_CYR                 0x0001
#define OPT_PRESS               0x0080
//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
#define OPT_CENTERY_RIGHTX      (OPT_CENTERY | OPT_RIGHTX)
#define OPT_CY_RX               (OPT_CENTERY | OPT_RIGHTX)

#define OPT_CENTERY_CENTERX     (OPT_CENTERY | OPT_CENTERX)
#define OPT_CY_CX               (OPT_CENTERY | OPT_CENTERX)
//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
/*
// НЕ РАЗБИРАЛОСЬ и НЕ ТЕСТИРОВАЛОСЬ!!!
#define VERTEX2F(x,y) ((1UL<<30)|(((x)&32767UL)<<15)|(((y)&32767UL)<<0))
#define VERTEX2II(x,y,handle,cell) ((2UL<<30)|(((x)&511UL)<<21)|(((y)&511UL)<<12)|(((handle)&31UL)<<7)|(((cell)&127UL)<<0))
#define BITMAP_SOURCE(addr) ((1UL<<24)|(((addr)&4194303UL)<<0))
#define CLEAR_COLOR_RGB(red,green,blue) ((2UL<<24)|(((red)&255UL)<<16)|(((green)&255UL)<<8)|(((blue)&255UL)<<0))
#define TAG(s) ((3UL<<24)|(((s)&255UL)<<0))
#define COLOR_RGB(red,green,blue) ((4UL<<24)|(((red)&255UL)<<16)|(((green)&255UL)<<8)|(((blue)&255UL)<<0))
#define BITMAP_HANDLE(handle) ((5UL<<24)|(((handle)&31UL)<<0))
#define CELL(cell) ((6UL<<24)|(((cell)&127UL)<<0))
#define BITMAP_LAYOUT(format,linestride,height) ((7UL<<24)|(((format)&31UL)<<19)|(((linestride)&1023UL)<<9)|(((height)&511UL)<<0))
#define BITMAP_SIZE(filter,wrapx,wrapy,width,height) ((8UL<<24)|(((filter)&1UL)<<20)|(((wrapx)&1UL)<<19)|(((wrapy)&1UL)<<18)|(((width)&511UL)<<9)|(((height)&511UL)<<0))
#define ALPHA_FUNC(func,ref) ((9UL<<24)|(((func)&7UL)<<8)|(((ref)&255UL)<<0))
#define STENCIL_FUNC(func,ref,mask) ((10UL<<24)|(((func)&7UL)<<16)|(((ref)&255UL)<<8)|(((mask)&255UL)<<0))
#define BLEND_FUNC(src,dst) ((11UL<<24)|(((src)&7UL)<<3)|(((dst)&7UL)<<0))
#define STENCIL_OP(sfail,spass) ((12UL<<24)|(((sfail)&7UL)<<3)|(((spass)&7UL)<<0))
#define POINT_SIZE(size) ((13UL<<24)|(((size)&8191UL)<<0))
#define LINE_WIDTH(width) ((14UL<<24)|(((width)&4095UL)<<0))
#define CLEAR_COLOR_A(alpha) ((15UL<<24)|(((alpha)&255UL)<<0))
#define COLOR_A(alpha) ((16UL<<24)|(((alpha)&255UL)<<0))
#define CLEAR_STENCIL(s) ((17UL<<24)|(((s)&255UL)<<0))
#define CLEAR_TAG(s) ((18UL<<24)|(((s)&255UL)<<0))
#define STENCIL_MASK(mask) ((19UL<<24)|(((mask)&255UL)<<0))
#define TAG_MASK(mask) ((20UL<<24)|(((mask)&1UL)<<0))
#define BITMAP_TRANSFORM_A(a) ((21UL<<24)|(((a)&131071UL)<<0))
#define BITMAP_TRANSFORM_B(b) ((22UL<<24)|(((b)&131071UL)<<0))
#define BITMAP_TRANSFORM_C(c) ((23UL<<24)|(((c)&16777215UL)<<0))
#define BITMAP_TRANSFORM_D(d) ((24UL<<24)|(((d)&131071UL)<<0))
#define BITMAP_TRANSFORM_E(e) ((25UL<<24)|(((e)&131071UL)<<0))
#define BITMAP_TRANSFORM_F(f) ((26UL<<24)|(((f)&16777215UL)<<0))
#define SCISSOR_XY(x,y) ((27UL<<24)|(((x)&2047UL)<<11)|(((y)&2047UL)<<0))
#define SCISSOR_SIZE(width,height) ((28UL<<24)|(((width)&4095UL)<<12)|(((height)&4095UL)<<0))
#define CALL(dest) ((29UL<<24)|(((dest)&65535UL)<<0))
#define JUMP(dest) ((30UL<<24)|(((dest)&65535UL)<<0))
#define BEGIN(prim) ((31UL<<24)|(((prim)&15UL)<<0))
#define COLOR_MASK(r,g,b,a) ((32UL<<24)|(((r)&1UL)<<3)|(((g)&1UL)<<2)|(((b)&1UL)<<1)|(((a)&1UL)<<0))
#define CLEAR(c,s,t) ((38UL<<24)|(((c)&1UL)<<2)|(((s)&1UL)<<1)|(((t)&1UL)<<0))
#define VERTEX_FORMAT(frac) ((39UL<<24)|(((frac)&7UL)<<0))
#define BITMAP_LAYOUT_H(linestride,height) ((40UL<<24)|(((linestride)&3UL)<<2)|(((height)&3UL)<<0))
#define BITMAP_SIZE_H(width,height) ((41UL<<24)|(((width)&3UL)<<2)|(((height)&3UL)<<0))
#define PALETTE_SOURCE(addr) ((42UL<<24)|(((addr)&4194303UL)<<0))
#define VERTEX_TRANSLATE_X(x) ((43UL<<24)|(((x)&131071UL)<<0))
#define VERTEX_TRANSLATE_Y(y) ((44UL<<24)|(((y)&131071UL)<<0))
#define NOP() ((45UL<<24))
#define END() ((33UL<<24))
#define SAVE_CONTEXT() ((34UL<<24))
#define RESTORE_CONTEXT() ((35UL<<24))
#define RETURN() ((36UL<<24))
#define MACRO(m) ((37UL<<24)|(((m)&1UL)<<0))
#define DISPLAY() ((0UL<<24))
*/
//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
#endif //__EVE_DATA_TYPES_H
//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
//------------------------------------------------------------------------------