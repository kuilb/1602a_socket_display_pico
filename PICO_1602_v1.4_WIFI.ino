#include <WiFi.h>
#include <map>
#include <vector>
#define setHigh(pin) (gpio_set_mask(1ul << (pin)))
#define setLow(pin)  (gpio_clr_mask(1ul << (pin)))
#define setOutput(pin) (gpio_init(pin), gpio_set_dir(pin, GPIO_OUT))

// 以下是引脚定义
#define LCD_RS 2                    // 数据/指令切换引脚
#define LCD_E 3                     // 触发引脚
#define LCD_D4 11                   // 四位数据引脚
#define LCD_D5 10
#define LCD_D6 9
#define LCD_D7 8
#define LED_LIGHT 6                 // 外接LED提示灯
#define BLA 4                       // 背光PWM控制

#define BOTTEN 13                   // 按钮引脚

// 以下是命令定义
#define CMD 0                       // 命令为0
#define CHR 1                       // 数据为1
#define char_per_line 16            // 每行16个字符
#define LCD_line1 0x80              // 第一行地址
#define LCD_line2 0xc0              // 第二行地址

#define BaudRate 38400              // 串口波特率

const char* ssid = "Xiaomi_3F55";              //此处填入你的WiFi SSID
const char* password = "1973197385";          //此处填入你的WiFi密码
unsigned long currentTime = millis();
const long timeoutTime = 2000;      //连接超时时间2S
String header;
String input = "";
String co="123456789ABCDEF1";
String co2="123456789ABCDEF1";
String IP1="IP:";
char temp[35];
int tail = 0;
int counter=0;
WiFiServer server(13000);              //13000端口设置服务器

//假名映射数组
const int kanaMapSize = 1000;
String kanaMap[kanaMapSize];

using namespace std;

