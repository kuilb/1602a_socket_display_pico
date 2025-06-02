# 1602a_socket_display_pico
Fork自之前的http协议(web)控制项目，将协议改为socket来优化性能  
使用树莓派pico WH + 1602a显示屏  
使用arduino IDE编写  

v1.0 一帧一个连接，效率低，但是先发着先  
v1.1 一个连接可以发送多个包  
v1.2 优化了解析包和假名的性能  
v1.3 修复了几乎所有的问题  
v1.4 修复了协议解析的问题,优化性能  

## 协议结构:  
协议字节流格式如下：

[Header]    0xAA 0x55  
[Length]    数据总长度（含头部）  
[Payload]   数据体（支持 0x00 普通字符 或 0x01 自定义字符）  

##### 普通字符格式（每个字符）：
0x00 + UTF-8 编码字节（1~3字节，每字节前加 0x00）
#### 自定义字符格式（每个字符）：
0x01 + 8 字节点阵（每行低 5 位有效）

传入的字符数据按照顺序从左到右从上到下显示  


## API用法  
#### 类Sender — 网络通信发送器 
##### 负责与 Pico 建立连接并发送数据：

| 方法签名                            | 说明                 |
| ------------------------------- | ------------------ |
| `public void connect()`         | 建立与 Pico 的 TCP 连接。 |
| `public void close()`           | 关闭当前连接。            |
| `public void send(byte[] data)` | 将字节流数据发送至 Pico 设备。 |


#### 类NormalChar implements CharPacket — 普通字符封装  
##### 用于构建普通字符协议数据 (如英文、符号、日文假名等):  

| 方法签名                                                | 说明                                  |
| --------------------------------------------------- | ----------------------------------- |
| `public byte[] toBytes()`                           | 转换为协议字节格式。                          |
| `public static NormalChar[] fromString(String str)` | 将字符串拆分为 `NormalChar[]`，支持 UTF-8 假名。 |

#### 类CustomChar implements CharPacket — 自定义字符封装  
##### 用于构建 LCD 显示器的点阵字符 (最多 8 个，每个为 5×8 格):

| 方法签名                                                   | 说明                                             |
| ------------------------------------------------------ | ---------------------------------------------- |
| `public CustomChar(byte[] dotMatrix)`                  | 以 8 字节数组构造单个自定义字符（每行低 5 位有效）。                  |
| `public byte[] toBytes()`                              | 转换为协议字节格式。                                     |
| `public static CustomChar[] fromByteArray(byte[] raw)` | 从长度为 `8×N` 的原始数据生成 `N` 个 `CustomChar`（支持任意倍数）。 |

#### 类ProtocolBuilder — 协议构造器  
##### 用于组合并封装字符数据成完整协议帧：

|方法签名|说明|
|---|---|
|`public static byte[] build(Object... inputs)`|接收任意数量的 `NormalChar`、`CustomChar`、数组或混合形式，自动组装协议头与数据体。最多支持 8 个自定义字符。|

### 注意事项
使用前请自定义WiFi ssid和密码，只支持2.4GWiFi  
(const char* ssid = "YOUR_SSID";  
const char* password = "YOUR_PASSWORD";)

请在代码头的define处修改GPIO序号  
按钮默认GPIO16，可自行修改

1602a的V0和RW接地

### 配置方法
1.使用最新版arduino的内置下载器选择raspberry PI pico WH下载sdk编译  

2.arduino-首选项-附加开发板管理器网址填入  
https://github.com/earlephilhower/arduino-pico/releases/download/global/package_rp2040_index.json

前往开发板管理器下载pico_sdk  
编译烧录