import java.awt.*;
import javax.swing.*;

public class GameFrametest extends JFrame{
    //int dotGridSize = 9;
    
    static int turn = 0;

    
    
    public JPanel buildGame( int dotGridSize ){
        int gridSize = dotGridSize + (dotGridSize - 1);
        JButton[][] linesGrid = new JButton[gridSize][];
        JLabel[][] boxes = new JLabel[dotGridSize - 1][dotGridSize - 1];
        // boolean[][] boxesFilled = new boolean[dotGridSize - 1][dotGridSize - 1];
        JPanel containerPanel = new JPanel();
        JPanel gridPanel = new JPanel(new GridLayout(gridSize, gridSize));
        gridPanel.setBackground(Color.decode("#F7FFF7"));
        containerPanel.setBackground(Color.decode("#4ECDC4"));

        for(int row = 0; row < gridSize; row++) {
            if(row % 2 == 0) linesGrid[row] = new JButton[dotGridSize - 1];
            else linesGrid[row] = new JButton[dotGridSize];
            for(int col = 0; col < gridSize; col++){

                if(row % 2 == 0) {
                    if(col % 2 == 0) {
                        JLabel dot = new JLabel(new ImageIcon("./sprites/dot_sprite.png"));
                        dot.setPreferredSize(new Dimension(50, 50));
                        gridPanel.add(dot);
                    } else {
                        linesGrid[row][(col - 1) / 2] = new JButton(new ImageIcon("./sprites/horz_line_unfilled_sprite.png"));
                        linesGrid[row][(col - 1) / 2].setOpaque(false);
                        linesGrid[row][(col - 1) / 2].setContentAreaFilled(false);
                        linesGrid[row][(col - 1) / 2].setBorderPainted(false);
                        linesGrid[row][(col - 1) / 2].setFocusPainted(false);
                        linesGrid[row][(col - 1) / 2].setPreferredSize(new Dimension(100, 50));
                        gridPanel.add(linesGrid[row][(col - 1) / 2]);
                        //linesGrid[row][(col - 1) / 2].addActionListener(this);
                    }
                } else  {
                    if(col % 2 == 0) {
                        linesGrid[row][col / 2] = new JButton(new ImageIcon("./sprites/vert_line_unfilled_sprite.png"));
                        linesGrid[row][col / 2].setOpaque(false);
                        linesGrid[row][col / 2].setContentAreaFilled(false);
                        linesGrid[row][col / 2].setBorderPainted(false);
                        linesGrid[row][col / 2].setFocusPainted(false);
                        linesGrid[row][col / 2].setPreferredSize(new Dimension(50, 100));
                        gridPanel.add(linesGrid[row][col / 2]);
                        //linesGrid[row][col / 2].addActionListener(this);
                    } else {
                        JLabel box = new JLabel(new ImageIcon("./sprites/box_filled_sprite.png"));
                        boxes[(row - 1) / 2][(col - 1) / 2] = box;
                        boxes[(row - 1) / 2][(col - 1) / 2].setPreferredSize(new Dimension(50, 50));
                        boxes[(row - 1) / 2][(col - 1) / 2].setVisible(false);
                        gridPanel.add(boxes[(row - 1) / 2][(col - 1) / 2]);
                    }
                }

                //adds the button matrix to panel - Deric
            }
        }
        gridPanel.setPreferredSize(new Dimension(600, 600));
        containerPanel.add(gridPanel);
        getContentPane().add(containerPanel);
        pack();
        return(gridPanel);
    
    }
}