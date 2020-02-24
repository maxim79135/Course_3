package edu.javacourse.jar;

import java.util.ArrayList;

public class MatrixHandler {
    protected ArrayList<ArrayList<Integer>> mtx;

    public MatrixHandler(ArrayList<ArrayList<Integer>> _mx, int _size)
    {
        mtx = new ArrayList<>(_size);
        mtx = _mx;
    }

    public MatrixHandler addMatrix(MatrixHandler Adder) throws IllegalArgumentException
    {
        if (this.mtx.size() == Adder.mtx.size() && this.mtx.get(0).size() == Adder.mtx.get(0).size())
        {
            MatrixHandler Answer = new MatrixHandler(this.mtx,this.mtx.size());
            for (int i = 0; i < this.mtx.size();i++)
            {
                for (int j = 0; j < this.mtx.get(0).size();j++)
                    Answer.mtx.get(i).set(j,(this.mtx.get(i).get(j).intValue() + Adder.mtx.get(i).get(j).intValue()));
            }
            return Answer;
        }
        else throw new IllegalArgumentException("Matrix's dimensions should be same");
    }
}
