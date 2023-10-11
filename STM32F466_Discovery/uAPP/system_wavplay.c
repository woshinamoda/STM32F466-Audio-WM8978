#include "system_wavplay.h"  
#include "fatfs.h"
#include "system_malloc.h"	  
#include "audio_wm8978_I2C.h"
#include "system_key.h"


#include "i2s.h"

__wavctrl wavctrl;			  				//WAV���ƽṹ��


#define BUF_SIZE 4096							//4KB����Ƶ����
uint8_t wav_buff[BUF_SIZE];				//��Ƶ�Ļ�����
uint8_t buff_state;								// 1����������� 0 - 1/2 ǰ�벿��
																	// 2����������� 1/2 - 1 ��벿��
uint8_t PlayingSig;								// 0:ֹͣ����			1�����ڲ���
extern uint32_t File_Size;				//�ļ���С!
extern uint8_t volum;							//������С
extern uint16_t act_volum;	


extern uint8_t K4_set_OK;
extern uint8_t play_cnt;
extern uint8_t *play_name;


uint8_t	wav_decode_init(uint8_t* fname, __wavctrl* wavx)
{

	uint8_t	buf[512];			//����ָ��
	uint32_t br=0;
	uint8_t res=0;

	ChunkRIFF *riff;
	ChunkFMT *fmt;
	ChunkFACT *fact;
	ChunkDATA *data;
	res=f_open(&SDFile,(TCHAR*)fname,FA_READ);		//��SD���ļ���
	if(res == FR_OK)
	{//�ļ��ܴ�
		uint32_t number_buf;
		f_read(&SDFile,buf,512,&br);	//��ȡ512�ֽ�������
		riff=(ChunkRIFF *)buf;		//��ȡRIFF��
		if(riff->Format==0X45564157)//��WAV�ļ�
		{
			fmt=(ChunkFMT *)(buf+12);	//��ȡFMT�� 
			fact=(ChunkFACT *)(buf+12+8+fmt->ChunkSize);//��ȡFACT��
			if(fact->ChunkID==0X74636166||fact->ChunkID==0X5453494C)wavx->datastart=12+8+fmt->ChunkSize+8+fact->ChunkSize;//����fact/LIST���ʱ��(δ����)
			else wavx->datastart=12+8+fmt->ChunkSize;  
			data=(ChunkDATA *)(buf+wavx->datastart);	//��ȡDATA��
			if(data->ChunkID==0X61746164)//�����ɹ�!
			{
				wavx->audioformat=fmt->AudioFormat;			//��Ƶ��ʽ
				wavx->nchannels=fmt->NumOfChannels;			//ͨ����
				wavx->samplerate=fmt->SampleRate;				//������
				wavx->bitrate=fmt->ByteRate*8;					//�õ�λ��
				wavx->blockalign=fmt->BlockAlign;				//�����
				wavx->bps=fmt->BitsPerSample;						//λ��,16/24/32λ
				wavx->datasize=data->ChunkSize;					//���ݿ��С
				wavx->datastart=wavx->datastart+8;			//��������ʼ�ĵط�. 
				number_buf = wavx->datasize;
				number_buf = number_buf + wavx->datastart;
				File_Size = number_buf;
				uPrintf("wavx->audioformat:%d\r\n",wavx->audioformat);
				uPrintf("wavx->nchannels:%d\r\n",wavx->nchannels);
				uPrintf("wavx->samplerate:%d\r\n",wavx->samplerate);
				uPrintf("wavx->bitrate:%d\r\n",wavx->bitrate);
				uPrintf("wavx->blockalign:%d\r\n",wavx->blockalign);
				uPrintf("wavx->bps:%d\r\n",wavx->bps);
				uPrintf("wavx->datasize:%d\r\n",wavx->datasize);
				uPrintf("wavx->datastart:%d\r\n",wavx->datastart);  
			}else res=3;//data����δ�ҵ�.
		}else res=2;//��wav�ļ�	
	}else res=1;//���ļ�����
f_close(&SDFile);	
	return 0;
}
	





