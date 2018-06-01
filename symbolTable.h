#ifndef SYMBOLTABLE_H
#define SYMBOLTABLE_H

#include "token.h"
#include "parserUtils.h"

#define TABLE_SIZE 1021


/*an entry to store the variable name, its datatype, and its line of declaration */
typedef struct TableEntry {
	char *name;
	int datatype;
	int line;
	int constant;
	struct TableEntry* next;
}TableEntry;

/* HashTable is a hash table built from pointers to table enteries,
	each table entry is a one way linked list */
typedef TableEntry* HashTable[TABLE_SIZE];

/* a table node represent a single Symbol Table in a stack,
	one single table node is dedicated to a single scope
	each node is linked with its "father" node */
typedef struct TableNode {
	HashTable* hash_table;
	struct TableNode* father;
}TableNode;

TableNode* cur_table_node = NULL;

void make_table ();

void pop_table ();

TableEntry* insert (char* id_name);

TableEntry* lookup (HashTable* curr_table, char* id_name);

TableEntry* find (char* id_name);

void set_id_type (TableEntry* id_entry, int datatype);

int get_id_type (TableEntry* id_entry);

int is_integer(TableEntry* id_entry);

void initHashTable(HashTable* newHashTable);
void deleteHashTable(HashTable* hash_table);
int hash(char *lexeme);

#endif