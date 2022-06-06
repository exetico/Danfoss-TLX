/*
    Name: Danfoss Tlx       
    
    Based on TLX-ino by Torben https://github.com/TorbenNor/Danfoss-TLX

    Board LOLIN D32                                         
    https://www.aliexpress.com/item/32808551116.htm
      Pin 16  :RXD2 Seriel in
      Pin 17  :TXD2 Seriel out
    R411A01 mini 3.3V Auto RS485 to TTL232 Converter Board  
    https://www.aliexpress.com/item/32782552104.html
    
*/ 

String Program="Danfoss-TLX";
String Date   = "2022-06-06";
String Board  = "LOLIN D32";

const unsigned short FCSTable[256] =
{
0x0000, 0x1189, 0x2312, 0x329b, 0x4624, 0x57ad, 0x6536, 0x74bf,
0x8c48, 0x9dc1, 0xaf5a, 0xbed3, 0xca6c, 0xdbe5, 0xe97e, 0xf8f7,
0x1081, 0x0108, 0x3393, 0x221a, 0x56a5, 0x472c, 0x75b7, 0x643e,
0x9cc9, 0x8d40, 0xbfdb, 0xae52, 0xdaed, 0xcb64, 0xf9ff, 0xe876,
0x2102, 0x308b, 0x0210, 0x1399, 0x6726, 0x76af, 0x4434, 0x55bd,
0xad4a, 0xbcc3, 0x8e58, 0x9fd1, 0xeb6e, 0xfae7, 0xc87c, 0xd9f5,
0x3183, 0x200a, 0x1291, 0x0318, 0x77a7, 0x662e, 0x54b5, 0x453c,
0xbdcb, 0xac42, 0x9ed9, 0x8f50, 0xfbef, 0xea66, 0xd8fd, 0xc974,
0x4204, 0x538d, 0x6116, 0x709f, 0x0420, 0x15a9, 0x2732, 0x36bb,
0xce4c, 0xdfc5, 0xed5e, 0xfcd7, 0x8868, 0x99e1, 0xab7a, 0xbaf3,
0x5285, 0x430c, 0x7197, 0x601e, 0x14a1, 0x0528, 0x37b3, 0x263a,
0xdecd, 0xcf44, 0xfddf, 0xec56, 0x98e9, 0x8960, 0xbbfb, 0xaa72,
0x6306, 0x728f, 0x4014, 0x519d, 0x2522, 0x34ab, 0x0630, 0x17b9,
0xef4e, 0xfec7, 0xcc5c, 0xddd5, 0xa96a, 0xb8e3, 0x8a78, 0x9bf1,
0x7387, 0x620e, 0x5095, 0x411c, 0x35a3, 0x242a, 0x16b1, 0x0738,
0xffcf, 0xee46, 0xdcdd, 0xcd54, 0xb9eb, 0xa862, 0x9af9, 0x8b70,
0x8408, 0x9581, 0xa71a, 0xb693, 0xc22c, 0xd3a5, 0xe13e, 0xf0b7,
0x0840, 0x19c9, 0x2b52, 0x3adb, 0x4e64, 0x5fed, 0x6d76, 0x7cff,
0x9489, 0x8500, 0xb79b, 0xa612, 0xd2ad, 0xc324, 0xf1bf, 0xe036,
0x18c1, 0x0948, 0x3bd3, 0x2a5a, 0x5ee5, 0x4f6c, 0x7df7, 0x6c7e,
0xa50a, 0xb483, 0x8618, 0x9791, 0xe32e, 0xf2a7, 0xc03c, 0xd1b5,
0x2942, 0x38cb, 0x0a50, 0x1bd9, 0x6f66, 0x7eef, 0x4c74, 0x5dfd,
0xb58b, 0xa402, 0x9699, 0x8710, 0xf3af, 0xe226, 0xd0bd, 0xc134,
0x39c3, 0x284a, 0x1ad1, 0x0b58, 0x7fe7, 0x6e6e, 0x5cf5, 0x4d7c,
0xc60c, 0xd785, 0xe51e, 0xf497, 0x8028, 0x91a1, 0xa33a, 0xb2b3,
0x4a44, 0x5bcd, 0x6956, 0x78df, 0x0c60, 0x1de9, 0x2f72, 0x3efb,
0xd68d, 0xc704, 0xf59f, 0xe416, 0x90a9, 0x8120, 0xb3bb, 0xa232,
0x5ac5, 0x4b4c, 0x79d7, 0x685e, 0x1ce1, 0x0d68, 0x3ff3, 0x2e7a,
0xe70e, 0xf687, 0xc41c, 0xd595, 0xa12a, 0xb0a3, 0x8238, 0x93b1,
0x6b46, 0x7acf, 0x4854, 0x59dd, 0x2d62, 0x3ceb, 0x0e70, 0x1ff9,
0xf78f, 0xe606, 0xd49d, 0xc514, 0xb1ab, 0xa022, 0x92b9, 0x8330,
0x7bc7, 0x6a4e, 0x58d5, 0x495c, 0x3de3, 0x2c6a, 0x1ef1, 0x0f78
};


