#include <stdio.h>
#include <stdint.h>
#include <wiringPi.h>
#include <gertboard.h>

// read SPI data from MCP3008 chip, 8 possible adc's (0 thru 7)
int readadc(adcnum)
{
    uint8_t buff[3];
    int adc;
    if ((adcnum > 7) || (adcnum < 0))
        return -1;
    buff[0] = 1;
    buff[1] = (8+adcnum)<<4;
    buff[2] = 0;
    wiringPiSPIDataRW(0, buff, 3);
    adc = ((buff[1]&3) << 8) + buff[2];
    return adc;
}

int main(int argc, char *argv[])
{
    int i, delay_ms;
    uint32_t x1;
    int print_output;
 
    printf ("SPI test program\n") ;
    // initialize the WiringPi API
    if (wiringPiSPISetup (0, 1000000) < 0)
        return -1 ;

    if (argc>2)
        print_output = 0;
    else
        print_output = 1;
 
    // get the channel to read, default to 0
    if (argc>1)
        delay_ms = atoi(argv[1]);
    else
        delay_ms = 1; //we don't use this here...
    int counter_wrap = 100;
    if (delay_ms != 0) counter_wrap = 500/delay_ms;
    int counter = 0;    
    // run until killed with Ctrl-C
    while (1)
    {
        counter++;
        // read data and add to total
        int chan;
        for (chan = 0; chan<8; chan++)
        {
            x1 = readadc(chan);
	    if (print_output)
            {
		if (counter > counter_wrap)
		{
		    printf("CH %i val= %d  ", chan, x1);
		}
            }
        }
        if (counter > counter_wrap)
        {
            if (print_output)
		printf("\n");
            counter = 0;
        }
        delay(delay_ms);
    }
    return 0 ;
}
