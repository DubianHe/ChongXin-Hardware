#include <dth11.h>//头文件

//写时序的函数
void DHT11_delay_us(uchar n)
{
    while (--n);
}

void DHT11_delay_ms(uint z)
{
    uint i, j;
    for (i = z; i > 0; i--)
        for (j = 110; j > 0; j--);
}

/*------------------------------------------------
              DHT11开始信号
------------------------------------------------*/
void DHT11_start() {
	Data = 1;
	DHT11_delay_us(2);
	Data = 0;
	DHT11_delay_ms(20);   //延时18ms以上
	Data = 1;
	DHT11_delay_us(30);
}

/*------------------------------------------------
              接收八位二进制
------------------------------------------------*/
uchar DHT11_rec_byte()
{
    uchar i, dat = 0;
    for (i = 0; i < 8; i++)
    {
        while (!Data);
        DHT11_delay_us(8);
        dat <<= 1;
        if (Data == 1)
            dat += 1;
        while (Data);
    }
    return dat;
}

/*------------------------------------------------
              接收40bit数据
------------------------------------------------*/
void getdata(uchar *sensordata) {    //接收40位的数据
	uchar R_H, R_L, T_H, T_L, RH, RL, TH, TL, revise;
	DHT11_start();

	if (Data == 0) {

		while (Data == 0); //等待拉高

		DHT11_delay_us(40);  //拉高后延时80us
		R_H = DHT11_rec_byte();  //接收湿度高八位
		R_L = DHT11_rec_byte();  //接收湿度低八位
		T_H = DHT11_rec_byte();  //接收温度高八位
		T_L = DHT11_rec_byte();  //接收温度低八位
		revise = DHT11_rec_byte(); //接收校正位

		DHT11_delay_us(25);    //结束

		if ((R_H + R_L + T_H + T_L) == revise) { //校正
		    RH = R_H;
            RL = R_L;
            TH = T_H;
            TL = T_L;	
		} 
		sensordata[0] = RH;
		sensordata[1] = TH;

	}
}