String RS485_SrcAddr = "0002"; //Program adresse
String RS485_StartFrame = "FF03";
String RS485_StartStop = "7E";
String RS485_DstAddr = "FFFF"; //Inverter Adressen "C6B1" - AM TLX as reported by findaddr
String RS485_CRC    = "0000";
String PreGet       = "0A01C808D0"; //For ULX Change to "0A01C804D0"
String PostGet      = "8000000000"; 

String GetAddr      = "0015"; // Ping Size 0x00,Type 0x15 

String GetPvTemp    = "0204"; // Temperatures [C]
String GetOutTemp   = "0203";

String GetPvV1      = "0228"; // PV Voltage, input 1 [V/10]
String GetPvV2      = "0229";
String GetPvV3      = "022A";
String GetPvA1      = "022D"; // PV Current, input 1 [mA]
String GetPvA2      = "022E";
String GetPvA3      = "022F";
//String GetPvP1      = "0232"; // PV Power, input 1 [W] //\\TLX Pro+ 6kW does not return these
//String GetPvP2      = "0233";
//String GetPvP3      = "0234";

String GetHZ1       = "0261"; // Grid frequency, phase L1 [mHz]v
String GetHZ2       = "0262";
String GetHZ3       = "0263";
String GetHZ        = "0250";

String GetGridDC1   = "024C"; // Grid current, DC content, phase L1 [mA]
String GetGridDC2   = "024D";
String GetGridDC3   = "024E";
String GetGridV1    = "023C"; // Grid voltage, phase L1 [V/10]
String GetGridV2    = "023D";
String GetGridV3    = "023E";
String GetGridA1    = "023F"; // Grid current, phase L1 [mA]
String GetGridA2    = "0240";
String GetGridA3    = "0241";
String GetGridP1    = "0242"; // Grid power, phase L1 [W]
String GetGridP2    = "0243";
String GetGridP3    = "0244";
String GetGridP     = "0246"; // Grid power, sum of L1, L2 and L3 [W]

String GetTotalE    = "0102"; // Total Energy Production [Wh]
String GetProdTday  = "780A"; // Inverter production today [Wh]
//String GetProdTweek = "780B"; // Inverter production this week [Wh] - not supported by TLX
//String GetProdTmonth= "780C"; // Inverter production this month [Wh] - not supported by TLX
String GetProdTyear = "7832"; // Inverter production this year [Wh]

//String GetProdTdaY="0104";  // Energy Production Today [Wh] 0x01 0x04 does note return it on TLX/DLX - ULX Only
String GetGridEL1day="0247"; // Grid Energy Today, phase L1 [Wh]
String GetGridEL2day="0248"; // Grid Energy Today, phase L2 [Wh]
String GetGridEL3day="0249"; // Grid Energy Today, phase L3 [Wh]
String GetGridEday = "024A"; // Grid Energy Today, sum of L1, L2 and L3 [Wh]

String GetProdL1year = "7833";
String GetProdL2year = "7834";
String GetProdL3year = "7835";
String GetProdL4year = "7836";
String GetProdL5year = "7837";
String GetProdL6year = "7838";
String GetProdL7year = "7839";
String GetProdL8year = "783A";
// etc until "7846" for 20 years ago 

String GetOpMode    = "0A02";  // Get operating mode

enum Mode_e{ OFFGRIDOFF, BOOT, CONNECT, ONGRID, FAILSAFE, OFFGRID, MODES };

const char* TLX_modes_txt[MODES] = {
  "Off Grid, OFF   ", // Off Grid The complete inverter is shut down
  "Connecting, Boot", // The inverter is booting,initializing itself etc.
  "Connecting      ", // The inverter is monitoring the grid, preparing to connect.
  "On Grid         ", // The inverter is connected to grid and is producing energy.
  "Fail safe       ", // The inverter is disconnected from grid because of an error situation.
  "Off Grid, Comm  " };// The inverter is shutdown (except for theuser interface and the communication interfaces

#define BLYNK_PRINT Serial
#define RXD2 16
#define TXD2 17


String TXData ;
byte DataAsc[100];

