#include "radio_functions.h"


static int fd = -1;
static int factor;
static int mix_fd = -1;
static int mix_chans = (1 << SOUND_MIXER_NRDEVICES) - 1;

struct mix_channel
{
    char *label;

    int left;
    int right;
    int dev_num;

} mix_channel[20];


//init mixer device
int init_mixer(const char *device)
{
    mix_fd = open(device, O_RDWR);

    if (mix_fd < 0)
    {
        fprintf(stderr,"Can't open mixer device %s\n", device);
        return -1;
    }

    if (ioctl(mix_fd, SOUND_MIXER_READ_DEVMASK, &mix_chans) < 0)
    {
        fprintf(stderr,"Can't get supported channels for device %s, \"%s\"\n",
                device, strerror(errno));
        fprintf(stderr,"Assume that all possible are present\n");
        mix_chans = (1 << SOUND_MIXER_NRDEVICES) - 1;
    }

    // fprintf(stderr,"Found channels %x for device %s\n", mix_chans, device);

    return 1;
}


// is a channel supported by mixer?
int mixer_has_channel(int dev)
{
    if ((dev < 0) || (dev >= SOUND_MIXER_NRDEVICES))
        return 0;
    if (mix_fd < 0)
        return 1; /* turned off is assumed to have all channels */
    return ((mix_chans & (1 << dev)) != 0);
}


// find a channel
int channel_by_name(const char* name)
{
    const char* devices_table[] = SOUND_DEVICE_NAMES;
    int i;
    for (i = 0; i < SOUND_MIXER_NRDEVICES; i++)
    {
        if (!strcmp(devices_table[i],name))
            return i;
    }
    return -1;
}


//close mixer device
void close_mixer(void)
{
    if (mix_fd > 0)
        close(mix_fd);
    mix_fd = -1;
    mix_chans = (1 << SOUND_MIXER_NRDEVICES) - 1;
}


void set_volume(int vol, int dev)
{
    unsigned char temp[4];
    int channel = 1;
    int left = vol;
    int right = vol;
    if (mix_fd < 0)
    {
        fprintf(stderr,"Mixer device is not open, can't set volume\n");
        return;
    }
//    if (!mixer_has_channel(dev))
//    {
//        fprintf(stderr, "No channel %d, can't set volume\n", dev);
//        return;
//    }
    mix_channel[channel].left = left;
    mix_channel[channel].right = right;
    mix_channel[channel].dev_num = dev;
    temp[0]=(unsigned char)left;
    temp[1]=(unsigned char)right;
    temp[2]=temp[3]=0;
    if (ioctl(mix_fd, MIXER_WRITE(mix_channel[channel].dev_num), temp) < 0)
        fprintf(stderr,"Can't set volume for device %d, \"%s\"\n", dev, strerror(errno));
}


int get_volume(int dev)
{
    unsigned char temp[4];
    int channel = 1;
    if (mix_fd < 0)
    {
        fprintf(stderr,"Mixer device is not open, can't get volume\n");
        return 0;
    }
//    if (!mixer_has_channel(dev))
//    {
//        fprintf(stderr, "No channel %d, can't get volume, returning 0\n", dev);
//        return 0;
//    }
    mix_channel[channel].dev_num = dev;
    if (ioctl(mix_fd, MIXER_READ(mix_channel[channel].dev_num), &temp) < 0)
    {
        fprintf(stderr,"Can't get volume for device %d, \"%s\"\n", dev, strerror(errno));
        return 0;
    }

    if ( temp[0] >= temp[1] )
    {
        if (temp[0] <= 100)
            return temp[0];
        else
            return 0;
    }
    else
    {
        if (temp[1] <=100)
            return temp[1];
        else
            return 0;
    }
}


int initialize(const char *device)
{
    struct v4l2_capability caps;
    struct v4l2_tuner tuner;

    fd = open(device,O_RDWR);
    if (fd < 0)
    {
        fprintf(stderr,"Can't open device %s, error \"%s\"\n", device, strerror(errno));
        return 0;/*fail*/
    }
    else
        fprintf(stderr,"Device %s opened\n", device);

    memset(&caps,0,sizeof(caps));
    if (ioctl(fd,VIDIOC_QUERYCAP,&caps) < 0)
    {
        fprintf(stderr,"Requesting v4l2 ended up with \"%s\"\n",strerror(errno));
        close(fd);
        fd = -1;
        return 0;
    }
    else
    {
        fprintf(stderr,"Video4Linux2 detected\n");
    }

    if ((caps.capabilities & V4L2_CAP_TUNER) == 0)
    {
        fprintf(stderr,"Not a radio tuner\n");
        close(fd);
        fd = -1;
        return 0;
    }

    memset(&tuner,0,sizeof(tuner));
    if (ioctl(fd,VIDIOC_G_TUNER, &tuner) < 0)
    {
        fprintf(stderr,"Not a radio tuner\n");
        close(fd);
        fd = -1;
        return 0;
    }

    if ((tuner.type & V4L2_TUNER_RADIO) == 0)
    {
        fprintf(stderr,"Not a radio tuner\n");
        close(fd);
        fd = -1;
        return 0;
    }

    if ((tuner.capability & V4L2_TUNER_CAP_LOW) == 0)
        factor = 16;
    else
        factor = 16000;

    return 1; /*success*/
}


void close_radio(void)
{
    mute(1);

    if ( fd > 0)
        close(fd);
    fd = -1;
}


