#include <stdio.h>

/**
 * return:
 *      0: cpu leakage current is less than or equal to 20mA
 *      1: cpu leakage current is greater than 20mA
 *      <0: fail to read leakage current
 */
int check_leakage_current(void);

int main(int argc, char *argv[])
{
    int ret = check_leakage_current();
    if (ret < 0) {
        printf("failed to read leakage current value\n");
    } else if (ret == 0) {
        printf("leakage current meets requirement\n");
    } else if (ret == 1) {
        printf("leakage current doesn't meet requirement\n");
    } else {
        printf("unknown error\n");
        return -1;
    }
    return 0;
}
