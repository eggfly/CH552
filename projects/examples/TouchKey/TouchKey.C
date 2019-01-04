
/********************************** (C) COPYRIGHT *******************************
* File Name          : TouchKey.C
* Author             : WCH
* Version            : V1.0
* Date               : 2017/01/20
* Description        : CH554 ������������������á�ͨ��ѡ����л����жϴ�������   
*******************************************************************************/

#include "..\Public\CH554.H"                                                       
#include "..\Public\Debug.H"
#include "TouchKey.H"
#include "stdio.h"

#pragma  NOAREGS

UINT16	KeyFree[KEY_LAST-KEY_FIRST+1];                                        //��������ֵ�洢�����ڱȽϰ���״̬���������ֵС�ڻ�׼ֵ������������
UINT8V	KeyBuf;                                                               //��������״̬��Ϊ0��ʾ�ް�������0��ʾ��ǰ��ⰴ��������

/*******************************************************************************
* Function Name  : GetTouckKeyFreeBuf()
* Description    : ��ȡ���������ռ�״̬��ֵ
* Input          : None								 
* Output         : None
* Return         : None
*******************************************************************************/
void GetTouckKeyFreeBuf()
{
  UINT8 i,j;
  UINT8 TmpSum = 0;
  KeyBuf = 0;                                                                 //��ʼ������Ϊ�ް���״̬
  for(i=KEY_FIRST;i<(KEY_LAST+1);i++)
  {
		j = KEY_BASE_SAMPLE_TIME;                                                 //�ɶ����ƽ��ֵ��Ϊ�����ο�
	  TKEY_CTRL = (TKEY_CTRL & 0xF8 | i)+1;                                     //���ò���ͨ��
    while(j--)
    {
        while((TKEY_CTRL&bTKC_IF) == 0);                                      //bTKC_IF��Ϊ1ʱ�������ڲ������
        TmpSum += TKEY_DAT&0x0F;                                              //����ֵ�ȶ���ȡ��4λ�͹���
    }		
    KeyFree[i] = TKEY_DAT&0x07F0 + TmpSum/5;                                  //�������ֵ 
  }
#if INTERRUPT_TouchKey
    IE_TKEY = 1;                                                              //ʹ��Touch_Key�ж�
#endif   
}

/*******************************************************************************
* Function Name  : TouchKeyChannelSelect(UINT8 ch)
* Description    : ��������ͨ��ѡ��
* Input          : UINT8 ch ����ͨ��
                   0~5 �ֱ��������ͨ��
* Output         : None
* Return         : �ɹ� SUCCESS
                   ʧ�� FAIL  ��֧�ֵ�ͨ��
*******************************************************************************/
UINT8 TouchKeyChannelSelect(UINT8 ch)
{
    if(ch < 6)
    {
        TKEY_CTRL = (TKEY_CTRL & 0xF8 | ch)+1;
        return SUCCESS;			
    }
    return FAIL;
}

#if INTERRUPT_TouchKey
/*******************************************************************************
* Function Name  : TouchKeyInterrupt(void)
* Description    : Touch_Key �жϷ������
*******************************************************************************/
void	TouchKeyInterrupt( void ) interrupt INT_NO_TKEY using 1                //Touch_Key�жϷ������,ʹ�üĴ�����1
{ 
	  UINT8	ch;
    UINT16 KeyData;

    KeyData = TKEY_DAT;                                                       //����87us,����ȡ��
    ch = TKEY_CTRL&7;                                                         //��ȡ��ǰ����ͨ��
    if ( ch > KEY_LAST ){
       TKEY_CTRL = TKEY_CTRL & 0xF8 | KEY_FIRST;                              //����ͨ����ʼ����
    }			
    else
    {
       TKEY_CTRL ++;                                                          //�л�����һ������ͨ��
    }
    if ( KeyData < (KeyFree[ch-KEY_FIRST] - KEY_ACT) )                        //���������㣬������������   
    {
        KeyBuf=ch;                                                            //�����ڴ˴����а����������������ñ�־֪ͨmain���д���
    }
}
#else
/*******************************************************************************
* Function Name  : TouchKeyChannelQuery()
* Description    : ��������ͨ��״̬��ѯ
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void TouchKeyChannelQuery()
{
	  UINT8	ch;
    UINT16 KeyData;

    while((TKEY_CTRL&bTKC_IF) == 0);                                          //bTKC_IF��Ϊ1ʱ�������ڲ������
    KeyData = TKEY_DAT;                                                       //����87us,����ȡ��
    ch = TKEY_CTRL&7;                                                         //��ȡ��ǰ����ͨ��
    if ( ch > KEY_LAST ){
       TKEY_CTRL = TKEY_CTRL & 0xF8 | KEY_FIRST;                              //����ͨ����ʼ����
    }			
    else
    {
       TKEY_CTRL ++;                                                          //�л�����һ������ͨ��
    }
    if ( KeyData < (KeyFree[ch-KEY_FIRST] - KEY_ACT) )                        //���������㣬������������   
    {
        KeyBuf=ch;                                                            //�����ڴ˴����а����������������ñ�־֪ͨmain���д���
    }
}
#endif
