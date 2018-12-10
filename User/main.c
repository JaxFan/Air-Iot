/**
  ******************************************************************************
  * @file    main.c
  * @author  fire
  * @version V1.0
  * @date    2014-12-xx
  * @brief   WiFiģ�����
  ******************************************************************************
  * @attention
  *
  * ʵ��ƽ̨:Ұ�� iSO STM32 ������ 
  * ��̳    :http://www.chuxue123.com
  * �Ա�    :http://firestm32.taobao.com
  *
  ******************************************************************************
  */ 
 
#include "stm32f10x.h"
#include "bsp_gpio.h"
#include "bsp_usart1.h"
#include "bsp_usart2.h"
#include "wifi_config.h"
#include "wifi_function.h"
#include "bsp_SysTick.h"


#define NULL                   0
#define NET_MODE_TEST          1                                                  //NET_MODE_TEST = 1   ��������̫��ģʽ����  
                                                                                  //              = 2   ����������STAģʽ����  
                                                                                  //              = 3   ����������APģʽ���� 
                                                                                  //              = ������������ 		
																																									

STRUCT_Net_ConfigTypeDef  Net_Test_ConfigStructure =
{
	#if NET_MODE_TEST == 1                                                          //��̫��ģʽ����               
		"at+netmode=1",                                                               //��̫��ģʽ
		NULL,                                                                         //��ģʽ�¿��Բ�����
		NULL,
		NULL,
		NULL,
		"at+dhcpc=0",                                                                 //�ر�DHCP�ͻ��ˣ��ֶ�����IP
		"at+net_ip=192.168.11.254,255.255.255.0,192.168.11.1",                        //�ֶ����õı���IP,�������룬����
		"at+net_dns=192.168.11.1,0.0.0.0",                                            //�ֶ����õı�����ҪDNS������DNS
		"at+remotepro=tcp",                                                           //TCPЭ��
		"at+mode=server",                                                             //������
		"at+remoteip=192.168.1.104",                                                  //ģ�����ͻ���ʱҪ���ӵķ�����IP
		"at+remoteport=8080",                                                         //�������˿�
		"at+CLport= 13",                                                              //ģ�����ͻ���ʱ�Ķ˿�
		"at+timeout=0",                                                               //Ĭ��ֵ
		"at+uart=115200,8,n,1",                                                       //Ĭ��ֵ
		"at+uartpacklen=64",                                                          //Ĭ��ֵ
		"at+uartpacktimeout=10"                                                       //Ĭ��ֵ		
	
	#elif NET_MODE_TEST == 2                                                        //����STAģʽ����  
		"at+netmode=2",                                                               //����STAģʽ
		NULL,
		NULL,
		NULL,
		"at+wifi_conf=wildfire,wpawpa2_aes,wildfire",                                 //Ҫ���ӵ�WiFi��������ƣ����ܷ�ʽ����Կ
		"at+dhcpc=1",                                                                 //����DHCP�ͻ��ˣ�ģ���Զ���·������ȡIP
		"at+net_ip=192.168.11.254,255.255.255.0,192.168.11.1",                      
		"at+net_dns=192.168.11.1,0.0.0.0",                                        
		"at+remotepro=tcp",
		"at+mode=server",
		"at+remoteip=192.168.1.104",
		"at+remoteport=8080",
		"at+CLport= 13",   
		"at+timeout=0",                                                               //Ĭ��ֵ
		"at+uart=115200,8,n,1",                                                       //Ĭ��ֵ
		"at+uartpacklen=64",                                                          //Ĭ��ֵ
		"at+uartpacktimeout=10"                                                       //Ĭ��ֵ		
  
	#elif NET_MODE_TEST == 3                                                        //����APģʽ����  
		"at+netmode=3",                                                               //����APģʽ
		"at+dhcpd=1",                                                                 //����DHCP���������Զ�����IP������WiFi�豸
		"at+dhcpd_ip=192.168.11.100,192.168.11.200,255.255.255.0,192.168.11.1",       //ģ������IP��Χ���������룬����
		"at+dhcpd_dns=192.168.11.1,0.0.0.0",                                          //ģ��������ҪDNS������DNS
		"at+wifi_conf=wildfire2,wpawpa2_aes,wildfire",                                //������WiFi���������ƣ����ܷ�ʽ����Կ
		NULL,                                                                         
		"at+net_ip=192.168.11.254,255.255.255.0,192.168.11.1",                        //����IP,�������룬����
		"at+net_dns=192.168.11.1,0.0.0.0",                                            //������ҪDNS������DNS
		"at+remotepro=tcp",
		"at+mode=server",
		"at+remoteip=192.168.1.104",
		"at+remoteport=8080",
		"at+CLport= 13",   
		"at+timeout=0",                                                               //Ĭ��ֵ
		"at+uart=115200,8,n,1",                                                       //Ĭ��ֵ
		"at+uartpacklen=64",                                                          //Ĭ��ֵ
		"at+uartpacktimeout=10"                                                       //Ĭ��ֵ	
	
	#else                                                                           //������
	  NULL
	
  #endif	
	
};


/**
  * @brief  ������
  * @param  ��
  * @retval ��
  */
int main(void)
{	
	/* ��ʼ�� */
	WiFi_Config();                                                                  //��ʼ��WiFiģ��ʹ�õĽӿں�����
	SysTick_Init();                                                                 //���� SysTick Ϊ 1ms �ж�һ�� 

	printf("\r\nҰ�� HLK-RMO4 WiFiģ���������\r\n");                                //��ӡ����������ʾ��Ϣ

	#if NET_MODE_TEST                                                               
	 hlk_rm04_quit_trans();                                                         //��WiFiģ���˳�͸��ģʽ
	 hlk_rm04_net_config( & Net_Test_ConfigStructure );                             //����WiFiģ����в���
	#endif

	for(;;);
	
}


/*********************************************END OF FILE**********************/
