#include <stdio.h>
#include <linux/rtc.h>
#include <sys/ioctl.h>
#include <sys/time.h>
#include <sys/types.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>
#include <errno.h>

int main()
{
    int i, fd, retval, irqcount = 0;
    unsigned long tmp, data;
    struct rtc_time rtc_tm;
   
    fd = open("/dev/rtc0", O_RDWR);

    if (fd ==  -1) {
        exit(errno);
    }

    fprintf(stderr, "\n\t\t\tRTC Driver Test Example.\n\n");
   

//  fill this strcture
/*    rtc_tm.tm_sec      =59;  
    rtc_tm.tm_min      =59;
    rtc_tm.tm_hour     =11;
    rtc_tm.tm_mday     =1;
    rtc_tm.tm_mon      =12;
    rtc_tm.tm_year     =100;
    rtc_tm.tm_wday     =1;
  */ 
    /* Write the RTC time/date */
//    retval = ioctl(fd, RTC_SET_TIME, &rtc_tm);
//    if (retval == -1) {
//        printf("RTC_WRITE_TIME ioctl \n");
//        exit(errno);
  //  }
   
   // sleep(62);
   
    /* Read the RTC time/date */
    retval = ioctl(fd, RTC_RD_TIME, &rtc_tm);
    if (retval == -1) {
        printf("RTC_RD_TIME ioctl \n");
        exit(errno);
    }

    printf("\n\nCurrent RTC date/time is %d-%d-%d, %02d:%02d:%02d.\n",
        rtc_tm.tm_mday, rtc_tm.tm_mon, rtc_tm.tm_year+1900,
        rtc_tm.tm_hour, rtc_tm.tm_min, rtc_tm.tm_sec);

   
   
    printf("\n\n\t\t\t *** Test complete ***\n");

    close(fd);

    return 0;
}

