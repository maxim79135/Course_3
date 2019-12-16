package com.petuh;

import java.io.BufferedReader;
import java.io.IOException;
import java.io.InputStreamReader;
import java.util.Arrays;
import java.util.function.Predicate;
import java.util.List;
import java.util.stream.Collectors;
import java.util.stream.Stream;

interface Operationable {
    double calculate(double x, double y);
}
public class Lyambda {
    public void run() throws IOException {
        BufferedReader reader = new BufferedReader(new InputStreamReader(System.in));
        double a = Double.parseDouble(reader.readLine());
        double b = Double.parseDouble(reader.readLine());
        System.out.println(task1(a, b));

        int value = Integer.parseInt(reader.readLine());
        List<Integer> numbers = Arrays.asList(6, 5, 4, 3, 2, 1);
        System.out.println(task2(value, numbers, i -> i < value));
    }

    public double task1(double a, double b) {
        Operationable operation;
        operation = (x, y) -> Math.sqrt(x * x + y * y);

        double result = operation.calculate(a, b);
        return result;
    }

    public List<Integer> task2(int value, List<Integer> numbers, Predicate<Integer> predicate) {
        return numbers.parallelStream().filter(predicate).sorted((o1, o2) -> o2 - o1).collect(Collectors.toList());
    }
}
