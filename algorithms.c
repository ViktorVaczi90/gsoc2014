#include "algorithms.h"
#include "planet.h"
#include "datastructures.h"
#include <string.h>
#include <math.h>
//#define DEBUG
#define timestep 600
#define grav_const 6.67384*pow(10,-11)


//This function calculates the acceleration change caused by the acting planet on the target planet.
//We can add accelerations like vectors because we can add forces like vectors, and mass is a scalar.
void calculate_gravity_acc(planet* planet_target ,planet* planet_acting)
{
    double force_const = grav_const *
    (1/pow(sqrt(pow(planet_target->pos.x - planet_acting->pos.x,2) +
        pow(planet_target->pos.y - planet_acting->pos.y,2)
        +pow(planet_target->pos.z - planet_acting->pos.z,2) ),3));
    planet_target->acc.x += force_const * (planet_acting->pos.x - planet_target->pos.x )*planet_acting->mass;
    planet_target->acc.y += force_const * (planet_acting->pos.y - planet_target->pos.y )*planet_acting->mass;
    planet_target->acc.z += force_const * (planet_acting->pos.z - planet_target->pos.z )*planet_acting->mass;

    planet_acting->acc.x += -force_const * (planet_acting->pos.x - planet_target->pos.x )*planet_target->mass;
    planet_acting->acc.y += -force_const * (planet_acting->pos.y - planet_target->pos.y )*planet_target->mass;
    planet_acting->acc.z += -force_const * (planet_acting->pos.z - planet_target->pos.z )*planet_target->mass;
}

void one_iteration(planet_list* list_of_planets)//Rewrite for iteration function pointer.
{
    list_planet_item* planet_modified;
    list_planet_item* planet_modifier = list_of_planets->first_planet;
    planet_modified = list_of_planets->first_planet;
    while(planet_modifier = planet_modifier->next_planet)// Scanning trough the planets, and zero out all the acc-s.
    {
        planet_modifier->planet->acc.x=planet_modifier->planet->acc.y=planet_modifier->planet->acc.z=0;
    }
    while(planet_modified = planet_modified->next_planet)// Scanning trough all the planets and calculate the gravity
    {                                                       //This is a bubble-short like algorithm, so we count each force just once.
        planet_modifier = planet_modified;
        while(planet_modifier = planet_modifier->next_planet)//In this double loop we scan trough the planets, and calculate the accelerations.
        {
            calculate_gravity_acc(planet_modified->planet,planet_modifier->planet);// Calculate the acceleration caused by two planets
        }
    }
    //After we calculated all the accelerations we do one iteration on each planet
    planet_modified = list_of_planets->first_planet;
    while(planet_modified = planet_modified->next_planet)
    {
        iterate_second_order_RK(planet_modified->planet);
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
    outdata->columns = list_of_planets->number_of_planets*(3/*name,pos,vel,acc*/) +1/*time*/;
    outdata->rows=time/time_step +1;//Just to have some room
    outdata->column_headers=(char**)calloc(outdata->columns,sizeof(char*));
    outdata->column_headers[0]= "time";
    outdata->datas=(double**)calloc(outdata->rows,sizeof(double*));
    for(i=0;i<outdata->rows;i++)
    {
        outdata->datas[i]=(double*)calloc(outdata->columns,sizeof(double));
    }
    for(i=1;i<outdata->columns;i+=3)//Fill the headers of the columns
    {
        char* tempstring;

        tempstring = (char*)calloc(strlen(current_planet->planet->name),strlen("_x"));
        sprintf(tempstring,"%s%s",current_planet->planet->name,"_x");
        outdata->column_headers[i]=(char*)calloc(1,strlen(tempstring));
        strcpy(outdata->column_headers[i],tempstring);

        tempstring = (char*)calloc(strlen(current_planet->planet->name),strlen("_y"));
        sprintf(tempstring,"%s%s",current_planet->planet->name,"_y");
        outdata->column_headers[i+1]=(char*)calloc(1,strlen(tempstring));
        strcpy(outdata->column_headers[i+1],tempstring);

        tempstring = (char*)calloc(strlen(current_planet->planet->name),strlen("_z"));
        sprintf(tempstring,"%s%s",current_planet->planet->name,"_z");
        outdata->column_headers[i+2]=(char*)calloc(1,strlen(tempstring));
        strcpy(outdata->column_headers[i+2],tempstring);
        free(tempstring);

        current_planet = current_planet->next_planet;
    }
    for(current_row=0;current_time<time;current_time+=time_step)// Maybe this should be modified (possible error source)
    {

        one_iteration(list_of_planets);
        outdata->datas[current_row][0]=current_time;
        current_planet = list_of_planets->first_planet->next_planet;
        for(i=1;i<outdata->columns;i+=3)//fill the data
        {
            outdata->datas[current_row][i] = current_planet->planet->pos.x;
            outdata->datas[current_row][i+1] = current_planet->planet->pos.y;
            outdata->datas[current_row][i+2] = current_planet->planet->pos.z;
            current_planet = current_planet->next_planet;
        }
        current_row++;
    }
}

void iterate_second_order_RK(planet* planet_target)//One step of second order Runge-Kutta on one planet
{
    planet_target->pos.x += planet_target->vel.x*timestep +(1/2)* planet_target->acc.x*timestep*timestep;
    planet_target->pos.y += planet_target->vel.y*timestep +(1/2)* planet_target->acc.y*timestep*timestep;
    planet_target->pos.z += planet_target->vel.z*timestep +(1/2)* planet_target->acc.z*timestep*timestep;

    planet_target->vel.x += planet_target->acc.x*timestep;
    planet_target->vel.y += planet_target->acc.y*timestep;
    planet_target->vel.z += planet_target->acc.z*timestep;
}
void iterate_euler(planet* planet_target)// One step of euler on one planet, this should be solved with function pointers.
{
    planet_target->pos.x += planet_target->vel.x*timestep;
    planet_target->pos.y += planet_target->vel.y*timestep;
    planet_target->pos.z += planet_target->vel.z*timestep;

    planet_target->vel.x += planet_target->acc.x*timestep;
    planet_target->vel.y += planet_target->acc.y*timestep;
    planet_target->vel.z += planet_target->acc.z*timestep;
}

