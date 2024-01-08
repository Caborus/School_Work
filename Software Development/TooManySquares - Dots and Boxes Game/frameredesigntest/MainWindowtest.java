import java.awt.*;
import javax.swing.*;

public class MainWindowtest extends JPanel{


    public void build(int playercount, Gamertest [] players){
        JFrame frame = new JFrame("FrameDemo");
        GameFrametest newGame = new GameFrametest();
        Scoreboardtest newScoreboard = new Scoreboardtest();
        ChatBox newChat = new ChatBox();
        JPanel sidePanel = new JPanel();
        sidePanel.setPreferredSize(new Dimension(350, 650));

        frame.setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);
        frame.getContentPane().add(newGame.buildGame(9), BorderLayout.WEST);
        frame.getContentPane().add(sidePanel, BorderLayout.EAST);
        sidePanel.add(newScoreboard.buildScoreboard(playercount, players), BorderLayout.NORTH);
        sidePanel.add(newChat.buildChat(), BorderLayout.SOUTH);
        frame.pack();
        frame.setBackground(Color.decode("#292F36"));
        frame.setSize(1000,700);
        frame.setVisible(true);
    }

}