#include <stdio.h>
#include <conio.h>

//static int i = 0;
int cases= 0,deaths= 0,discharges= 0;

void plot(){
    int i; 
    FILE *gnupipe = NULL;
    //without colors
    // char *Gnucommands[] = {"set title \"CASES | DEATHS | DISCHARGES\"",
    // "set style data histogram","unset key","set style fill solid 1.0 border lt -1","set ylabel \"cases,deaths and discharges\"","plot for [COL=1:3:1] 'data.tmp' using COL lc \"green\""};
    //with colors
     char *Gnucommands[] = {"set title \"CASES(purple)| DEATHS(green) | DISCHARGES(light blue)\"","set style data histogram","set style fill transparent solid 0.5 noborder","set palette model RGB defined (0 \"green\", 1 \"yellow\", 2 \"red\")","set xlabel \"DAYS\"","set ylabel \"cases,deaths and discharges\"","plot for [COL=1:3:1] 'update_cases.txt' using COL ,' ' using 1:2:($2<=10 ? 0 : $2<=20 ? 1 : 2) with boxes palette"};

    gnupipe = _popen("gnuplot -persitent","w");

    for(i = 0; i<7; i++){
        fprintf(gnupipe,"%s\n",Gnucommands[i]);
    }
    return;
}

int main(){
    // FILE *fp = fopen("update_cases.txt","a"); 
    // fprintf(fp,"%d %d %d\n",cases,deaths,discharges);
    // fclose(fp);  
    plot();
    return 0;
}