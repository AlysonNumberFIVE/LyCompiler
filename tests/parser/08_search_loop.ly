// Tests: linear search – pointer param, while + if + break + return inside loop
func find(haystack char*, target i64) -> i64 {
    var i i64 = 0;
    var found i64 = 0;
    while (found == 0) {
        if (haystack->value == target) {
            found = 1;
            break;
        } else {
            haystack = haystack->next;
            i = i + 1;
        }
    }
    return i;
}
