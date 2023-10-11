#ifndef	SYSTEM_AUDIOPLAY_H
#define SYSTEM_AUDIOPLAY_H

#include "system_wavplay.h"  
#include "fatfs.h"
#include "ff.h"	



#define I2S_TX_DMA_BUF_SIZE    10240	//����TX DMA �����С(����192Kbps@24bit��ʱ��,��Ҫ����8192��Ų��Ῠ)


//���ֲ��ſ�����
typedef __packed struct
{  
	//2��I2S�����BUF
	uint8_t *i2sbuf1;
	uint8_t *i2sbuf2; 	
	uint8_t *tbuf;						//��ʱ����,����24bit�����ʱ����Ҫ�õ�
//	FIL *file;								//��Ƶ�ļ�ָ��
	uint8_t status;						//bit0:0,��ͣ����;1,��������
														//bit1:0,��������;1,�������� 
}__audiodev; 
extern __audiodev audiodev;	//���ֲ��ſ�����





void wav_i2s_dma_callback(void);

void audio_start(void);
void audio_stop(void);
uint16_t audio_get_tnum(uint8_t *path);
void audio_index_show(uint16_t index,uint16_t total);
void audio_msg_show(uint32_t totsec,uint32_t cursec,uint32_t bitrate);
void audio_play(void);
uint8_t audio_play_song(uint8_t* fname);























#endif