//���buf
//buf:������
//size:���������
//bits:λ��(16/24)
//����ֵ:���������ݸ���
uint8_t tbuf[4];
uint8_t wav_buffill(uint8_t *buf,uint16_t size,uint8_t bits)
{
	uint16_t	readlen = 0;
	uint32_t	bread;
	uint16_t	i;
	uint8_t	*	p;
	uint8_t	res;
	if(bits==24)													//24bit��Ƶ,��Ҫ����һ��
	{
		readlen=(size/4)*3;									//�˴�Ҫ��ȡ���ֽ���
		
		res = f_read(&SDFile,tbuf,readlen,(UINT*)&bread);	
		p=tbuf;
		for(i=0;i<size;)
		{
			buf[i++]=p[1];
			buf[i]=p[2]; 
			i+=2;
			buf[i++]=p[0];
			p+=3;
		} 
		bread=(bread*4)/3;									//ȥ����3��8λ���������ݣ� 4bytes -> 3bytes
	}
	else 
	{
		res = f_read(&SDFile,buf,size,(UINT*)&bread);	//16bit��Ƶ,ֱ�Ӷ�ȡ����  
		if(bread<size)																//����������,����0
		{
			for(i=bread;i<size-bread;i++)buf[i]=0; 
		}
	}
	return res;
}





//��������
uint8_t wav_play_song(uint8_t *filename)
{
	uint8_t t=0;
	uint8_t res;
	uint32_t fillnum;
	
	if(PlayingSig == 0)	//û���ڲ��Ÿ��������Բ����µ�����
	{
		res = wav_decode_init(filename, &wavctrl);	//�õ��ļ�����Ϣ
		if(res == 0)
		{//�����ļ��ɹ�
			if(wavctrl.bps==16)	
			{
				WM8978_Audio_FormatCFG(AUDIO_FORMAT_I2S,AUDIO_WORD_16BIT);
				I2S2_ReInit(I2S_STANDARD_PHILIPS,I2S_MODE_MASTER_TX,I2S_CPOL_LOW,I2S_DATAFORMAT_16B_EXTENDED);
			}
			else if(wavctrl.bps==24)
			{
				WM8978_Audio_FormatCFG(AUDIO_FORMAT_I2S,AUDIO_WORD_24BIT);
				I2S2_ReInit(I2S_STANDARD_PHILIPS,I2S_MODE_MASTER_TX,I2S_CPOL_LOW,I2S_DATAFORMAT_24B);
			}
			
			I2S_AUDIO_SetFrequency(I2S_AUDIOFREQ_44K);	//I2S��ʼ������MAX_I2S_Init(), ��������һ��Ƶ���޸�			
			
	/******************������ɣ�������ļ����뻺����**********************************/
			res=f_open(&SDFile,(TCHAR*)filename,FA_READ);	//���ļ�
			if(res == 0)
			{
				f_lseek(&SDFile, wavctrl.datastart);								//�����ļ�ͷ
				wav_buffill(wav_buff , BUF_SIZE, wavctrl.bps);			//�����������
				I2S_AUDIO_Play((uint16_t*)&wav_buff[0], BUF_SIZE);  //�������䣨�ڲ�����DMA��
				PlayingSig = 1;																			//����״̬����ʼ����
			}else res = 0xff;
		}else res = 0xff;
	}
	return res;
}


#include "dma.h"

//���DMA��������仺�庯��
void Audio_Process(void)
{
	uint16_t br;
	if(PlayingSig == 1)    //���벥��
	{
		if(buff_state == 1)  //�Ѿ�������һ��,���ǰ�벿��
		{
			if(wav_buffill(wav_buff, BUF_SIZE/2, wavctrl.bps)!= FR_OK)
			{
				//���ʧ�ܣ��Ƴ�����
				//Audio_stop();
				PlayingSig = 0; 
				f_close(&SDFile);
			}
			//�����ɣ��޸ı�־
			buff_state = 0;
		}
		if(buff_state == 2)  //�Ѿ�������һ��,���ǰ�벿��
		{
			if(wav_buffill(wav_buff+BUF_SIZE/2, BUF_SIZE/2, wavctrl.bps)!= FR_OK)
			{
				//���ʧ�ܣ��Ƴ�����
				//Audio_stop();
				PlayingSig = 0; 
				f_close(&SDFile);
			}
			//�����ɣ��޸ı�־
			buff_state = 0;
		}
		if(File_Size ==  SDFile.fptr) //�ļ���ȡָ������ļ���С
		{
			File_Size = 0;
			PlayingSig = 0;	
			f_close(&SDFile);
		}	
		if(K4_set_OK == 1)
		{
			K4_set_OK = 0;
			PlayingSig = 0;	
			f_close(&SDFile);
		}
	}
}	
				
				
				
				//������ɻص�,��HAL_SAI_TxCpltCallback�е���
