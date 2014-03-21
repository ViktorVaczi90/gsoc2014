#include "algorithms.h"
#include "planet.h"
#include "datastructures.h"
#include <string.h>
#include <math.h>
//#define DEBUG
#define timestep 600
#define grav_const 6.67384*pow(10,-11)
/*This function calculates the acceleration change caused by the acting planet on the target planet.
We can add accelerations like vectors because we can add forces like vectors, and mass is a scalar.
We dont need the mass of the target planet, although theres a room for optimalization: we could calculate the force
for one planet, and then apply the same force for the acting planet, and do a bubble-short like algorithm, witch is fairly simple
This multiplies the speed by two.
*/
void calculate_gravity_acc(planet* planet_target ,planet* planet_acting)
{
    double force_const = grav_const * (planet_acting->mass) *
    (1/pow(sqrt(pow(planet_target->pos.x - planet_acting->pos.x,2) +
        pow(planet_target->pos.y - planet_acting->pos.y,2)
        +pow(planet_target->pos.z - planet_acting->pos.z,2) ),3))/*planet_target->mass*/;// We also have to multiply with the planet_target mass for the optimalization
    planet_target->acc.x += force_const * (planet_acting->pos.x - planet_target->pos.x );
    planet_target->acc.y += force_const * (planet_acting->pos.y - planet_target->pos.y );
    planet_target->acc.z += force_const * (planet_acting->pos.z - planet_target->pos.z );
/*Optimalization:
    planet_target->acc.x += force_const * (planet_acting->pos.x - planet_target->pos.x )/planet_target->mass;
    planet_target->acc.y += force_const * (planet_acting->pos.y - planet_target->pos.y )/planet_target->mass;
    planet_target->acc.z += force_const * (planet_acting->pos.z - planet_target->pos.z )/planet_target->mass;

    planet_acting->acc.x += -force_const * (planet_acting->pos.x - planet_target->pos.x )/planet_action->mass;
    planet_acting->acc.y += -force_const * (planet_acting->pos.y - planet_target->pos.y )/planet_action->mass;
    planet_acting->acc.z += -force_const * (planet_acting->pos.z - planet_target->pos.z )/planet_action->mass;
*/
}
void iterate_euler(planet* planet_target)// On step of euler on one planet, this should be solved with function pointers.
{
    planet_target->vel.x += planet_target->acc.x*timestep;
    planet_target->vel.y += planet_target->acc.y*timestep;
    planet_target->vel.z += planet_target->acc.z*timestep;

    planet_target->pos.x += planet_target->vel.x*timestep;
    planet_target->pos.y += planet_target->vel.y*timestep;
    planet_target->pos.z += planet_target->vel.z*timestep;
}
void one_iteration(planet_list* list_of_planets)//Rewrite for iteration function pointer.
{
    list_planet_item* planet_modified;
    list_planet_item* planet_modifier;
    planet_modified = list_of_planets->first_planet;

    while(planet_modified->next_planet)
    {
        planet_modified = planet_modified->next_planet;
        planet_modified->planet->acc.x=planet_modified->planet->acc.y=planet_modified->planet->acc.z=0;
        planet_modifier = list_of_planets->first_planet;//The other version changes this to planet_modifier = planet_modified;
        while(planet_modifier->next_planet)//In this double loop we scan trough the planets, and calculate the accelerations.
        {
            planet_modifier = planet_modifier->next_planet;
            if(planet_modified != planet_modifier) //This line isn't needed in the other version.
            {
                calculate_gravity_acc(planet_modified->planet,planet_modifier->planet);

            }
        }
    }
    //After we calculated all the accelerations we do one iteration
    planet_modified = list_of_planets->first_planet;
    while(planet_modified->next_planet)
    {
        planet_modified = planet_modified->next_planet;
        iterate_euler(planet_modified->planet);//The function pointer should be used here
    }
}
//This function iterates trough a given time
void iterate_all(double time, double time_step,planet_list* list_of_planets,output_data* outdata)
   {
    double current_time=0;
    int i,current_row;
    struct planet_list_item* current_planet;
    current_planet = list_of_planets->first_planet->next_planet;
    //Here we create the space for the data array
    outdata->columns = list_of_planets->number_of_planets*(1+3*3/*name,pos,vel,acc*/) +1/*time*/;
    outdata->rows=time/time_step +1;//Just to have some room
    outdata->column_headers=(char**)calloc(outdata->columns,sizeof(char*));
    outdata->column_headers[0]= "time";
    outdata->datas=(double**)calloc(outdata->rows,sizeof(double*));
    for(i=0;i<outdata->rows;i++)
    {
        outdata->datas[i]=(double*)calloc(sizeof(double),outdata->columns);
    }
    for(i=1;i<outdata->columns;i+=10)//Fill the headers of the columns,maybe a later place would be better.
    {
        outdata->column_headers[i]=(char*)calloc(1,strlen(current_planet->planet->name));
        strcpy(outdata->column_headers[i],current_planet->planet->name);
        outdata->column_headers[i+1]=(char*)calloc(1,strlen("pos_x"));
        strcpy(outdata->column_headers[i+1],"pos_x");
        outdata->column_headers[i+2]=(char*)calloc(1,strlen("pos_x"));
        strcpy(outdata->column_headers[i+2],"pos_y");
        outdata->column_headers[i+3]=(char*)calloc(1,strlen("pos_x"));
        strcpy(outdata->column_headers[i+3],"pos_z");
        outdata->column_headers[i+4]=(char*)calloc(1,strlen("pos_x"));
        strcpy(outdata->column_headers[i+4],"vel_x");
        outdata->column_headers[i+5]=(char*)calloc(1,strlen("pos_x"));
        strcpy(outdata->column_headers[i+5],"vel_y");
        outdata->column_headers[i+6]=(char*)calloc(1,strlen("pos_x"));
        strcpy(outdata->column_headers[i+6],"vel_z");
        outdata->column_headers[i+7]=(char*)calloc(1,strlen("pos_x"));
        strcpy(outdata->column_headers[i+7],"acc_x");
        outdata->column_headers[i+8]=(char*)calloc(1,strlen("pos_x"));
        strcpy(outdata->column_headers[i+8],"acc_y");
        outdata->column_headers[i+9]=(char*)calloc(1,strlen("pos_x"));
        strcpy(outdata->column_headers[i+9],"acc_z");
        current_planet = current_planet->next_planet;
    }
    for(current_row=0;current_time<time;current_time+=time_step)
    {

        one_iteration(list_of_planets);
        outdata->datas[current_row][0]=current_time;
        current_planet = list_of_planets->first_planet->next_planet;
        for(i=2;i<outdata->columns;i+=10)//fill the data
        {
            outdata->datas[current_row][i] = current_planet->planet->pos.x;
            outdata->datas[current_row][i+1] = current_planet->planet->pos.y;
            outdata->datas[current_row][i+2] = current_planet->planet->pos.z;
            outdata->datas[current_row][i+3] = current_planet->planet->vel.x;
            outdata->datas[current_row][i+4] = current_planet->planet->vel.y;
            outdata->datas[current_row][i+5] = current_planet->planet->vel.z;
            outdata->datas[current_row][i+6] = current_planet->planet->acc.x;
            outdata->datas[current_row][i+7] = current_planet->planet->acc.y;
            outdata->datas[current_row][i+8] = current_planet->planet->acc.z;
            current_planet = current_planet->next_planet;
        }
        #ifdef DEBUG
        print_planet_data(list_of_planets->first_planet->next_planet->planet);
        print_planet_data(list_of_planets->first_planet->next_planet->next_planet->planet);
        #endif // DEBUG
        current_row++;
    }
}
