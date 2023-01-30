# IconAcquisition
A tool class for obtaining high-definition images, which is used to replace FileSystemView and ShellFolder before JKD17.

## Simple use example
```java
import system.jni.JavaFXUtils;

class Test{
    public static void main(String[] args) {
        //init the class
        JavaFXUtils.init();
        //get the icon
        JavaFXUtils.getIconImageFX(new File("test.exe"));
        JavaFXUtils.getIconImageBytes(new File("test.exe"));
    }
}
```
## Compare with the API before JDK17
### This is a temporary display panel
![image](image/Pane.png)
### Use the following code to control the API for obtaining Icon
```
public static Image getIcon(File file) throws IOException {
    if (file == null) {
        throw new FileNotFoundException("File is null");
    }
    try {
        return /*API*/;
    }catch (Exception e){
        log.error("Failed to get icon", e);
    }
    return null;
}
```
### Use FileSystemView to get the icon and display it
```
try {
    return toFXImage(FileSystemView.getFileSystemView().getSystemIcon(file));
}catch (Exception e){
    log.error("Failed to get icon", e);
}
```
![image](image/FileSystemView.png)
### Use ShellFolder to get the icon and display it
```
try {
    return toFXImage(ShellFolder.getShellFolder(file).getIcon(true));
}catch (Exception e){
    log.error("Failed to get icon", e);
}
```
![image](image/ShellFolder.png)
### Use IconAcquisition to get the icon and display it
```
try {
    return JavaFXUtils.getIconImageFX(file,1200);
}catch (Exception e){
    log.error("Failed to get icon", e);
}
```
![image](image/IconAcquisition.png)

