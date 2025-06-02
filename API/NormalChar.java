import java.io.UnsupportedEncodingException;
import java.util.ArrayList;
import java.util.List;

public class NormalChar implements CharPacket {
    private final byte[] utf8Bytes;

    private NormalChar(byte[] utf8Bytes) {
        this.utf8Bytes = utf8Bytes;
    }

    @Override
    public byte[] toBytes() {
        byte[] result = new byte[1 + utf8Bytes.length];
        result[0] = 0x00; // 标志位表示普通字符
        System.arraycopy(utf8Bytes, 0, result, 1, utf8Bytes.length);
        return result;
    }

    public static NormalChar[] fromString(String str) {
        List<NormalChar> result = new ArrayList<>();
        try {
        byte[] utf8 = str.getBytes("UTF-8");
            for (byte b : utf8) {
                result.add(new NormalChar(new byte[]{b}));
            }
        } catch (UnsupportedEncodingException e) {
            e.printStackTrace();
        }

        return result.toArray(new NormalChar[0]);
    }

}
