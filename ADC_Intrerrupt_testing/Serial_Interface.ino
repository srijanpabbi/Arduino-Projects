


#include <SPI.h>
// SS:   pin 10
// MOSI: pin 11
// MISO: pin 12
// SCK:  pin 13

// WinBond flash commands
#define WB_WRITE_ENABLE       0x06
#define WB_WRITE_DISABLE      0x04
#define WB_CHIP_ERASE         0xc7 
#define WB_READ_STATUS_REG_1  0x05
#define WB_READ_DATA          0x03
#define WB_PAGE_PROGRAM       0x02
#define WB_JEDEC_ID           0x9F
#define WB_SECTOR_ERASE       0x20   //mine


/* 
 * These global variables enable aSSembly of the user serial 
 * input command.
 */
boolean g_command_ready(false);
String g_command;

/*
 * print_page_bytes() is a simple helperf function that formats 256
 * bytes of data into an easier to read grid.
 */
void print_page_bytes(byte *page_buffer) {
  char buf[10];
  for (int i = 0; i < 16; ++i) {
    for (int j = 0; j < 16; ++j) {
      sprintf(buf, "%02u", page_buffer[i * 16 + j]);   //%02x for hexadecimal
    //  String one=String(buf,HEX);
      Serial.print(buf);Serial.print(',');
    }
    Serial.println();
  }
}

/* 
 * The JEDEC ID is fairly generic, I use this function to verify the setup
 * is working properly.
 */
void get_jedec_id(void) {
  Serial.println("command: get_jedec_id");
  byte b1, b2, b3;
  _get_jedec_id(&b1, &b2, &b3);
  char buf[128];
  sprintf(buf, "Manufacturer ID: %02xh\nMemory Type: %02xh\nCapacity: %02xh",
    b1, b2, b3);
  Serial.println(buf);
  Serial.println("Ready");
} 

void chip_erase(void) {
  Serial.println("command: chip_erase");
  _chip_erase();
  Serial.println("Ready");
}

void read_page(unsigned int page_number) {
  char buf[80];
  sprintf(buf, "command: read_page(%04xh)", page_number);
  Serial.println(buf);
  byte page_buffer[256];
  _read_page(page_number, page_buffer);
  print_page_bytes(page_buffer);
  Serial.println("Ready");
}

void read_all_pages(void) {
  Serial.println("command: read_all_pages");
  byte page_buffer[256];
  for (int i = 0; i < 4096; ++i) {
    _read_page(i, page_buffer);
    print_page_bytes(page_buffer);
  }
  Serial.println("Ready");
}

void write_byte(word page, byte offset, byte databyte) {
  char buf[80];
  sprintf(buf, "command: write_byte(%04xh, %04xh, %02xh)", page, offset, databyte);
  Serial.println(buf);
  byte page_data[256];
  _read_page(page, page_data);
  page_data[offset] = databyte;
  _write_page(page, page_data);
  Serial.println("Ready");
}



void _get_jedec_id(byte *b1, byte *b2, byte *b3) {
  digitalWrite(SS, HIGH);
  digitalWrite(SS, LOW);
  SPI.transfer(WB_JEDEC_ID);
  *b1 = SPI.transfer(0); // manufacturer id
  *b2 = SPI.transfer(0); // memory type
  *b3 = SPI.transfer(0); // capacity
  digitalWrite(SS, HIGH);
  not_busy();
}  

/*
 * See the timing diagram in section SS.2.26 of the
 * data sheet, "Chip Erase (C7h / 06h)". (Note:
 * either opcode works.)
 */
void _chip_erase(void) {
  digitalWrite(SS, HIGH);
  digitalWrite(SS, LOW);  
  SPI.transfer(WB_WRITE_ENABLE);
  digitalWrite(SS, HIGH);
  digitalWrite(SS, LOW);  
  SPI.transfer(WB_CHIP_ERASE);
  digitalWrite(SS, HIGH);
  /* See notes on rev 2 
  digitalWrite(SS, LOW);  
  SPI.transfer(WB_WRITE_DISABLE);
  digitalWrite(SS, HIGH);
  */
  not_busy();
}

/*
 * See the timing diagram in section SS.2.10 of the
 * data sheet, "Read Data (03h)".
 */
void _read_page(word page_number, byte *page_buffer) {
  digitalWrite(SS, HIGH);
  digitalWrite(SS, LOW);
  SPI.transfer(WB_READ_DATA);
  // Construct the 24-bit addreSS from the 16-bit page
  // number and 0x00, since we will read 256 bytes (one
  // page).
  SPI.transfer((page_number >> 8) & 0xFF);
  SPI.transfer((page_number >> 0) & 0xFF);
  SPI.transfer(0);
  for (int i = 0; i < 256; ++i) {
    page_buffer[i] = SPI.transfer(0);
  }
  digitalWrite(SS, HIGH);
  not_busy();
}
 
/*
 * See the timing diagram in section SS.2.21 of the
 * data sheet, "Page Program (02h)".
 */
void _write_page(word page_number, byte *page_buffer) {
  digitalWrite(SS, HIGH);
  digitalWrite(SS, LOW);  
  SPI.transfer(WB_WRITE_ENABLE);
  digitalWrite(SS, HIGH);
  digitalWrite(SS, LOW);  
  SPI.transfer(WB_PAGE_PROGRAM);
  SPI.transfer((page_number >>  8) & 0xFF);
  SPI.transfer((page_number >>  0) & 0xFF);
  SPI.transfer(0);
  for (int i = 0; i < 256; ++i) {
    SPI.transfer(page_buffer[i]);
  }
  digitalWrite(SS, HIGH);
  /* See notes on rev 2
  digitalWrite(SS, LOW);  
  SPI.transfer(WB_WRITE_DISABLE);
  digitalWrite(SS, HIGH);
  */
  not_busy();
}

void _erase_page(word page_number ) {
  digitalWrite(SS, HIGH);
  digitalWrite(SS, LOW);  
  SPI.transfer(WB_WRITE_ENABLE);
  digitalWrite(SS, HIGH);
  digitalWrite(SS, LOW);  
  SPI.transfer(WB_SECTOR_ERASE);
  SPI.transfer((page_number >>  8) & 0xFF);
  SPI.transfer((page_number >>  0) & 0xFF);
  SPI.transfer(0);
  digitalWrite(SS, HIGH);
  /* See notes on rev 2
  digitalWrite(SS, LOW);  
  SPI.transfer(WB_WRITE_DISABLE);
  digitalWrite(SS, HIGH);
  */
  not_busy();
}

/* 
 * not_busy() polls the status bit of the device until it
 * completes the current operation. Most operations finish
 * in a few hundred microseconds or leSS, but chip erase 
 * may take 500+ms. Finish all operations with this poll.
 *
 * See section SS.2.8 of the datasheet
 */
 
void not_busy(void) {
  digitalWrite(SS, HIGH);  
  digitalWrite(SS, LOW);
  SPI.transfer(WB_READ_STATUS_REG_1);       
  while (SPI.transfer(0) & 1) {}; 
  digitalWrite(SS, HIGH);  
}


