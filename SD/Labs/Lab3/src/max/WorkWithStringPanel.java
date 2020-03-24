package max;

import javax.swing.*;
import javax.swing.border.EmptyBorder;
import java.awt.*;
import java.awt.event.ActionEvent;
import java.util.jar.JarEntry;
import max.tools.MyString;

public class WorkWithStringPanel extends JPanel {
    private JLabel result;
    JTextField firstStringField;
    JTextField secondStringField;
    JLabel resultMethod;

    private String selectedMethod = "isPrefix";

    public WorkWithStringPanel() {
        super();
        this.setLayout(new BoxLayout(this, BoxLayout.Y_AXIS));
        //this.setBorder(new EmptyBorder(10, 10, 10, 10))

        JButton confirmEnterButton = new JButton("Check");
        confirmEnterButton.addActionListener(this::onConfirmButtonClicked);

        JLabel firstStringLabel = new JLabel("First string");
        firstStringLabel.setMaximumSize(new Dimension(80, 20));
        JLabel secondStringLabel = new JLabel("Second string");
        secondStringLabel.setMaximumSize(new Dimension(100, 20));

        firstStringField = new JTextField();
        firstStringField.setMaximumSize(new Dimension(200, 20));

        secondStringField = new JTextField();
        secondStringField.setMaximumSize(new Dimension(200, 20));

        JPanel firstStringPanel = new JPanel();
        firstStringPanel.setLayout(new BoxLayout(firstStringPanel, BoxLayout.Y_AXIS));
        firstStringPanel.setBorder(new EmptyBorder(20, 20, 20, 20));
        firstStringPanel.add(firstStringLabel);
        firstStringPanel.add(secondStringLabel);

        JPanel secondStringPanel = new JPanel();
        secondStringPanel.setLayout(new BoxLayout(secondStringPanel, BoxLayout.Y_AXIS));
        secondStringPanel.setBorder(new EmptyBorder(20, 20, 20,20));
        secondStringPanel.add(firstStringField);
        secondStringPanel.add(secondStringField);

        JRadioButton selectPrefix = new JRadioButton("isPrefix");
        selectPrefix.setActionCommand("isPrefix");
        selectPrefix.setSelected(true);
        selectPrefix.addActionListener(e -> selectedMethod = e.getActionCommand());

        JRadioButton selectSuffix = new JRadioButton("isSuffix");
        selectSuffix.setActionCommand("isSuffix");
        selectSuffix.setSelected(true);
        selectSuffix.addActionListener(e -> selectedMethod = e.getActionCommand());

        JRadioButton selectSubstring = new JRadioButton("isSubstring");
        selectSubstring.setActionCommand("isSubstring");
        selectSubstring.setSelected(true);
        selectSubstring.addActionListener(e -> selectedMethod = e.getActionCommand());

        JRadioButton selectSubsequence = new JRadioButton("isSubsequence");
        selectSubsequence.setActionCommand("isSubsequence");
        selectSubsequence.setSelected(true);
        selectSubsequence.addActionListener(e -> selectedMethod = e.getActionCommand());

        ButtonGroup selectGroup = new ButtonGroup();
        selectGroup.add(selectPrefix);
        selectGroup.add(selectSuffix);
        selectGroup.add(selectSubstring);
        selectGroup.add(selectSubsequence);

        JPanel radioButtonsPanel = new JPanel();
        radioButtonsPanel.setLayout(new BoxLayout(radioButtonsPanel, BoxLayout.Y_AXIS));
        radioButtonsPanel.add(selectPrefix);
        radioButtonsPanel.add(selectSuffix);
        radioButtonsPanel.add(selectSubstring);
        radioButtonsPanel.add(selectSubsequence);

        JPanel firstSecondPanel = new JPanel();
        firstSecondPanel.setLayout(new BoxLayout(firstSecondPanel, BoxLayout.X_AXIS));
        firstSecondPanel.setBorder(new EmptyBorder(20, 20, 20, 20));
        firstSecondPanel.add(firstStringPanel);
        firstSecondPanel.add(secondStringPanel);
        firstSecondPanel.add(radioButtonsPanel);

        JLabel resultLabel = new JLabel("Result");
        resultMethod = new JLabel();

        JPanel resultPanel = new JPanel();
        resultPanel.setLayout(new BoxLayout(resultPanel, BoxLayout.X_AXIS));
        resultPanel.setBorder(new EmptyBorder(20, 20, 20, 20));
        resultPanel.add(resultLabel);
        resultPanel.add(new JSeparator(SwingConstants.HORIZONTAL));
        resultPanel.add(resultMethod);

        this.add(firstSecondPanel);
        this.add(confirmEnterButton);
        this.add(resultPanel);
    }

    private void onConfirmButtonClicked(ActionEvent e) {
        boolean result = false;
        switch (selectedMethod) {
            case "isPrefix":
                result = MyString.isPrefix(firstStringField.getText(), secondStringField.getText());
                break;
            case "isSuffix":
                result = MyString.isSuffix(firstStringField.getText(), secondStringField.getText());
                break;
            case "isSubstring":
                result = MyString.isSubstring(firstStringField.getText(), secondStringField.getText());
                break;
            case "isSubsequence":
                result = MyString.isSubsequence(firstStringField.getText(), secondStringField.getText());
                break;
        }
        resultMethod.setText(result ? "True" : "False");
    }

}
