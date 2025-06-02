public class CustomChar implements CharPacket {
    private final byte[] dotMatrix;

    public CustomChar(byte[] dotMatrix) {
        if (dotMatrix == null || dotMatrix.length != 8) {
            throw new IllegalArgumentException("自定义字符需要有8字节,当前只有" + dotMatrix.length);
        }

        this.dotMatrix = new byte[8];
        for (int i = 0; i < 8; i++) {
            // 保留低5位（每行最多5个点）
            this.dotMatrix[i] = (byte) (dotMatrix[i] & 0x1F);
        }
    }

    @Override
    public byte[] toBytes() {
        byte[] result = new byte[9];
        result[0] = 0x01; // 自定义字符标识
        System.arraycopy(dotMatrix, 0, result, 1, 8);
        return result;
    }

    public static CustomChar[] fromByteArray(byte[] raw) {
        if (raw == null || raw.length % 8 != 0) {
            throw new IllegalArgumentException("输入字节数必须是8的倍数! 当前字节数" + raw.length);
        }

        int count = raw.length / 8;
        CustomChar[] chars = new CustomChar[count];

        for (int i = 0; i < count; i++) {
            byte[] slice = new byte[8];
            System.arraycopy(raw, i * 8, slice, 0, 8);
            chars[i] = new CustomChar(slice);
        }

        return chars;
    }
}
