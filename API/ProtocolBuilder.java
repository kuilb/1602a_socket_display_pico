import java.util.ArrayList;
import java.util.List;

public class ProtocolBuilder {
    public static byte[] build(Object... inputs) {
    List<Byte> result = new ArrayList<>();

    // 协议头
    result.add((byte) 0xAA);
    result.add((byte) 0x55);

    List<Byte> body = new ArrayList<>();

    for (Object input : inputs) {
        if (input instanceof CharPacket) {
            for (byte b : ((CharPacket) input).toBytes()) {
                body.add(b);
            }
        } else if (input instanceof CharPacket[]) {
            for (CharPacket cp : (CharPacket[]) input) {
                for (byte b : cp.toBytes()) {
                    body.add(b);
                }
            }
        } else {
            throw new IllegalArgumentException("不支持的类型: " + input.getClass());
        }
    }

    // 添加数据体长度
    int fullSize;
    fullSize=body.size() + 3;
    result.add((byte) fullSize);

    // 添加数据体
    result.addAll(body);

    // 转换为 byte[]
    byte[] packet = new byte[result.size()];
    for (int i = 0; i < result.size(); i++) {
        packet[i] = result.get(i);
    }
    return packet;
}

}
