#include <math.h>
#include <mcs51/8052.h>
#include <stdio.h>
#include <stdlib.h>

#define Data_Port P2
#define Bep       P3_6  // Buzzer
#define DQ        P3_7  // Data pin of the DS18B20
#define RST       P0_2
#define CS2       P0_3
#define CS1       P0_4
#define E         P0_5
#define RW        P0_6
#define RS        P0_7

__code char font[][5] = {
    {0x00, 0x00, 0x00, 0x00, 0x00},  // 20 (Space)
    {0x7e, 0x11, 0x11, 0x11, 0x7e},  // 41 A
    {0x7f, 0x49, 0x49, 0x49, 0x36},  // 42 B
    {0x3e, 0x41, 0x41, 0x41, 0x22},  // 43 C
    {0x7f, 0x41, 0x41, 0x22, 0x1c},  // 44 D
    {0x7f, 0x49, 0x49, 0x49, 0x41},  // 45 E
    {0x7f, 0x09, 0x09, 0x09, 0x01},  // 46 F
    {0x3e, 0x41, 0x49, 0x49, 0x7a},  // 47 G
    {0x7f, 0x08, 0x08, 0x08, 0x7f},  // 48 H
    {0x00, 0x41, 0x7f, 0x41, 0x00},  // 49 I
    {0x20, 0x40, 0x41, 0x3f, 0x01},  // 4a J
    {0x7f, 0x08, 0x14, 0x22, 0x41},  // 4b K
    {0x7f, 0x40, 0x40, 0x40, 0x40},  // 4c L
    {0x7f, 0x02, 0x0c, 0x02, 0x7f},  // 4d M
    {0x7f, 0x04, 0x08, 0x10, 0x7f},  // 4e N
    {0x3e, 0x41, 0x41, 0x41, 0x3e},  // 4f O
    {0x7f, 0x09, 0x09, 0x09, 0x06},  // 50 P
    {0x3e, 0x41, 0x51, 0x21, 0x5e},  // 51 Q
    {0x7f, 0x09, 0x19, 0x29, 0x46},  // 52 R
    {0x46, 0x49, 0x49, 0x49, 0x31},  // 53 S
    {0x01, 0x01, 0x7f, 0x01, 0x01},  // 54 T
    {0x3f, 0x40, 0x40, 0x40, 0x3f},  // 55 U
    {0x1f, 0x20, 0x40, 0x20, 0x1f},  // 56 V
    {0x3f, 0x40, 0x38, 0x40, 0x3f},  // 57 W
    {0x63, 0x14, 0x08, 0x14, 0x63},  // 58 X
    {0x07, 0x08, 0x70, 0x08, 0x07},  // 59 Y
    {0x61, 0x51, 0x49, 0x45, 0x43}   // 5a Z
};

// Delay func (not so accurate)
void delay_us(unsigned char t) {
    while (t--)
        ;
}

void delay(unsigned int k) {
    int i, j;
    for (i = 0; i < k; i++)
        for (j = 0; j < 112; j++)
            ;
}

void GLCD_Command(char Command) {
    Data_Port = Command; /* Copy command on data pin */
    RS = 0;              /* Make RS LOW to select command register */
    RW = 0;              /* Make RW LOW to select write operation */
    E = 1;               /* Make HIGH to LOW transition on Enable */
    delay_us(1);
    E = 0;
    delay_us(1);
}

void GLCD_Data(char Data) {
    Data_Port = Data; /* Copy data on data pin */
    RS = 1;           /* Make RS HIGH to select data register */
    RW = 0;           /* Make RW LOW to select write operation */
    E = 1;            /* Make HIGH to LOW transition on Enable */
    delay_us(1);
    E = 0;
    delay_us(1);
}

void GLCD_Init(void) {
    CS1 = 1;
    CS2 = 1;            /* Select left & right half of display */
    RST = 1;            /* Keep reset pin high */
    delay_us(20);
    GLCD_Command(0x3E); /* Display OFF */
    GLCD_Command(0x40); /* Set y address (column=0) */
    GLCD_Command(0xB8); /* Set x address (page=0) */
    GLCD_Command(0xC0); /* Set z address (start line=0) */
    GLCD_Command(0x3F); /* Display ON */
}

void GLCD_ClearAll(void) {
    int i, j;
    CS1 = 1;
    CS2 = 1;                      /* Select left & right half of display */
    for (i = 0; i < 8; i++) {
        GLCD_Command((0xB8) + i); /* Increment page */
        for (j = 0; j < 64; j++) {
            GLCD_Data(0);         /* Write zeros to all 64 column */
        }
    }
    GLCD_Command(0x40); /* Set Y address (column=0) */
    GLCD_Command(0xB8); /* Set X address (page=0) */
}

void GLCD_Plot_Coordinate_Axes(void) {
    int i, j;
    unsigned int Page = ((0xB8) + 2), Y_address = 0;
    float Page_inc = 0.5;

    // Draw Y-axis
    CS1 = 1;
    CS2 = 0;                      /* Select right half of display */
    for (i = 0; i < 8; i++) {
        GLCD_Command((0xB8) + i); /* Increment page */
        if (i >= 2) {
            GLCD_Command(0x40 + 5);
            if (i < 7)
                GLCD_Data(0xFF);
            else
                GLCD_Data(0x3F);
        }
    }
    // Arrow
    GLCD_Command(0xB8 + 2);     /* Select Page 3 (left) */
    GLCD_Command(0x40 + 5 - 2); /* Select Column 1 */
    GLCD_Data(0x04);
    GLCD_Command(0x40 + 5 - 1); /* Select Column 2 */
    GLCD_Data(0x02);
    GLCD_Command(0x40 + 5 + 1); /* Select Column 1 */
    GLCD_Data(0x02);
    GLCD_Command(0x40 + 5 + 2); /* Select Column 2 */
    GLCD_Data(0x04);

    // Draw X-axis
    CS1 = 1;
    CS2 = 0;                /* Select left half of display */
    GLCD_Command((0xB8) + 7);
    GLCD_Command(0x40 + 6); /* Set Y address (column=1) */
    for (j = 6; j < 64; j++) {
        GLCD_Data(0x20);
    }

    CS1 = 0;
    CS2 = 1;            /* Select right half of display */
    GLCD_Command((0xB8) + 7);
    GLCD_Command(0x40); /* Set Y address (column=1) */
    for (j = 0; j < 32; j++) {
        GLCD_Data(0x20);
    }
    GLCD_Command(0x40 + 30); /* Select Column 1 */
    GLCD_Data(0x70);
    GLCD_Command(0x40 + 29);
    GLCD_Data(0xA8);
}

