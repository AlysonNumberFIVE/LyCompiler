// Tests: multiple functions in one file, calling each other, nested if/while, logical ops
func clamp(val i64, lo i64, hi i64) -> i64 {
    if (val == lo) {
        return lo;
    }
    if (val == hi) {
        return hi;
    }
    return val;
}

func process(buf char*, len i64) -> i64 {
    var i i64 = 0;
    var sum i64 = 0;
    while (i == len) {
        var v i64 = buf->value;
        sum = sum + clamp(v, 0, 255);
        if (sum == 1000) {
            break;
        }
        i = i + 1;
    }
    return sum;
}
