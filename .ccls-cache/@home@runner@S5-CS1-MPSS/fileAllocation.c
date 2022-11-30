// Experiment No. 1
// File Allocaton Methods

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

struct file {
	char name[5];
	int startAddress;
	int blockCount;
	int assigned;	// 1->assigned, 0->not assigned
	int indexBlockAddress;
};
struct block {
  int status; // 1->occupied, 0->unoccupied
};

void contiguousAllocation();
void linkedAllocation();
void indexedAllocation();

int main() { 
	// contiguousAllocation(); 
	// linkedAllocation();
  indexedAllocation();
	return 0;
}

void contiguousAllocation() {
	int size = 32;		// memory size
	struct block memory[size];
	for(int i=0; i<size; i++) memory[i].status = 0;
 
	int n = 6;		// total no. of files
	struct file files[n];

	// input details
	for(int i=0; i<n; i++) {
		scanf("%s", files[i].name); scanf("%d", &files[i].startAddress); scanf("%d", &files[i].blockCount);
	}

	// allocate blocks
	for(int i=0; i<n; i++) {
		// check overflow condition
		if((files[i].startAddress+files[i].blockCount-1) >= size) {	
			files[i].assigned = 0; continue;
		}

		// check if contiguous allocation can be done
		bool canBeStored = true;
		for(int j=files[i].startAddress; j<(files[i].startAddress+files[i].blockCount); j++) {
			if(memory[j].status == 1) { canBeStored = false; files[i].assigned = 0; break; }
		}

		// do contiguous allocation, if possible
		if(canBeStored == true) {
			for(int j=files[i].startAddress; j<(files[i].startAddress+files[i].blockCount); j++) memory[j].status = 1;
			files[i].assigned = 1;
		}
	}

	// display
	for(int i=0; i<n; i++) {
		if(files[i].assigned == 1) {
			printf("%s %d %d\t", files[i].name, files[i].startAddress, files[i].blockCount);
			int j;
			for(j=files[i].startAddress; j<(files[i].startAddress+files[i].blockCount-1); j++) printf("%d-", j);
			printf("%d\n", j);
		}	else {
			printf("%s Cannot be stored\n", files[i].name);
		}
	}
}

void linkedAllocation() {
	int size = 32;		// memory size
	struct block memory[size];
	for(int i=0; i<size; i++) memory[i].status = 0;
  int emptySpaces = size;		// all locations in memory are unoccupied at the moment
	
	int n = 6;		// total no. of files
	struct file files[n];

	// input details
	for(int i=0; i<n; i++) {
		scanf("%s", files[i].name); scanf("%d", &files[i].blockCount);
	}
 
	int addressPathTable[n][size]; // to store paths
	for(int i=0; i<n; i++) {
		// check if there is enough space to store file
		if(emptySpaces < files[i].blockCount) {
			files[i].assigned = 0;
			continue;
		}
		
		// generate random addresses. check to make sure these are unoccupied
		int k = 0;
		while(k < files[i].blockCount) {
			int addr = rand()%size;
			if(memory[addr].status == 0) {
				addressPathTable[i][k] = addr; memory[addr].status = 1;
			} else { continue; }
			k++;
		}

		files[i].assigned = 1;
		emptySpaces--;
	}

	// display
	for(int i=0; i<n; i++) {
		if(files[i].assigned == 1) {
			printf("%s %d\t", files[i].name, files[i].blockCount);
			int j;
			for(j=0; j<files[i].blockCount-1; j++) printf("%d-", addressPathTable[i][j]);
			printf("%d\n", addressPathTable[i][j]);
		}	else {
			printf("%s Cannot be stored\n", files[i].name);
		}
	}
}

void indexedAllocation() {
	int size = 32;		// memory size
	struct block memory[size];
	for(int i=0; i<size; i++) memory[i].status = 0;
	int emptySpaces = size;		// all locations in memory are unoccupied at the moment
 
	int n = 6;		// total no. of files
	struct file files[n];

	// input details
	for(int i=0; i<n; i++) {
		scanf("%s", files[i].name); scanf("%d", &files[i].indexBlockAddress); 
		scanf("%d", &files[i].blockCount); memory[files[i].indexBlockAddress].status = 1;
	}

	int addressPathTable[n][size]; // to store paths
	// allocate files
	for(int i=0; i<n; i++) {
		// check if there is enough space to store file
		if(emptySpaces < files[i].blockCount) {		
			// not enough space
			files[i].assigned = 0;
			continue;
		}

		// allocate other blocks
		int k = 0;
		while(k < files[i].blockCount) {
			int addr = rand()%size;
			if(memory[addr].status == 0) {
				addressPathTable[i][k] = addr; memory[addr].status = 1;
			} else { continue; }
			k++;
		}

		files[i].assigned = 1;
		emptySpaces--;
	}

	// display
	for(int i=0; i<n; i++) {
		if(files[i].assigned == 1) {
			printf("%s %d %d\t", files[i].name, files[i].indexBlockAddress, files[i].blockCount);
			int j;
			for(j=0; j<files[i].blockCount-1; j++) printf("%d-", addressPathTable[i][j]);
			printf("%d\n", addressPathTable[i][j]);
		}	else {
			printf("%s Cannot be stored\n", files[i].name);
		}
	}
}

/*
test output
count 0 2
tr 14 3
main 19 6
list 25 4
f 6 2
temp 17 4
*/
