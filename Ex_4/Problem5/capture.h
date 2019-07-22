/*
 *
 *  Adapted by Sam Siewert for use with UVC web cameras and Bt878 frame
 *  grabber NTSC cameras to acquire digital video from a source,
 *  time-stamp each frame acquired, save to a PGM or PPM file.
 *
 *  The original code adapted was open source from V4L2 API and had the
 *  following use and incorporation policy:
 * 
 *  This program can be used and distributed without restrictions.
 *
 *      This program is provided with the V4L2 API
 * see http://linuxtv.org/docs.php for more information
 *
 * Compile by terminal: $ gcc capture.c problem5.h brighten.h sharpen.h capture.h -o capture
 */
 
/********************************************
* Code is base on the following code provided in class 
* - simple-capture/capture.c
* Compile code by using the gcc command below:
* $ gcc capture.c problem5.h brighten.h sharpen.h capture.h -o capture
* or use the Makefile and type "make"
*******************************************/

#ifndef CAP
#define CAP

#include <errno.h>

#define CLEAR(x) memset(&(x), 0, sizeof(x))
#define COLOR_CONVERT

/* Need to redefine based on user input */
#define HRES_STR "160"
#define VRES_STR "120"


/* Global Variables */
// Format is used by a number of functions, so made as a file global
static struct v4l2_format fmt;
char ppm_header[]="P6\n#9999999999 sec 9999999999 msec \n"HRES_STR" "VRES_STR"\n255\n";
char ppm_dumpname[]="pic000000.ppm";
static char            *dev_name;
static int              fd = -1;
struct buffer          *buffers;
static unsigned int     n_buffers;
static int              out_buf;
static int              force_format=1;
static int              frame_count = 5;
unsigned int framecnt=0;
unsigned char bigbuffer[(1280*960)];
static const char short_options[10] = "d:hmruofc:";
/*static const struct option
long_options[] = {
        { "device", required_argument, NULL, 'd' },
        { "help",   no_argument,       NULL, 'h' },
        { "mmap",   no_argument,       NULL, 'm' },
        { "read",   no_argument,       NULL, 'r' },
        { "userp",  no_argument,       NULL, 'u' },
        { "output", no_argument,       NULL, 'o' },
        { "format", no_argument,       NULL, 'f' },
        { "count",  required_argument, NULL, 'c' },
        { 0, 0, 0, 0 }
};*/
enum io_method 
{
        IO_METHOD_READ,
        IO_METHOD_MMAP,
        IO_METHOD_USERPTR,
};

//static enum io_method   io = IO_METHOD_USERPTR;
//static enum io_method   io = IO_METHOD_READ;
static enum io_method   io = IO_METHOD_MMAP;

struct buffer 
{
        void   *start;
        size_t  length;
};

/* Function Prototypes */
static void errno_exit(const char *s);
static int xioctl(int fh, int request, void *arg);
static void dump_ppm(const void *p, int size, unsigned int tag, struct timespec *time);
void yuv2rgb(int y, int u, int v, unsigned char *r, unsigned char *g, unsigned char *b);
static void process_image(const void *p, int size);
static int read_frame(void);
static void mainloop(void);
static void stop_capturing(void);
static void start_capturing(void);
static void uninit_device(void);
static void init_read(unsigned int buffer_size);
static void init_mmap(void);
static void init_userp(unsigned int buffer_size);
static void init_device(void);
static void close_device(void);
static void open_device(void);
static void usage(FILE *fp, int argc, char **argv);

#endif
