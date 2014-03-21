#include "datastructures.h"
#include <stdio.h>
void data_to_csv(output_data* out_data){
    FILE *fp;
    int i,j;
    fp = fopen("output.txt","w");

    for(i = 0; i < out_data->columns ;i++)
    {
        fprintf (fp, "%s;",out_data->column_headers[i]);
    }
    fprintf (fp, "\n");
    for(i=1;i<out_data->rows;i++)
    {
        for(j=0;j<out_data->columns;j++)
        {
            fprintf(fp,"%.4f;",out_data->datas[i][j]);
        }
        fprintf(fp,"\n");
    }
    fclose(fp);
}
