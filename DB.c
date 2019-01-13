#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <stdarg.h> //for variable length args countEntries
#include "DB.h"


/*global database*/
DataBase *Db;

int main()
{
    importDB("PicnicTablesExample.csv");
    countEntries("surfaceMaterialTable","Wood");
    exportDB("PicnicTablesExample.csv");
  
    editTableEntry(2, "surfaceMaterialId", "Concrete");
	exportDB("PicnicTablesExample1.csv");
    return(0);

}


//-------------------------------IMPORT----------------------------------------------------

/* importDB takes the name of a csv file for picnic tables
 * and fills the global database with the given information*/

void importDB(char fileName[])
{
    char line[1024];

    FILE *file;

    /* Sets initial memory for database arrays */
    initializer(1);

    /* opens given file */
    file = fopen(fileName, "r");
    if (!file)
    {
        printf("File opening failed");
        exit(EXIT_FAILURE);
    }

    /*Pass the initial line with column titles */
    fgets(line, 1024, file);

    /*Parse each line*/
    while(fgets(line, 1024, file))
    {
        Table Type;
        Table Surfacemats;
        Table Structmats;
        Table neighb;
        PicnicTable picnic;

        char *temp = strdup(line);
        int ii, count0 = 0, count1 = 0, templen = strlen(temp);

        /*checks each character and looks for ','*/
        for(ii = 0; ii < templen; ii++)
        {

            /*finds ',' and handles preceding word apropriatly*/
            if (temp[ii-1] == ',')
            {
                /* temporary buffer for the value about to enter database */
                char field[50] = "";

                /* sets the table id */
                picnic.tableId = Db->picnicTableTable.cursize;

                /* parses data into picnic array and tables if applicable */
                switch(count1)
                {
                    case 0:
                        /* sets the siteId */
                        strncpy(field, temp, ii);
                        picnic.siteId = atoi(field);
                        break;

                    case 1:
                        /* checks table then puts into picnic for table type */
                        strncpy(field, (temp + count0), ii-count0);
                        field[ii-count0-1] = '\0';

                        /* not in table type id*/
                        if(Db->tableTypeTable.cursize == 0)
                        {
                            Type.type_id = 0;
                            strcpy(Type.name, field);

                            Db->tableTypeTable.data[Db->tableTypeTable.cursize++] = Type;
                        }
                        else
                        {

                            int ij, isIn = 0;
                            for (ij = 0; ij < Db->tableTypeTable.cursize; ij++)
                            {
                                if (strcmp(Db->tableTypeTable.data[ij].name, field) == 0)
                                {
                                    isIn = 1;
                                    break;
                                }
                            }
                            if (!isIn)
                            {
                                /* not in table type id*/
                                if (Db->tableTypeTable.cursize == Db->tableTypeTable.maxsize)
                                {
                                    resize(0);
                                }
                                Type.type_id = Db->tableTypeTable.cursize;
                                strcpy(Type.name, field);

                                Db->tableTypeTable.data[Db->tableTypeTable.cursize++] = Type;
                            }
                        }
                        int ij;
                        /* places id in picnic */
                        for (ij = 0; ij < Db->tableTypeTable.cursize; ij++)
                        {
                            if (strcmp(Db->tableTypeTable.data[ij].name,field) == 0)
                            {
                                picnic.tableTypeId = Db->tableTypeTable.data[ij].type_id;
                            }
                        }
                        break;

                    case 2:
                        /* populates surfaceMaterials lookup if required, populates picnic array*/
                        strncpy(field, (temp + count0), ii-count0);
                        field[ii-count0-1] = '\0';

                        /* lookup is empty */
                        if(Db->surfaceMaterialTable.cursize == 0)
                        {
                            Surfacemats.type_id = 0;
                            strcpy(Surfacemats.name, field);

                            Db->surfaceMaterialTable.data[Db->surfaceMaterialTable.cursize++] = Surfacemats;
                        }
                        else
                        {

                            int isIn = 0;
                            for (ij = 0; ij < Db->surfaceMaterialTable.cursize; ij++)
                            {
                                if (strcmp(Db->surfaceMaterialTable.data[ij].name, field) == 0)
                                {
                                    isIn = 1;
                                    break;
                                }
                            }
                            if (!isIn)
                            {
                                /* name is not in lookup */
                                if (Db->surfaceMaterialTable.cursize == Db->surfaceMaterialTable.maxsize)
                                {
                                    resize(1);
                                }
                                Surfacemats.type_id = Db->surfaceMaterialTable.cursize;
                                strcpy(Surfacemats.name, field);

                                Db->surfaceMaterialTable.data[Db->surfaceMaterialTable.cursize++] = Surfacemats;
                            }
                        }
                        /* places id in surfaceMaterialsId column */
                        for (ij = 0; ij < Db->surfaceMaterialTable.cursize; ij++)
                        {
                            if (strcmp(Db->surfaceMaterialTable.data[ij].name,field) == 0)
                            {
                                picnic.surfaceMaterialId = Db->surfaceMaterialTable.data[ij].type_id;
                            }
                        }
                        break;

                    case 3:
                        /* populates structuralMaterials lookup if required, populates picnic array*/
                        strncpy(field, (temp + count0), ii-count0);
                        field[ii-count0-1] = '\0';

                        /* lookup is empty */
                        if(Db->structMaterialTable.cursize == 0)
                        {
                            Structmats.type_id = 0;
                            strcpy(Structmats.name, field);

                            Db->structMaterialTable.data[Db->structMaterialTable.cursize++] = Structmats;
                        }
                        else
                        {

                            int isIn = 0;
                            for (ij = 0; ij < Db->structMaterialTable.cursize; ij++)
                            {
                                if (strcmp(Db->structMaterialTable.data[ij].name, field) == 0)
                                {
                                    isIn = 1;
                                    break;
                                }
                            }
                            if (!isIn)
                            {
                                /* value is not in lookup table yet */
                                if (Db->structMaterialTable.cursize == Db->structMaterialTable.maxsize)
                                {
                                    resize(2);
                                }
                                Structmats.type_id = Db->structMaterialTable.cursize;
                                strcpy(Structmats.name, field);

                                Db->structMaterialTable.data[Db->structMaterialTable.cursize++] = Structmats;
                            }
                        }
                        /* places Id in picnic array */
                        for (ij = 0; ij < Db->structMaterialTable.cursize; ij++)
                        {
                            if (strcmp(Db->structMaterialTable.data[ij].name,field) == 0)
                            {
                                picnic.structuralMaterialId = Db->structMaterialTable.data[ij].type_id;
                            }
                        }
                        break;

                    case 4:
                        /* places streetAvenue value in picnic array */
                        strncpy(field, (temp + count0), ii-count0);
                        field[ii-count0-1] = '\0';
                        strcpy(picnic.streetAvenue, field);
                        break;

                    case 5:
                        /* places neighbourhood Id into picnic array */
                        strncpy(field, (temp + count0), ii-count0);
                        field[ii-count0-1] = '\0';
                        picnic.neighbourhood_id = atoi(field);
                        break;

                    case 6:
                        /* checks if in neighbourhood lookup table, places in picnic*/
                        strncpy(field, (temp + count0), ii-count0);
                        field[ii-count0-1] = '\0';

                        if(Db->neighbourhoodTable.cursize == 0)
                        {
                            /* neigh lookup is empty */
                            neighb.type_id = picnic.neighbourhood_id;
                            strcpy(neighb.name, field);

                            Db->neighbourhoodTable.data[Db->neighbourhoodTable.cursize++] = neighb;
                        }
                        else
                        {

                            int isIn = 0;
                            for (ij = 0; ij < Db->neighbourhoodTable.cursize; ij++)
                            {
                                if (strcmp(Db->neighbourhoodTable.data[ij].name, field) == 0)
                                {
                                    isIn = 1;
                                    break;
                                }
                            }
                            if (!isIn)
                            {
                                /* value not in lookup yet */
                                if (Db->neighbourhoodTable.cursize == Db->neighbourhoodTable.maxsize)
                                {
                                    resize(3);
                                }
                                neighb.type_id = picnic.neighbourhood_id;
                                strcpy(neighb.name, field);

                                Db->neighbourhoodTable.data[Db->neighbourhoodTable.cursize++] = neighb;
                            }
                        }
                        /* places in picnic */
                        for (ij = 0; ij < Db->neighbourhoodTable.cursize; ij++)
                        {
                            if (strcmp(Db->neighbourhoodTable.data[ij].name,field) == 0)
                            {
                                picnic.neighbourhood_id = Db->neighbourhoodTable.data[ij].type_id;
                            }
                        }
                        break;

                    case 7:
                        /* places ward in picnic array */
                        strncpy(field, (temp + count0), ii-count0);
                        field[ii-count0-1] = '\0';
                        strcpy(picnic.ward, field);
                        break;

                    case 8:
                        /* places latitude in array */
                        strncpy(field, (temp + count0), ii-count0);
                        field[ii-count0-1] = '\0';
                        strcpy(picnic.latitude,field);
                        break;

                    case 9:
                        /* places longitude in picnic array*/
                        strncpy(field, (temp + count0), ii-count0);
                        field[ii-count0-1] = '\0';
                        strcpy(picnic.longitude,field);
                        break;

                }
                /*sets parsing variables to next value*/
                count1++;
                count0 = ii;
            }

        }
        /* inputs picnic array into Db, resizes if necessary */
        if (Db->picnicTableTable.cursize == Db->picnicTableTable.maxsize)
        {
            resize(4);
        }
        Db->picnicTableTable.data[Db->picnicTableTable.cursize++] = picnic;
    }
    fclose(file);
}



