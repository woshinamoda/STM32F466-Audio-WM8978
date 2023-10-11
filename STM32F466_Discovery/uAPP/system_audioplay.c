#include "system_audioplay.h"
#include "audio_wm8978_I2C.h"
#include "system_malloc.h"	
#include "system_i2s.h"  
#include "system_exfuns.h"




//音乐播放控制器
/*******************************
*		*i2sbuf1	缓冲区1
*		*i2sbuf2	缓冲区2
*		*tbuf			24bit临时数组
*		file			音频文件指针
*		status		播放状态
*******************************/
__audiodev	 audiodev;			  



//开始音频播放
void audio_start(void)
{
	audiodev.status=3<<0;//开始播放+非暂停
	I2S_Play_Start();
} 


//关闭音频播放
void audio_stop(void)
{
	audiodev.status=0;
	I2S_Play_Stop();
}  

//得到path路径下,目标文件的总个数
//path:路径		    
//返回值:总有效文件数
uint16_t audio_get_tnum(uint8_t *path)
{	  
	uint8_t res;
	uint16_t rval=0;
 	DIR tdir;	 																								//FatFs下的->临时目录
	FILINFO* tfileinfo;																				//临时文件信息	 	
	tfileinfo=(FILINFO*)mymalloc(SRAMIN,sizeof(FILINFO));			//临时文件，申请内存
	res=f_opendir(&tdir,(const TCHAR*)path); 									//打开目录（临时文件）
	if(res==FR_OK&&tfileinfo)																	//如果打开成功，且内存申请成功
	{
		while(1)//查询总的有效文件数
		{
			res=f_readdir(&tdir,tfileinfo);       								/*按照顺序读取目录项*/
	        if(res!=FR_OK||tfileinfo->fname[0]==0)break;			//如果读取发生错误
																														//或者文件主名称 = 0
			res=f_typetell((uint8_t*)tfileinfo->fname);						//解析主名称文件类型，详请查阅system_exfun内容介绍
			if((res&0XF0)==0X40)																	//取高四位,看看是不是音乐文件
			{
				rval++;//有效文件数增加1
			}	    
		}  
	}  
	myfree(SRAMIN,tfileinfo);																	//释放临时文件内存
	return rval;																							//返回音乐的文件数
}


//播放音乐
void audio_play(void)
{
	uint8_t res;
 	DIR wavdir;	 								//目录
	FILINFO *wavfileinfo;				//文件信息 
	uint8_t *pname;							//带路径的文件名
	uint16_t totwavnum; 				//音乐文件总数
	uint16_t curindex;					//当前索引
 	uint32_t temp;
	uint32_t *wavoffsettbl;			//音乐offset索引表
	
	WM8978_ADDA_Cfg(1,0);				//开启DAC
	WM8978_Input_Cfg(0,0,0);		//关闭输入通道
	WM8978_Output_Cfg(1,0);			//开启DAC输出   
	
 	f_opendir(&wavdir,"0:/MUSIC");//打开音乐文件夹
	totwavnum=audio_get_tnum("0:/MUSIC"); //得到总有效文件数
	if(totwavnum==NULL)
	{
		while(1);	//音乐文件总数为0
	}		
						   
	wavfileinfo=(FILINFO*)mymalloc(SRAMIN,sizeof(FILINFO));	//申请内存
 	pname=mymalloc(SRAMIN,_MAX_LFN*2+1);										//为带路径的文件名分配内存
 	wavoffsettbl=mymalloc(SRAMIN,4*totwavnum);							//申请4*totwavnum个字节的内存,用于存放音乐文件off block索引
 

	if(!wavfileinfo||!pname||!wavoffsettbl)
	{
		while(1);	//内存分配出错
	}
 
 	//记录索引
	res=f_opendir(&wavdir,"0:/MUSIC"); 											//打开目录
	if(res==FR_OK)
	{
		curindex=0;//当前索引为0
		while(1)//全部查询一遍
		{
			temp=wavdir.dptr;								//记录当前index 
	        res=f_readdir(&wavdir,wavfileinfo);       		//读取目录下的一个文件
	        if(res!=FR_OK||wavfileinfo->fname[0]==0)break;	//错误了/到末尾了,退出 		 
			res=f_typetell((uint8_t*)wavfileinfo->fname);	
			if((res&0XF0)==0X40)//取高四位,看看是不是音乐文件	
			{
				wavoffsettbl[curindex]=temp;//记录索引
				curindex++;
			}    
		} 
	}   
   	curindex=0;											//从0开始显示
   	res=f_opendir(&wavdir,(const TCHAR*)"0:/MUSIC"); 	//打开目录
										  
	myfree(SRAMIN,wavfileinfo);			//释放内存			    
	myfree(SRAMIN,pname);				//释放内存			    
	myfree(SRAMIN,wavoffsettbl);		//释放内存 
} 





//播放某个音频文件
uint8_t audio_play_song(uint8_t* fname)
{
	uint8_t res;  
	res=f_typetell(fname); 
	switch(res)
	{
		case T_WAV:
			res=wav_play_song(fname);
			break;
		default://其他文件,自动跳转到下一曲
//			printf("can't play:%s\r\n",fname);
//			res=KEY0_PRES;
			break;
	}
	return res;
}




























