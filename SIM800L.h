/*
 * SIM800L.h 
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

#ifndef __SIM800L_H__
#define __SIM800L_H__

#include <SoftwareSerial.h>
#include <Arduino.h>

#define DEFAULT_TIMEOUT     		 5   //seconds
#define DEFAULT_INTERCHAR_TIMEOUT 1500   //miliseconds

enum DataType {
    CMD     = 0,
    DATA    = 1,
};

void  SIM800L_init(void * uart_device, uint32_t baud);
void  SIM800L_end();
int   SIM800L_check_readable();
int   SIM800L_wait_readable(int wait_time);
void  SIM800L_flush_serial();
void  SIM800L_read_buffer(char* buffer,int count,  unsigned int timeout = DEFAULT_TIMEOUT, unsigned int chartimeout = DEFAULT_INTERCHAR_TIMEOUT);
void  SIM800L_clean_buffer(char* buffer, int count);
void  SIM800L_send_byte(uint8_t data);
void  SIM800L_send_char(const char c);
void  SIM800L_send_cmd(const char* cmd);
void  SIM800L_send_cmd(const __FlashStringHelper* cmd);
void  SIM800L_send_cmd_P(const char* cmd);   
void  SIM800L_send_AT(void);
void  SIM800L_send_End_Mark(void);
boolean  SIM800L_wait_for_resp(const char* resp, DataType type, unsigned int timeout = DEFAULT_TIMEOUT, unsigned int chartimeout = DEFAULT_INTERCHAR_TIMEOUT);
boolean  SIM800L_check_with_cmd(const char* cmd, const char *resp, DataType type, unsigned int timeout = DEFAULT_TIMEOUT, unsigned int chartimeout = DEFAULT_INTERCHAR_TIMEOUT);
boolean  SIM800L_check_with_cmd(const __FlashStringHelper* cmd, const char *resp, DataType type, unsigned int timeout = DEFAULT_TIMEOUT, unsigned int chartimeout = DEFAULT_INTERCHAR_TIMEOUT);

#endif
