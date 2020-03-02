#include <stdio.h>
#include <stdlib.h>

/* month_day function's prototype*/
void month_day(int year, int yearday, int *pmonth, int *pday);

static char daytab[2][13] = {
        {0, 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31},
        {0, 31, 29, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31}
};

char *month_name(int n) {

    static char *name[] = {
            "Illegal month",
            "January", "February", "March",
            "April", "May", "June",
            "July", "August", "September",
            "October", "November", "December"
    };

    return (n < 1 || n > 12) ? name[0] : name[n];
}

void month_day(int year, int yearday, int *pmonth, int *pday){
    int leap, count;
    leap = year%4 == 0 && year%100 != 0 || year%400 == 0;
    count = 0;

    while((yearday - daytab[leap][count])>0){
        yearday = yearday - daytab[leap][count];
        count ++;
    }
    *pmonth = count;
    *pday = yearday;
}


int main(int argc, char** argv) {
    int year, yearday, month, day;

    year = atoi(argv[1]);
    yearday = atoi(argv[2]);

    month_day(year,yearday, &month, &day);

    printf("%s %d, %d \n",month_name(month),day, year);

    return 0;
}
