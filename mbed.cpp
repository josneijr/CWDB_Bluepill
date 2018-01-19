#include "stm32f103c8t6.h"
#include "mbed.h"
#include "TCPSocket.h"
#include "ESP8266Interface.h"
#include "TinyGPSPlus.h"

#include <string.h>
#include <ctype.h>
#include <stdlib.h>

//Timer para usar algo equivalente a millis do Arduino
Timer t;
//Classe do GPS
TinyGPSPlus gps;

Serial device_pc(PA_9, PA_10);
Serial GPS_S(PA_2, PA_3);

void callback() {
    // Note: you need to actually read from the serial to clear the RX interrupt
    gps.encode(GPS_S.getc());
}

static void printDateTime(TinyGPSDate &d, TinyGPSTime &t)
{
    if (d.isValid() && t.isValid())
        device_pc.printf("%02d/%02d/%02d %02d:%02d:%02d \r\n", d.month(), d.day(), d.year(), t.hour(), t.minute(), t.second());
    
    device_pc.printf("DATE_AGE: %d \r\n", d.age());
}
 
int main()
{   
    int i=0;
    
    t.start();

    while(1)
    {        
        GPS_S.baud(9600);
        device_pc.baud(115200);
      
        device_pc.printf("\r\n\n\nCWdB %d\r\n\n", i++);
        
        device_pc.printf("GPS VALUE: %d \r\n", gps.satellites.value());
        device_pc.printf("HDOP VALUE: %d \r\n", gps.hdop.value());
        device_pc.printf("GPS LAT: %.3f \r\n", gps.location.lat());
        device_pc.printf("GPS LONG: %.3f \r\n", gps.location.lng());
      
        device_pc.printf("GPS ALTITUDE: %.2f \r\n", gps.altitude.meters());
        device_pc.printf("GPS COURSE: %.2f \r\n", gps.course.deg());
        device_pc.printf("GPS SPEED: %.2f \r\n", gps.speed.kmph());
        
        printDateTime(gps.date, gps.time);
        
        if(gps.course.isValid())
            device_pc.printf("DIREÇÃO: %s", TinyGPSPlus::cardinal(gps.course.value()));
        
        wait(1);
        
        GPS_S.attach(&callback);
    }
}