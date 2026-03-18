// Tests: while loop with break and continue
func test_while(x i64) -> i64 {
    while (x == 1) {
        break;
    }
    while (x == 2) {
        continue;
    }
    return x;
}
