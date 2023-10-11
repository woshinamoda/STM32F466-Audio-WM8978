#ifndef	SYSTEM_AUDIOPLAY_H
#define SYSTEM_AUDIOPLAY_H

#include "system_wavplay.h"  
#include "fatfs.h"
#include "ff.h"	



#define I2S_TX_DMA_BUF_SIZE    10240	//定义TX DMA 数组大小(播放192Kbps@24bit的时候,需要设置8192大才不会卡)


//音乐播放控制器
typedef __packed struct
{  
	//2个I2S解码的BUF
	uint8_t *i2sbuf1;
	uint8_t *i2sbuf2; 	
	uint8_t *tbuf;						//零时数组,仅在24bit解码的时候需要用到
//	FIL *file;								//音频文件指针
	uint8_t status;						//bit0:0,暂停播放;1,继续播放
														//bit1:0,结束播放;1,开启播放 
}__audiodev; 
extern __audiodev audiodev;	//音乐播放控制器





void wav_i2s_dma_callback(void);

void audio_start(void);
void audio_stop(void);
uint16_t audio_get_tnum(uint8_t *path);
void audio_index_show(uint16_t index,uint16_t total);
void audio_msg_show(uint32_t totsec,uint32_t cursec,uint32_t bitrate);
void audio_play(void);
uint8_t audio_play_song(uint8_t* fname);























#endif