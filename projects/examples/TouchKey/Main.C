
/********************************** (C) COPYRIGHT *******************************
* File Name          : Main.C
* Author             : WCH
* Version            : V1.0
* Date               : 2017/01/20
* Description        : CH554 ���������жϺͲ�ѯ��ʽ���вɼ������浱ǰ����ͨ������״̬��������ʼ���Ͱ�����������ʾ����  
*******************************************************************************/
#include "..\Public\CH554.H"                                                   
#include "..\Public\Debug.H"
#include "TouchKey.H"
#include "stdio.h"

#pragma  NOAREGS

main( ) 
{
    UINT8 i;
    CfgFsys( );                                                                //CH554ʱ��ѡ������   
    mDelaymS(5);                                                               //�޸���Ƶ�����Լ���ʱ�ȴ�оƬ�����ȶ�	
    mInitSTDIO( );                                                             //����0��ʼ��
    printf("start ...\n"); 

	  P1_DIR_PU &= 0x0C;                                                         //���д���ͨ������Ϊ�������룬�ò�����ͨ�����Բ�����
    TouchKeyQueryCyl2ms();                                                     //TouchKey��ѯ����2ms	
    GetTouckKeyFreeBuf();                                                      //��ȡ������׼ֵ
#if DE_PRINTF
    for(i=KEY_FIRST;i<(KEY_LAST+1);i++)                                        //��ӡ������׼ֵ
    {
      printf("Channel %02x base sample %04x\n",(UINT16)i,KeyFree[i]);
		}
#endif

#if INTERRUPT_TouchKey
    EA = 1;
    while(1)                                                                   
    {
      if(KeyBuf)                                                               //key_buf��0����ʾ��⵽��������
      {                       			
        printf("INT TouchKey Channel %02x \n",(UINT16)KeyBuf);                 //��ӡ��ǰ����״̬ͨ��	
        KeyBuf	= 0;                                                           //����������±�־   				
        mDelaymS(100);                                                         //��ʱ�����壬ģ�ⵥƬ������������			
      }
      mDelaymS(100);                                                           //��ʱ�����壬ģ�ⵥƬ����������				
    } 
#else	
    while(1)                                                                   
    {			
      TouchKeyChannelQuery();                                                  //��ѯ��������״̬
      if(KeyBuf)                                                               //key_buf��0����ʾ��⵽��������
      {                        			
        printf("Query TouchKey Channel %02x \n",(UINT16)KeyBuf);               //��ӡ��ǰ����״̬ͨ��				
        KeyBuf	= 0;                                                           //����������±�־  				
        mDelaymS(100);                                                         //��ʱ�����壬ģ�ⵥƬ������������			
      }
      mDelaymS(100);                                                           //��ʱ�����壬ģ�ⵥƬ����������				
    }
#endif
}