//-------------------------------EXPORT----------------------------------------------------

// Export function
void exportDB(char fileName[]){
    int i;

    FILE *outFile = fopen(fileName, "w");

    if (outFile == NULL){
        fprintf(stderr, "Error opening file!\n");
        exit(EXIT_FAILURE);
    }

    fprintf(outFile, "ID, Table Type, Surface Material, Structural Material, Street/Avenue, Neighbourhood ID, Neighbourhood Name, Ward, Latitude, Longtitude, Location\n");

    for (i = 0; i < Db->picnicTableTable.cursize; i++){

        // Declaring the types we need to send out. If they are ints, we can just store info
        int id = Db->picnicTableTable.data[i].siteId;
        char tableTypeStr[50];
        char surfaceMatStr[50];
        char structMatStr[50];
        char streetAveStr[50];
        int neighId = Db->picnicTableTable.data[i].neighbourhood_id;
        char neighName[50];
        char wardStr[50];
        char latitude[50];
        char longitude[50];
        char location[100] = "\"(";

        strcpy(tableTypeStr, lookUpStr(Db->picnicTableTable.data[i].tableTypeId, 0));
        strcpy(surfaceMatStr, lookUpStr(Db->picnicTableTable.data[i].surfaceMaterialId, 1));
        strcpy(structMatStr, lookUpStr(Db->picnicTableTable.data[i].structuralMaterialId, 2));

        strcpy(streetAveStr, Db->picnicTableTable.data[i].streetAvenue);


        strcpy(neighName, lookUpStr(Db->picnicTableTable.data[i].neighbourhood_id,3));

        strcpy(wardStr, Db->picnicTableTable.data[i].ward);
        strcpy(latitude, Db->picnicTableTable.data[i].latitude);
        strcpy(longitude, Db->picnicTableTable.data[i].longitude);

        /* concatanates location variable to format (latitude, longitude) */
        strcat(location, latitude);
        strcat(location, ", ");
        strcat(location, longitude);
        strcat(location, ")\"");

        // Print
        //if (neighId == 0){
          //  fprintf(outFile,"%d,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s\n", id, tableTypestr, surfaceMatStr, structMatStr, streetAveStr, "", neighName, wardStr, latitude, longitude, location);
        //}
        //else{
        fprintf(outFile,"%d,%s,%s,%s,%s,%d,%s,%s,%s,%s,%s\n", id, tableTypeStr, surfaceMatStr, structMatStr, streetAveStr, neighId, neighName, wardStr, latitude, longitude, location);
        //}
    }
    fclose(outFile);
}


