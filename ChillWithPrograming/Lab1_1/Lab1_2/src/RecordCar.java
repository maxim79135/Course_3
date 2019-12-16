import java.util.Scanner;

class Car {
	String model, number;
	int year;
	double probeg, cost;
}

public class RecordCar {
	public static Car[] setCarArray(int k) {
		Scanner sc = new Scanner(System.in, "cp1251");
		
		Car cars[] = new Car[k];
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
		return cars;
	}
	
	public static void showArray(Car[] cars) {
		System.out.println("\n�������������� �����:");
		for (int i = 0; i < cars.length; i++) {
			System.out.println((i + 1) + "." + cars[i].model + "\t" + cars[i].number + "\t" + cars[i].year + "\t" + cars[i].probeg + "\t" + cars[i].cost);
		}
	}
	
	public static void showCar(Car car) {
		System.out.println(car.model + "\t" + car.number + "\t" + car.year + "\t" + car.probeg + "\t" + car.cost);
	}
	
	public static Car minCar(Car[] cars) {
		// ����� ������� ����������
				int minIndex = 0;
				double min = cars[0].cost;
				for (int i = 0; i < cars.length; i++) {
					if (cars[i].cost < min) {
						min = cars[i].cost;
						minIndex = i;
					}
				}
		return cars[minIndex];
	}
	
	public static Car[] sortCost(Car[] cars) {
		// �������������� ������� �� �������� ���� �������
		for (int i = 0; i < cars.length - 1; i++)
			for (int j = 0; j < cars.length - 1 - i; j++) 
				if (cars[j].year < cars[j + 1].year) {
					Car tmp = cars[j];
					cars[j] = cars[j + 1];
					cars[j + 1] = tmp;
				}
		return cars;
	}
	
	public static Car findNumber(Car[] cars, String name) {
		// ����� �� ���������������� ������
		boolean nom = false;
		int index = -1;
		for (int i = 0; i < cars.length; i++)
			if (name.equalsIgnoreCase(cars[i].number)) {
				nom = true;
				index = i;
			}
		if (nom)
			return cars[index];
		else
			return null;
	}
	
	public static Car correctCar(Car[] cars) {
		Scanner sc = new Scanner(System.in, "cp1251");
		// ����������� ������ �� �����
		System.out.println("������� ����� ������ ��� �����������:");
		int n = sc.nextInt();
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
		default:
			return null;
		}
		return cars[n - 1];
	}
	
	public static void main(String[] args) {
		Scanner sc = new Scanner(System.in, "cp1251");
		
		System.out.println("������� ���������� �����: ");
		int n = sc.nextInt();
		
		Car cars[] = setCarArray(n);
		showArray(cars);
		
		Car min = minCar(cars);
		System.out.println("\n����� ������� ���������� ����� ���������: " + min.cost);
		showCar(min);
		
		cars = sortCost(cars);
		System.out.println("\n ��������������� ������ �� �������� ���� �������:");
		showArray(cars);
		
		sc.nextLine();
		System.out.println("������� ��������������� ����� ������ ��� ������:");
		String name = sc.nextLine();
		Car tmp = findNumber(cars, name);
		if (tmp != null) {
			System.out.println("����� ������ ���� � ������. ���: ");
			showCar(tmp);
		} else {
			System.out.println("����� ������ ��� � ������");
		}
		
		tmp = correctCar(cars);
		if (tmp != null) {
			System.out.println("������ � ����������� �������:");
			showCar(tmp);
		}
	}
}
