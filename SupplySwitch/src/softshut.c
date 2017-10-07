/*-
    softshut.c
    compile: gcc softshut.c -l rt -l bcm2835 -std=gnu99 -o softshut

require bcm2835-1.52
* 
-*/

#include <stdio.h>   // printf, scanf
#include <bcm2835.h>
#include <syslog.h>
#include <unistd.h>  //getuid

#define SOFT_OFF_PIN 23 // GPIO 23 = PIN 16, monitor soft off pin
#define KEEP_POWERED_PIN   24 // GPIO 24 = PIN 18, keeps power pin

//#define RELAY_INP2  25 // GPIO #25

int prog_exit(void);

int main(int argc, char **argv)
{
    int relay = 0;
    int onoff = 0;
    setlogmask (LOG_UPTO (LOG_NOTICE));
    openlog ("softshut", LOG_CONS | LOG_PID | LOG_NDELAY, LOG_LOCAL1);
    syslog (LOG_NOTICE, "Program started by User %d", getuid ());
    syslog (LOG_INFO, "softshut start");

 // GPIO 初始化
    printf("bcm2835_init!\n");
    if(!bcm2835_init())
        return 1;

 // 設定接腳狀態       
    printf("set KEEP_POWERED_PIN\n");
    bcm2835_gpio_fsel(KEEP_POWERED_PIN, BCM2835_GPIO_FSEL_OUTP);
    printf("set SOFT_OFF_PIN\n");
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
            printf("low event detect for pin 16\n");
        }
        // wait a bit
        delay(500);
    }
    closelog ();

    return prog_exit();
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