void GLCD_WriteChar(unsigned char page_no) {
    unsigned int column, Page = 0xB8, Y_address = 0;
    float Page_inc = 0.5;

    CS1 = 1;
    CS2 = 0;
    GLCD_Command((0xB8) + page_no);
    GLCD_Command(0x40);
    for (column = 0; column < 5; column++) {
        GLCD_Data(font[1][column]);
        if ((Y_address + 1) % 64 == 0) {
            CS1 = !CS1;
            CS2 = !CS2;
            GLCD_Command((Page + Page_inc));
            Page_inc = Page_inc + 0.5;
        }
        Y_address++;
    }
}

//------------------------------------------------------------------
// Hàm trễ dành cho DS18B20 (không quá chính xác)
void delay_us_DS18B20(unsigned int time) {
    // Trễ us
    while (time--)
        ;
}

// Khởi tạo cảm biến
void init_DS18B20() {
    DQ = 1;
    delay_us_DS18B20(10);
    // Đặt lại xung trong một khoảng thời gian tối thiểu.
    DQ = 0;
    delay_us_DS18B20(65);
    DQ = 1;                // Cạnh tăng
    delay_us_DS18B20(20);  // Đợi DS18B20 phản hồi
}

// Đọc 1 byte từ Scratchpad
unsigned char readByteFromScratchpad() {
    unsigned char j = 0;
    unsigned char byte = 0;
    for (j = 8; j > 0; j--) {
        DQ = 0;
        byte >>= 1;
        DQ = 1;                // The master bus releases the 1-Wire bus
        if (DQ) byte |= 0x80;  // Read 1? Otherwise, Read 0
        delay_us_DS18B20(20);
    }
    return (byte);
}

// Ghi 1 byte vào Scratchpad
void writeByteToScratchpad(unsigned char byte) {
    unsigned char j = 0;
    for (j = 8; j > 0; j--) {
        DQ = 0;
        DQ = byte & 0x01;
        // delay_us_DS18B20(5);
        if (DQ)  // Write 1
            delay_us_DS18B20(3);
        else     // Write 0
            delay_us_DS18B20(15);
        DQ = 1;  // The master bus releases the 1-Wire bus
        byte >>= 1;
    }
}

// Hàm đọc nhiệt độ lưu vào biến temp
int read_temp_DS18B20() {
    unsigned char tmp = 0;
    unsigned char Byte0 = 0;  // lưu trữ byte 0 của Scratchpad
    unsigned char Byte1 = 0;  // lưu trữ byte 1 của Scratchpad
    EA = 0;  // Nếu có ngắt thì cấm ngắt để tránh ảnh hưởng quá trình giao tiếp
    init_DS18B20();               // Khởi tạo cảm biến DS18B20
    writeByteToScratchpad(0xCC);  // The master issues Skip ROM [CCh] command
    writeByteToScratchpad(0x44);  // Convert T [44h] command. To initiate a temp
                                  // measurement and A-to-D conversion.
    delay_us_DS18B20(10);
    init_DS18B20();
    writeByteToScratchpad(0xCC);  // The master issues Skip ROM [CCh] command
    writeByteToScratchpad(0xBE);  // Read temp value.
                                  // Read Scratpad [BEh] command.
    delay_us_DS18B20(10);
    Byte0 = readByteFromScratchpad();  // Read Byte0 of the Scratchpad (low byte
                                       // of the temp value)
    Byte1 = readByteFromScratchpad();  // Read Byte1 of the Scratchpad (high
                                       // byte of the temp value)
    // Calculate the temp value from Byte0 & Byte1 and save it to temp variable
    tmp = ((Byte1 * 256 + Byte0) >> 4);
    EA = 1;  // Đọc xong thì cho phép ngắt
    if (tmp <= 128)
        return tmp;
    else
        return tmp - 256;
}
//------------------------------------------------------------------

void main(void) {
    int temp, i, pageNum = 7, column = 5, cs = 0;
    GLCD_Init(); /* Initialize GLCD */
    GLCD_ClearAll();
    GLCD_Plot_Coordinate_Axes();
    GLCD_WriteChar(0);
    delay(500);
    while (1) {
        pageNum = 7;
        temp = read_temp_DS18B20();
        if (column == 64) {
            column = 0;
            CS1 = !CS1;
            CS2 = !CS2;
            cs = 1;
        } else
            column++;

        if (cs == 1 && column == 12) {
            GLCD_ClearAll();
            GLCD_Plot_Coordinate_Axes();
            GLCD_WriteChar(0);
            cs = 0;
            column = 6;
            delay(500);
        }

        for (i = temp; i >= 0; i -= 8) {
            if (i >= 8)
                pageNum--;
            else
                break;
        }
        GLCD_Command((0xB8) + pageNum);
        GLCD_Command((0x40) + column);
        GLCD_Data(0x80 >> i);

        delay(1000);
    }
}
