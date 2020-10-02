#ifndef AOCLS_H
#define AOCLS_H

#include <ao/ao.h>

#define DEFAULT_SAMPLE_RATE		24000
#define DEFAULT_BUF_LENGTH		(1 * 16384)
#define MAXIMUM_OVERSAMPLE		8
#define MAXIMUM_BUF_LENGTH		(MAXIMUM_OVERSAMPLE * DEFAULT_BUF_LENGTH)
#define AUTO_GAIN			-100
#define BUFFER_DUMP			4096

#define FREQUENCIES_LIMIT		1000
class AoCls
{
public:
    AoCls(int bits, int rate, int channels);
    ~AoCls();
    bool play(uint8_t* buff, size_t sz);

private:

    ao_device*       _dev = nullptr;
    ao_sample_format _format;
};

#endif // AOCLS_H
