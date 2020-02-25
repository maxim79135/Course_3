package edu.javacourse.jar;


import java.io.IOException;
import java.io.InputStreamReader;
import java.rmi.MarshalException;
import java.util.ArrayList;
import java.util.Scanner;

public class MainHandler {
    public static void errorException(MatrixHandler Operand1, MatrixHandler Operand2)
    {
        if (Operand1.mtx.size() > Operand2.mtx.size())
        { Operand2.resizeCol(Operand1);}
        if (Operand1.mtx.size() < Operand2.mtx.size())
        { Operand1.resizeCol(Operand2);}
        if (Operand1.mtx.get(0).size() > Operand2.mtx.get(0).size())
        { Operand2.resizeRow(Operand1);}
        if (Operand1.mtx.get(0).size() < Operand2.mtx.get(0).size())
        { Operand1.resizeRow(Operand2); }
    }



    public static void main(String[] args) throws IOException {
        MatrixHandler Operand1 = null;
        MatrixHandler Operand2= null;
        while (true) {
            System.out.print(
                    "Выберите действие:\n" +
                            "1.Ввод данных\n" +
                            "2.Сложение\n" +
                            "3.Вычитание\n" +
                            "4.Умножение\n" +
                            "5.Выход\n" +
                            "Ваш выбор:"
            );
            Scanner scanner = new Scanner(System.in);
            int c = scanner.nextInt();
            System.out.println();
            switch (c) {
                case 1:
                    System.out.println("Введите размерность первой матрицы: ");

                    int n = scanner.nextInt();
                    int m = scanner.nextInt();
                    ArrayList<Integer> fillerMassive = new ArrayList<>(m);
                    ArrayList<ArrayList<Integer>> fillerMatrix = new ArrayList<>(n);
                    System.out.println("Введите элементы матрицы:");
                    for (int i = 0; i < n; i++) {
                        for (int j = 0; j < m; j++) {

                            int num = scanner.nextInt();
                            fillerMassive.add(j, num);
                        }
                        fillerMatrix.add(i, fillerMassive);
                    }
                    Operand1 = new MatrixHandler(fillerMatrix, fillerMatrix.size());
                    System.out.println("Введите размерность второй матрицы: ");

                    int rw = scanner.nextInt();
                    int cl = scanner.nextInt();
                    ArrayList<Integer> fillerMass = new ArrayList<>(cl);
                    ArrayList<ArrayList<Integer>> fillerMat = new ArrayList<>(rw);
                    System.out.println("Введите элементы матрицы:");
                    for (int i = 0; i < rw; i++) {
                        for (int j = 0; j < cl; j++) {

                            int num = scanner.nextInt();
                            fillerMass.add(j, num);
                        }
                        fillerMat.add(i, fillerMassive);
                    }
                    Operand2 = new MatrixHandler(fillerMat, fillerMat.size());
                    break;
                case 2:
                    System.out.println("Операция: Сложение A + B");
                    System.out.println("Размерность первой матрицы: " + Operand1.mtx.size() + " x " + Operand1.mtx.get(0).size());
                    System.out.println(Operand1.mtx);
                    System.out.println("Размерность второй матрицы: " + Operand2.mtx.size() + " x " + Operand2.mtx.get(0).size());
                    System.out.println(Operand2.mtx);
                    try {
                        System.out.println("Ответ:");
                        System.out.println(Operand1.addMatrix(Operand2));
                    } catch (IllegalArgumentException err) {
                        System.err.println("Матрицы имеют разную размерность");
                        errorException(Operand1, Operand2);
                    }

                    System.out.println("Операция: Сложение B + A");
                    System.out.println("Размерность первой матрицы: " + Operand2.mtx.size() + " x " + Operand2.mtx.get(0).size());
                    System.out.println(Operand2.mtx);
                    System.out.println("Размерность второй матрицы: " + Operand1.mtx.size() + " x " + Operand1.mtx.get(0).size());
                    System.out.println(Operand1.mtx);
                    try {
                        System.out.println("Ответ:");
                        System.out.println(Operand2.addMatrix(Operand1).mtx);
                    } catch (IllegalArgumentException err) {
                        System.err.println("Матрицы имеют разную размерность");
                        errorException(Operand1, Operand2);
                    }
                    break;
                case 3:
                    System.out.println("Операция: Вычитание A - B");
                    System.out.println("Размерность первой матрицы: " + Operand1.mtx.size() + " x " + Operand1.mtx.get(0).size());
                    System.out.println(Operand1.mtx);
                    System.out.println("Размерность второй матрицы: " + Operand2.mtx.size() + " x " + Operand2.mtx.get(0).size());
                    System.out.println(Operand2.mtx);
                    try {
                        System.out.println(Operand1.subMatrix(Operand2).mtx);
                    } catch (IllegalArgumentException err) {
                        System.err.println("Матрицы имеют разную размерность");
                        errorException(Operand1, Operand2);
                    }

                    System.out.println("Операция: Вычитание B - A");
                    System.out.println("Размерность первой матрицы: " + Operand2.mtx.size() + " x " + Operand2.mtx.get(0).size());
                    System.out.println(Operand2.mtx);
                    System.out.println("Размерность второй матрицы: " + Operand1.mtx.size() + " x " + Operand1.mtx.get(0).size());
                    System.out.println(Operand1.mtx);
                    try {
                        System.out.println(Operand2.subMatrix(Operand1).mtx);
                    } catch (IllegalArgumentException err) {
                        System.err.println("Матрицы имеют разную размерность");
                        errorException(Operand1, Operand2);
                    }
                    break;
                case 4:
                    System.out.println("Операция: Умножение A * B");
                    System.out.println("Размерность первой матрицы: " + Operand1.mtx.size() + " x " + Operand1.mtx.get(0).size());
                    System.out.println(Operand1.mtx);
                    System.out.println("Размерность второй матрицы: " + Operand2.mtx.size() + " x " + Operand2.mtx.get(0).size());
                    System.out.println(Operand2.mtx);
                    try {
                        System.out.println(Operand1.mulMatrix(Operand2).mtx);
                    } catch (IllegalArgumentException err) {
                        System.err.println("Кол-во столбцов матрицы А не равно кол-ву строк матрицы В");
                        if (Operand1.mtx.size() > Operand2.mtx.get(0).size()) {
                            Operand2.resizeRowMul(Operand1);
                        }
                        if (Operand1.mtx.size() < Operand2.mtx.get(0).size()) {
                            Operand1.resizeColMul(Operand2);
                        }
                    }

                    System.out.println("Операция: Умножение B * A");
                    System.out.println("Размерность первой матрицы: " + Operand2.mtx.size() + " x " + Operand2.mtx.get(0).size());
                    System.out.println(Operand2.mtx);
                    System.out.println("Размерность второй матрицы: " + Operand1.mtx.size() + " x " + Operand1.mtx.get(0).size());
                    System.out.println(Operand1.mtx);
                    try {
                        System.out.println(Operand2.mulMatrix(Operand1).mtx);
                    } catch (IllegalArgumentException err) {
                        System.err.println("Кол-во столбцов матрицы А не равно кол-ву строк матрицы В");
                        if (Operand1.mtx.size() > Operand2.mtx.get(0).size()) {
                            Operand2.resizeRowMul(Operand1);
                        }
                        if (Operand1.mtx.size() < Operand2.mtx.get(0).size()) {
                            Operand1.resizeColMul(Operand2);
                        }
                    }
                    break;
                case 5:
                    System.exit(0);
                default:
                    System.out.println("Неизвестная команда");
                    break;
            }


        }
    }
}
