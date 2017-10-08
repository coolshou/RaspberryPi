/*-
    softshut.c
    compile: gcc softshut.c -l rt -l wiringPi -std=gnu99 -o softshut

require wiringPi
* 
-*/

#include <stdio.h>   // printf, scanf
#include <wiringPi.h>
#include <syslog.h>
#include <unistd.h>  //getuid

#define SOFT_OFF_PIN 4 // PIN 16, monitor soft off pin
#define KEEP_POWERED_PIN   5 // PIN 18, keeps power pin

int prog_exit(void);

int main(int argc, char **argv)
{
    setlogmask (LOG_UPTO (LOG_NOTICE));
    openlog("softshut", LOG_CONS | LOG_PID | LOG_NDELAY, LOG_LOCAL1);
    syslog(LOG_NOTICE, "Program started by User %d", getuid());

    if(wiringPiSetup()==-1)
    {
        syslog(LOG_ERR, "wiringPi Setup error");
        return -1 ;
    }
    syslog(LOG_INFO, "softshut start for Raspberry Pi revision: %d", piBoardRev());
// 設定接腳狀態
    syslog(LOG_INFO,"set KEEP_POWERED_PIN %d output", KEEP_POWERED_PIN);
    pinMode(KEEP_POWERED_PIN,OUTPUT);
    syslog(LOG_INFO,"set SOFT_OFF_PIN %d input", SOFT_OFF_PIN);
    pinMode(SOFT_OFF_PIN,INPUT);
// 初始狀態
    digitalWrite(KEEP_POWERED_PIN, 1);
    pullUpDnControl(SOFT_OFF_PIN, PUD_UP);
    syslog(LOG_INFO,"get SOFT_OFF_PIN value: %d", digitalRead(SOFT_OFF_PIN));
    while(1)
    {
        int rs = digitalRead(SOFT_OFF_PIN);
        syslog(LOG_INFO,"get SOFT_OFF_PIN value: %d", rs);
        delay(500);
    }
    digitalWrite(KEEP_POWERED_PIN,0);

    closelog ();    
    return 0;



    /*
    int relay = 0;
    int onoff = 0;

 // GPIO 初始化
    syslog(LOG_INFO, "bcm2835_init!");
    if(!bcm2835_init()) {
        syslog(LOG_ERR, "bcm2835 init Error!");
        return 1;
    }

 // 設定接腳狀態       
    syslog(LOG_INFO,"set KEEP_POWERED_PIN %d output", KEEP_POWERED_PIN);
    bcm2835_gpio_fsel(KEEP_POWERED_PIN, BCM2835_GPIO_FSEL_OUTP);
    syslog(LOG_INFO,"set SOFT_OFF_PIN %d input", SOFT_OFF_PIN);
    bcm2835_gpio_fsel(SOFT_OFF_PIN, BCM2835_GPIO_FSEL_INPT);
 
    // 初始輸出狀態
    bcm2835_gpio_write(KEEP_POWERED_PIN, HIGH);
    //  with a pullup
    bcm2835_gpio_set_pud(SOFT_OFF_PIN, BCM2835_GPIO_PUD_UP);
    // And a low detect enable
    bcm2835_gpio_len(SOFT_OFF_PIN);

    while(1)
    {
        if (bcm2835_gpio_eds(SOFT_OFF_PIN))
        {   // Now clear the eds flag by setting it to 1
            bcm2835_gpio_set_eds(SOFT_OFF_PIN);
            syslog(LOG_INFO,"low event detect for PIN %d", SOFT_OFF_PIN);
            syslog(LOG_INFO, "SOFT_OFF_PIN current level: %d", bcm2835_gpio_lev(SOFT_OFF_PIN));            
        }
        // wait a bit
        delay(500);
    }
    closelog ();

    return prog_exit();
    */
}

int prog_exit()
{
    bcm2835_gpio_write(KEEP_POWERED_PIN, LOW); // turn off relay
    //bcm2835_gpio_write(RELAY_INP2, HIGH);
    
    // 關閉 bcm2835 函式庫
    if (!bcm2835_close())
    {
        printf("Failed to close the library, deallocating any allocated memory and closing /dev/mem\n");
        printf("\nPress any key to exit...");
        return 2;
    }
    return 0;
}
