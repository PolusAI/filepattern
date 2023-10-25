package filepattern.java;

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

    @Override
    public String toString() {
        return "[" + this.first.toString() + ", " + this.second.toString() + "]";
    }

    @Override
    public boolean equals(Object obj) {

        if (obj == this) {
            return true;
        }

        if (!(obj instanceof Pair)) {
            return false;
        }
         
        // typecast o to Complex so that we can compare data members 
        Pair pair = (Pair) obj;

        if ((pair.first.equals(this.first) && pair.second.equals(this.second)) || (pair.first == this.first && pair.second == this.second)) {
            return true;
        } else {
            return false;
        }
    }
}
