import java.net.Socket;
import java.net.ServerSocket;
import java.util.Set;
import java.util.HashSet;

/**
* RelayServer: main class that drives the message relay server.
* It is most likely a singleton class, but theoretically an app
* could have more than one set of relay servers, so we don't 
* enforce the Singleton pattern. One RelayServer object manages
* a set of clients that connect on one port number, and it does
* a simple broadcast of any message that it receives (i.e., if a
* client sends a message, all <b>other</b> clients receive it).
* A new thread is created for each client, in order to wait for
* messages from that client, and the original thread that invoked
* "start()" on the RelayServer object is used to listen for new
* clients to connect. If an app wants to use multiple RelayServer
* objects it should create a thread for each.
**/
public class RelayServer implements Runnable
{
   /** clients that are connected **/
   public Set<ClientHandler> clients;
   /** my top-level socket **/
   private ServerSocket mySocket;
   int defaultPort = 9000;
   int playerCount;
   private RelayReceiver localRx;
   private boolean debug = true;

   Game serverGame;
   
   /** 
   * Constructor
   * @param port is the port number to listen on
   **/
   public RelayServer(int port)
   { 
      clients = new HashSet<ClientHandler>();
      mySocket = null;
      localRx = null;
      defaultPort = port;
   }

   /** 
   * Constructor
   * @param rx is the local receiver if this is used in a combo app
   * @param port is the port number to listen on
   **/
   public RelayServer(RelayReceiver rx, int port, int maxPlayers, Game serverGame)
   { 
      clients = new HashSet<ClientHandler>();
      mySocket = null;
      localRx = rx;
      defaultPort = port;
      playerCount = maxPlayers;
      this.serverGame = serverGame;
   }
   
   /**
   * Thread entry point, acts as "main()" for the server
   */
   public void run()
   {
      if (start(defaultPort))
         doService();
   }

   /**
   * Start the server. Port is opened for connections.
   * @param portNumber is the port to open
   * @return true if all starts ok, false if port is taken or other error
   **/
   public boolean start(int portNumber)
   {
      if (debug) System.err.println("Starting...");
      try {
         mySocket = new ServerSocket(portNumber);
      } catch (Exception e) {
         System.err.println("Error creating socket: " + e);
         return false;
      }
      return true;
   }
   
   /**
   * Begin the service. Client connections are accepted; a new thread 
   * is started for each client.
   **/
  //Change to add gamers as clients
   public void doService()
   {
      Socket clientSocket;
      ClientHandler client;
      System.out.println(clients.size());
      while (clients.size() < playerCount - 1) {
         try {
            clientSocket = mySocket.accept();
         } catch (Exception e) {
            System.err.println("Client connection failed: "  + e);
            continue;
         }
         if (debug) System.out.println("Connecting new client...");
         client = new ClientHandler(this, clientSocket);
         clients.add(client);
         (new Thread(client)).start();
         System.out.println(clients.size());
      }
      while(!serverGame.isGameOver()) {
         // for (ClientHandler c: clients) {
         //    relayMessage(c, new boolean[1][1]);
         // }
         System.out.println("");
         if(serverGame.refresh) {
            serverGame.refresh = false;
            relayMessage(null, serverGame);
         }
      }
   }

   /**
   * Relay a received message to all other clients. Note that this method
   * is called from a client-handling thread and so is protected using
   * 'synchronized' so that multiple client threads don't execute it at once.
   * @param sender is the sending client (don't relay to this one)
   * @param message is the message that is being relayed
   **/
   public synchronized void relayMessage(ClientHandler sender, Game game)
   {
      for (ClientHandler client: clients) {
         if (client == sender)
            continue; // don't send message to self
         if (debug) System.out.println("Relaying message: (" + game + ")");
         client.send(game);
      }
      if (sender != null && localRx != null)
         localRx.receiveMessage(game);
   }

   /**
   * Remove a client 
   * @param client is the client to remove from the relay set
   **/
   public synchronized void remove(ClientHandler client)
   {
      try {
         clients.remove(client);
      } catch (Exception e) {}
   }

} // end class

