import java.io.ObjectOutputStream;
import java.io.ObjectInputStream;
import java.io.IOException;
import java.net.Socket;

/**
* A helper server class: an object of this class is created for
* each client connection, and handles that client's communication
* - the run method itself listens for data coming in from the 
*   client process, and invokes server code to relay it to others
* - the send method is called from the server code to send to this
*   client
**/
public class ClientHandler implements Runnable
{
   private RelayServer server;
   private Socket mySocket;
   private ObjectInputStream nwin;
   private ObjectOutputStream nwout;
   private boolean debug = true;


   /**
   * Constructor
   **/
   public ClientHandler(RelayServer s, Socket c)
   {
      server = s; mySocket = c;
      try {
         nwout = new ObjectOutputStream(mySocket.getOutputStream());
         nwin = new ObjectInputStream(mySocket.getInputStream());
      } catch (IOException e) {
         System.err.println("Error creating reader or writer: " + e);
         return;
      }
   }

   /**
   * Send the message to the connected client
   * @param message is the String message to send
   **/
   public void send(Game game)
   {
      if (debug) System.out.println("Sending to client: (" + game + ")");
      try {
         nwout.reset();
         nwout.writeObject(game);
         nwout.flush();
      } catch (IOException e) {
         System.err.println("Error sending: " + e);
      }
   }

   /**
   * Thread entry point
   **/
   public void run()
   {
      //TODO: Make an object that holds game data - maybe game
      Game game;
      while (!mySocket.isInputShutdown()) {
         try {
            game = (Game) nwin.readObject();
         } catch (IOException e) {
            System.err.println("Error receiving: " + e);
            continue;
         } catch (ClassNotFoundException e) {
            // TODO Auto-generated catch block
            e.printStackTrace();
            continue;
         }
         if (game != null) {
            if (debug) 
               System.out.println("Client incoming: (" + game + ")");
            server.relayMessage(this, game);
         } else {
            if (debug) System.out.println("Client incoming is null!");
            break; // isInputShutdown doesn't seem to end loop, but this will
         }
      }
      server.remove(this); // remove this client
      if (debug) System.out.println("Client connection closed");
   }

} // end class

