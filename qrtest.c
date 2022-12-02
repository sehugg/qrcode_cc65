
// QR code demo for 8-bit computers.
// https://8bitworkshop.com/docs/posts/2022/8bit-qr-code.html
// Uses qrtiny:
// - https://github.com/danielgjackson/qrtiny
// - Copyright (c) 2020, Dan Jackson. All rights reserved.

#include "qrtiny.h"
//#link "qrtiny.c"

#include <stdio.h>

#include <tgi.h>


const char* URLstring = "8BITWORKSHOP.COM";


void main() {
  
  // Use a (26 byte) buffer for holding the encoded payload and ECC calculations
  uint8_t buffer[QRTINY_BUFFER_SIZE];
  
  // Choose a format for the QR Code: a mask pattern (binary `000` to `111`) and an error correction level (`LOW`, `MEDIUM`, `QUARTILE`, `HIGH`).
  uint16_t formatInfo = QRTINY_FORMATINFO_MASK_000_ECC_MEDIUM;
  
  // Encode one or more segments text to the buffer
  size_t payloadLength = 0;
  bool result;
  
  puts("Computing...\n");
  payloadLength += QrTinyWriteAlphanumeric(buffer, payloadLength, URLstring);
//  payloadLength += QrTinyWriteNumeric(buffer, payloadLength, "1234567890");
//  payloadLength += QrTinyWrite8Bit(buffer, payloadLength, "!");
  
  // Compute the remaining buffer contents: any required padding and the calculated error-correction information
  result = QrTinyGenerate(buffer, payloadLength, formatInfo);
  printf("Done! result = %d\n", result);

  // draw to screen using TGI driver
  if (result) {
    int x,y;
    int x0 = 9;
    int y0 = 9;

    tgi_install (a2_lo_tgi);
    tgi_init ();
    tgi_clear ();
    tgi_setcolor(TGI_COLOR_WHITE);
    tgi_bar(x0-2, y0-2, x0+21+2, y0+21+2);

    for (y=0; y<21; y++) {
      for (x=0; x<21; x++) {
        int module = QrTinyModuleGet(buffer, formatInfo, x, y);
        tgi_setcolor(module ? TGI_COLOR_BLACK : TGI_COLOR_WHITE);
        tgi_setpixel(x+x0, y+y0);
      }
    }
  }

  while (1);
}
