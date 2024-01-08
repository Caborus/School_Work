import java.awt.*;
import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;
import java.io.Serializable;

import javax.swing.*;

public class GameFrame implements ActionListener, Serializable {
    int dotGridSize;
    int gridSize;

    JButton[][] linesGrid;
    JLabel[][] boxes;

    Game game;

    //Probably change this
    static int turn = 0;

    //TODO:Change variables if necessary for game functionality
    public JPanel buildGame(int dotGridSize, int gridSize, Game game) {
        this.dotGridSize = dotGridSize;
        this.gridSize = gridSize;
        this.game = game;

        linesGrid = new JButton[gridSize][];
        boxes = new JLabel[dotGridSize - 1][dotGridSize - 1];
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
                        linesGrid[row][(col - 1) / 2].addActionListener(this);
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
                        linesGrid[row][col / 2].addActionListener(this);
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

        FillBoxFromOthers();

        gridPanel.setPreferredSize(new Dimension(600, 600));
        containerPanel.add(gridPanel);
        return(gridPanel);
    }

    @Override
    public void actionPerformed(ActionEvent e) {
        JButton b = (JButton) e.getSource();
        boolean lineFilled = false; // for color change issue fix - deric

        if(b instanceof JButton) {
            game.refresh = true;
            for(int i = 0;  i < linesGrid.length; i++){
                for(int j = 0; j < linesGrid[i].length; j++){
                    if(linesGrid[i][j] == b){
                        
                        if(i % 2 == 0){
                            switch(turn){
                                case 1: if(!lineFilled){
                                            linesGrid[i][j].setIcon(new ImageIcon("./sprites/horz_line_blue.png"));
                                            lineFilled = true;
                                        }
                                        break;

                                case 2: if(!lineFilled){
                                        linesGrid[i][j].setIcon(new ImageIcon("./sprites/horz_line_pink.png"));
                                        lineFilled = true;
                                        }
                                        break;

                                case 3: if(!lineFilled){
                                            linesGrid[i][j].setIcon(new ImageIcon("./sprites/horz_line_green.png"));
                                            lineFilled = true;
                                        }
                                        break;

                                default: if(!lineFilled){
                                    linesGrid[i][j].setIcon(new ImageIcon("./sprites/horz_line_grey.png"));
                                    lineFilled = true;
                                }
                                        break;
                            }
                            
                        }else if(i % 2 != 0){
                            switch(turn){
                                case 1: if(!lineFilled){
                                    linesGrid[i][j].setIcon(new ImageIcon("./sprites/vert_line_blue.png"));
                                    lineFilled = true;
                                    }
                                        break;
                                case 2: if(!lineFilled){
                                    linesGrid[i][j].setIcon(new ImageIcon("./sprites/vert_line_pink.png"));
                                    lineFilled = true;
                                }
                                        break;
                                case 3: if(!lineFilled){
                                    linesGrid[i][j].setIcon(new ImageIcon("./sprites/vert_line_green.png"));
                                    lineFilled = true;
                                }
                                        break;
                                default: if(!lineFilled){
                                    linesGrid[i][j].setIcon(new ImageIcon("./sprites/vert_line_grey.png"));
                                    lineFilled = true;
                                }
                                        break;
                            }
                        } else{
                            break;
                        }
                        b.setRolloverEnabled(false);
                        FillBox(i, j);
                    }
                }
            }

            game.refresh = true;
        }
    } 

    public void FillBoxFromOthers() {
        for(int i = 0; i < game.boxesFilled.length - 1; i++) {
            for(int j = 0; j < game.boxesFilled[i].length - 1; j++) {
                if(game.boxesFilled[i][j]) {
                    boxes[i][j].setVisible(true);
                }
            }
        }
    }

    public void FillBox(int row, int col){
        int box1Row, box1Col, box2Row, box2Col;
        boolean boxfilled = false;
        if(row % 2 == 0){
            box1Row = (row / 2) - 1;
            box1Col = col;
            box2Row = (row / 2);
            box2Col = col;
        } else{
            box1Row = (row - 1) / 2;
            box1Col = col - 1;
            box2Row = (row - 1) / 2;
            box2Col = col;
        }

        
        if(box1Row >= 0 && box1Row < dotGridSize - 1 && box1Col >= 0 && box1Col < dotGridSize - 1) {
            if(!boxes[box1Row][box1Col].isVisible()){
                System.out.println("box1 not visible");
                selectBox1Icon(box1Row, box1Col);
                if(!linesGrid[2*box1Row][box1Col].isRolloverEnabled() && !linesGrid[2*box1Row+1][box1Col+1].isRolloverEnabled() && 
                   !linesGrid[2*box1Row+2][box1Col].isRolloverEnabled() && !linesGrid[2*box1Row+1][box1Col].isRolloverEnabled()){
                    System.out.println("box1 visible");
                    boxes[box1Row][box1Col].setVisible(true);
                    game.boxesFilled[box1Row][box1Col] = true;
                    boxfilled = true;
                    // m[box1Row][box1Col].setEnabled(false);
                    
                }
            }
        }
        
        if(box2Row >= 0 && box2Row < dotGridSize - 1 && box2Col >= 0 && box2Col < dotGridSize - 1) {
            if(!boxes[box2Row][box2Col].isVisible()){
                System.out.println("box2 not visible");
                selectBox2Icon(box2Row, box2Col);
                if(!linesGrid[2*box2Row][box2Col].isRolloverEnabled() && !linesGrid[2*box2Row+1][box2Col+1].isRolloverEnabled() && 
                   !linesGrid[2*box2Row+2][box2Col].isRolloverEnabled() && !linesGrid[2*box2Row+1][box2Col].isRolloverEnabled()){
                    System.out.println("box1 visible");
                    boxes[box2Row][box2Col].setVisible(true);
                    game.boxesFilled[box2Row][box2Col] = true;
                    boxfilled = true;
                    // m[box1Row][box1Col].setEnabled(false);
                    
                }
            }
        }

        if(boxfilled == false){
            turn = Game.currentGamerTurn();
        }
    }

    public void selectBox1Icon(int box1Row, int box1Col) {
        switch(turn){
            case 1: boxes[box1Row][box1Col].setIcon(new ImageIcon("./sprites/blueboxman2_0.png"));
                    break;
            case 2: boxes[box1Row][box1Col].setIcon(new ImageIcon("./sprites/pinkboxman2_0.png"));
                    break;
            case 3: boxes[box1Row][box1Col].setIcon(new ImageIcon("./sprites/greenboxman2_0.png"));
                    break;
            default: boxes[box1Row][box1Col].setIcon(new ImageIcon("./sprites/greyboxman2_0.png"));
                     break;
        }
    }

    public void selectBox2Icon(int box2Row, int box2Col) {
        switch(turn){
            case 1: 
                    boxes[box2Row][box2Col].setIcon(new ImageIcon("./sprites/blueboxman2_0.png"));
                    break;
            case 2: 
                    boxes[box2Row][box2Col].setIcon(new ImageIcon("./sprites/pinkboxman2_0.png"));
                    break;
            case 3: 
                    boxes[box2Row][box2Col].setIcon(new ImageIcon("./sprites/greenboxman2_0.png"));
                    break;
            default: 
                     boxes[box2Row][box2Col].setIcon(new ImageIcon("./sprites/greyboxman2_0.png"));
                     break;
        }
    }
}
