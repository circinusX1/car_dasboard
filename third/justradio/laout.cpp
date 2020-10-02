
#include <errno.h>
#include <string.h>
#include <assert.h>
#include <assert.h>
#include <stdio.h>
#include <stdint.h>
#include "laout.h"



AoCls::AoCls(int bits, int rate, int channels)
{
    _format.bits = bits;
    _format.rate = rate;
    _format.channels = channels;
    _format.byte_format = AO_FMT_NATIVE;
    _format.matrix = 0;
    ::ao_initialize();

    int driver = ::ao_default_driver_id();
    _dev = ao_open_live(driver, &_format, NULL);
    if(_dev==nullptr)
    {
        perror("ao_open(): trying alsa");
        driver = ::ao_driver_id("alsa");
        _dev = ::ao_open_live(driver, &_format, NULL);
    }
    if(_dev==nullptr)
    {
        perror("ao_open(): ");
    }

}

AoCls::~AoCls()
{
    if(_dev)
        ::ao_close(_dev);
    ::ao_shutdown();
}


bool AoCls::play(uint8_t* buff, size_t sz)
{
    ::ao_play(_dev, (char*)buff, sz);
    return true;
}

