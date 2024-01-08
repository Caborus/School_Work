import javax.swing.SwingUtilities;
import java.awt.*;

/**
* Example Relay app that is a combined client and server. This 
* allows the client app to directly connect to another client app
* without running a server somewhere. This can be an easier model
* for users to start up. Clients start in two modes: one has to 
* be started with an command line argument of "host"; this one will
* act as the server for the others. Then the other clients have to
* start up with the IP address of the host client.
*
* It allows the user to type in messages
* and then it sends these to the server, which relays them to all other
* clients. A thread is used to asynchronously receive messages from the 
* server while waiting for user input, but other read mechanisms are 
* possible (e.g., polling). 
**/
public class ComboApp implements RelayReceiver
{
   private RelayClient relayer;
   public RelayServer server;
   private boolean debug = true;

   Game game;

   /**
   * Process a message received from the server. This will be called 
   * in the relayer thread and so it should not do something that takes
   * too long, since the relayer thread needs to get back to taking care
   * of incoming messages. This thread should extract the message info and
   * set up something for the main application thread, or some other thread,
   * do use at it runs the app.
   * @param msg is the string message that was received
   **/
   public void receiveMessage(Game game)
   {
      System.out.println("Received: (" + game + ")");
      if (server != null)
         // if we are the server, send it to our server object
         server.serverGame = game;
         // server.serverGame
      else
         // else send it through our client message relayer
         this.game = game;
         // SwingUtilities.updateComponentTreeUI(this.game.display.frame);
         this.game.display.frame.getContentPane().remove(0);
         this.game.display.frame.getContentPane().add(this.game.display.gameFrame.buildGame(this.game.dotGridSize, this.game.gridSize, this.game), BorderLayout.WEST);
   }
   
   /**
   * Send the message, either using the local server object if
   * we are the server, or the relay client object if we are not
   **/
   public void sendMessage(Game game)
   {
      if (server != null)
         // if we are the server, send it to our server object
         server.relayMessage(null,game);
      else
         // else send it through our client message relayer
         relayer.send(game);
   }
   
   /**
   * Helper method for main, just starts the relay client and connects
   * it to the server
   * @param serverHost is the hostname or IP address (as a string)
   * @param serverPort is the port number the server is using
   **/
   public boolean startRelayService(String serverHost, int serverPort, Game clientGame)
   {
      relayer = new RelayClient(this); // needs RelayReceiver object
      this.game = clientGame;
      if (!relayer.connect(serverHost,serverPort, clientGame)) {
         System.err.println("Could not connect to server:9001");
         return false;
      }
      if (debug) System.out.println("Connected...");
      // RelayClient object MUST be run in its own thread
      (new Thread(relayer)).start();
      return true;
   }
   
   /**
   * Main application method, called from main.
   **/
   public void doApp()
   {
      while(!this.game.isGameOver()) {
         // for (ClientHandler c: clients) {
         //    relayMessage(c, new boolean[1][1]);
         // }
         System.out.println("");
         if(this.game.refresh) {
            this.game.refresh = false;
            sendMessage(game);
         }
      }
   }

} // end class