//-------------------------------LOOKUPSTR----------------------------------------------------

// Look up function for ID to type name
const char *lookUpStr(int id, int tableNum){
    int i;

    // If the table number is 0, then we are looking for table types.
    if (tableNum == 0){
        for (i = 0; i < Db->tableTypeTable.cursize; i++){
            if (Db->tableTypeTable.data[i].type_id == id){
                return (Db->tableTypeTable.data[i].name);
            }
        }
    }

        // Else if the table number is 1, we are looking for surface materials
    else if (tableNum == 1){
        for (i = 0; i < Db->surfaceMaterialTable.cursize; i++){
            if (Db->surfaceMaterialTable.data[i].type_id == id){
                return (Db->surfaceMaterialTable.data[i].name);
            }
        }
    }

        // Else if, number is 2. Looking for structural material
    else if (tableNum == 2){
        for (i = 0; i < Db->structMaterialTable.cursize; i++){
            if (Db->structMaterialTable.data[i].type_id == id){
                return (Db->structMaterialTable.data[i].name);
            }
        }
    }

        // Else if, number is 3. Looking for Neighbourhood.
    else if (tableNum == 3){
        for (i = 0; i < Db->neighbourhoodTable.cursize; i++){
            if (Db->neighbourhoodTable.data[i].type_id == id){
                return (Db->neighbourhoodTable.data[i].name);
            }
        }
    }

        // Else, error.
    else{
        printf("Either invaild type or type was not found!\n");
        return NULL;
    }
  return NULL;
}


//-------------------------------INITIALIZER----------------------------------------------------

