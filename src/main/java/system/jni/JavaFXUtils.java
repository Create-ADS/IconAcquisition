package system.jni;

import javafx.scene.image.Image;

import java.io.ByteArrayInputStream;
import java.io.File;
import java.io.FileOutputStream;
import java.io.InputStream;
import java.nio.charset.Charset;

public class JavaFXUtils {
    /**
     * In the current folder DLL and load release.
     */
    public static void init(){
        init(new File("./"));
    }

    /**
     * Will be released into the specified folder DLL loading.
     * @param dir folder
     */
    public static void init(File dir){
        if (dir.isFile()){
            throw new RuntimeException("Cannot be a file!");
        }
        if (!System.getProperty("os.name").toLowerCase().contains("windows")){
            throw new RuntimeException("This system is not windows!");
        }
        InputStream inputStream;
        if (System.getProperty("os.arch").toLowerCase().contains("64")){
            inputStream = JavaFXUtils.class.getClassLoader().getResourceAsStream("system/jni/resources/javafx_utils_windows_x64.dll");
            if (inputStream == null) {
                throw new RuntimeException("javafx_utils_windows_x64.dll is not found in jar");
            }
        }else {
            inputStream = JavaFXUtils.class.getClassLoader().getResourceAsStream("system/jni/resources/javafx_utils_windows_x86.dll");
            if (inputStream == null) {
                throw new RuntimeException("javafx_utils_windows_x86.dll is not found in jar");
            }
        }
        dir.mkdirs();
        File file = new File(dir, "javafx_utils_windows.dll");
        if (file.exists()){
            return;
        }
        try {
            FileOutputStream fileOutputStream = new FileOutputStream(file);
            byte[] buffer = new byte[1024];
            int len;
            while ((len = inputStream.read(buffer)) != -1) {
                fileOutputStream.write(buffer, 0, len);
            }
            fileOutputStream.close();
            inputStream.close();
        } catch (java.io.IOException e) {
            throw new RuntimeException("javafx_utils_windows.dll is not found in jar");
        }
        System.load(file.getAbsolutePath());
    }

    /**
     * The default character set.
     */
    private static final Charset DEFAULT_CHARSET = Charset.defaultCharset();

    public static Image getIconImageFX(File file){
        if (file == null) {
            throw new RuntimeException();
        }
        byte[] png = getIconForPng(file.getAbsoluteFile().getAbsolutePath().getBytes(DEFAULT_CHARSET), 256);
        if (png == null){
            return null;
        }
        return new Image(new ByteArrayInputStream(png));
    }

    public static Image getIconImageFX(File file, Charset charset){
        if (file == null || charset == null) {
            throw new RuntimeException();
        }
        byte[] png = getIconForPng(file.getAbsoluteFile().getAbsolutePath().getBytes(charset), 256);
        if (png == null){
            return null;
        }
        return new Image(new ByteArrayInputStream(png));
    }

    public static Image getIconImageFX(File file, int pixel){
        if (file == null) {
            throw new RuntimeException();
        }
        byte[] png = getIconForPng(file.getAbsoluteFile().getAbsolutePath().getBytes(DEFAULT_CHARSET), pixel);
        if (png == null){
            return null;
        }
        return new Image(new ByteArrayInputStream(png));
    }

    public static Image getIconImageFX(File file, int pixel, Charset charset){
        if (file == null || charset == null) {
            throw new RuntimeException();
        }
        byte[] png = getIconForPng(file.getAbsoluteFile().getAbsolutePath().getBytes(charset), pixel);
        if (png == null){
            return null;
        }
        return new Image(new ByteArrayInputStream(png));
    }

    public static byte[] getIconImageBytes(File file){
        if (file == null) {
            throw new RuntimeException();
        }
        return getIconForPng(file.getAbsoluteFile().getAbsolutePath().getBytes(DEFAULT_CHARSET), 256);
    }

    public static byte[] getIconImageBytes(File file, Charset charset){
        if (file == null || charset == null) {
            throw new RuntimeException();
        }
        return getIconForPng(file.getAbsoluteFile().getAbsolutePath().getBytes(charset), 256);
    }

    public static byte[] getIconImageBytes(File file, int pixel){
        if (file == null) {
            throw new RuntimeException();
        }
        return getIconForPng(file.getAbsoluteFile().getAbsolutePath().getBytes(DEFAULT_CHARSET), pixel);
    }

    public static byte[] getIconImageBytes(File file, int pixel, Charset charset){
        if (file == null || charset == null) {
            throw new RuntimeException();
        }
        return getIconForPng(file.getAbsoluteFile().getAbsolutePath().getBytes(charset), pixel);
    }

    private static native byte[] getIconForPng(byte[] bytes,int pixel);
}
