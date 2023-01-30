package test;

import system.jni.JavaFXUtils;

import java.io.File;
import java.nio.charset.StandardCharsets;

public class Main {
    //��ֻ��һ������ʹ��ʾ��
    public static void main(String[] args) {
        //���÷�����ʼ��native
        //���ݵ�·���������ͷ�dll����ص��ļ��У���д��Ϊ��ǰĿ¼��
        JavaFXUtils.init(new File("lib"));
        /*
         * ��ȡһ��javafx Image����, �����ͼƬΪָ�����ļ�ͼ��
         * ������ֵΪ1000��1000��ʮ�ֵ���������������Ҫ��ϸߣ�һ�������ʹ��Ĭ�Ͼ��У�
         * �������Ϊ��StandardCharsets.UTF_8(�����ʱ�򲻷������޸����ΪGBK����Ч)
         *
         * ����null����û��ȡ�ɹ�����һ����������ļ��а�������ת��ķ��ŵ��µ�
         */
        JavaFXUtils.getIconImageFX(new File("C:\\Windows\\System32\\cmd.exe"), 1000, StandardCharsets.UTF_8);
        /*
         * ����ֵΪpng��ʽ���ֽ�����
         */
        JavaFXUtils.getIconImageBytes(new File("C:\\Windows\\System32\\cmd.exe"), 1000, StandardCharsets.UTF_8);
    }
}