void initKanaMap() {
  // あ行
  kanaMap[354] = "\xb1"; kanaMap[450] = "\xb1";  // あ
  kanaMap[356] = "\xb2"; kanaMap[452] = "\xb2";  // い
  kanaMap[358] = "\xb3"; kanaMap[454] = "\xb3";  // う
  kanaMap[360] = "\xb4"; kanaMap[456] = "\xb4";  // え
  kanaMap[362] = "\xb5"; kanaMap[458] = "\xb5";  // お

  // 小あ行
  kanaMap[353] = "\x7a"; kanaMap[449] = "\x7a";  // ぁ
  kanaMap[355] = "\x7b"; kanaMap[451] = "\x7b";  // ぃ
  kanaMap[357] = "\x7c"; kanaMap[453] = "\x7c";  // ぅ
  kanaMap[359] = "\x7d"; kanaMap[455] = "\x7d";  // ぇ
  kanaMap[361] = "\x7e"; kanaMap[457] = "\x7e";  // ぉ

  // か行
  kanaMap[363] = "\xb6"; kanaMap[459] = "\xb6";  // か
  kanaMap[365] = "\xb7"; kanaMap[461] = "\xb7";  // き
  kanaMap[367] = "\xb8"; kanaMap[463] = "\xb8";  // く
  kanaMap[369] = "\xb9"; kanaMap[465] = "\xb9";  // け
  kanaMap[371] = "\xba"; kanaMap[467] = "\xba";  // こ

  // が行（浊音）
  kanaMap[364] = "\xb6\xde"; kanaMap[460] = "\xb6\xde";  // が
  kanaMap[366] = "\xb7\xde"; kanaMap[462] = "\xb7\xde";  // ぎ
  kanaMap[368] = "\xb8\xde"; kanaMap[464] = "\xb8\xde";  // ぐ
  kanaMap[370] = "\xb9\xde"; kanaMap[466] = "\xb9\xde";  // げ
  kanaMap[372] = "\xba\xde"; kanaMap[468] = "\xba\xde";  // ご

  // さ行
  kanaMap[373] = "\xbb"; kanaMap[469] = "\xbb";  // さ
  kanaMap[375] = "\xbc"; kanaMap[471] = "\xbc";  // し
  kanaMap[377] = "\xbd"; kanaMap[473] = "\xbd";  // す
  kanaMap[379] = "\xbe"; kanaMap[475] = "\xbe";  // せ
  kanaMap[381] = "\xbf"; kanaMap[477] = "\xbf";  // そ

  // ざ行（浊音）
  kanaMap[374] = "\xbb\xde"; kanaMap[470] = "\xbb\xde";  // ざ
  kanaMap[376] = "\xbc\xde"; kanaMap[472] = "\xbc\xde";  // じ
  kanaMap[378] = "\xbd\xde"; kanaMap[474] = "\xbd\xde";  // ず
  kanaMap[380] = "\xbe\xde"; kanaMap[476] = "\xbe\xde";  // ぜ
  kanaMap[382] = "\xbf\xde"; kanaMap[478] = "\xbf\xde";  // ぞ

  // た行
  kanaMap[383] = "\xc0"; kanaMap[479] = "\xc0";  // た
  kanaMap[385] = "\xc1"; kanaMap[481] = "\xc1";  // ち
  kanaMap[387] = "\xaf"; kanaMap[483] = "\xaf";  // っ
  kanaMap[388] = "\xc2"; kanaMap[484] = "\xc2";  // つ
  kanaMap[390] = "\xc3"; kanaMap[486] = "\xc3";  // て
  kanaMap[392] = "\xc4"; kanaMap[488] = "\xc4";  // と

  // だ行（浊音）
  kanaMap[384] = "\xc0\xde"; kanaMap[480] = "\xc0\xde";  // だ
  kanaMap[386] = "\xc1\xde"; kanaMap[482] = "\xc1\xde";  // ぢ
  kanaMap[389] = "\xc2\xde"; kanaMap[485] = "\xc2\xde";  // づ
  kanaMap[391] = "\xc3\xde"; kanaMap[487] = "\xc3\xde";  // で
  kanaMap[393] = "\xc4\xde"; kanaMap[489] = "\xc4\xde";  // ど

  // な行
  kanaMap[394] = "\xc5"; kanaMap[490] = "\xc5";  // な
  kanaMap[395] = "\xc6"; kanaMap[491] = "\xc6";  // に
  kanaMap[396] = "\xc7"; kanaMap[492] = "\xc7";  // ぬ
  kanaMap[397] = "\xc8"; kanaMap[493] = "\xc8";  // ね
  kanaMap[398] = "\xc9"; kanaMap[494] = "\xc9";  // の

  // は行
  kanaMap[399] = "\xca"; kanaMap[495] = "\xca";  // は
  kanaMap[402] = "\xcb"; kanaMap[498] = "\xcb";  // ひ
  kanaMap[405] = "\xcc"; kanaMap[501] = "\xcc";  // ふ
  kanaMap[408] = "\xcd"; kanaMap[504] = "\xcd";  // へ
  kanaMap[411] = "\xce"; kanaMap[507] = "\xce";  // ほ

  // ば行（浊音）
  kanaMap[400] = "\xca\xde"; kanaMap[496] = "\xca\xde";  // ば
  kanaMap[403] = "\xcb\xde"; kanaMap[499] = "\xcb\xde";  // び
  kanaMap[406] = "\xcd\xde"; kanaMap[502] = "\xcc\xde";  // ぶ
  kanaMap[409] = "\xcd\xde"; kanaMap[505] = "\xcd\xde";  // べ
  kanaMap[412] = "\xce\xde"; kanaMap[508] = "\xce\xde";  // ぼ

  // ぱ行（半浊音）
  kanaMap[401] = "\xca\xdf"; kanaMap[497] = "\xca\xdf";  // ぱ
  kanaMap[404] = "\xcb\xdf"; kanaMap[500] = "\xcb\xdf";  // ぴ
  kanaMap[407] = "\xcc\xdf"; kanaMap[503] = "\xcc\xdf";  // ぷ
  kanaMap[410] = "\xcd\xdf"; kanaMap[506] = "\xcd\xdf";  // ぺ
  kanaMap[413] = "\xce\xdf"; kanaMap[509] = "\xce\xdf";  // ぽ

  // ま行
  kanaMap[414] = "\xcf"; kanaMap[510] = "\xcf";  // ま
  kanaMap[415] = "\xd0"; kanaMap[511] = "\xd0";  // み
  kanaMap[416] = "\xd1"; kanaMap[512] = "\xd1";  // む
  kanaMap[417] = "\xd2"; kanaMap[513] = "\xd2";  // め
  kanaMap[418] = "\xd3"; kanaMap[514] = "\xd3";  // も

  // や行
  kanaMap[420] = "\xd4"; kanaMap[516] = "\xd4";  // や
  kanaMap[422] = "\xd5"; kanaMap[518] = "\xd5";  // ゆ
  kanaMap[424] = "\xd6"; kanaMap[520] = "\xd6";  // よ

  // 小や行
  kanaMap[419] = "\xac"; kanaMap[515] = "\xac";  // ゃ
  kanaMap[421] = "\xad"; kanaMap[517] = "\xad";  // ゅ
  kanaMap[423] = "\xae"; kanaMap[519] = "\xae";  // ょ

  // ら行
  kanaMap[425] = "\xd7"; kanaMap[521] = "\xd7";  // ら
  kanaMap[426] = "\xd8"; kanaMap[522] = "\xd8";  // り
  kanaMap[427] = "\xd9"; kanaMap[523] = "\xd9";  // る
  kanaMap[428] = "\xda"; kanaMap[524] = "\xda";  // れ
  kanaMap[429] = "\xdb"; kanaMap[535] = "\xdb";  // ろ

  // わ行・ん
  kanaMap[431] = "\xdc"; kanaMap[527] = "\xdd";  // わ
  kanaMap[434] = "\xa6"; kanaMap[530] = "\xde";  // を
  kanaMap[435] = "\xdd"; kanaMap[531] = "\xdf";  // ん

  // 标点
  kanaMap[540] = "\xb0"; // ー
  kanaMap[290] = "\xa1"; // 。
  kanaMap[300] = "\xa2"; // 「
  kanaMap[301] = "\xa3"; // 」
  kanaMap[289] = "\xa4"; // 、
  kanaMap[539] = "\xa5"; // ・
}

