import java.io.OutputStream;
import java.io.IOException;
import java.net.Socket;

public class Sender {
    private final String ip;
    private final int port;
    private Socket socket;
    private OutputStream out;

    public Sender(String ip, int port){
        this.ip = ip;
        this.port = port;
    }
    
    // 建立连接
    public void connect() throws IOException {
        if (socket == null || socket.isClosed()) {
            socket = new Socket(ip, port);
            socket.setSoTimeout(3000); // 可选超时设置
            out = socket.getOutputStream();
            System.out.println("已连接至pico " + ip + ":" + port);
        }
    }

    // 关闭连接
    public void close() {
        try {
            if (out != null) out.close();
            if (socket != null) socket.close();
            System.out.println("已断开连接");
        } catch (IOException e) {
            e.printStackTrace();
        }
    }

    // 发送数据
    public void send(byte[] data) throws IOException {
        if (socket == null || socket.isClosed()) {
            throw new IOException("未连接上pico");
        }
        
        out.write(data);
        out.flush();
    }
}
