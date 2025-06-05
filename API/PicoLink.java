import java.io.OutputStream;
import java.io.IOException;
import java.io.InputStream;
import java.net.Socket;
import java.util.concurrent.atomic.AtomicBoolean;

public class PicoLink implements Runnable{
    private final String ip;
    private final int port;
    private Socket socket;
    private OutputStream out;
    private InputStream in;
    private Thread listenerThread;
    private Thread heartbeatThread;
    private final AtomicBoolean running = new AtomicBoolean(false);
    
    // 接口：按钮事件回调
    public interface ButtonListener {
        void onButtonPressed(String buttonName);
    }

    private ButtonListener buttonListener;

    public PicoLink(String ip, int port){
        this.ip = ip;
        this.port = port;
    }

    // 设置按钮监听器
    public void setButtonListener(ButtonListener listener) {
        this.buttonListener = listener;
    }
    
    // 建立连接
    public void connect() throws IOException {
        if (socket == null || socket.isClosed()) {
            socket = new Socket(ip, port);
            socket.setSoTimeout(0);     // 不超时，持续监听
            out = socket.getOutputStream();
            in = socket.getInputStream();
            running.set(true);
            listenerThread = new Thread((Runnable) this);
            listenerThread.start();

            startHeartbeat();
            System.out.println("[PicoLink] 已连接至 pico " + ip + ":" + port);
        }
    }

    // 关闭连接
    public void close() {
        running.set(false); // 先停止线程循环

        try {
            if (heartbeatThread != null && heartbeatThread.isAlive()) {
                heartbeatThread.interrupt();
                heartbeatThread.join();
            }

            if (listenerThread != null && listenerThread.isAlive()) {
                listenerThread.interrupt();
                listenerThread.join();
            }

            // 释放资源
            if (out != null) out.close();
            if (in != null) in.close();
            if (socket != null) socket.close();

            System.out.println("[PicoLink] 已断开连接");
        } catch (IOException | InterruptedException e) {
            e.printStackTrace();
        }
    }

    // 发送数据
    public void send(byte[] data) throws IOException {
        if (socket == null || socket.isClosed()) {
            throw new IOException("未连接pico");
        }
        
        out.write(data);
        out.flush();
    }

    // 监听按键数据
    @Override
    public void run() {
        byte[] buffer = new byte[64];
        try {
            System.out.println("[PicoLink] 正在监听按键...");
            while (running.get()) {
                if (in.available() > 0) {
                    int len = in.read(buffer);
                    if (len > 0) {
                        String msg = new String(buffer, 0, len).trim();
                        if (msg.startsWith("KEY:")) {
                            String key = msg.substring(4);
                            //System.out.println("[PicoLink] 按键: " + key);
                            if (buttonListener != null) {
                                buttonListener.onButtonPressed(key);  // 触发回调
                            }
                        } else {
                            //System.out.println("[PicoLink] Received: " + msg);
                        }
                    }
                }
                Thread.sleep(10); // 减少CPU占用
            }
        } catch (Exception e) {
            if (running.get()) {
                System.err.println("[PicoLink] 按键监听器错误: " + e.getMessage());
            }
        }
    }

    // 启动心跳线程
    private void startHeartbeat() {
        final byte[] heartbeat = new byte[]{(byte) 0xAA, 0x55, 0x04, 0x02};

        heartbeatThread = new Thread(() -> {
            while (running.get()) {
                try {
                    send(heartbeat);
                    Thread.sleep(2000); // 每2秒发一次
                } catch (IOException e) {
                    System.err.println("[PicoLink] 无法发送Heartbeat: " + e.getMessage());
                    break;
                } catch (InterruptedException e) {
                    break;
                }
            }
        });
        heartbeatThread.start();
    }
}
