import java.io.IOException;
import java.util.Arrays;

public class SimpleDemo {
    public static void main(String[] args) {
        // ✅ 配置目标 Pico 的 IP 与端口
        String ip = "192.168.8.125";  
        int port = 13000;

        // ✅ 初始化发送器，负责建立连接并发送数据
        Sender sender = new Sender(ip, port);

        // ✅ 将字符串转换为普通字符数组（支持假名、符号等）
        NormalChar[] normalchar = NormalChar.fromString("いじょうなし ");

        // ✅ 创建单个自定义字符（8 字节，每字节低 5 位有效）
        byte[] customData = new byte[] {
            0x1F, 0x04, 0x04, 0x07, 0x14, 0x14, 0x14, 0x1F
        };
        CustomChar singleCustom = new CustomChar(customData);

        // ✅ 批量构造多个自定义字符（数组长度必须是 8 的倍数）
        byte[] customArray = new byte[] {
            0x1f,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
            0x00,0x1f,0x00,0x00,0x00,0x00,0x00,0x00,
            0x00,0x00,0x1f,0x00,0x00,0x00,0x00,0x00,
            0x00,0x00,0x00,0x1f,0x00,0x00,0x00,0x00,
            0x00,0x00,0x00,0x00,0x1f,0x00,0x00,0x00,
            0x00,0x00,0x00,0x00,0x00,0x1f,0x00,0x00,
            0x00,0x00,0x00,0x00,0x00,0x00,0x1f,0x00,
            0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x1f,
        };

        // ✅ 将 customArray 切片为两个 CustomChar 数组，每段代表 4 个字符
        CustomChar[] cusArr0 = CustomChar.fromByteArray(Arrays.copyOfRange(customArray, 0, 32));
        CustomChar[] cusArr1 = CustomChar.fromByteArray(Arrays.copyOfRange(customArray, 32, 64));

        try {
            sender.connect(); // 连接至 Pico

            // ✅ 构建并发送数据（支持混合多个 NormalChar 和 CustomChar）
            sender.send(ProtocolBuilder.build(
                cusArr0,
                // NormalChar.fromString("     いじょうなし"),
                NormalChar.fromString("ぁぃぅぇぉ"),
                cusArr1
            ));

            Thread.sleep(1000); // 可自行替换，但是要保证连接时长不能太短
        } catch (IOException | InterruptedException e) {
            e.printStackTrace(); 
        } finally {
            sender.close(); // 释放连接资源
        }
    }
}
