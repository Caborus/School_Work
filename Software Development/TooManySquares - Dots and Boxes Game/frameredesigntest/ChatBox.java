import java.awt.*;
import javax.swing.*;

public class ChatBox{
    JPanel newChat = new JPanel();
    public JPanel buildChat(){
        newChat.setPreferredSize(new Dimension(500,300));
        return(newChat);
    }
}