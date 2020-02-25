package edu.javacourse.jar;

import java.rmi.MarshalException;
import java.util.ArrayList;

/*
0 5  0 5
6 9  6 9
 */


public class MainHandler {
    public static void main(String[] args) {
        ArrayList<ArrayList<Integer>> mtx = new ArrayList<>();
        ArrayList<Integer> msv = new ArrayList<>();
        msv.add(0);
        msv.add(5);
        mtx.add(msv);
        msv = new ArrayList<>();
        msv.add(6);
        msv.add(9);
        mtx.add(msv);
        MatrixHandler M1 = new MatrixHandler(mtx,mtx.size());
        MatrixHandler M2 = new MatrixHandler(mtx,mtx.size());
        MatrixHandler Answer = new MatrixHandler(mtx,mtx.size());
        Answer = M1.mulMatrix(M2);
        System.out.print(Answer.mtx);
    }
}
