package sosok;

import java.util.Scanner;

class Car {
	String model, number;
	int year;
	double probeg, cost;
}

public class RecordCar {
	public static void main(String[] args) {
		Scanner sc = new Scanner(System.in, "cp1251");
		
		System.out.println("������� ���������� �����: ");
		int n = sc.nextInt();
		Car cars[] = new Car[n];
		System.out.println("������� ���������� � �������: ");
		for (int i = 0; i < cars.length; i++) {
			sc.nextLine();
			cars[i] = new Car();
			System.out.print("������ " + (i + 1) + "-� ������");
			cars[i].model = sc.nextLine();
			System.out.print("����� " + (i + 1) + "-� ������");
			cars[i].number = sc.nextLine();
			System.out.print("��� ������� " + (i + 1) + "-� ������");
			cars[i].year = sc.nextInt();
			System.out.print("������ " + (i + 1) + "-� ������");
			cars[i].probeg = sc.nextDouble();
			System.out.print("��������� " + (i + 1) + "-� ������");
			cars[i].cost = sc.nextDouble();
		}
		
		System.out.println("\n�������������� �����:");
		for (int i = 0; i < cars.length; i++) {
			System.out.println((i + 1) + "." + cars[i].model + "\t" + cars[i].number + "\t" + cars[i].year + "\t" + cars[i].probeg + "\t" + cars[i].cost);
		}
		
		// ����� ������� ����������
		int minIndex = 0;
		double min = cars[0].cost;
		for (int i = 0; i < cars.length; i++) {
			if (cars[i].cost < min) {
				min = cars[i].cost;
				minIndex = i;
			}
		}
		System.out.println("\n����� ������� ���������� ����� ���������: " + min);
		System.out.println((minIndex + 1) + "." + cars[minIndex].model + "\t" + cars[minIndex].number + "\t" + cars[minIndex].year + "\t" + cars[minIndex].probeg + "\t" + cars[minIndex].cost);
		
		// �������������� ������� �� �������� ���� �������
		for (int i = 0; i < cars.length - 1; i++)
			for (int j = 0; j < cars.length - 1 - i; j++) 
				if (cars[j].year < cars[j + 1].year) {
					Car tmp = cars[j];
					cars[j] = cars[j + 1];
					cars[j + 1] = tmp;
				}
		System.out.println("\n ��������������� ������ �� �������� ���� �������:");
		for (int i = 0; i < cars.length; i++) {
			System.out.println((i + 1) + "." + cars[i].model + "\t" + cars[i].number + "\t" + cars[i].year + "\t" + cars[i].probeg + "\t" + cars[i].cost);
		}
		
		// ����� �� ���������������� ������
		sc.nextLine();
		System.out.println("������� ��������������� ����� ������ ��� ������:");
		String name = sc.nextLine();
		boolean nom = false;
		int index = -1;
		for (int i = 0; i < cars.length; i++)
			if (name.equalsIgnoreCase(cars[i].number)) {
				nom = true;
				index = i;
			}
		if (nom) {
			System.out.println("����� ������ ���� � ������. ���: ");
			System.out.println((index + 1) + "." + cars[index].model + "\t" + cars[index].number + "\t" + cars[index].year + "\t" + cars[index].probeg + "\t" + cars[index].cost);
		} else {
			System.out.println("����� ������ ��� � ������");
		}
		
		// ����������� ������ �� �����
		System.out.println("������� ����� ������ ��� �����������:");
		n = sc.nextInt();
		while (n < 0 || n > cars.length) { 
			System.out.println("�������� ���������� �����. ��������� ����");
			n = sc.nextInt();
		}
		
		System.out.println("������� �������� ���� ��� ����������� (������, �����, ���, ������, ���������):");
		sc.nextLine();
		String namePole = sc.nextLine();
		switch(namePole) {
		case "������":
			System.out.println("������� ����� ������:");
			cars[n - 1].model = sc.nextLine();
			break;
		case "�����":
			System.out.println("������� ����� �����:");
			cars[n - 1].number = sc.nextLine();
			break;
		case "���":
			System.out.println("������� ����� ���:");
			cars[n - 1].year = sc.nextInt();
			break;
		case "������":
			System.out.println("������� ����� ������:");
			cars[n - 1].probeg = sc.nextDouble();
			break;
		case "���������":
			System.out.println("������� ����� ���������:");
			cars[n - 1].cost = sc.nextDouble();
			break;
		}
		System.out.println("������ � ����������� �������:");
		System.out.println(n + "." + cars[n - 1].model + "\t" + cars[n - 1].number + "\t" + cars[n - 1].year + "\t" + cars[n - 1].probeg + "\t" + cars[n - 1].cost);
		
		
	}
}
