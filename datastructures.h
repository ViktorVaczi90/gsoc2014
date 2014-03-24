#ifndef DATAS
#define DATAS
#include <stdio.h>
#include <stdlib.h>
#include <math.h>



typedef struct dvec3{double x,y,z} dvec3;
typedef struct output_data{
char** column_headers;
int rows,columns;
double** datas;
}output_data;
void free_output_data(output_data* outdata);
void data_to_pos(output_data* out_data);
void data_to_csv(output_data* out_data);

//void data_to_csv_positions(output_data* out_data,struct planet_list* list_of_planets);
#endif
