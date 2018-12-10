#ifndef __WIFI_CONFIG_H
#define	__WIFI_CONFIG_H


#include "stm32f10x.h"


#if defined ( __CC_ARM   )
#pragma anon_unions
#endif


//* �������� */
typedef struct                                                     //��������WiFiģ���ָ��Ľṹ������
{
	char * netmode;
	char * dhcpd;
	char * dhcpd_ip;
	char * dhcpd_dns;
	char * wifi_conf;
	char * dhcpc;
	char * net_ip;
	char * net_dns;
	char * remotepro;
	char * mode;
	char * remoteip;
	char * remoteport;
	char * CLport;   
	char * timeout;                                                  //Ĭ��ֵ
	char * uart;                                                     //Ĭ��ֵ  
	char * uartpacklen;                                              //Ĭ��ֵ
	char * uartpacktimeout;  	                                       //Ĭ��ֵ
} STRUCT_Net_ConfigTypeDef;


#define RX_BUF_MAX_LEN     1024                                    //�����ջ����ֽ���

extern struct  STRUCT_USARTx_Fram                                  //��������֡�Ĵ���ṹ��
{
	u8  Data_RX_BUF[ RX_BUF_MAX_LEN ];
	
  union {
    __IO u16 InfAll;
    struct {
		  __IO u16 FramLength       :15;                               // 14:0 
		  __IO u16 FramFinishFlag   :1;                                // 15 
	  } InfBit;
  }; 
	
} strUSART1_Fram_Record, strUSART2_Fram_Record;


//* ���� */
#define WiFi_ES_INIT                   GPIO_Config                //WiFiʹ�õ�ES���ų�ʼ��������������� 
#define WiFi_USART1_INIT               USART1_Config              //WiFiʹ�õĴ���1��ʼ��������������115200
#define WiFi_USART2_INIT               USART2_Config              //WiFiʹ�õĴ���2��ʼ��������������115200 
#define WiFi_TIM2_INIT                 TIM2_Configuration
#define WiFi_NVIC_INIT                 NVIC_Configuration         //NVIC�ж�����

void WiFi_Config( void );
void NVIC_Configuration( void );


#endif    /* __WIFI_CONFIG_H */