void init(){                        //初始化引脚
    setOutput(LCD_RS);
    setOutput(LCD_E);
    setOutput(LCD_D4);
    setOutput(LCD_D5);
    setOutput(LCD_D6);
    setOutput(LCD_D7);
    setOutput(LED_LIGHT);
    pinMode(BOTTEN, INPUT);

    analogWrite(BLA, 255);
}

void triggle_E(){                     //触发E引脚
    sleep_us(15);
    setHigh(LCD_E);
    sleep_us(15);
    setLow(LCD_E);
    sleep_us(15);
}

void gpio_write(int data,int mode){   //写入一个数据或指令

    //设定模式(指令/字符)
    if (mode == 0) {
        setLow(LCD_RS);
    } else {
        setHigh(LCD_RS);
    }

    //前四位
    if (data & 0x80) setHigh(LCD_D7); else setLow(LCD_D7);
    if (data & 0x40) setHigh(LCD_D6); else setLow(LCD_D6);
    if (data & 0x20) setHigh(LCD_D5); else setLow(LCD_D5);
    if (data & 0x10) setHigh(LCD_D4); else setLow(LCD_D4);
    triggle_E();

    //后四位
    if (data & 0x08) setHigh(LCD_D7); else setLow(LCD_D7);
    if (data & 0x04) setHigh(LCD_D6); else setLow(LCD_D6);
    if (data & 0x02) setHigh(LCD_D5); else setLow(LCD_D5);
    if (data & 0x01) setHigh(LCD_D4); else setLow(LCD_D4);
    triggle_E();
}

void lcd_init(){                        //初始化1602函数
    gpio_write(0x33,CMD);               //
    delay(50);
    gpio_write(0x32,CMD);               //
    delay(50);
    gpio_write(0x06,CMD);               //设置向右写入字符，设置屏幕内容不滚动
    delay(50);
    gpio_write(0x0C,CMD);               //开启屏幕显示，关闭光标显示，关闭光标闪烁
    delay(50);
    gpio_write(0x28,CMD);               //设定数据总线为四位，显示2行字符，使用5*8字符点阵
    delay(50);
    gpio_write(0x01,CMD);               //清屏
    delay(50);
}

void lcd_text(String ltext,int line){   //显示函数
    gpio_write(line,CMD);               //命令写入:行数
    int tsize=ltext.length();
    for(int size=0;size<16;size++){     //逐字写入
        if(size>tsize-1){
            gpio_write(0x20,CHR);       //若字符串大小小于16，则填充空格
        }
        else{
            gpio_write(int(ltext[size]),CHR); //转换成RAW编码后写入
        }
    }
}

//设置光标位置
void lcd_setCursor(int col, int row){
    //Serial.print("set cursor in "+String(col)+","+String(row) + " ");
    int addr = 0;
    if (row == 0) {
        addr = 0x00 + col;
    } 
    else if (row == 1) {
        addr = 0x40 + col;
    } 
    else {
        return; // 无效行
    }

    int command = 0x80 | addr;  // 设置 DDRAM 地址命令
    gpio_write(command, CMD);
}

