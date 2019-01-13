/*
 *  Interface specification for your database implementation
 *
 *  DO NOT MODIFY ANYTHING THAT IS PART OF THE INTERFACE SPECIFICATION (UNLESS EXPLICITLY
 *  INDICATED).  TO DO SO WILL PREVENT YOUR CODE FROM LINKING WITH OUR CODE!!
 */


#ifndef DB_H
#define DB_H
/*
DECLARE AND TYPEDEF HERE THE STRUCTS Table, NeighbourhoodTable, AND PicnicTable
REFERED TO IN THE STRUCT DataBase BELOW. YOU MAY DECLARE ADDITIONAL STRUCTS AND FUNCTIONS AS NEEDED.
*/

// int and string variable
// int for specific type, string is the type itself
typedef struct {
    int type_id; // Start at 1
    char name[50];

}Table;


// NeighbourhoodTable: 2 strings
/*typedef struct {
  char *neighbourhood_id;
  char *neighbourhood_name;

}NeighbourhoodTable;*/


// PicnicTable: Everything in each line.
typedef struct {
    int tableId;
    int siteId;
    int tableTypeId;
    int surfaceMaterialId;
    int structuralMaterialId;
    char  streetAvenue[50];
    int  neighbourhood_id;
    char  neighbourhood_name[50];
    char  ward[50]; // If we want, we can have int ward instead (will atoi so best keep as is  -Nolan)
    char  latitude[50];
    char  longitude[50];
    char  location[50]; // don't need this, use (lat, long)

}PicnicTable;


// This points to the individual datas
typedef struct {
    int maxsize;
    int cursize;
    Table *data; // change to **data, then malloc for it

}TableArray;


// This points to the table itself
typedef struct {
    int maxsize;
    int cursize;
    PicnicTable *data; // **data too, malloc
	// Then Db->picnictable.data->members

}Ptarray;
/*
 * You may change the internal details of the struct below,
 * only keep it typedef'ed to DataBase
 */

typedef struct {
    TableArray tableTypeTable;
    TableArray surfaceMaterialTable;
    TableArray structMaterialTable;
    TableArray neighbourhoodTable;
    Ptarray picnicTableTable;
} DataBase;

/* Declare a global DataBase variable*/
/* That should be the only global variable declared*/
/* DB.c should have the definition of the variable*/
extern DataBase *Db;//****

// ive read that by default everything is extern unless you also plan to use static so not sure why it was provided -N


/*
 * Take the name of a .csv file as parameter and create and populate the Database
 * with the corresponding data set information.
 */
void importDB(char *fileName);

/*
 * Take the name of a .csv file as parameter and create a .csv file containing the
 * information of the Database.
 *  NOTE: the exported .csv file must be exactly the same as the original .csv file
 * from which the Database was created.
 */
void exportDB(char *fileName);

/*
 * Take the name of a member of the picnicTable entry and a value for that member
 * as parameters and return the number of entries in the picnicTable which have
 * that value in the memberName.
 * memberName: tableTypeTable->type, surfaceMaterialTable->type, structMaterialTable->table
 */
int countEntries(char *memberName, char *value);

/*
 * Take the name of a member of the picnicTable entry as an argument
 * and sort the table in ascending order of the entry values of that member.
 * The function must work for all member types (numeric or text).
 */
void sortByMember(char *memberName);

/*
 * Take a tableID, the name of a member of the picnicTable entry and a value for that
 * member as parameters, and find the entry which has that tableID and
 * change its memberName value to newValue.
 * memberName: tableTypeTable->type, surfaceMaterialTable->type, structMaterialTable->table
 */
void editTableEntry(int tableID, char *memberName, char *newValue);

/*
 * print a listing of picnic tables grouped by wards in ascending order.
 */
void reportByWard();

/*
 * print a listing of picnic tables grouped by neigbourhoods in ascending
 * alphabetical order.
 */
void reportByNeighbourhood();

void resize(int tableNum);

void initializer(int isNew);

const char *lookUpStr(int id, int tableNum);

#endif