void GetNPrintAll()
{
    Serial.println("Total Energy Production     :" + String(GetInvData(GetTotalE) / 1000) + " KWh");
    Serial.println("Production this year        :" + String(GetInvData(GetProdTyear) / 1000) + " KWh");
    //Serial.println("Production this month       :" + String(GetInvData(GetProdTmonth) / 1000) + " KWh");
    //Serial.println("Production this week        :" + String(GetInvData(GetProdTweek) / 1000) + " KWh");
    Serial.println("Production this Day         :" + String(GetInvData(GetProdTday) / 1000) + " KWh");    
    Serial.println("--");
    Serial.println("Grid Energy L1 Day          :" + String(GetInvData(GetGridEL1day) / 1000) + " KWh");
    Serial.println("Grid Energy L2 Day          :" + String(GetInvData(GetGridEL2day) / 1000) + " KWh");  
    Serial.println("Grid Energy L3 Day          :" + String(GetInvData(GetGridEL3day) / 1000) + " KWh");
    Serial.println("Grid Energy Day             :" + String(GetInvData(GetGridEday) / 1000) + " KWh");
    Serial.println("PV Voltage input P1         :" + String(GetInvData(GetPvV1) / 10) + " V");
    Serial.println("PV Voltage input P2         :" + String(GetInvData(GetPvV2) / 10) + " V");
    Serial.println("PV Voltage input P3         :" + String(GetInvData(GetPvV3) / 10) + " V");
    Serial.println("PV Current input P1         :" + String(GetInvData(GetPvA1) / 1000) + " A");
    Serial.println("PV Current input P2         :" + String(GetInvData(GetPvA2) / 1000) + " A");
    Serial.println("PV Current input P3         :" + String(GetInvData(GetPvA3) / 1000) + " A");
    //Serial.println("PV Power input P1           :" + String(GetInvData(GetPvP1) / 1) + " W");
    //Serial.println("PV Power input P2           :" + String(GetInvData(GetPvP2) / 1) + " W");
    //Serial.println("PV Power input P3           :" + String(GetInvData(GetPvP3) / 1) + " W");
    
    //Frequency works
    //Serial.println("Grid Frequency L1           :" + String(GetInvData(GetHZ1) / 1000) + " Hz");
    //Serial.println("Grid Frequency L2           :" + String(GetInvData(GetHZ2) / 1000) + " Hz");
    //Serial.println("Grid Frequency L3           :" + String(GetInvData(GetHZ3) / 1000) + " Hz");
    //Serial.println("Grid Frequency (avg)        :" + String(GetInvData(GetHZ) / 1000) + " Hz");    
    Serial.println("Grid Voltage L1             :" + String(GetInvData(GetGridV1) / 10) + " V");
    Serial.println("Grid Voltage L2             :" + String(GetInvData(GetGridV2) / 10) + " V");
    Serial.println("Grid Voltage L3             :" + String(GetInvData(GetGridV3) / 10) + " V");
    Serial.println("Grid Current L1             :" + String(GetInvData(GetGridA1) / 1000) + " A");
    Serial.println("Grid Current L2             :" + String(GetInvData(GetGridA2) / 1000) + " A");
    Serial.println("Grid Current L3             :" + String(GetInvData(GetGridA3) / 1000) + " A");
    Serial.println("Grid Power L1               :" + String(GetInvData(GetGridP1) / 1) + " W");
    Serial.println("Grid Power L2               :" + String(GetInvData(GetGridP2) / 1) + " W");
    Serial.println("Grid Power L3               :" + String(GetInvData(GetGridP3) / 1) + " W");
    Serial.println("Grid Power (L1+L2+L3)       :" + String(GetInvData(GetGridP) / 1 /*1000*/) + " W");

    Serial.println("Grid DC Current L1          :" + String(GetInvData(GetGridDC1) / 1) + " mA");
    Serial.println("Grid DC Current L2          :" + String(GetInvData(GetGridDC2) / 1) + " mA");
    Serial.println("Grid DC Current L3          :" + String(GetInvData(GetGridDC3) / 1) + " mA");
    
 /*  Working but not currently of interest 
    Serial.println("Production 1 years ago      :" + String(GetInvData(GetProdL1year) / 1000) + " KWh");
    Serial.println("Production 2 years ago      :" + String(GetInvData(GetProdL2year) / 1000) + " KWh");
    Serial.println("Production 3 years ago      :" + String(GetInvData(GetProdL3year) / 1000) + " KWh");
    Serial.println("Production 4 years ago      :" + String(GetInvData(GetProdL4year) / 1000) + " KWh");
    Serial.println("Production 5 years ago      :" + String(GetInvData(GetProdL5year) / 1000) + " KWh");
    Serial.println("Production 6 years ago      :" + String(GetInvData(GetProdL6year) / 1000) + " KWh");
    Serial.println("Production 7 years ago      :" + String(GetInvData(GetProdL7year) / 1000) + " KWh");
    Serial.println("Production 8 years ago      :" + String(GetInvData(GetProdL8year) / 1000) + " KWh");
*/
    // No sensors connected
    //Serial.println("Panel Temperature           :" + String(GetInvData(GetPvTemp) / 1) + " C");
    //Serial.println("Out Temperature             :" + String(GetInvData(GetOutTemp) / 1) + " C");

    int mode = GetInvData(GetOpMode);
    
    Mode_e TLXMode = MODES;
    
    if (mode <= 9){
      TLXMode = OFFGRIDOFF;
    }else if(mode>=10 && mode<=49){
      TLXMode = BOOT;
    }else if(mode>=50 && mode<=59){
      TLXMode = CONNECT;
    }else if(mode>=60 && mode<=69){
      TLXMode = ONGRID;
    }else if(mode>=70 && mode<=79){
      TLXMode = FAILSAFE;
    }else if(mode>=80 && mode<=89){
      TLXMode = OFFGRID;
    }
    
    if (TLXMode < MODES){
      Serial.println("Operation mode              :" + String(TLX_modes_txt[TLXMode]) + String(mode) );   
    }else {
      Serial.println("Operation mode              :" + String("ERROR")+ String(mode) );
      
    }
}

