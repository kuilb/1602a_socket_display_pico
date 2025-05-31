# 1602a_socket_display_pico
Fork自之前的http协议(web)控制项目，将协议改为socket来优化性能  
使用树莓派pico WH + 1602a显示屏  
使用arduino IDE编写  

v1.0 一帧一个连接，效率低，但是先发着先  
v1.1 一个连接可以发送多个包

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

### 使用方法
