package com.max;
import max.tools.MyString;

import java.util.InputMismatchException;
import java.util.Scanner;

public class Main {
    static final String RED = "\033[0;31m";
    static final String RESET = "\033[0m";

    static void mainMenu(int index) {
        System.out.println("Possible optinons:");
        String options[] = {"1. Value of sub a string prefix\n",
                            "2. Value of sub a string suffix\n",
                            "3. Value of sub a substring\n",
                            "4. Value of sub a subsequence\n",
                            "5. Exit\n"};
        if (index != -1) {
            for (int i = 0; i < index; i++)
                System.out.print(RESET + options[i]);
            System.out.print(RED + options[index]);
            for (int i = index + 1; i < options.length; i++)
                System.out.print(RESET + options[i]);
        } else {
            for (int i = 0; i < options.length; i++)
                System.out.print(RESET + options[i]);
        }
        System.out.print("Enter the option: ");
    }

    public static void main(String[] args) {
        int currentPos = -1;
        while(true) {
            System.out.println("=======================================");
            mainMenu(currentPos);
            Scanner scanner = new Scanner(System.in);
            try {
                int code = scanner.nextInt();
                System.out.println(code);
                if (code > 5) { currentPos = -1; continue; }
                else currentPos = code - 1;
                if (code == 5) break;

                System.out.print("Enter first string: ");
                scanner.nextLine();
                String str = scanner.nextLine();
                System.out.print("Enter second string: ");
                String sub = scanner.nextLine();
                boolean result = true;
                switch (code) {
                    case 1:
                        result = MyString.isPrefix(str, sub);
                        break;
                    case 2:
                        result = MyString.isSuffix(str, sub);
                        break;
                    case 3:
                        result = MyString.isSubstring(str, sub);
                        break;
                    case 4:
                        result = MyString.isSubsequence(str, sub);
                        break;
                }
                System.out.printf("Result: %s\n", result ? "True" : "False");
                System.out.print("Enter for continue: ");
                scanner.nextLine();
            }
            catch (InputMismatchException e) {
                continue;
            }

        }
    }
}
