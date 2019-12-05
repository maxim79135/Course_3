public class Program{
    public static void main(String[] args) {
        Printable printables[10];
        for (int i = 0; i < 5; i++)
            printables[i] = new Book("name", "123");

    }
}

interface Printable {
    void print();
}

class Book implements Printable {
    String name;
    String author;

    Book(String name, String author) {
        this.name = name;
        this.author = author;
    }

    public void print() {
        System.out.printf("%s (%s) \n", name, author);
    }
}

class Magazine implements Printable {
    String name;

    Magazine(String name) {
        this.name = name;
    }

    public void print() {
        System.out.printf("%s (%s) \n", name);
    }
}