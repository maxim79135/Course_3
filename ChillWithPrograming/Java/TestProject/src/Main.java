import java.util.ArrayList;

public class Main {

    public static void main(String[] args) {
        ArrayList <ArrayList<Integer>> arrayList = new ArrayList<>();
        ArrayList<Integer> a = new ArrayList<>();
        a.add(20);
        arrayList.add(a);
        System.out.print(arrayList.get(0).get(0));
    }
}
