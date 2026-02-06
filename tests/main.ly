func main() -> int {
    var a = i64 10;
    var b = i64 20;

    while (a < b && b != 100) {
        if (a == 15 || b == 50) {
            return 1;
        }
        a = a + 1;
    }
    return 0;
}
