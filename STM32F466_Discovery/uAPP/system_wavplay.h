#ifndef _AUDIO_WAVPLAY_H
#define _AUDIO_WAVPLAY_H


#include "main.h"

#ifdef __cplusplus
extern   "C" {
#endif
	
#include "system_exfuns.h"
#include "fatfs.h"
#include "ff.h"	

	
	

	
#define WAV_I2S_TX_DMA_BUFSIZE    4096		//����WAV TX DMA �����С(����192Kbps@24bit��ʱ��,��Ҫ����8192��Ų��Ῠ)

	



//RIFF��
typedef __packed struct
{
    uint32_t ChunkID;		   	//chunk id;����̶�Ϊ"RIFF",��0X46464952
    uint32_t ChunkSize ;		//���ϴ�С;�ļ��ܴ�С-8
    uint32_t Format;	   		//��ʽ;WAVE,��0X45564157
}ChunkRIFF ;
//fmt��
typedef __packed struct
{
    uint32_t ChunkID;		   	//chunk id;����̶�Ϊ"fmt ",��0X20746D66
    uint32_t ChunkSize ;		//�Ӽ��ϴ�С(������ID��Size);����Ϊ:20.
    uint16_t AudioFormat;	  //��Ƶ��ʽ;0X01,��ʾ����PCM;0X11��ʾIMA ADPCM
	uint16_t NumOfChannels;		//ͨ������;1,��ʾ������;2,��ʾ˫����;
	uint32_t SampleRate;			//������;0X1F40,��ʾ8Khz
	uint32_t ByteRate;				//�ֽ�����; 
	uint16_t BlockAlign;			//�����(�ֽ�); 
	uint16_t BitsPerSample;		//�����������ݴ�С;4λADPCM,����Ϊ4
//	uint16_t ByteExtraData;		//���ӵ������ֽ�;2��; ����PCM,û���������
}ChunkFMT;	   
//fact�� 
typedef __packed struct 
{
    uint32_t ChunkID;		   	//chunk id;����̶�Ϊ"fact",��0X74636166;
    uint32_t ChunkSize ;		//�Ӽ��ϴ�С(������ID��Size);����Ϊ:4.
    uint32_t NumOfSamples;	//����������; 
}ChunkFACT;
//LIST�� 
typedef __packed struct 
{
    uint32_t ChunkID;		   	//chunk id;����̶�Ϊ"LIST",��0X74636166;
    uint32_t ChunkSize ;		//�Ӽ��ϴ�С(������ID��Size);����Ϊ:4. 
}ChunkLIST;

//data�� 
typedef __packed struct 
{
    uint32_t ChunkID;		   	//chunk id;����̶�Ϊ"data",��0X5453494C
    uint32_t ChunkSize ;		//�Ӽ��ϴ�С(������ID��Size) 
}ChunkDATA;

//wavͷ
typedef __packed struct
{ 
	ChunkRIFF riff;	//riff��
	ChunkFMT fmt;  	//fmt��
//	ChunkFACT fact;	//fact�� ����PCM,û������ṹ��	 
	ChunkDATA data;	//data��		 
}__WaveHeader; 

//wav ���ſ��ƽṹ��
typedef __packed struct
{ 
	uint16_t audioformat;			//��Ƶ��ʽ;0X01,��ʾ����PCM;0X11��ʾIMA ADPCM
	uint16_t nchannels;				//ͨ������;1,��ʾ������;2,��ʾ˫����; 
	uint16_t blockalign;			//�����(�ֽ�);  
	uint32_t datasize;				//WAV���ݴ�С 
	uint32_t totsec ;					//���׸�ʱ��,��λ:��
	uint32_t cursec ;					//��ǰ����ʱ��
	uint32_t bitrate;	   			//������(λ��)
	uint32_t samplerate;			//������ 
	uint16_t bps;							//λ��,����16bit,24bit,32bit
	uint32_t datastart;				//����֡��ʼ��λ��(���ļ������ƫ��)
}__wavctrl; 




uint8_t	wav_decode_init(uint8_t* fname, __wavctrl* wavx);				//WAV����
uint8_t wav_buffill(uint8_t *buf,uint16_t size,uint8_t bits);	//buf���
void wav_i2s_dma_tx_callback(void);															//DMA�ж���Ļص�����
																																//�����ϻ������
uint8_t wav_play_song(uint8_t *filename);												//�������֣�����������ִ��	
void Audio_Process(void);
	
	
void Audio_stop();
void Audio_start();
void Key_SetAudio();
#ifdef __cplusplus
        }
#endif


#endif
				
				
				