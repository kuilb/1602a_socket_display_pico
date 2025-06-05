# 1602a_socket_display_pico
Fork自之前的http协议(web)控制项目，将协议改为socket来优化性能  
使用树莓派pico WH + 1602a显示屏  
使用arduino IDE编写  

v1.0 一帧一个连接，效率低，但是先发着先  
v1.1 一个连接可以发送多个包  
v1.2 优化了解析包和假名的性能  
v1.3 修复了几乎所有的问题  
v1.4 修复了协议解析的问题,优化性能  
v1.5 添加了配网功能，给帧传输加上了缓存，添加了一个四向按键并更新了通信协议API，通过心跳包现在支持长长长长长连接了，更新了对应的按键监听器  

## 协议结构:  
协议字节流格式如下：

|字段|长度|说明|
|---|---|---|
|Header|2 字节|固定字节 0xAA 0x55|
|Length|1 字节|整帧长度（包含 Header、Length、FrameInterval 和 Payload）|
|FrameInterval|2 字节|帧间隔毫秒数（无符号 16 位整数，范围 0~65535 ms）|
|Payload|N 字节|数据体（支持普通字符和自定义字符格式）|
### Payload 数据格式

- **普通字符格式（每个字符）**：
    每个字符由若干字节组成，每个字节前面加 0x00，格式为：
    0x00 + UTF-8 编码字节（1~3字节，每字节前加 0x00）

- 自定义字符格式（每个字符）**：
    每个字符由 9 字节组成：
    0x01 + 8 字节点阵数据（每行低 5 位有效）

传入的字符数据按照顺序从左到右从上到下显示  


## API用法  
#### 类 PicoLink — Pico 网络通信管理器
##### 负责与 Pico 设备建立 TCP 连接、发送数据以及监听按键事件，同时维护心跳包保持连接活跃：

|方法签名|说明|
|---|---|
|`public PicoLink(String ip, int port)`|构造函数，初始化 PicoLink，设置目标设备 IP 和端口。|
|`public void setButtonListener(ButtonListener listener)`|设置按钮事件监听器，接收来自 Pico 的按键事件回调。|
|`public void connect() throws IOException`|建立与 Pico 设备的 TCP 连接，并启动监听线程和心跳线程。|
|`public void close()`|关闭连接，停止监听和心跳线程，释放资源。|
|`public void send(byte[] data) throws IOException`|向 Pico 设备发送字节数据。|
|`public void run()`|监听线程执行体，持续读取来自 Pico 的数据，检测按键事件。|


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

| 方法签名                                                          | 说明                                                                            |
| ------------------------------------------------------------- | ----------------------------------------------------------------------------- |
| `public static byte[] build(frameInterval， Object... inputs)` | 接收帧率(毫秒) + 任意数量的 `NormalChar`、`CustomChar`、数组或混合形式，自动组装协议头与数据体。最多支持 8 个自定义字符。 |

### 注意事项
第一次使用会进入配网模式，只支持2.4GWiFi  
连接SSID:PicoW_Config  
打开浏览器连接屏幕显示的IP，输入Wifi的SSID和密码  

开机时按住中心按键也可以进入配网模式  

请在代码头的define处修改GPIO序号  

### 配置方法
arduino-首选项-附加开发板管理器网址填入  
https://github.com/earlephilhower/arduino-pico/releases/download/global/package_rp2040_index.json

前往开发板管理器下载pico_sdk  
烧录前在 工具-Flash Size 选择64KB文件系统  
编译烧录  