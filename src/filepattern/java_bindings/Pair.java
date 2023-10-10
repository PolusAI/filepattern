package filepattern2.java_bindings;

public class Pair<T, S> {
    public T first;
    public S second;

    public T get0() {
        return this.first;
    }

    public  S get1() {
        return this.second;
    }

    public void set0(T value) {
        this.first = value;
    }

    public void set1(S value) {
        this.second = value;
    }

}
