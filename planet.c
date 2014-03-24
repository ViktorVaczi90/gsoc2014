#include "planet.h"
#include <stdlib.h>
int add_planet(dvec3 pos_in,dvec3 vel_in, double mass_in, char* name_in, struct planet_list * list_of_planets)
{
    struct planet_list_item* current_planet;
        if(!list_of_planets->first_planet)//If we haven't got any planet yet
    {
        list_of_planets->first_planet = (struct planet_list_item*)calloc(1,sizeof(struct planet_list_item));//
    }
    current_planet = list_of_planets->first_planet;//The current planet is the first one in the planet list
    for(;current_planet->next_planet !=NULL;current_planet = current_planet->next_planet);// Seek for the end of the list
    current_planet->next_planet = (struct planet_list_item*)calloc(1,sizeof(struct planet_list_item));//Make space for the new planet
    current_planet = current_planet->next_planet;//The current planet will be the new one
    if(!(current_planet->planet = (struct planet*) calloc(1, sizeof(struct planet))) )
    {
        return -1;
    }//Make space for the planet in the list, if problem return -1
    current_planet->planet->pos=pos_in;// Assign all the input data to the new planet
    current_planet->planet->vel=vel_in;
    current_planet->planet->mass=mass_in;
    current_planet->planet->name=name_in;
    list_of_planets->number_of_planets++;
    current_planet->next_planet = NULL;//This is redundant because of calloc, but doesn't hurt.
    return 0;
}
void print_planet_data(planet* planet_input)
{
    printf("%s :pos: (%.3f,%.3f,%.3f),vel:(%.3f,%.3f,%.3f),acc: (%.3f,%.3f,%.3f)\n",planet_input->name, planet_input->pos.x,planet_input->pos.y,planet_input->pos.z
           ,planet_input->vel.x,planet_input->vel.y,planet_input->vel.z,
           planet_input->acc.x,planet_input->acc.y,planet_input->acc.z);
}
void free_planet_list(struct planet_list * list_of_planets)
{

    struct planet_list_item* current_planet, *previous_planet;
    current_planet = list_of_planets->first_planet;
    while(current_planet->next_planet)
    {
        previous_planet = current_planet;
        current_planet = current_planet->next_planet;
        free(previous_planet->planet);
        free(previous_planet);
    }
}
