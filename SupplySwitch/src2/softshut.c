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
#include <string.h>
#include <errno.h>

#define SOFT_OFF_PIN 4 // PIN 16, monitor soft off pin
#define KEEP_POWERED_PIN   5 // PIN 18, keeps power pin

/*
 * softoffInterrupt:
 *********************************************************************************
 */
static volatile int globalCounter = 0 ;
void softoffInterrupt (void)
{
  ++globalCounter ;
  syslog(LOG_NOTICE,"softoffInterrupt value: %d", globalCounter);
}

int main(int argc, char **argv)
{
    setlogmask (LOG_UPTO (LOG_NOTICE)); // discard log message bacause LOG_INFO priority level is below LOG_NOTICE
    openlog("softshut", LOG_CONS | LOG_PID | LOG_NDELAY, LOG_LOCAL1);
    syslog(LOG_NOTICE, "Program started by User %d", getuid());

    if(wiringPiSetup()==-1) {
        syslog(LOG_ERR, "wiringPi Setup error");
        return -1 ;
    }
    syslog(LOG_NOTICE, "softshut start for Raspberry Pi revision: %d", piBoardRev());
// 設定接腳狀態
    syslog(LOG_NOTICE,"set KEEP_POWERED_PIN %d output", KEEP_POWERED_PIN);
    pinMode(KEEP_POWERED_PIN,OUTPUT);
    syslog(LOG_NOTICE,"set SOFT_OFF_PIN %d input", SOFT_OFF_PIN);
    pinMode(SOFT_OFF_PIN,INPUT);
// 初始狀態
    digitalWrite(KEEP_POWERED_PIN, 1);
    if (wiringPiISR (SOFT_OFF_PIN, INT_EDGE_FALLING, &softoffInterrupt) < 0) {
        syslog(LOG_ERR, "Unable to setup ISR: %s\n", strerror(errno)) ;
        return 1 ;
    }
    //pullUpDnControl(SOFT_OFF_PIN, PUD_UP);//http://wiringpi.com/reference/core-functions/
    //syslog(LOG_NOTICE,"get SOFT_OFF_PIN value: %d", digitalRead(SOFT_OFF_PIN));
    while(1)
    {
        int rs = digitalRead(SOFT_OFF_PIN);
        syslog(LOG_NOTICE,"get SOFT_OFF_PIN value: %d", rs);
        delay(500);
    }
    digitalWrite(KEEP_POWERED_PIN,0);

    closelog ();    
    return 0;

}

