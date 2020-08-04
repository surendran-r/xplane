#include "xpc.h"
#include <Arduino.h>
#include <WiFiUdp.h>

void sendXPCPacket(char *buff, int len)
{
    WiFiUDP Udp;
    Udp.beginPacket("192.168.0.112", 49009);
    Udp.write(buff, len);
    Udp.endPacket();
}

void sendControlPacket(float throttle)
{
    union converter {
        float data;
        char bytes[4];
    } conv;
    float values[5] = {0.0f};
    values[2] = throttle;
    char udpControlPacket[31] = "CTRL";

    int cur = 5, size = 5;
    int i; // iterator
    char *udpControlPacketPtr = &(udpControlPacket[0]);
    int j = 0;
    for (i = 0; i < 6; i++)
    {
        float val = -998;
        if (i < size)
        {
            val = values[i];
        }

        if (i == 4)
        {
            udpControlPacket[cur++] = val == -998 ? -1 : (unsigned char)val;
        }
        else
        {
            conv.data = val;
            for (j = 0; j < 4; j++)
            {
                udpControlPacketPtr[cur++] = conv.bytes[j];
            }
        }
    }
    udpControlPacket[26] = 0;
    udpControlPacket[27] = -998;
    sendXPCPacket(udpControlPacket, 31);
}
