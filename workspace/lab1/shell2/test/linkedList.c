#include <criterion/criterion.h>
#include "../src/lib/linkedList.h"

ShellThreadTable table = NULL;

Test(linkedList, testBaseThreadCountShouldBeZero) {
    int count = getTableLength(table);
    cr_assert_eq(0, count);
}

Test(linkedList, testBaseToString) {
    char expected[512] = "===\tBackground Tasks Table\t===\n";
    cr_assert_str_eq(tableToString(table), expected);
}