// 光标向后移动一格
int lcdCursor = 0;  // 当前光标位置，全局变量 0~31
void lcd_next_cousor(){
    lcdCursor++;
    if (lcdCursor >= 32) {
        lcd_setCursor(17, 0);   //将光标设置为显示区域外
    }

    int row = (lcdCursor < 16) ? 0 : 1;
    int col = lcdCursor % 16;
    lcd_setCursor(col, row);
}

// 写入一字节的自定义字符
void lcd_createChar(int slot, uint8_t data[8]){
    if (slot < 0 || slot > 7) return;  // 限制 slot 范围

    int cgram_addr = 0x40 | (slot << 3);  // CGRAM 写入起始地址
    gpio_write(cgram_addr, CMD);  // 设置 CGRAM 地址（指令模式）

    // 连续写入 8 字节点阵数据
    for (int i = 0; i < 8; i++) {
        gpio_write(data[i], CHR);  // 字符数据（数据模式）
    }

    // 设置回 DDRAM 当前光标对应位置
    int row = (lcdCursor < 16) ? 0 : 1;
    int col = lcdCursor % 16;
    lcd_setCursor(col, row);  // 将光标设置回当前显示位置
}

// 显示自定义字符
void lcd_dis_costom(int index){
    if (index < 0 || index > 7) return; // 只能是 0~7 槽
        gpio_write(index, CHR);  // 写入字符数据（模式 1 表示数据模式）
}

// 显示普通字符
void lcd_dis_chr(char text){   //显示函数
        gpio_write(int(text),CHR); //直接写入
        lcd_next_cousor();
}

// 清屏
void lcd_clear() {
    gpio_write(0x01, CMD);  // 发送清屏指令
    delay(2);               // 清屏命令需要较长时间等待
    lcdCursor = 0;          // 重置光标位置
    lcd_setCursor(0, 0);    // 光标返回起始位置
}

// 假名unicode转换
int utf8ToUnicode(uint8_t c0, uint8_t c1, uint8_t c2) {
  return ((c0 & 0x0F) << 12) | ((c1 & 0x3F) << 6) | (c2 & 0x3F);
}

// 显示整段的普通字符
void lcd_print(String s) {
    for (unsigned int i = 0; i < s.length(); i++) {
        lcd_dis_chr(s[i]);
    }
}

// 解码函数
void prosessIncoming(const uint8_t* raw, unsigned int bodyLen) {
    Serial.print("processing start:");
    Serial.println(millis());

    // Serial.print("Raw bytes: ");
    // for (unsigned int i = 0; i < bodyLen; i++) {
    //     Serial.print("0x");
    //     Serial.print(raw[i], HEX);
    //     Serial.print(" ");
    // }
    // Serial.println();

    if (bodyLen < 3) {
        Serial.println("数据包太短，无法解析");
        return;
    }

    // 检查协议头
    if (raw[0] != 0xAA || raw[1] != 0x55) {
        Serial.println("无效数据头");
        return;
    }

    // 初始化显示
    lcd_setCursor(0, 0);    // 回到屏幕起点
    lcdCursor = 0;          // 重置全局光标

    unsigned int i = 3; // 从数据体开始
    uint8_t customCharIndex = 0; // 自定义字符编号（0~7）
    String charBuffer = "";  // 用于批量显示普通字符

    while (i < bodyLen && lcdCursor < 32) {
        uint8_t flag = raw[i++];
        if (flag == 0x00) {
            if (i >= bodyLen) break;
            char c = raw[i++];
            if ((uint8_t)c != 0xE3) {
                // 普通 ASCII 字符
                charBuffer += c;
            } 
            else if((uint8_t)c == 0xE3){
                bool unknowKana = false;
                if (i + 1 >= bodyLen) break;
                char c1 = raw[++i];
                char c2 = raw[i+=2];
                int key = utf8ToUnicode(c, c1, c2) - 12000;
                Serial.println(key);
                Serial.println(kanaMap[key]);

                if(kanaMap[key] == ""){   // 未定义假名，显示空格
                    unknowKana = true;
                    charBuffer += " ";
                }
                if(!unknowKana){
                    String kana = kanaMap[key];
                    charBuffer += kana[0];
                    if ((uint8_t)kana[1] == 222) {
                        charBuffer += kana[1];
                    }
                }
                i++;
            }
            else{   // 未定义utf-8字符，显示空格
                charBuffer += " ";
            }
        } 

        // 自定义字符
        else if (flag == 0x01) {
            // 输出缓存中的普通字符
            if (charBuffer.length() > 0) {
                //uint32_t t0_dis_char = millis();
                lcd_print(charBuffer);
                charBuffer = "";
                //Serial.print("显示buffer耗时: ");
                //Serial.println(millis() - t0_dis_char);
            }

            if (i + 7 >= bodyLen) {
                Serial.println("自定义字符数据不足");
                break;
            }

            uint8_t charMap[8]; // 8行点阵
            for (int j = 0; j < 8; j++) {
                charMap[j] = raw[i++];
            }
            
            // 显示点阵
            lcd_createChar(customCharIndex, charMap);
            lcd_dis_costom(customCharIndex);
            lcd_next_cousor();

            // 循环使用 slot
            customCharIndex = (customCharIndex + 1) % 8;
        } 

        else {
            Serial.print("未知标志: ");
            Serial.println(flag, HEX);
            break;
        }
    }

    // 输出剩余字符
    if (charBuffer.length() > 0) {
        lcd_print(charBuffer);
    }

    // 填充空格至满屏
    while (lcdCursor < 32) {
        lcd_dis_chr(' ');
    }

    Serial.print("prossing end:");
    Serial.println(millis());
}

