package max;

import javax.swing.*;
import java.awt.*;

public class WorkWithString implements Runnable {
    private JFrame mainFrame = new JFrame("WorkWithString");

    @Override
    public void run() {
        mainFrame.setPreferredSize(new Dimension(640, 480));

    }
}