void setup()
{
    Serial.begin(115200) ;  // Debug console
    Serial2.begin(19200, SERIAL_8N1, RXD2, TXD2); // TLX
    delay(1000);
    Serial.println(Program);
    Serial.println(Date);
    Serial.println(Board);
    FindInvAddr();
    GetNPrintAll();
 //   Serial.println(RXData);
}

void loop()
{
    delay(60000);
    GetNPrintAll();    
}

float GetInvData(String GetLocal){
    String RXData;
    TX_TLX(GetLocal);
    RXData = RX_TLX();
    return HexToDec(RXData.substring(36, 36 + 2) + RXData.substring(34, 34 + 2) + RXData.substring(32, 32 + 2) + RXData.substring(30, 30 + 2));
}

String RX_TLX() {
    const int RX_TIMEOUT=50;
    const int RX_LENGTH =2;
    String RxBuffer;
    String RXData = "";
    unsigned long TimeNow = millis();

    delay(25); //\\ 25 was not enough. typical delay 28-31ms. So wait for RX_LENGTH;
    while(Serial2.available() < RX_LENGTH){
      delay(1);
      if(millis() - TimeNow >RX_TIMEOUT)break;
    }
    //\\Serial.println("RX time                     :" + String(millis()-TimeNow) + " ms" );   

    
    while (Serial2.available() > 0) {
      RxBuffer = String(Serial2.read(), HEX);
      if (RxBuffer.length() == 1)  RxBuffer = "0" + RxBuffer;
      RXData = RXData + RxBuffer;
    }
    //\\Serial.println("RX Length                   :" + String(RxBuffer.length() ));   
    RXData.toUpperCase();
    RXData.replace("7D5E", "7E");
    RXData.replace("7D5D", "7D");
    return RXData;
}

void FindInvAddr() {
    TX_TLX(GetAddr);
    RS485_DstAddr = RX_TLX().substring(6, 6 + 4);
    Serial.print("RS485_DstAddr :");
    Serial.println(RS485_DstAddr);
}

void TX_TLX(String TXLocal) {
    int x;
    TXData = RS485_StartFrame + RS485_SrcAddr + RS485_DstAddr + PreGet + TXLocal + PostGet;
    TXData = RS485_StartStop + TXData + pppfcs16(DataAsc, ChrHex2Asc()) + RS485_StartStop;
//    Serial.println(TXData + " Send ");
    for (x = 0; x < TXData.length(); x = x + 2) {
     Serial2.write(HexToDec(TXData.substring(x, x + 2)));
    }
}

unsigned int ChrHex2Asc() {
    int y = 0;
    for (int x = 0; x < TXData.length(); x = x + 2) {
        DataAsc[y] = HexToDec(TXData.substring(x, x + 2));
        y++;
    }
    return y;
}

String pppfcs16(byte* pByte, int length)
{
    int fcs = 0xFFFF;
    while (length--)
        fcs = (fcs >> 8) ^ FCSTable[(fcs ^ *pByte++) & 0xff];
    fcs ^= 0xFFFF;
    String str_fcs = String(fcs, HEX);
    str_fcs = str_fcs.substring(2, 4) + str_fcs.substring(0, 2);
    return str_fcs;
}

unsigned int HexToDec(String hexString) {
    unsigned int decValue = 0;
    int nextInt;
    for (int i = 0; i < hexString.length(); i++) {
        nextInt = int(hexString.charAt(i));
        if (nextInt >= 48 && nextInt <= 57) nextInt = map(nextInt, 48, 57, 0, 9);
        if (nextInt >= 65 && nextInt <= 70) nextInt = map(nextInt, 65, 70, 10, 15);
        if (nextInt >= 97 && nextInt <= 102) nextInt = map(nextInt, 97, 102, 10, 15);
        nextInt = constrain(nextInt, 0, 15);
        decValue = (decValue * 16) + nextInt;
    }
    return decValue;
}