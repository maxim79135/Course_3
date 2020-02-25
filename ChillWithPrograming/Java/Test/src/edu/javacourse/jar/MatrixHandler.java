package edu.javacourse.jar;

import java.util.ArrayList;

public class MatrixHandler {
    protected ArrayList<ArrayList<Integer>> mtx;

    public MatrixHandler(ArrayList<ArrayList<Integer>> _mx, int _size)
    {
        this.mtx = new ArrayList<>(_size);
        for (int i = 0;i < _size;i++)
            this.mtx.add(i,_mx.get(i));

    }

    public MatrixHandler(int _col, int _row)
    {
        ArrayList<Integer> change = new ArrayList<>(_row);
        mtx = new ArrayList<>(_col);
        for (int i = 0; i < _col; i++)
        {
            for (int j = 0; j < _row; j++)
                change.add(j,0);
            this.mtx.add(i,change);
            change = new ArrayList<>(_row);
        }
    }

    public MatrixHandler addMatrix(MatrixHandler Adder) throws IllegalArgumentException
    {
        if (this.mtx.size() == Adder.mtx.size() && this.mtx.get(0).size() == Adder.mtx.get(0).size())
        {
            MatrixHandler Answer = new MatrixHandler(this.mtx.size(),this.mtx.get(0).size());
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
            MatrixHandler Answer = new MatrixHandler(this.mtx.size(),this.mtx.get(0).size());
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
            MatrixHandler Answer = new MatrixHandler(this.mtx.size(), Mullet.mtx.get(0).size());
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

    public MatrixHandler resizeCol(MatrixHandler Reciter)
    {
        ArrayList<Integer> zeroFiller = new ArrayList<>();
        for (int i = this.mtx.size(); i < Reciter.mtx.size();i++ )
        {
            for (int j = 0;j < this.mtx.get(0).size(); j++)
                zeroFiller.add(j,0);
            this.mtx.add(i,zeroFiller);
            zeroFiller = new ArrayList<>();
        }
        return this;
    }

    public MatrixHandler resizeRow(MatrixHandler Reciter)
    {
        for (int i = 0; i < this.mtx.size();i++ )
        {
            for (int j = this.mtx.get(i).size();j < Reciter.mtx.get(0).size(); j++)
                this.mtx.get(i).add(j,0);
        }
        return this;
    }

    public MatrixHandler resizeColMul(MatrixHandler Reciter)
    {
        ArrayList<Integer> zeroFiller = new ArrayList<>();
        for (int i = this.mtx.size(); i < Reciter.mtx.get(0).size();i++ )
        {
            for (int j = 0;j < this.mtx.get(0).size(); j++)
                zeroFiller.add(j,0);
            this.mtx.add(i,zeroFiller);
            zeroFiller = new ArrayList<>();
        }
        return this;
    }

    public MatrixHandler resizeRowMul(MatrixHandler Reciter)
    {
        for (int i = 0; i < this.mtx.size();i++ )
        {
            for (int j = this.mtx.get(i).size();j < Reciter.mtx.size(); j++)
                this.mtx.get(i).add(j,0);
        }
        return this;
    }
}
