import java.awt.*;
import javax.swing.*;

public class Maintest {
    public static void main(String [] args){
        int playercount = 3;
        Gamertest [] players = new Gamertest[playercount];
        players[0] = new Gamertest();
        players[0].SetGamerName("Alex");
        players[0].SetGamerScore(100);
        players[1] = new Gamertest();
        players[1].SetGamerName("Said");
        players[1].SetGamerScore(100);
        players[2] = new Gamertest();
        players[2].SetGamerName("Deric");
        players[2].SetGamerScore(99);
        MainWindowtest newGame = new MainWindowtest();
        newGame.build(playercount, players);
    }
}
