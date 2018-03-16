#include "sys.h"
#include "led.h"
#include "beep.h"
#include "key.h"
#include "exti.h"
#include "wdg.h"
#include "timer.h"
#include "tpad.h"
#include "lcd.h"
#include "qr_encode.h"

#define QRCODE_Y 	80		//TFT二维码显示坐标y

void DISPLAY_RENCODE_TO_TFT(u8 *qrcode_data)
{
    u8 i, j;
    u16 x, y, p;
    u8 qrencode_buff[12];			//存放LCD ID字符串
    EncodeData((char *)qrcode_data);
    //LCD_Fill(10,110,15,115,BLACK);
    LCD_Fill(0, 40, 240, 320, WHITE);
    sprintf((char*)qrencode_buff, "size:%d", m_nSymbleSize); //将LCD ID打印到lcd_id数组。
    LCD_ShowString(10, 40, 200, 16, 16, qrencode_buff);		//显示LCD ID
    if(m_nSymbleSize * 2 > 240)
    {
        LCD_ShowString(10, 60, 200, 16, 16, (u8 *)"The QR Code is too large!"); //太大显示不下
        return;
    }
    for(i = 0; i < 10; i++)
    {
        if((m_nSymbleSize * i * 2) > 240)	break;
    }
    p = (i - 1) * 2; //点大小
    x = (240 - m_nSymbleSize * p) / 2;
    y = QRCODE_Y;
    sprintf((char*)qrencode_buff, "piont:%d", p); //将LCD ID打印到lcd_id数组。
    LCD_ShowString(10, 60, 200, 16, 16, qrencode_buff);
    for(i = 0; i < m_nSymbleSize; i++)
    {
        for(j = 0; j < m_nSymbleSize; j++)
        {
            //USART1_SendData(m_byModuleData[j][i]);
            if(m_byModuleData[i][j] == 1)
                LCD_Fill(x + p * i, y + p * j, x + p * (i + 1) - 1, y + p * (j + 1) - 1, BLACK);

        }

    }
}
//0 0 5 5
//6 6 11 11
const u8 codetest[] = //微信名片，可以去百度上搜索二维码名片 查看
{
    "BEGIN:VCARD\r\n"
    "VERSION:3.0\r\n"
    "N:外星人\r\n"//名字
    "TEL:13888888888\r\n"//手机号
    "TEL;TYPE=WORK,VOICE:0571-88888888\r\n"//工作号码
    "TEL;TYPE=HOME,VOICE:0571-66666666\r\n"//家庭号码
    "NOTE:QQ:12345\r\n"//QQ号码
    "EMAIL:12345@qq.com\r\n"//邮箱地址
    "ORG:公司名称\r\n"//公司
    "TITLE:职位名称\r\n"
    "URL:http://个人主页\r\n"
    "ADR;TYPE=WORK:工作地址\r\n"//地址
    "ADR;TYPE=HOME:家庭住址\r\n"
    "END:VCARD"
};

int main(void)
{
    u16 i, len;
    Stm32_Clock_Init(9);	//系统时钟设置
    uart_init(72, 256000);	//串口初始化为9600
    delay_init(72);	   	 	//延时初始化
    LED_Init();		  		//初始化与LED连接的硬件接口
    BEEP_Init();			//蜂鸣器初始化
    KEY_Init();				//按键初始化
    LCD_Init();				//TFT 液晶初始化
    LCD_Clear(WHITE);
    POINT_COLOR = RED;
    LCD_ShowString(10, 0, 200, 16, 16, (u8 *)"WarShip STM32 ^_^");
    LCD_ShowString(10, 20, 200, 16, 16, (u8 *)"QR_ENCODE TEST");
    DISPLAY_RENCODE_TO_TFT((u8 *)codetest);
    memset(USART_RX_BUF, '\0', USART_REC_LEN);

    while(1)
    {
        if(USART_RX_STA & 0x8000)
        {
            len = USART_RX_STA & 0x3fff; //得到此次接收到的数据长度
            USART_RX_STA = 0;
            printf("接收到的数据长度为:%d\r\n", len);
            printf("接收到的数据为:%d\r\n", len);
            USART1_SendPtr(USART_RX_BUF, len);
            printf("\r\n");
            DISPLAY_RENCODE_TO_TFT(USART_RX_BUF);
            memset(USART_RX_BUF, '\0', USART_REC_LEN);

        }
        else
        {
            if(++i >= 100)
            {
                i = 0;
                LED0 = !LED0;
            }
            delay_ms(10);
        }

    }

}










