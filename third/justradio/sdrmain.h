#ifndef SDRMAIN_H
#define SDRMAIN_H

typedef struct _Au
{
	int freq[8];
	int sq[8];
	int idx;
}Au;



int vmain(const char* modulation,			//-M
				   uint32_t tuner_freq,			//-f
				   uint32_t sample_rate,		//-s
				   uint32_t re_sample_rate,		//-r
				   int gain=-100,			//-g
				   const char* edge=nullptr,	//-g
				   int devindex=0);				//-d

void break_palyer();
void set_freq(int freq);
void set_mod(const char* modulation);
int scan_freq(int freq);



#endif // SDRMAIN_H