/* initializes memory for global database */
void initializer(int isNew)
{
  Db = malloc(sizeof(DataBase));
    Db->tableTypeTable.maxsize = 10;
    Db->tableTypeTable.cursize = 0;
    if(isNew)
    {
        Db->tableTypeTable.data = malloc(sizeof(Table) * (Db->tableTypeTable.maxsize));
    }
    else
    {
        Db->tableTypeTable.data = realloc(NULL, sizeof(Table) * (Db->tableTypeTable.maxsize));
    }

    if (!Db->tableTypeTable.data)
    {
        printf("tableTypeTable memory allocation failed\n");
        exit(1);
    }

    Db->surfaceMaterialTable.maxsize = 10;
    Db->surfaceMaterialTable.cursize = 0;
    if(isNew)
    {
        Db->surfaceMaterialTable.data = malloc(sizeof(Table) * (Db->surfaceMaterialTable.maxsize));
    }
    else
    {
        Db->surfaceMaterialTable.data = realloc(NULL, sizeof(Table) * (Db->surfaceMaterialTable.maxsize));
    }

    if (!Db->surfaceMaterialTable.data)
    {
        printf("surfaceMaterialTable memory allocation failed\n");
        exit(1);
    }

    Db->structMaterialTable.maxsize = 10;
    Db->structMaterialTable.cursize = 0;
    if(isNew)
    {
        Db->structMaterialTable.data = malloc(sizeof(Table) * (Db->structMaterialTable.maxsize));
    }
    else
    {
        Db->structMaterialTable.data = realloc(NULL, sizeof(Table) * (Db->structMaterialTable.maxsize));
    }
    if (!Db->structMaterialTable.data)
    {
        printf("structMaterialTable memory allocation failed\n");
        exit(1);
    }

    Db->neighbourhoodTable.maxsize = 10;
    Db->neighbourhoodTable.cursize = 0;
    if(isNew)
    {
        Db->neighbourhoodTable.data = malloc(sizeof(Table) * (Db->neighbourhoodTable.maxsize));
    }
    else
    {
        Db->neighbourhoodTable.data = realloc(NULL, sizeof(Table) * (Db->neighbourhoodTable.maxsize));
    }
    if (!Db->neighbourhoodTable.data)
    {
        printf("neighbourhoodTable memory allocation failed\n");
        exit(1);
    }

    Db->picnicTableTable.maxsize = 10;
    Db->picnicTableTable.cursize = 0;
    if(isNew)
    {
        Db->picnicTableTable.data = malloc(sizeof(PicnicTable) * (Db->picnicTableTable.maxsize));
    }
    else
    {
        Db->picnicTableTable.data = realloc(NULL, sizeof(PicnicTable) * (Db->picnicTableTable.maxsize));
    }
    if (!Db->picnicTableTable.data)
    {
        printf("picnicTableTable memory allocation failed\n");
        exit(1);
    }
}



//-------------------------------RESIZE----------------------------------------------------

/* resizes given table in database */
void resize(int tableNum)
{
    /* 0 = tableTypeTable
     * 1 = surfaceMaterialTable
     * 2 = structMaterialTable
     * 3 = neighbourhoodTable
     * 4 = picnicTableTable*/

    if (tableNum == 0)
    {
        Db->tableTypeTable.maxsize *= 2;
        Db->tableTypeTable.data = realloc(Db->tableTypeTable.data, (sizeof(Table) * (Db->tableTypeTable.maxsize)));

        if (!Db->tableTypeTable.data)
        {
            printf("tableTypeTable memory reallocation failed\n");
            exit(EXIT_FAILURE);
        }
    }
    else if (tableNum == 1)
    {
        Db->surfaceMaterialTable.maxsize *= 2;
        Db->surfaceMaterialTable.data = realloc(Db->surfaceMaterialTable.data, (sizeof(Table) * (Db->surfaceMaterialTable.maxsize)));

        if (!Db->surfaceMaterialTable.data)
        {
            printf("surfaceMaterialTable memory reallocation failed\n");
            exit(EXIT_FAILURE);
        }
    }
    else if (tableNum == 2)
    {
        Db->structMaterialTable.maxsize *= 2;
        Db->structMaterialTable.data = realloc(Db->structMaterialTable.data, (sizeof(Table) * (Db->structMaterialTable.maxsize)));

        if (!Db->structMaterialTable.data)
        {
            printf("structMaterialTable memory reallocation failed\n");
            exit(EXIT_FAILURE);
        }
    }
    else if (tableNum == 3)
    {
        Db->neighbourhoodTable.maxsize *= 2;
        Db->neighbourhoodTable.data = realloc(Db->neighbourhoodTable.data, (sizeof(Table) * (Db->neighbourhoodTable.maxsize)));

        if (!Db->neighbourhoodTable.data)
        {
            printf("neighbourhoodTable memory reallocation failed\n");
            exit(EXIT_FAILURE);
        }
    }
    else if (tableNum == 4)
    {
        Db->picnicTableTable.maxsize *= 2;
        Db->picnicTableTable.data = realloc(Db->picnicTableTable.data, (sizeof(PicnicTable) * (Db->picnicTableTable.maxsize)));

        if (!Db->picnicTableTable.data)
        {
            printf("picnicTableTable memory reallocation failed\n");
            exit(EXIT_FAILURE);
        }
    }
}


