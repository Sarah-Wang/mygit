#include<stdio.h>
#include<math.h>
#inlcude<string.h>

struct relation {
    int val;
    char c;
};

struct relation relations[26];

void makelink(struct relation relations) {
    int i = 0;
    for (i; i < 26; i++) {
        relations[i].val = i + 1;
        relations[i].c = 'a' + i;
    }
}

int getval(char tmp, struct relation relations) {
    int i = 0;
    for (i; i < 26; i++) {
        if (tmp == relations[i].c) {
            return relations[i].val;
            break;
        }
    }
}

void main(void)
{
    char *str = "aab";

    int len;
    int i = 0;
    int sum = 0;
    char tmp;
    int val;

    makelink(relations);
    len = strlen(str);
    for(i = len - 1; i >=0 ; i--) {
        tmp = str[i];
        val = getval(tmp, relations)

        sum += val * pow(26, (len -1) - i);
    }
    printf("%d\n", sum);
}
