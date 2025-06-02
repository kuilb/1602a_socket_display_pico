public class CustomCharBuilder {

    /**
     * 将 8 行、每行 5 位的二进制数字（0/1）转换为 8 字节数组
     */
    public static byte[] build(int[][] rows) {
        byte[] result = new byte[8];

        for (int i = 0; i < 8; i++) {
            int value = 0;
            for (int j = 0; j < 5; j++) {
                value |= (rows[i][j] << (4 - j)); // 高位在左
            }
            result[i] = (byte) (value & 0x1F); // 仅保留低5位
        }

        return result;
    }

    public static void main(String[] args) {
        // 定义你想要的 5x8 点阵图案
        int[][] sampleChar = {
            {1, 0, 1, 0, 1}, 
            {1, 1, 1, 1, 1}, 
            {0, 1, 1, 1, 0}, 
            {0, 1, 0, 1, 0}, 
            {0, 1, 1, 1, 0},  
            {0, 0, 1, 0, 0},  
            {1, 1, 1, 1, 1},  
            {1, 0, 1, 0, 1}  
        };

        byte[] data = build(sampleChar);

        // 输出为十六进制
        for (byte b : data) {
            System.out.printf("0x%02X, ", b);
        }
    }
}