//-------------------------------COUNTENTRIES----------------------------------------------------

// memberName = tableTypeId to represent Db->picnicTableTable.tableTypeId
// value will have to be passed in as 0 - n (of types) in string format for all TableArray items.

int countEntries(char *memberName, char *value){
  int j;
  int count = 0;
  
  // 1. Site Id
  if (strcmp(memberName, "siteId") == 0){
    
    // Going through each line until the current size of the table
    for (j = 0; j < Db->picnicTableTable.cursize; j++){
      // If site ID at a specific line is the same as the integer value of value, then increase count
      if (Db->picnicTableTable.data[j].siteId == atoi(value)){
        count++;
      }
    }
  }
    
  // 2. Table type
  else if (strcmp(memberName, "tableTypeId") == 0){
    // Going through each line until the current size of the table
    for (j = 0; j < Db->picnicTableTable.cursize; j++){
      // If tableTypeId at a specific line is the same as the integer value of value, then increase count
      if (Db->picnicTableTable.data[j].tableTypeId == atoi(value)){
        count++;
      }
    }
  }
  
  // 3. Surface mat
  else if (strcmp(memberName, "surfaceMaterialId") == 0){
    // Going through each line until the current size of the table
    for (j = 0; j < Db->picnicTableTable.cursize; j++){
      // If surfaceMaterialId at a specific line is the same as the integer value of value, then increase count
      if (Db->picnicTableTable.data[j].surfaceMaterialId == atoi(value)){
        count++;
      }
    }
  }
  
  // 4. Structural mat
  else if (strcmp(memberName, "structuralMaterialId") == 0){
    // Going through each line until the current size of the table
    for (j = 0; j < Db->picnicTableTable.cursize; j++){
      // If structuralMaterialId at a specific line is the same as the integer value of value, then increase count
      if (Db->picnicTableTable.data[j].structuralMaterialId == atoi(value)){
        count++;
      }
    }
  }
  
  // 5. Street/Avenue
  else if (strcmp(memberName, "streetAvenue") == 0){
      
    for (j = 0; j < Db->picnicTableTable.cursize; j++){
      if (strcmp(Db->picnicTableTable.data[j].streetAvenue, value) == 0){
        count++;
      }
    }
  }
  
  // 6. Neighbourhood ID
  else if (strcmp(memberName, "neighbourhood_id") == 0){
      
    for (j = 0; j < Db->picnicTableTable.cursize; j++){
      if (Db->picnicTableTable.data[j].neighbourhood_id == atoi(value)){
        count++;
      }
    }
  }
  
  // 7. Ward
  else if (strcmp(memberName, "ward") == 0){
      
    for (j = 0; j < Db->picnicTableTable.cursize; j++){
      if (strcmp(Db->picnicTableTable.data[j].ward, value) == 0){
        count++;
      }
    }
  }
  
  // 8. Latitude
  else if (strcmp(memberName, "latitude") == 0){
     
    for (j = 0; j < Db->picnicTableTable.cursize; j++){
      if (strcmp(Db->picnicTableTable.data[j].latitude, value) == 0){
        count++;
      }
    }
  }
  
  // 9. Longitude
  else if (strcmp(memberName, "longitude") == 0){
    for (j = 0; j < Db->picnicTableTable.cursize; j++){
      if (strcmp(Db->picnicTableTable.data[j].longitude, value) == 0){
        count++;
      }
    }
  }
  
  return count;
}

    
    


//-------------------------------SORTBYMEMBER----------------------------------------------------

