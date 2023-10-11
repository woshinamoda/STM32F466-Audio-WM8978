#include "system_audioplay.h"
#include "audio_wm8978_I2C.h"
#include "system_malloc.h"	
#include "system_i2s.h"  
#include "system_exfuns.h"




//���ֲ��ſ�����
/*******************************
*		*i2sbuf1	������1
*		*i2sbuf2	������2
*		*tbuf			24bit��ʱ����
*		file			��Ƶ�ļ�ָ��
*		status		����״̬
*******************************/
__audiodev	 audiodev;			  



//��ʼ��Ƶ����
void audio_start(void)
{
	audiodev.status=3<<0;//��ʼ����+����ͣ
	I2S_Play_Start();
} 


//�ر���Ƶ����
void audio_stop(void)
{
	audiodev.status=0;
	I2S_Play_Stop();
}  

//�õ�path·����,Ŀ���ļ����ܸ���
//path:·��		    
//����ֵ:����Ч�ļ���
uint16_t audio_get_tnum(uint8_t *path)
{	  
	uint8_t res;
	uint16_t rval=0;
 	DIR tdir;	 																								//FatFs�µ�->��ʱĿ¼
	FILINFO* tfileinfo;																				//��ʱ�ļ���Ϣ	 	
	tfileinfo=(FILINFO*)mymalloc(SRAMIN,sizeof(FILINFO));			//��ʱ�ļ��������ڴ�
	res=f_opendir(&tdir,(const TCHAR*)path); 									//��Ŀ¼����ʱ�ļ���
	if(res==FR_OK&&tfileinfo)																	//����򿪳ɹ������ڴ�����ɹ�
	{
		while(1)//��ѯ�ܵ���Ч�ļ���
		{
			res=f_readdir(&tdir,tfileinfo);       								/*����˳���ȡĿ¼��*/
	        if(res!=FR_OK||tfileinfo->fname[0]==0)break;			//�����ȡ��������
																														//�����ļ������� = 0
			res=f_typetell((uint8_t*)tfileinfo->fname);						//�����������ļ����ͣ��������system_exfun���ݽ���
			if((res&0XF0)==0X40)																	//ȡ����λ,�����ǲ��������ļ�
			{
				rval++;//��Ч�ļ�������1
			}	    
		}  
	}  
	myfree(SRAMIN,tfileinfo);																	//�ͷ���ʱ�ļ��ڴ�
	return rval;																							//�������ֵ��ļ���
}


//��������
void audio_play(void)
{
	uint8_t res;
 	DIR wavdir;	 								//Ŀ¼
	FILINFO *wavfileinfo;				//�ļ���Ϣ 
	uint8_t *pname;							//��·�����ļ���
	uint16_t totwavnum; 				//�����ļ�����
	uint16_t curindex;					//��ǰ����
 	uint32_t temp;
	uint32_t *wavoffsettbl;			//����offset������
	
	WM8978_ADDA_Cfg(1,0);				//����DAC
	WM8978_Input_Cfg(0,0,0);		//�ر�����ͨ��
	WM8978_Output_Cfg(1,0);			//����DAC���   
	
 	f_opendir(&wavdir,"0:/MUSIC");//�������ļ���
	totwavnum=audio_get_tnum("0:/MUSIC"); //�õ�����Ч�ļ���
	if(totwavnum==NULL)
	{
		while(1);	//�����ļ�����Ϊ0
	}		
						   
	wavfileinfo=(FILINFO*)mymalloc(SRAMIN,sizeof(FILINFO));	//�����ڴ�
 	pname=mymalloc(SRAMIN,_MAX_LFN*2+1);										//Ϊ��·�����ļ��������ڴ�
 	wavoffsettbl=mymalloc(SRAMIN,4*totwavnum);							//����4*totwavnum���ֽڵ��ڴ�,���ڴ�������ļ�off block����
 

	if(!wavfileinfo||!pname||!wavoffsettbl)
	{
		while(1);	//�ڴ�������
	}
 
 	//��¼����
	res=f_opendir(&wavdir,"0:/MUSIC"); 											//��Ŀ¼
	if(res==FR_OK)
	{
		curindex=0;//��ǰ����Ϊ0
		while(1)//ȫ����ѯһ��
		{
			temp=wavdir.dptr;								//��¼��ǰindex 
	        res=f_readdir(&wavdir,wavfileinfo);       		//��ȡĿ¼�µ�һ���ļ�
	        if(res!=FR_OK||wavfileinfo->fname[0]==0)break;	//������/��ĩβ��,�˳� 		 
			res=f_typetell((uint8_t*)wavfileinfo->fname);	
			if((res&0XF0)==0X40)//ȡ����λ,�����ǲ��������ļ�	
			{
				wavoffsettbl[curindex]=temp;//��¼����
				curindex++;
			}    
		} 
	}   
   	curindex=0;											//��0��ʼ��ʾ
   	res=f_opendir(&wavdir,(const TCHAR*)"0:/MUSIC"); 	//��Ŀ¼
										  
	myfree(SRAMIN,wavfileinfo);			//�ͷ��ڴ�			    
	myfree(SRAMIN,pname);				//�ͷ��ڴ�			    
	myfree(SRAMIN,wavoffsettbl);		//�ͷ��ڴ� 
} 





//����ĳ����Ƶ�ļ�
uint8_t audio_play_song(uint8_t* fname)
{
	uint8_t res;  
	res=f_typetell(fname); 
	switch(res)
	{
		case T_WAV:
			res=wav_play_song(fname);
			break;
		default://�����ļ�,�Զ���ת����һ��
//			printf("can't play:%s\r\n",fname);
//			res=KEY0_PRES;
			break;
	}
	return res;
}




























