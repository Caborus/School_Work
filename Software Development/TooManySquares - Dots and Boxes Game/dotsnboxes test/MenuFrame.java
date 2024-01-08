import java.awt.*;
import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;

import javax.swing.*;

public class MenuFrame implements ActionListener {
    Display parentDisplay;
    Boolean switchDisplay = false;

    int buttonWidth = 400;
    int buttonHeight = 60; 
    int buttonSpacing = 50;

    JPanel createGame = new JPanel(new GridLayout(0, 1));
    JPanel joinGame = new JPanel(new GridLayout(0, 1));
    JPanel menuContainer = new JPanel(new GridLayout(1, 0));

    JButton create = new JButton();
    JButton join = new JButton();
    JTextField hostPort = new JTextField();
    JTextField hostPlayerName = new JTextField();
    JTextField playerCount = new JTextField();
    JTextField connectIP = new JTextField();
    JTextField connectPort = new JTextField();
    JTextField connectPlayerName = new JTextField();
    JLabel hostPlayerNameLabel = new JLabel("Enter your desired name:");
    JLabel playerCountLabel = new JLabel("Enter amount of gamers playing:");
    JLabel connectPlayerNameLabel = new JLabel("Enter your desired name:");
    JLabel hostPortLabel = new JLabel("Enter your desired Port:");
    JLabel connectIPLabel = new JLabel("Enter Host's IP Address:");
    JLabel connectPortLabel = new JLabel("Enter Host's Port:");

    public JPanel buildMenu(Display parentDisplay) {
        this.parentDisplay = parentDisplay;

        create.setText("Create Game");
        join.setText("Join Game");

        create.setPreferredSize(new Dimension(buttonWidth, buttonHeight));
        join.setPreferredSize(new Dimension(buttonWidth, buttonHeight));

        //Create Game Row
        createGame.add(hostPortLabel);
        createGame.add(hostPort);
        createGame.add(playerCountLabel);
        createGame.add(playerCount);
        createGame.add(hostPlayerNameLabel);
        createGame.add(hostPlayerName);
        createGame.add(Box.createVerticalStrut(buttonSpacing));
        createGame.add(create);
        
        //Join Game Col
        joinGame.add(connectIPLabel);
        joinGame.add(connectIP);
        joinGame.add(connectPortLabel);
        joinGame.add(connectPort);
        joinGame.add(connectPlayerNameLabel);
        joinGame.add(connectPlayerName);
        joinGame.add(Box.createVerticalStrut(buttonSpacing));
        joinGame.add(join);

        create.addActionListener(this);
        join.addActionListener(this);

        menuContainer.setLayout(new GridBagLayout());

        menuContainer.add(createGame);
        menuContainer.add(Box.createHorizontalStrut(buttonSpacing));
        menuContainer.add(joinGame);

        // getContentPane().add(menuContainer);
        // pack();

        return menuContainer;
    }

    @Override
    public void actionPerformed(ActionEvent e) {
        Game game = new Game();
        int port;
        int playerCount;
        String host;

        //Switching Displays
        //if(e.getSource() == create) parentDisplay.buildGame();
        // this.parentDisplay.buildGame();
        if(e.getSource() == create) {
            port = Integer.parseInt(hostPort.getText());
            playerCount = Integer.parseInt(this.playerCount.getText());

            game.startGameAsServer(port, playerCount, this.parentDisplay);
        }
        if(e.getSource() == join) {
            host = connectIP.getText();
            port = Integer.parseInt(connectPort.getText());

            game.startGameAsClient(host, port, this.parentDisplay);
        }
        //TODO: Implement Player Name soon
        // if(e.getSource() == create) {
        //     //this.player.SetGamerName(hostPlayerName.getText());
        //     port = Integer.parseInt(hostPort.getText());
        //     playerCount = Integer.parseInt(this.playerCount.getText());

        //     app.server = new RelayServer(app, port, playerCount);
        //     (new Thread(app.server)).start(); 
        //     System.out.println("Started as server host...");
        // }
        // if(e.getSource() == join) {
        //     //this.player.SetGamerName(connectPlayerName.getText());
        //     host = connectIP.getText();
        //     port = Integer.parseInt(connectPort.getText());

        //     if (!app.startRelayService(host, port))
        //     return;
        //     System.out.println("Started as client...");
        // }

        // app.doApp();
    }
}
