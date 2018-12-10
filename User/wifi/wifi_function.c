#include "wifi_function.h"
#include "wifi_config.h"
#include "bsp_gpio.h"
#include "bsp_usart1.h"
#include "bsp_usart2.h"
#include "bsp_SysTick.h"
#include <string.h> 


/*
 * ��������hlk_rm04_check_cmd
 * ����  ����WF-HLK-RM04����ָ���,�����յ���Ӧ��
 * ����  ��str���ڴ���Ӧ������
 * ����  : 0��Ӧ��ʧ��
 *         ������Ӧ��ɹ�
 * ����  ����hlk_rm04_send_cmd����
 */
u8 * hlk_rm04_check_cmd( char * str )
{
	char * strx = 0;
	
	if( strUSART2_Fram_Record .InfBit .FramFinishFlag )		                                       //�ѽ��յ�һ֡����
	strx = strstr( ( const char * ) strUSART2_Fram_Record .Data_RX_BUF, ( const char * ) str ); 
	
	return ( u8 * ) strx;
	
}


/*
 * ��������hlk_rm04_send_cmd
 * ����  ����WF-HLK-RM04����ָ��
 * ����  ��cmd�������͵�ָ��
           ack���ڴ���Ӧ������
           waittime���ڴ�Ӧ��ĳ�ʱʱ�䣨ms��
 * ����  : 0��ָ���ʧ��
 *         1��ָ��ͳɹ�
 * ����  ����hlk_rm04_quit_trans��hlk_rm04_net_config����
 */
u8 hlk_rm04_send_cmd( char * cmd, char * ack, u16 waittime )
{
	u8 ReturnValue = 1; 
	
	strUSART2_Fram_Record .InfAll = 0;

	USART2_printf( USART2, "%s\r", cmd );                          	                             //��������
	
	if( ack && waittime )		                                                                     //��Ҫ�ȴ�Ӧ��
	{
		while( -- waittime )	                                                                     //�ȴ�����ʱ
		{
			Delay_ms( 1 );
			
			if( strUSART2_Fram_Record .InfBit .FramFinishFlag )                                      //���յ��ڴ���Ӧ����
			{
				if( hlk_rm04_check_cmd( ack ) ) break;                                                 //�õ���Ч���� 
				strUSART2_Fram_Record .InfBit .FramFinishFlag = 0;
			} 
		} 
		
		if( waittime == 0 ) ReturnValue = 0; 
		
	}
	
	return ReturnValue;
	
} 


/*
 * �������ahlk_rm04_quit_trans
 * ����  ����WF-HLK-RM04�˳�͸��ģʽ
 * ����  ����
 * ����  : ��
 * ����  �����ⲿ����
 */
void hlk_rm04_quit_trans( void )
{
	do{
  ES_LOW_LEVEL();
	Delay_ms( 120 );
	ES_HIGH_LEVEL();
	} while( ! hlk_rm04_send_cmd( "at", "\r\n", 1000 ) );                                        //�ж��Ƿ����˳�͸��ģʽ

}


/*
 * �������ahlk_rm04_net_config
 * ����  ������WF-HLK-RM04ģ��
 * ����  ��p������WF-HLK-RM04ģ���ָ�
 * ����  : ��
 * ����  �����ⲿ����
 */
 void hlk_rm04_net_config( STRUCT_Net_ConfigTypeDef * p )
{
	hlk_rm04_send_cmd( p -> netmode, "ok", 500 );

	if ( ! strstr( p -> netmode, "1" ) )
	{
		hlk_rm04_send_cmd( p -> wifi_conf, "ok", 500 ); 

		if ( strstr( p -> netmode, "3" ) )
		{
			hlk_rm04_send_cmd( p -> dhcpd, "ok", 500 );
			
			hlk_rm04_send_cmd( p -> dhcpd_ip, "ok", 500 );
			
			hlk_rm04_send_cmd( p -> dhcpd_dns, "ok", 500 );	
			
		}
		
	}	

  if ( strstr( p -> netmode, "3" ) )	
	{
		hlk_rm04_send_cmd( p -> net_ip, "ok", 500 );
		
		hlk_rm04_send_cmd( p -> net_dns, "ok", 500 );		
		
	}	
	else
	{
		hlk_rm04_send_cmd( p -> dhcpc, "ok", 500 );
		
		if ( strstr( p -> dhcpc, "0" ) )	
		{
			hlk_rm04_send_cmd( p -> net_ip, "ok", 500 );
			
			hlk_rm04_send_cmd( p -> net_dns, "ok", 500 );		
			
		}
	
	}
	
	hlk_rm04_send_cmd( p -> remotepro, "ok", 500 );
	
	hlk_rm04_send_cmd( p -> mode, "ok", 500 );
	
  hlk_rm04_send_cmd( p -> remoteport, "ok", 500 );
	
  if ( strstr( p -> mode, "client" ) )	
	{
		hlk_rm04_send_cmd( p -> remoteip, "ok", 500 ); 
		
		hlk_rm04_send_cmd( p -> CLport, "ok", 500 );		
		
	}	
	
	hlk_rm04_send_cmd( p -> timeout, "ok", 500 );
	
	hlk_rm04_send_cmd( p -> uart, "ok", 500 );
	
	hlk_rm04_send_cmd( p -> uartpacklen, "ok", 500 );
	
	hlk_rm04_send_cmd( p -> uartpacktimeout, "ok", 500 );
	
	if ( strstr( p -> netmode, "3" ) )
	hlk_rm04_send_cmd( "at+net_commit=1", "\r\n\r\n", 20000 );
	else 
	hlk_rm04_send_cmd( "at+net_commit=1", "ok", 20000 );
	
	hlk_rm04_send_cmd( "at+net_wanip=?", ".", 500 );
	
	hlk_rm04_send_cmd( "at+reconn=1", "ok", 500 );	
	
}


