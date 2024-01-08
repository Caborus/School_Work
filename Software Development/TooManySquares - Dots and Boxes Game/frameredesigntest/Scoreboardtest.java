import java.awt.*;
import javax.swing.*;

public class Scoreboardtest {

    public JTextPane buildScoreboard(int playercount, Gamertest [] gamers){
        JTextPane scoreboard = new JTextPane();
        String tmp = "";

        for(int i = 0; i < playercount; i++){
            tmp += (String.format("%s: %d\n", gamers[i].GetGamerName(), gamers[i].GetGamerScore()));
        }
        scoreboard.setText(tmp);
        scoreboard.setPreferredSize(new Dimension(300, 200));

        return(scoreboard);
    }
}