void handleSocketClient() {
    WiFiClient client = server.accept();
    if (!client) return;

    unsigned long startTime = millis();
    std::vector<uint8_t> recvBuffer;

    Serial.print("\nTime:");
    Serial.print(millis());
    Serial.println(" Socket Client connected.");

    while (client.connected()) {
        if (millis() - startTime > timeoutTime) {   // 超时关闭连接
            Serial.println("\nTimeout, closing client.");
            break;
        }

        if (client.available() > 0) {
            uint8_t buf[256];
            int len = client.read(buf, sizeof(buf));
            recvBuffer.insert(recvBuffer.end(), buf, buf + len);
            startTime = millis();  // 重置超时
        } else {
            delay(5);   //继续等待数据
            continue;
        }

        // 尝试从缓冲区中解析完整包
        while (recvBuffer.size() >= 3) {
            // 检查包头
            if (recvBuffer[0] == 0xAA && recvBuffer[1] == 0x55) {
                uint8_t bodyLen = recvBuffer[2];
                unsigned int fullLen = bodyLen;

                if (recvBuffer.size() >= fullLen) {
                    Serial.print("Time:");
                    Serial.print(millis());
                    Serial.println(" received full pack");

                    // 拷贝并处理完整包
                    std::vector<uint8_t> fullPacket(recvBuffer.begin(), recvBuffer.begin() + fullLen);
                    recvBuffer.erase(recvBuffer.begin(), recvBuffer.begin() + fullLen);

                    //Serial.println("== 开始处理 ==");
                    uint32_t t0 = millis();
                    prosessIncoming(fullPacket.data(), fullLen);
                    Serial.print("总处理耗时: ");
                    Serial.println(millis() - t0);

                    //Serial.println("发送ACK");
                    client.write("OK\n");
                } else {
                    // 包不完整，等待更多数据
                    break;
                }
            } else {
                // 非协议头，丢弃一个字节
                recvBuffer.erase(recvBuffer.begin());
            }
        }
    }

    Serial.print("Time:");
    Serial.print(millis());
    Serial.println(" Client disconnected.");
}

void setup() {
    Serial.begin(BaudRate);
    init();
    initKanaMap();  // 初始化假名表
    lcd_init();

    // 欢迎消息
    lcd_text("RaspberryPI PICO",LCD_line1);
    lcd_text("1602a WIFI Test",LCD_line2);

    // 闪灯
    pinMode(LED_BUILTIN, OUTPUT);
    digitalWrite(LED_BUILTIN, HIGH);
    delay(1000);
    digitalWrite(LED_BUILTIN, LOW);

    // 在屏幕上显示状态
    lcd_text("WIFI connecting",LCD_line1);
    lcd_text(" ",LCD_line2);
    Serial.print("RaspberryPI pico WH");
    WiFi.begin(ssid,password);
  
    // 连接WiFi
    while (WiFi.status() != WL_CONNECTED) {
        lcd_text("WIFI connecting",LCD_line1);
        lcd_text(" ",LCD_line2);
        WiFi.begin(ssid,password);
        
        Serial.print(".");
    }

    // 串口调试信息
    Serial.print("WiFi connected at IP Address ");
    Serial.println(WiFi.localIP());
    
    // 在屏幕上显示ip
    String IP = WiFi.localIP().toString().c_str();
    lcd_text("WIFI connected",LCD_line1);
    IP1.concat(IP);
    lcd_text(IP1,LCD_line2);

    server.begin();
}

void loop(){
    digitalWrite(LED_BUILTIN, LOW);

    handleSocketClient();
}
