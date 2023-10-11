#include "system_wavplay.h"  
#include "fatfs.h"
#include "system_malloc.h"	  
#include "audio_wm8978_I2C.h"
#include "system_key.h"


#include "i2s.h"

__wavctrl wavctrl;			  				//WAV控制结构体


#define BUF_SIZE 4096							//4KB的音频缓冲
uint8_t wav_buff[BUF_SIZE];				//音频的缓冲区
uint8_t buff_state;								// 1：传输完成了 0 - 1/2 前半部分
																	// 2：传输完成了 1/2 - 1 后半部分
uint8_t PlayingSig;								// 0:停止播放			1：正在播放
extern uint32_t File_Size;				//文件大小!
extern uint8_t volum;							//音量大小
extern uint16_t act_volum;	


extern uint8_t K4_set_OK;
extern uint8_t play_cnt;
extern uint8_t *play_name;


uint8_t	wav_decode_init(uint8_t* fname, __wavctrl* wavx)
{

	uint8_t	buf[512];			//缓存指针
	uint32_t br=0;
	uint8_t res=0;

	ChunkRIFF *riff;
	ChunkFMT *fmt;
	ChunkFACT *fact;
	ChunkDATA *data;
	res=f_open(&SDFile,(TCHAR*)fname,FA_READ);		//打开SD卡文件夹
	if(res == FR_OK)
	{//文件能打开
		uint32_t number_buf;
		f_read(&SDFile,buf,512,&br);	//读取512字节在数据
		riff=(ChunkRIFF *)buf;		//获取RIFF块
		if(riff->Format==0X45564157)//是WAV文件
		{
			fmt=(ChunkFMT *)(buf+12);	//获取FMT块 
			fact=(ChunkFACT *)(buf+12+8+fmt->ChunkSize);//读取FACT块
			if(fact->ChunkID==0X74636166||fact->ChunkID==0X5453494C)wavx->datastart=12+8+fmt->ChunkSize+8+fact->ChunkSize;//具有fact/LIST块的时候(未测试)
			else wavx->datastart=12+8+fmt->ChunkSize;  
			data=(ChunkDATA *)(buf+wavx->datastart);	//读取DATA块
			if(data->ChunkID==0X61746164)//解析成功!
			{
				wavx->audioformat=fmt->AudioFormat;			//音频格式
				wavx->nchannels=fmt->NumOfChannels;			//通道数
				wavx->samplerate=fmt->SampleRate;				//采样率
				wavx->bitrate=fmt->ByteRate*8;					//得到位速
				wavx->blockalign=fmt->BlockAlign;				//块对齐
				wavx->bps=fmt->BitsPerSample;						//位数,16/24/32位
				wavx->datasize=data->ChunkSize;					//数据块大小
				wavx->datastart=wavx->datastart+8;			//数据流开始的地方. 
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
			}else res=3;//data区域未找到.
		}else res=2;//非wav文件	
	}else res=1;//打开文件错误
f_close(&SDFile);	
	return 0;
}
	





//填充buf
//buf:数据区
//size:填充数据量
//bits:位数(16/24)
//返回值:读到的数据个数
uint8_t tbuf[4];
uint8_t wav_buffill(uint8_t *buf,uint16_t size,uint8_t bits)
{
	uint16_t	readlen = 0;
	uint32_t	bread;
	uint16_t	i;
	uint8_t	*	p;
	uint8_t	res;
	if(bits==24)													//24bit音频,需要处理一下
	{
		readlen=(size/4)*3;									//此次要读取的字节数
		
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
		bread=(bread*4)/3;									//去掉第3个8位的无用数据， 4bytes -> 3bytes
	}
	else 
	{
		res = f_read(&SDFile,buf,size,(UINT*)&bread);	//16bit音频,直接读取数据  
		if(bread<size)																//不够数据了,补充0
		{
			for(i=bread;i<size-bread;i++)buf[i]=0; 
		}
	}
	return res;
}





//播放音乐
uint8_t wav_play_song(uint8_t *filename)
{
	uint8_t t=0;
	uint8_t res;
	uint32_t fillnum;
	
	if(PlayingSig == 0)	//没有在播放歌曲，可以播放新的语音
	{
		res = wav_decode_init(filename, &wavctrl);	//得到文件的信息
		if(res == 0)
		{//解析文件成功
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
			
			I2S_AUDIO_SetFrequency(I2S_AUDIOFREQ_44K);	//I2S初始化采用MAX_I2S_Init(), 这里制作一个频率修改			
			
	/******************解析完成，下面打开文件填入缓冲区**********************************/
			res=f_open(&SDFile,(TCHAR*)filename,FA_READ);	//打开文件
			if(res == 0)
			{
				f_lseek(&SDFile, wavctrl.datastart);								//跳过文件头
				wav_buffill(wav_buff , BUF_SIZE, wavctrl.bps);			//填充整个数据
				I2S_AUDIO_Play((uint16_t*)&wav_buff[0], BUF_SIZE);  //启动传输（内部开启DMA）
				PlayingSig = 1;																			//语音状态：开始播放
			}else res = 0xff;
		}else res = 0xff;
	}
	return res;
}


#include "dma.h"

//配合DMA，程序填充缓冲函数
void Audio_Process(void)
{
	uint16_t br;
	if(PlayingSig == 1)    //进入播放
	{
		if(buff_state == 1)  //已经传输了一半,填充前半部分
		{
			if(wav_buffill(wav_buff, BUF_SIZE/2, wavctrl.bps)!= FR_OK)
			{
				//填充失败，推出播放
				//Audio_stop();
				PlayingSig = 0; 
				f_close(&SDFile);
			}
			//填充完成，修改标志
			buff_state = 0;
		}
		if(buff_state == 2)  //已经传输了一半,填充前半部分
		{
			if(wav_buffill(wav_buff+BUF_SIZE/2, BUF_SIZE/2, wavctrl.bps)!= FR_OK)
			{
				//填充失败，推出播放
				//Audio_stop();
				PlayingSig = 0; 
				f_close(&SDFile);
			}
			//填充完成，修改标志
			buff_state = 0;
		}
		if(File_Size ==  SDFile.fptr) //文件读取指针等于文件大小
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
				
				
				
				//传输完成回调,在HAL_SAI_TxCpltCallback中调用
void BSP_AUDIO_OUT_TransferComplete_CallBack(void)
{
	buff_state = 2;
	I2S_AUDIO_ChangeBuffer((uint16_t*)&wav_buff[0], BUF_SIZE /2);
}

//传输完成一半回调,在HAL_SAI_TxHalfCpltCallback中调用
void BSP_AUDIO_OUT_HalfTransfer_CallBack(void)
{ 
	buff_state = 1;
	I2S_AUDIO_ChangeBuffer((uint16_t*)&wav_buff[BUF_SIZE /2], BUF_SIZE /2);
}

//传输错误回调
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
		//100显示音量，转换真实61量程音量
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
		
		//100显示音量，转换真实61量程音量
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
		{//按键还被按着
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
		{//按键松开
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







