void BSP_AUDIO_OUT_TransferComplete_CallBack(void)
{
	buff_state = 2;
	I2S_AUDIO_ChangeBuffer((uint16_t*)&wav_buff[0], BUF_SIZE /2);
}

//�������һ��ص�,��HAL_SAI_TxHalfCpltCallback�е���
void BSP_AUDIO_OUT_HalfTransfer_CallBack(void)
{ 
	buff_state = 1;
	I2S_AUDIO_ChangeBuffer((uint16_t*)&wav_buff[BUF_SIZE /2], BUF_SIZE /2);
}

//�������ص�
void BSP_AUDIO_OUT_Error_CallBack(void)
{
	
}

/*********************************************************************************/
/*********************************************************************************/			
/**************************audio control set**************************************/				
/*********************************************************************************/				
/*********************************************************************************/				
void Audio_stop()
{
	//WM8978_SetMute(1);
//	SPI2->CR2 &= 0 << 1;	//txdmaen disable
}

void Audio_start()
{
//	SPI2->CR2 |= 1 << 1;	//Tx buffer DMA enable
}
				
//**********************************************************************
//**********************************************************************
//**********************************************************************
#include "output_aep.h"

uint8_t key_cnt;
void Key_SetAudio()
{
	if(key_data.K2_Flag == 1)
	{
		key_data.K2_Flag = 0;
		volum++;
		if(volum >= 100)
			volum = 100;
		//100��ʾ������ת����ʵ61��������
		act_volum = volum * 61 / 100;
		WM8978_SetPhoneVolume(act_volum,act_volum);
		WM8978_SetSPKVolume(act_volum);
	}

	if(key_data.K3_Flag == 1)
	{
		key_data.K3_Flag = 0;
		if(volum == 0)
			volum = 0;
		else
			volum--;
		
		//100��ʾ������ת����ʵ61��������
		act_volum = volum * 61 / 100;
		WM8978_SetPhoneVolume(act_volum,act_volum);
		WM8978_SetSPKVolume(act_volum);
	}

	if(key_data.K4_Flag == 1)
	{
		key_data.K4_Flag = 0;		
		K4_fucntion();
		K4_set_OK = 1;
	}
	if(key_data.K2_Long_Flag == 1)
	{
		if(KEY_2 == GPIO_PIN_SET)
		{//������������
			key_cnt++;
			if(key_cnt == 50)
			{
				key_cnt = 0;
				volum++;
				if(volum >= 100)
					volum = 100;
				act_volum = volum * 61 / 100;
				WM8978_SetPhoneVolume(act_volum,act_volum);
				WM8978_SetSPKVolume(act_volum);
			}
		}
		else
		{//�����ɿ�
			key_cnt = 0;
		  key_data.K2_Long_Flag = 0;		
		}
	}
	if(key_data.K3_Long_Flag == 1)
	{
		if(KEY_3 == GPIO_PIN_SET)	
		{
			key_cnt++;
			if(key_cnt == 50)
			{
				key_cnt = 0;
				if(volum == 0)
					volum = 0;
				else
					volum--;
				act_volum = volum * 61 / 100;
				WM8978_SetPhoneVolume(act_volum,act_volum);
				WM8978_SetSPKVolume(act_volum);
			}
		}
		else
		{
			key_cnt = 0;
		  key_data.K3_Long_Flag = 0;	
		}
	}
}







































