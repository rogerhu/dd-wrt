/*
 * The olsr.org Optimized Link-State Routing daemon(olsrd)
 * Copyright (c) 2004, Andreas T�nnesen(andreto@olsr.org)
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without 
 * modification, are permitted provided that the following conditions 
 * are met:
 *
 * * Redistributions of source code must retain the above copyright 
 *   notice, this list of conditions and the following disclaimer.
 * * Redistributions in binary form must reproduce the above copyright 
 *   notice, this list of conditions and the following disclaimer in 
 *   the documentation and/or other materials provided with the 
 *   distribution.
 * * Neither the name of olsr.org, olsrd nor the names of its 
 *   contributors may be used to endorse or promote products derived 
 *   from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS 
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT 
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS 
 * FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE 
 * COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, 
 * INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, 
 * BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; 
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER 
 * CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT 
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN 
 * ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE 
 * POSSIBILITY OF SUCH DAMAGE.
 *
 * Visit http://www.olsr.org for more information.
 *
 * If you find this software useful feel free to make a donation
 * to the project. For more information see the website or contact
 * the copyright holders.
 *
 * $Id: mantissa.c,v 1.7 2005/02/20 18:52:18 kattemat Exp $
 */


#include "mantissa.h"
#include "math.h"
#include "olsr_protocol.h"

/**
 *Function that converts a double to a mantissa/exponent
 *product as described in RFC3626:
 *
 * value = C*(1+a/16)*2^b [in seconds]
 *
 *  where a is the integer represented by the four highest bits of the
 *  field and b the integer represented by the four lowest bits of the
 *  field.
 *
 *@param interval the time interval to process
 *
 *@return a 8-bit mantissa/exponent product
 */

olsr_u8_t
double_to_me(double interval)
{
  int a, b;

  b = 0;

  while(interval / VTIME_SCALE_FACTOR >= pow((double)2, (double)b))
    b++;

  b--;
  if(b < 0)
    {
      a = 1;
      b = 0;
    } 
  else 
    if (b > 15)
      {
	a = 15;
	b = 15;
      } 
    else 
      { 
	a = (int)(16*((double)interval/(VTIME_SCALE_FACTOR*(double)pow(2,b))-1));
	while(a >= 16)
	  {
	    a -= 16;
	    b++;
	  }
      }

  //printf("Generated mantissa/exponent(%d/%d): %d from %f\n", a, b, (olsr_u8_t) (a*16+b), interval);
  //printf("Resolves back to: %f\n", me_to_double((olsr_u8_t) (a*16+b)));
  return (olsr_u8_t) (a*16+b);
}




/**
 *Function that converts a mantissa/exponent 8bit value back
 *to double as described in RFC3626:
 *
 * value = C*(1+a/16)*2^b [in seconds]
 *
 *  where a is the integer represented by the four highest bits of the
 *  field and b the integer represented by the four lowest bits of the
 *  field.
 *
 *@param me the 8 bit mantissa/exponen value
 *
 *@return a double value
 */
double
me_to_double(olsr_u8_t me)
{
  int a, b;

  a = me>>4;
  b = me - a*16;

  return (double)(VTIME_SCALE_FACTOR*(1+(double)a/16)*(double)pow(2,b));
}