void sortByMember(char memberName[]) {
    int ii;

    /* sorts tableId */
    if (strcmp(memberName,"tableId") == 0){
        for (ii = 0; ii < Db->picnicTableTable.cursize; ii++){
            PicnicTable temp;
            int ij;
          
            for (ij = ii; ij > 0; ij--){
                if (Db->picnicTableTable.data[ij].tableId < Db->picnicTableTable.data[ij - 1].tableId){
                    temp = Db->picnicTableTable.data[ij - 1];
                    Db->picnicTableTable.data[ij - 1] = Db->picnicTableTable.data[ij];
                    Db->picnicTableTable.data[ij] = temp;
                }
            }
        }
    }
  
        /* sorts siteId */
    else if (strcmp(memberName, "siteId") == 0){
        for (ii = 0; ii < Db->picnicTableTable.cursize; ii++){
            PicnicTable temp;
            int ij;
          
            for (ij = ii; ij > 0; ij--){
                if (Db->picnicTableTable.data[ij].siteId < Db->picnicTableTable.data[ij - 1].siteId){
                    temp = Db->picnicTableTable.data[ij - 1];
                    Db->picnicTableTable.data[ij - 1] = Db->picnicTableTable.data[ij];
                    Db->picnicTableTable.data[ij] = temp;
                }
            }
        }
    }
  
        /* sorts by tableTypeId */
    else if (strcmp(memberName, "tableTypeId") == 0){
        for (ii = 0; ii < Db->picnicTableTable.cursize; ii++){
            PicnicTable temp;
            int ij;
          
            for (ij = ii; ij > 0; ij--){
                if (Db->picnicTableTable.data[ij].tableTypeId < Db->picnicTableTable.data[ij - 1].tableTypeId){
                    temp = Db->picnicTableTable.data[ij - 1];
                    Db->picnicTableTable.data[ij - 1] = Db->picnicTableTable.data[ij];
                    Db->picnicTableTable.data[ij] = temp;
                }
            }
        }
    }
  
        /* sorts by SurfaceMaterialId */
    else if (strcmp(memberName, "surfaceMaterialId") == 0){
        for (ii = 0; ii < Db->picnicTableTable.cursize; ii++){
            PicnicTable temp;
            int ij;
          
            for (ij = ii; ij > 0; ij--){
                if (Db->picnicTableTable.data[ij].surfaceMaterialId < Db->picnicTableTable.data[ij - 1].surfaceMaterialId){
                    temp = Db->picnicTableTable.data[ij - 1];
                    Db->picnicTableTable.data[ij - 1] = Db->picnicTableTable.data[ij];
                    Db->picnicTableTable.data[ij] = temp;
                }
            }
        }
    }
  
        /* sorts by structuralMaterialId */
    else if (strcmp(memberName, "structuralMaterialId") == 0){
        for (ii = 0; ii < Db->picnicTableTable.cursize; ii++){
            PicnicTable temp;
            int ij;
          
            for (ij = ii; ij > 0; ij--){
                if (Db->picnicTableTable.data[ij].structuralMaterialId < Db->picnicTableTable.data[ij - 1].structuralMaterialId){
                    temp = Db->picnicTableTable.data[ij - 1];
                    Db->picnicTableTable.data[ij - 1] = Db->picnicTableTable.data[ij];
                    Db->picnicTableTable.data[ij] = temp;
                }
            }
        }
    }
  
        /* sorts by streetAvenue */
    else if (strcmp(memberName, "streetAvenue") == 0){
        for (ii = 0; ii < Db->picnicTableTable.cursize; ii++){
            PicnicTable temp;
            int ij;
          
            for (ij = ii; ij > 0; ij--){
                if (strcmp(Db->picnicTableTable.data[ij].streetAvenue, Db->picnicTableTable.data[ij - 1].streetAvenue) < 0){
                    temp = Db->picnicTableTable.data[ij - 1];
                    Db->picnicTableTable.data[ij - 1] = Db->picnicTableTable.data[ij];
                    Db->picnicTableTable.data[ij] = temp;
                }
            }
        }
    }
  
        /* sorts by neighbourhoodId */
    else if (strcmp(memberName, "neighbourhood_id") == 0) {
        for (ii = 0; ii < Db->picnicTableTable.cursize; ii++){
            PicnicTable temp;
            int ij;
          
            for (ij = ii; ij > 0; ij--){
                if (Db->picnicTableTable.data[ij].neighbourhood_id < Db->picnicTableTable.data[ij - 1].neighbourhood_id){
                    temp = Db->picnicTableTable.data[ij - 1];
                    Db->picnicTableTable.data[ij - 1] = Db->picnicTableTable.data[ij];
                    Db->picnicTableTable.data[ij] = temp;
                }
            }
        }
    }
  
        /* sorts by wards */
    else if (strcmp(memberName, "ward") == 0){
        for (ii = 0; ii < Db->picnicTableTable.cursize; ii++){
            PicnicTable temp;
            int ij;
          
            for (ij = ii; ij > 0; ij--){
                if (strcmp(Db->picnicTableTable.data[ij].ward, Db->picnicTableTable.data[ij - 1].ward) < 0){
                    temp = Db->picnicTableTable.data[ij - 1];
                    Db->picnicTableTable.data[ij - 1] = Db->picnicTableTable.data[ij];
                    Db->picnicTableTable.data[ij] = temp;
                }
            }
        }
    }

        /* sorts by latitude */
    else if (strcmp(memberName, "latitude") == 0){
        for (ii = 0; ii < Db->picnicTableTable.cursize; ii++){
            PicnicTable temp;
            int ij;
          
            for (ij = ii; ij > 0; ij--){
                if (strcmp(Db->picnicTableTable.data[ij].latitude, Db->picnicTableTable.data[ij - 1].latitude) < 0){
                    temp = Db->picnicTableTable.data[ij - 1];
                    Db->picnicTableTable.data[ij - 1] = Db->picnicTableTable.data[ij];
                    Db->picnicTableTable.data[ij] = temp;
                }
            }
        }
    }
  
        /* sorts by longitude */
    else if (strcmp(memberName, "longitude") == 0){
        for (ii = 0; ii < Db->picnicTableTable.cursize; ii++){
            PicnicTable temp;
            int ij;
          
            for (ij = ii; ij > 0; ij--){
                if (strcmp(Db->picnicTableTable.data[ij].longitude, Db->picnicTableTable.data[ij - 1].longitude) < 0){
                    temp = Db->picnicTableTable.data[ij - 1];
                    Db->picnicTableTable.data[ij - 1] = Db->picnicTableTable.data[ij];
                    Db->picnicTableTable.data[ij] = temp;
                }
            }
        }
    }
  
        /* sorts by neighbourhood names (not id) */
    else if (strcmp(memberName, "neighbourhood_name") == 0){
        for (ii = 0; ii < Db->picnicTableTable.cursize; ii++){
            PicnicTable temp;
            int ij;
          
            for (ij = ii; ij > 0; ij--){
                if (strcmp(lookUpStr(Db->picnicTableTable.data[ij].neighbourhood_id, 3),lookUpStr(Db->picnicTableTable.data[ij - 1].neighbourhood_id, 3)) < 0){
                    temp = Db->picnicTableTable.data[ij - 1];
                    Db->picnicTableTable.data[ij - 1] = Db->picnicTableTable.data[ij];
                    Db->picnicTableTable.data[ij] = temp;
                }
            }
        }
    }
  
    else{
        printf("That's not a valid column\n");
    }
}




