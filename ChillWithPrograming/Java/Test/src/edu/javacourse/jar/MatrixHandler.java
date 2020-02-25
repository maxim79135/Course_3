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
                    Answer.mtx.get(i).set(j,(this.mtx.get(i).get(j) + Adder.mtx.get(i).get(j)));
            }
            return Answer;
        }
        else throw new IllegalArgumentException("Matrix's dimensions should be same");
    }

    public MatrixHandler subMatrix(MatrixHandler Subnet) throws IllegalArgumentException
    {
        if (this.mtx.size() == Subnet.mtx.size() && this.mtx.get(0).size() == Subnet.mtx.get(0).size())
        {
            MatrixHandler Answer = new MatrixHandler(this.mtx,this.mtx.size());
            for (int i = 0; i < this.mtx.size();i++)
            {
                for (int j = 0; j < this.mtx.get(0).size();j++)
                    Answer.mtx.get(i).set(j,(this.mtx.get(i).get(j) - Subnet.mtx.get(i).get(j)));
            }
            return Answer;
        }
        else throw new IllegalArgumentException("Matrix's dimensions should be same");
    }

    public MatrixHandler mulMatrix(MatrixHandler Mullet) throws IllegalArgumentException
    {
        if (this.mtx.size() == Mullet.mtx.get(0).size())
        {
            MatrixHandler Answer = new MatrixHandler(this.mtx,this.mtx.get(0).size());
            for (int i = 0; i < this.mtx.size();i++)
            {
                for (int j = 0; j < Mullet.mtx.get(0).size();j++)
                {
                    int temp = 0;
                    for (int k = 0; k <  Mullet.mtx.size(); k++)
                        temp+= this.mtx.get(i).get(k) * Mullet.mtx.get(k).get(j);

                    Answer.mtx.get(i).set(j,temp);
                }

            }
            return Answer;
        }
        else throw new IllegalArgumentException("Matrix's dimensions should be same");
    }
}