int set_frequency(float freqn)
{
    struct v4l2_frequency freq;

    if ( fd < 0 )
        return 0;

    if ( (freqn > 108) || (freqn < 65) )
        return 1;

    memset(&freq, 0, sizeof(freq));
    freq.type = V4L2_TUNER_RADIO;
    freq.frequency = ( freqn+1.0/32 )*factor;

    if (ioctl(fd, VIDIOC_S_FREQUENCY, &freq) < 0)
    {
        fprintf(stderr,"Cannot set frequency\n");
        return -1;
    }

    return 0;
}


void mute(int value)
{
    struct v4l2_control ctrl;

    if ( fd < 0 )
        return;

    memset(&ctrl, 0, sizeof(ctrl));
    ctrl.id=V4L2_CID_AUDIO_MUTE;
    ctrl.value=value;

    if (ioctl(fd, VIDIOC_S_CTRL, &ctrl) < 0)
        fprintf(stderr,"Cannot %s radio tuner\n", (value ? "mute" : "unmute"));
}


int check_stereo(int force)
{
    struct v4l2_tuner tuner;

    if ( fd < 0 )
        return -1;

    memset(&tuner,0,sizeof(tuner));
    if (ioctl(fd,VIDIOC_G_TUNER,&tuner) < 0)
    {
        fprintf(stderr,"Cannot get tuner properties\n");
        return -1;
    }

    // fprintf(stderr,"Before: rxsubchans=%2x, audmode=%2x\n",
    //            tuner.rxsubchans, tuner.audmode);

    if (force == V4L2_TUNER_SUB_MONO)
    {
        if (tuner.audmode == V4L2_TUNER_MODE_MONO)
            goto out; /* already in mono */
        memset(&tuner,0,sizeof(tuner));
        tuner.audmode = V4L2_TUNER_MODE_MONO;
        if ( ioctl(fd,VIDIOC_S_TUNER,&tuner) < 0)
            fprintf(stderr,"Refused to set tuner to mono mode\n");
        else
            fprintf(stderr,"Tried to set tuner to mono mode\n");
        memset(&tuner,0,sizeof(tuner));
        if (ioctl(fd,VIDIOC_G_TUNER,&tuner) < 0)
        {
            fprintf(stderr,"Cannot get tuner properties after setting\n");
            return -1;
        }
    }
    else
    if ((force == V4L2_TUNER_SUB_STEREO) && (tuner.capability & V4L2_TUNER_CAP_STEREO))
    {
        if (tuner.audmode == V4L2_TUNER_MODE_STEREO)
            goto out; /* already in stereo */
        memset(&tuner,0,sizeof(tuner));
        tuner.audmode = V4L2_TUNER_MODE_STEREO;
        if ( ioctl(fd,VIDIOC_S_TUNER,&tuner) < 0)
            fprintf(stderr,"Refused to set tuner to stereo mode\n");
        else
            fprintf(stderr,"Tried to set tuner to stereo mode\n");
        memset(&tuner,0,sizeof(tuner));
        if (ioctl(fd,VIDIOC_G_TUNER,&tuner) < 0)
        {
            fprintf(stderr,"Cannot get tuner properties after setting\n");
            return -1;
        }
    }
    /* else nothing to do */
  out:
    // fprintf(stderr,"After: rxsubchans=%2x, audmode=%2x\n",
    //            tuner.rxsubchans, tuner.audmode);
    switch (tuner.audmode)
    {
        case V4L2_TUNER_MODE_MONO:
            return 0;
        case V4L2_TUNER_MODE_STEREO:
            return (tuner.rxsubchans == V4L2_TUNER_SUB_STEREO);
        default:
            return -1;
    }

}


int signal_strength(void)
{
    struct v4l2_tuner tuner;
    int strength;

    if ( fd < 0)
        return -1;

    memset(&tuner,0,sizeof(tuner));
    if (ioctl(fd, VIDIOC_G_TUNER, &tuner) < 0)
        return -1;

    strength = tuner.signal >> 13; // Always?
    // fprintf(stderr, "%d %d\n", tuner.signal, strength);
    return strength;
}


/*NOW INFO FUNCTIONS*/
int can_stereo(void)
{
    struct v4l2_tuner tuner;

    if ( fd < 0)
        return -1;

    memset(&tuner,0,sizeof(tuner));
    if (ioctl(fd, VIDIOC_G_TUNER, &tuner) < 0)
        return -1;
    if (tuner.capability & V4L2_TUNER_CAP_STEREO)
        return 1;
    else
        return 0;
}


int can_rds(void)
{
    struct v4l2_tuner tuner;

    if ( fd < 0)
        return -1;

    memset(&tuner,0,sizeof(tuner));
    if (ioctl(fd, VIDIOC_G_TUNER, &tuner) < 0)
        return -1;
    if (tuner.capability & V4L2_TUNER_CAP_RDS)
        return 1;
    else
        return 0;
}


/*
char* get_tuner_name(void)
{
	static struct v4l2_tuner tuner;

	if ( fd < 0)
		return ("Error");

	memset(&tuner,0,sizeof(tuner));
	if (ioctl(fd, VIDIOC_G_TUNER, &tuner) < 0)
		return ("Error");
	return(tuner.name);
}

char* get_radio_name(void)
{
	static struct v4l2_audio audio;

	if ( fd < 0)
		return ("Error");

	memset(&audio,0,sizeof(audio));
	if (ioctl(fd, VIDIOC_G_AUDIO, &tuner) < 0)
		return ("Error");
	return(audio.name);
}*/