//-------------------------------REPORTBYWARD----------------------------------------------------

/*sorts database by ward using sort by member */
void reportByWard()
{
    int ii;
    char curWard[10] = "";

    sortByMember("ward");

    for (ii = 0; ii < Db->picnicTableTable.cursize; ii++)
    {
        int siteId = Db->picnicTableTable.data[ii].siteId, neighId = Db->picnicTableTable.data[ii].neighbourhood_id;
        char tableType[50], surfMat[50], structMat[50], streetAve[50], neighName[50], latitude[50], longitude[50];
        char location[50] = "";

        strcpy(tableType, lookUpStr(Db->picnicTableTable.data[ii].tableTypeId,0));
        strcpy(surfMat, lookUpStr(Db->picnicTableTable.data[ii].surfaceMaterialId,1));
        strcpy(structMat, lookUpStr(Db->picnicTableTable.data[ii].structuralMaterialId,2));
        strcpy(streetAve, Db->picnicTableTable.data[ii].streetAvenue);
        strcpy(neighName, lookUpStr(Db->picnicTableTable.data[ii].neighbourhood_id,3));
        strcpy(latitude, Db->picnicTableTable.data[ii].latitude);
        strcpy(longitude, Db->picnicTableTable.data[ii].longitude);

        strcat(location, "(");
        strcat(location, latitude);
        strcat(location, ",");
        strcat(location, longitude);
        strcat(location, ")");

        if (strcmp(curWard, Db->picnicTableTable.data[ii].ward) != 0)
        {
            strcpy(curWard, Db->picnicTableTable.data[ii].ward);
            printf("%s\n", curWard);
        }
        printf("\t%-6d%-22s%-12s%-12s%-30s%-10d%-30s%-20s%-20s%-40s\n\n", siteId, tableType, surfMat, structMat, streetAve, neighId, neighName, latitude, longitude, location);
    }
}



//-------------------------------REPORTBYNEIGHBOURHOOD----------------------------------------------------

/* reports all picnic tables given in file according to neighbourhoods */
void reportByNeighbourhood()
{
    int ii;
    int curNeigh = -1;

    sortByMember("neighbourhood_name");

    for (ii = 0; ii < Db->picnicTableTable.cursize; ii++){
        int siteId = Db->picnicTableTable.data[ii].siteId, neighId = Db->picnicTableTable.data[ii].neighbourhood_id;
        char tableType[50], surfMat[50], structMat[50], streetAve[50], ward[50], latitude[50], longitude[50];
        char location[50] = "";

        strcpy(tableType, lookUpStr(Db->picnicTableTable.data[ii].tableTypeId,0));
        strcpy(surfMat, lookUpStr(Db->picnicTableTable.data[ii].surfaceMaterialId,1));
        strcpy(structMat, lookUpStr(Db->picnicTableTable.data[ii].structuralMaterialId,2));
        strcpy(streetAve, Db->picnicTableTable.data[ii].streetAvenue);
        strcpy(ward, Db->picnicTableTable.data[ii].ward);
        strcpy(latitude, Db->picnicTableTable.data[ii].latitude);
        strcpy(longitude, Db->picnicTableTable.data[ii].longitude);

        strcat(location, "(");
        strcat(location, latitude);
        strcat(location, ",");
        strcat(location, longitude);
        strcat(location, ")");

        if (curNeigh != Db->picnicTableTable.data[ii].neighbourhood_id)
        {
            curNeigh = Db->picnicTableTable.data[ii].neighbourhood_id;
            printf("%s\n", lookUpStr(curNeigh, 3));
        }
        printf("\t%-6d%-22s%-12s%-12s%-30s%-10d%-12s%-20s%-20s%-40s\n\n", siteId, tableType, surfMat, structMat, streetAve, neighId, ward, latitude, longitude, location);
    }

}





