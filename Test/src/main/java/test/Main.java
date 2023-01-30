package test;

import system.jni.JavaFXUtils;

import java.io.File;
import java.nio.charset.StandardCharsets;

public class Main {
    //这只是一个简答的使用示例
    public static void main(String[] args) {
        //调用方法初始化native
        //传递的路径参数是释放dll与加载的文件夹，不写则为当前目录下
        JavaFXUtils.init(new File("lib"));
        /*
         * 获取一个javafx Image对象, 保存的图片为指定的文件图标
         * 类像素值为1000（1000会十分的清晰，但对性能要求较高，一般情况下使用默认就行）
         * 编解码器为：StandardCharsets.UTF_8(乱码的时候不妨试试修改这个为GBK有神效)
         *
         * 返回null代表没获取成功，这一般就是由于文件中包含难以转义的符号导致的
         */
        JavaFXUtils.getIconImageFX(new File("C:\\Windows\\System32\\cmd.exe"), 1000, StandardCharsets.UTF_8);
        /*
         * 返回值为png格式的字节数组
         */
        JavaFXUtils.getIconImageBytes(new File("C:\\Windows\\System32\\cmd.exe"), 1000, StandardCharsets.UTF_8);
    }
}
