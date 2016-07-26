/*
 * SIM800L.cpp
 * The MIT License (MIT)
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 */

#include "SIM800L.h"

SoftwareSerial *serialSIM800L = NULL;

void  SIM800L_init(void * uart_device, uint32_t baud)
{
    serialSIM800L = (SoftwareSerial*)uart_device;
	serialSIM800L->begin(baud);
}
void  SIM800L_end()
{
	serialSIM800L->end();
}

int SIM800L_check_readable()
{
    return serialSIM800L->available();
}

int SIM800L_wait_readable (int wait_time)
{
    unsigned long timerStart;
    int dataLen = 0;
    timerStart = millis();
    while((unsigned long) (millis() - timerStart) > wait_time * 1000UL) {
        delay(500);
        dataLen = SIM800L_check_readable();
        if(dataLen > 0){
            break;
        }
    }
    return dataLen;
}

void SIM800L_flush_serial()
{
    while(SIM800L_check_readable()){
        char c = serialSIM800L->read();
    }
}

void SIM800L_read_buffer(char *buffer, int count, unsigned int timeout, unsigned int chartimeout)
{
    int i = 0;
    unsigned long timerStart, prevChar;
    timerStart = millis();
    prevChar = 0;
    while(1) {
        while (SIM800L_check_readable()) {
            char c = serialSIM800L->read();
            prevChar = millis();
            buffer[i++] = c;
            if(i >= count)break;
        }
        if(i >= count)break;
        if ((unsigned long) (millis() - timerStart) > timeout * 1000UL) {
            break;
        }
        //If interchar Timeout => return FALSE. So we can return sooner from this function. Not DO it if we dont recieve at least one char (prevChar <> 0)
        if (((unsigned long) (millis() - prevChar) > chartimeout) && (prevChar != 0)) {
            break;
        }
    }
}

void SIM800L_clean_buffer(char *buffer, int count)
{
    for(int i=0; i < count; i++) {
        buffer[i] = '\0';
    }
}

//HACERR quitar esta funcion ?
void SIM800L_send_byte(uint8_t data)
{
	serialSIM800L->write(data);
}

void SIM800L_send_char(const char c)
{
	serialSIM800L->write(c);
}

void SIM800L_send_cmd(const char* cmd)
{
  for(int i=0; i<strlen(cmd); i++)
    {
        SIM800L_send_byte(cmd[i]);
    }
}

void SIM800L_send_cmd(const __FlashStringHelper* cmd)
{
  int i = 0;
  const char *ptr = (const char *) cmd;
  while (pgm_read_byte(ptr + i) != 0x00) {
    SIM800L_send_byte(pgm_read_byte(ptr + i++));  
  }
}

void SIM800L_send_cmd_P(const char* cmd)
{
  while (pgm_read_byte(cmd) != 0x00)
    SIM800L_send_byte(pgm_read_byte(cmd++));  
}

void SIM800L_send_AT(void)
{
    SIM800L_check_with_cmd(F("AT\r\n"),"OK",CMD);
}

void SIM800L_send_End_Mark(void)
{
    SIM800L_send_byte((char)26);
}

boolean SIM800L_wait_for_resp(const char* resp, DataType type, unsigned int timeout, unsigned int chartimeout)
{
    int len = strlen(resp);
    int sum = 0;
    unsigned long timerStart, prevChar;    //prevChar is the time when the previous Char has been read.
    timerStart = millis();
    prevChar = 0;
    while(1) {
        if(SIM800L_check_readable()) {
            char c = serialSIM800L->read();
            prevChar = millis();
            sum = (c==resp[sum]) ? sum+1 : 0;
            if(sum == len)break;
        }
        if ((unsigned long) (millis() - timerStart) > timeout * 1000UL) {
            return false;
        }
        //If interchar Timeout => return FALSE. So we can return sooner from this function.
        if (((unsigned long) (millis() - prevChar) > chartimeout) && (prevChar != 0)) {
            return false;
        }
        
    }
    //If is a CMD, we will finish to read buffer.
    if(type == CMD) SIM800L_flush_serial();
    return true;   
}


boolean SIM800L_check_with_cmd(const char* cmd, const char *resp, DataType type, unsigned int timeout, unsigned int chartimeout)
{
    SIM800L_send_cmd(cmd);
    return SIM800L_wait_for_resp(resp,type,timeout,chartimeout);
}

//HACERR que tambien la respuesta pueda ser FLASH STRING
boolean SIM800L_check_with_cmd(const __FlashStringHelper* cmd, const char *resp, DataType type, unsigned int timeout, unsigned int chartimeout)
{
    SIM800L_send_cmd(cmd);
    return SIM800L_wait_for_resp(resp,type,timeout,chartimeout);
}

