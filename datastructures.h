#ifndef DATAS
#define DATAS
typedef struct dvec3{double x,y,z} dvec3;
typedef struct output_data{
char** column_headers;
int rows,columns;
double** datas;
}output_data;
void data_to_csv(output_data* out_data);
#endif
