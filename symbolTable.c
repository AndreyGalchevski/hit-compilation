#include "symbolTable.h"

void make_table(){
	HashTable* newHashTable = (HashTable*)malloc(sizeof(HashTable));;
	TableNode* newTableNode = (TableNode*)malloc(sizeof(TableNode));;
	initHashTable(newHashTable);
	newTableNode->hash_table = newHashTable;

	if(cur_table_node != NULL){
		newTableNode->father = cur_table_node;
	}
	else{
		newTableNode->father = NULL;
	}
	cur_table_node = newTableNode;
	printf("\n---- New Symbol table node created ----");
}

void initHashTable(HashTable* newHashTable)
{
	int i;
	for(i=0; i<TABLE_SIZE; i++)
		(*newHashTable)[i] = NULL;
}

void pop_table (){
	if(cur_table_node){
		TableNode* node_to_delete = cur_table_node;
		cur_table_node = cur_table_node->father;
		deleteHashTable(node_to_delete->hash_table);
		free(node_to_delete);
		printf("\n---- Top Symbol table node was droped ----");
	}
}

void deleteHashTable(HashTable* hash_table){
	TableEntry* current_entry = NULL;
	TableEntry* entery_for_delete = NULL;
	int i;
	
	for(i=0; i<TABLE_SIZE; i++)
	{
		current_entry = (*hash_table)[i];
		while(current_entry){
			entery_for_delete = current_entry;
			current_entry = current_entry->next;
			if(entery_for_delete->name){
				free(entery_for_delete->name);
			}
			free(entery_for_delete);
		}
	}
	free(hash_table);
}

TableEntry* insert (char* id_name){
	printf("\nSYMBOL TABLE: Trying to insert the ID name '%s' to this scope table", id_name);
	TableEntry* same_id_name_entry = lookup(cur_table_node->hash_table, id_name);
	if(same_id_name_entry){
		printf("\nSYMBOL TABLE: ID name '%s' already exists in this scope", id_name);
		return NULL;
	}
	else{		
		int hashed_key = hash(id_name);
		HashTable* curr_table = cur_table_node->hash_table;
		TableEntry* hashTableEntry = (*curr_table)[hashed_key];
		TableEntry* newEntry = (TableEntry*)malloc(sizeof(TableEntry));
		newEntry->name = (char*)malloc((strlen(id_name) + 1) * sizeof(char));
		strcpy(newEntry->name,id_name);
		newEntry->line = 0;

		if (hashTableEntry == NULL){
			newEntry->next = NULL;
			(*curr_table)[hashed_key] = newEntry;
			printf("\nSYMBOL TABLE: the id name '%s' was inserted to a new place (%d) in the scope table", id_name, hashed_key);
		}
		else{
			newEntry->next = hashTableEntry;
			(*curr_table)[hashed_key] = newEntry;
			printf("\nSYMBOL TABLE: the id name '%s' was inserted to the scope table in the top of existing entry (%d)", id_name, hashed_key);
		}
		return newEntry;
	}
}

TableEntry* lookup (HashTable* curr_table, char* id_name){
	printf("\nSYMBOL TABLE: Trying to find the id_name '%s' in the current scope", id_name);
	int hashed_key = hash(id_name);
	TableEntry* curr_entry = (*curr_table)[hashed_key];
	while(curr_entry != NULL){
		printf("\nSYMBOL TABLE: looking in hash entry %d: '%s' == '%s' ?", hashed_key, curr_entry->name, id_name);
		if( strcmp(curr_entry->name, id_name) == 0){
			printf("\nSYMBOL TABLE: the id name '%s' was found in this scope table, in entry [%d]", id_name, hashed_key);
			return curr_entry;
		}
		curr_entry = curr_entry->next;
	}
	printf("\nSYMBOL TABLE: the id name '%s' was not found in this scope table", id_name);
	return NULL;
}

TableEntry* find (char* id_name){
	printf("\nSYMBOL TABLE: Trying to find '%s' in any of the scopes", id_name);
	TableNode* curr_node = cur_table_node;
	HashTable* curr_hash_table;
	TableEntry* entry;
	while (curr_node != NULL){
		curr_hash_table = curr_node->hash_table;
		entry = lookup(curr_hash_table,id_name);
		if(entry != NULL){
			printf("\nSYMBOL TABLE: ID name '%s' found", id_name);
				return entry;
		}
		curr_node = curr_node->father;
	}
	printf("\nSYMBOL TABLE: ID name '%s' wasn't found in any of the scopes", id_name);
	return NULL;
}

void set_id_type (TableEntry* id_entry, int id_type){
	if(id_entry){
		printf("\nSYMBOL TABLE: set the id type to: %d", id_type);
		id_entry->datatype = id_type;
	}
	else{
		printf("\nSYMBOL TABLE: error! entry pointer in NULL (set_id_type)");
	}
}

int get_id_type (TableEntry* id_entry){
	if(id_entry){
		return id_entry->datatype;
	}
	printf("\nSYMBOL TABLE: error! entry pointer in NULL (get_id_type)");
	return -1;
}

void setLine(TableEntry *id_entry, int line)
{
	if(id_entry){
		id_entry->line = line;
	}
	else{
		printf("\nSYMBOL TABLE: error! entry pointer in NULL (setLine)");
	}
}

int getLine(TableEntry *id_entry)
{
	if(id_entry){
		return id_entry->line;
	}
	printf("\nSYMBOL TABLE: error! entry pointer in NULL (getLine)");
	return -1;
}

int is_integer(TableEntry* id_entry){
	if(id_entry){
		int datatype = get_id_type(id_entry);
		if(datatype == INT_NUM_T){
			return 1;
		}
		else{
			return 0;
		}
	}
	printf("\nSYMBOL TABLE: error! entry pointer in NULL (is_integer)");
	return -1;
}



int hash(char *lexeme) {
	const int k = 100;

	int i = 0;
	int n = strlen(lexeme);
	uint64_t h = 0;

	for(; i < n; i++) {
		h = k * h + lexeme[i];
	}

	h = h & 0x3FFFFFFF;
	
	return h % TABLE_SIZE;
}