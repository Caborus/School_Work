import java.io.Serializable;
import java.util.ArrayList;
import java.util.List;
import java.util.Random;

public class Game implements Serializable {
    int dotGridSize = 9;
    int gridSize = dotGridSize + (dotGridSize - 1);

    boolean[][] boxesFilled = new boolean[dotGridSize - 1][dotGridSize - 1];
    boolean[][] linesFilled = new boolean[gridSize][];

    Display display;
    boolean refresh = false;

    Gamertest[] gamers;
    static int turnNum = 1;

    public void startGameAsServer(int port, int playerCount, Display mainDisplay) {
        display = mainDisplay;
        mainDisplay.buildGame(dotGridSize, gridSize, this);
        ComboApp app = new ComboApp();

        app.server = new RelayServer(app, port, playerCount, this);
        (new Thread(app.server)).start(); 
        System.out.println("Started as server host...");
        app.game = this;
    }

    public void startGameAsClient(String host, int port, Display mainDisplay) {
        display = mainDisplay;
        mainDisplay.buildGame(dotGridSize, gridSize, this);
        ComboApp app = new ComboApp();
        
        if (!app.startRelayService(host, port, this))
        return;
        System.out.println("Started as client...");
        app.game = this;
    }

    public void stopGame() {}

    public boolean isGameOver() {
        //if(all lines are filled in) or if(all boxes are filled), whichever one is easier to do since they are essentially the same thing
        // return true;

        //else
        return false;
    }

    public void assignGamerOrder() {
        List<Integer> gamerOrders = new ArrayList<Integer>();
        Random r = new Random();
        int gamerOrder;
        
        gamerOrder = r.nextInt(this.gamers.length) + 1;
        
        while(gamerOrders.size() < this.gamers.length) {
            if(!gamerOrders.contains(gamerOrder)) gamerOrders.add(gamerOrder);
            gamerOrder = r.nextInt(this.gamers.length) + 1;
        }

        for(int i = 0; i < this.gamers.length; i++) {
            this.gamers[i].SetGamerNumber(gamerOrders.get(i));
        }
    }
    
    public static int currentGamerTurn(){
        int temp = turnNum;
        if(turnNum >= 4){
            turnNum = 1;
        } else{
            turnNum++;
        }
        return temp;
    }

    public void printGamerOrder() {
        for(int i = 0; i < this.gamers.length; i++) {
            System.out.println("Gamer Order: " + this.gamers[i].GetGamerNumber());
        }
    }
}