//-------------------------------EDITTABLEENTRY----------------------------------------------------

void editTableEntry(int tableID, char *memberName, char *newValue){
  int i, index;
  int isIn, newIndex;
  
  // Searching for the corresponding tableID
  for (i = 0; i < Db->picnicTableTable.cursize; i++){
    if (Db->picnicTableTable.data[i].tableId == tableID){
      index = i;
      break;
    }
  }
  
  // 1. If the memberName given matches siteId, we will change that int value
  if (strcmp(memberName, "siteId") == 0){
    // Replacing the old int value to the new int value of the newValue.
    Db->picnicTableTable.data[index].siteId = atoi(newValue);
  }
  
  // 4. tableTypeId, int
  // It will check to see if the id exists. If it does, it will change the id
  // If it doesn't, it will create a new type.
  else if (strcmp(memberName, "tableTypeId") == 0){
    isIn = 0;
    
    for (i = 0; i < Db->tableTypeTable.cursize; i++){
      if (strcmp(Db->tableTypeTable.data[i].name, newValue) == 0){
        isIn = 1;
      }
	  newIndex = i;
    }
    
          
    if (isIn == 1){
		printf("hi");
      // Replacing the old int value to the new int value of the newValue. 
      Db->picnicTableTable.data[index].tableTypeId = atoi(newValue);
    }
    else if (isIn == 0){
      // If the ID does not exist, we want to add it in.
      ++newIndex; // increment newIndex to point to one after the current existing data type
      strcpy(Db->tableTypeTable.data[newIndex].name, newValue);
      Db->tableTypeTable.data[newIndex].type_id = atoi(newValue);
      Db->picnicTableTable.data[index].tableTypeId = Db->tableTypeTable.data[newIndex].type_id;
	  
	  for (int j = 0; j < Db->tableTypeTable.cursize; j++){
		  printf("\nID: %d\n", Db->tableTypeTable.data[j].type_id);
		  printf("\nName: %s\n", Db->tableTypeTable.data[j].name);
	  }
    }
    
  }
  // 3. Surface mat, int
  else if (strcmp(memberName, "surfaceMaterialId") == 0){
    // Replacing the old int value to the new int value of the newValue. 
    Db->picnicTableTable.data[index].surfaceMaterialId = atoi(newValue);
  }
  
  // 4. Structural mat, int
  else if (strcmp(memberName, "structuralMaterialId") == 0){
    // Replacing the old int value to the new int value of the newValue. 
    Db->picnicTableTable.data[index].structuralMaterialId = atoi(newValue);
  }
  
  // 5. Street/Avenue, str
  else if (strcmp(memberName, "streetAvenue") == 0){
    // String copying the char newValue to replace the old value.
    strcpy(Db->picnicTableTable.data[index].streetAvenue, newValue);
  }
  
  // 6. Neighbourhood ID, int
  else if (strcmp(memberName, "neighbourhood_id") == 0){
    // Replacing the old int value to the new int value of the newValue.
    Db->picnicTableTable.data[index].neighbourhood_id = atoi(newValue);
  }
  
  // 7. Ward, str
  else if (strcmp(memberName, "ward") == 0){
    // String copying the char newValue to replace the old value.
    strcpy(Db->picnicTableTable.data[index].ward, newValue);
  }
  
  // 8. Latitude, str
  else if (strcmp(memberName, "latitude") == 0){
    // String copying the char newValue to replace the old value.
    strcpy(Db->picnicTableTable.data[index].latitude, newValue);
  }
  
  // 9. Longitude, str
  else if (strcmp(memberName, "longitude") == 0){
    // String copying the char newValue to replace the old value.
    strcpy(Db->picnicTableTable.data[index].longitude, newValue);
  }
  
  else{
    printf("The given member name was not found! Value could not be changed!\n");
    exit(1);
  }
}





/* destroys pointers in database */
void destroyData()
{
    free(Db->tableTypeTable.data);
    free(Db->surfaceMaterialTable.data);
    free(Db->structMaterialTable.data);
    free(Db->neighbourhoodTable.data);
    free(Db->picnicTableTable.data);
    initializer(0);
}
