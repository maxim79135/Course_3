package test;

import java.util.Scanner;
import java.util.Stack;

public class Skobki {
	public static boolean checkSkob(String str) {
		Stack st = new Stack();
		for (int i = 0; i < str.length(); i++) {
			if ('[' == str.charAt(i) || '(' == str.charAt(i) || '{' == str.charAt(i)) {
				st.push(str.charAt(i));
			}
			else if (']' == str.charAt(i) || ')' == str.charAt(i) || '}' == str.charAt(i)) {
				char t = (char) st.lastElement();
				if (t == '[' && ']' == str.charAt(i) || t == '(' && ')' == str.charAt(i) || t == '{' && '}' == str.charAt(i))
					st.pop();
				else
					return false;
			}
		}
		if (st.isEmpty()) return true; else return false;
	}
	
	public static void main(String[] args) {
		Scanner sc = new Scanner(System.in, "cp1251");
		System.out.println("¬ведите скобочную последовательность: ");
		String buf = sc.nextLine();
		boolean t = checkSkob(buf);
		if (t)
			System.out.println("ѕравильна€ скобочна€ последовательность");
		else
			System.out.println("Ќеправильна€ скобочна€ последовательность");
	}
}
