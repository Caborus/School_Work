import java.awt.*;
import java.io.Serializable;

import javax.swing.*;

public class Display implements Serializable {
    JFrame frame = new JFrame("Too Many Squares");
    GameFrame gameFrame = new GameFrame();
    MenuFrame menuFrame = new MenuFrame();

    public void buildMenu() {

        frame.getContentPane().removeAll();
        frame.setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);
        frame.getContentPane().add(menuFrame.buildMenu(this), BorderLayout.CENTER);
        
        frame.pack();
        frame.setBackground(Color.decode("#292F36"));
        frame.setSize(1000,700);
        frame.setVisible(true);
    }

    public void buildGame(int dotGridSize, int gridSize, Game game) {
        //TODO: Fix these too
        // Scoreboardtest newScoreboard = new Scoreboardtest();
        // ChatBox newChat = new ChatBox();

        // JPanel sidePanel = new JPanel();
        // sidePanel.setPreferredSize(new Dimension(350, 650));

        frame.getContentPane().removeAll();
        frame.setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);
        frame.getContentPane().add(gameFrame.buildGame(dotGridSize, gridSize, game), BorderLayout.WEST);
        //TODO: Fix these
        // frame.getContentPane().add(sidePanel, BorderLayout.EAST);
        // sidePanel.add(newScoreboard.buildScoreboard(playercount, players), BorderLayout.NORTH);
        // sidePanel.add(newChat.buildChat(), BorderLayout.SOUTH);
        
        frame.pack();
        frame.setBackground(Color.decode("#292F36"));
        frame.setSize(1000,700);
        frame.setVisible(true);
    }
}