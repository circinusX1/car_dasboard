#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>
#include <fcntl.h>
#include <sys/time.h>
#include <sys/ioctl.h>
#include <asm/types.h>
#include <linux/videodev2.h>
#include <sys/soundcard.h>

int init_mixer(const char*);
void close_mixer(void);
int mixer_has_channel(int);
int channel_by_name(const char*);
int volume_set(int, int);
int volume_get(int);

int initialize(const char*);
void close_radio(void);
int set_frequency(float);
void mute(int);
int check_stereo(int);
int signal_strength(void);
int can_stereo(void);
int can_rds(void);


