// Experiment No. 1 - File Allocation
// Noel John Robert


#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

struct file {
	char name[5];		// name of file, should be within 5
	int startAddress;	// starting address of file
	int blockCount;		// no. of blocks required
	int assigned;		// 1->assigned, 0->not assigned
	int nextAddress;	// address of next block
	int indexBlockAddress;	// address of index block
};

struct block {
	int status;			// 1->occupied, 0->free
};

void contiguousAllocation();
void indexedAllocation();
void linkedAllocation();

int main() {
	int choice = 0;
	
	do {
		printf("1. Contiguous Allocation \n2. Indexed Allocation \n3. Linked Allocation \n4. Exit \n");
		scanf("%d", &choice);
		
		switch(choice) {
			case 1 : contiguousAllocation(); printf("\n"); break;
			case 2 : indexedAllocation(); printf("\n"); break;
			case 3 : linkedAllocation(); printf("\n"); break;
			case 4 : printf("Exiting on user input\n"); break;
			default : printf("Wrong code entered\n\n"); break;
		}
	} while(choice != 4);
	

	return 0;
}

// Contiguous File Allocation
void contiguousAllocation() {
	// input main memory size
	int size; printf("Input memory size "); scanf("%d", &size); 
	struct block memory[size];
	for(int i=0; i<size; i++) memory[i].status=0;
	
	// input file details
	int n; printf("Input no. of files "); scanf("%d", &n);
	struct file files[n];
	for(int i=0; i<n; i++) {
		printf("Input file name, starting address, and blocks required for %dth file -- ", (i+1));
		scanf("%s", files[i].name); scanf("%d", &files[i].startAddress); scanf("%d", &files[i].blockCount);
		// printf("\n");
	}
	
	// allocate blocks
	for(int i=0; i<n; i++) {
		// overflow condition
		if ((files[i].startAddress + files[i].blockCount - 1) >= size) {
			files[i].assigned = 0;
			continue;
		}
		
		// checking for space in memory
		bool flag = true;
		for(int j=files[i].startAddress; j<(files[i].startAddress+files[i].blockCount-1); j++) {
			if(memory[j].status == 1) {	// file cannot be assigned
				flag = false;
				break;
			}
		}
		if(flag == false) {			// no space
			files[i].assigned = 0;
			continue;
		} else {					// can be stored
			files[i].assigned = 1;
			for(int j=files[i].startAddress; j<(files[i].startAddress+files[i].blockCount-1); j++)
				memory[j].status = 1;
		}
	}
	
	// display
	printf("name\tstartAddress\tblockCount\n");
	for(int i=0; i<n; i++) {
		printf("%s\t%d\t%d\t\t", files[i].name, files[i].startAddress, files[i].blockCount);
		if(files[i].assigned == 1) {
			int j;
			for(j=files[i].startAddress; j<(files[i].startAddress+files[i].blockCount-1); j++) {
				printf("%d-", j);
			}
			printf("%d\n", j);
		}
		else
			printf("%s	File cannot be stored\n", files[i].name);
	}
}

// Linked File Allocation
void linkedAllocation() {
	// input main memory size
	int size; printf("Input memory size "); scanf("%d", &size); 
	struct block memory[size];
	for(int i=0; i<size; i++) memory[i].status=0;
	int emptySpaces = size;
	
	// input file details
	int n; printf("Input no. of files "); scanf("%d", &n);
	struct file files[n];
	for(int i=0; i<n; i++) {
		printf("Input file name and blocks required for %dth file -- ", (i+1));
		scanf("%s", files[i].name); scanf("%d", &files[i].blockCount);
	}
	
	int addressPathTable[n][size];		// to store paths
	for(int i=0; i<n; i++) {
		// check if there is enough space to store the file
		if(emptySpaces < files[i].blockCount) {
			// not enough space
			files[i].assigned = 0;
			continue;
		}
		
		// generate random addresses to store other blocks
		int k = 0;
		while(k < files[i].blockCount) {
			int addr = rand()%size;
			if(memory[addr].status == 0) {
				addressPathTable[i][k] = addr;
				memory[addr].status = 1;
			} else { continue; }
			k++;
			emptySpaces--;
		}
		files[i].assigned = 1;
	}
	
	// display
	printf("name\tblockCount\taddressList\n");
	for(int i=0; i<n; i++) {
		printf("%s\t%d\t\t", files[i].name, files[i].blockCount);
		if(files[i].assigned == 1) {
			int j;
			for(j=0; j<files[i].blockCount-1; j++) {
				printf("%d-", addressPathTable[i][j]);
			}
			printf("%d\n", addressPathTable[i][j]);
		}
		else {
			printf("%s	File cannot be stored\n", files[i].name);
		}		
	}
}

// Indexed File Allocation
void indexedAllocation() {
	// input main memory size
	int size; printf("Input memory size "); scanf("%d", &size); 
	struct block memory[size];
	for(int i=0; i<size; i++) memory[i].status=0;
	int emptySpaces = size;
	
	// input file details
	int n; printf("Input no. of files "); scanf("%d", &n);
	struct file files[n];
	for(int i=0; i<n; i++) {
		printf("Input file name, index block address, and blocks required for %dth file -- ", (i+1));
		scanf("%s", files[i].name); scanf("%d", &files[i].indexBlockAddress); 
		memory[files[i].indexBlockAddress].status = 1; scanf("%d", &files[i].blockCount);
	}
	
	int addressPathTable[n][size];		// to store paths
	for(int i=0; i<n; i++) {
		// check if there is enough space to store the file
		if(emptySpaces < files[i].blockCount) {
			// not enough space or index block not free
			files[i].assigned = 0;
			continue;
		}
		
		// assigning index block
		// memory[files[i].indexBlockAddress].status = 1;
		
		// generate random addresses to store other blocks
		int k = 0;
		while(k < files[i].blockCount) {
			int addr = rand()%size;
			if(memory[addr].status == 0) {
				addressPathTable[i][k] = addr;
				memory[addr].status = 1;
			} else { continue; }
			k++;
			emptySpaces--;
		}
		files[i].assigned = 1;
	}
	
	// display
	printf("name\tindexBlockAddress\tblockCount\tcontentOfIndexBlock\n");
	for(int i=0; i<n; i++) {
		printf("%s\t%d\t%d\t\t", files[i].name, files[i].indexBlockAddress, files[i].blockCount);
		if(files[i].assigned == 1) {
			int j;
			for(j=0; j<files[i].blockCount-1; j++) {
				printf("%d-", addressPathTable[i][j]);
			}
			printf("%d\n", addressPathTable[i][j]);
		}
		else {
			printf("%s	File cannot be stored\n", files[i].name);
		}		
	}
}


/*OUTPUT
1. Contiguous Allocation 
2. Indexed Allocation 
3. Linked Allocation 
4. Exit 
1
Input memory size 32
Input no. of files 6
Input file name, starting address, and blocks required for 1th file -- count 0 2
Input file name, starting address, and blocks required for 2th file -- tr 14 3
Input file name, starting address, and blocks required for 3th file -- main 19 6
Input file name, starting address, and blocks required for 4th file -- list 25 4
Input file name, starting address, and blocks required for 5th file -- f 6 2
Input file name, starting address, and blocks required for 6th file -- temp 17 4
name startAddress blockCount
count	0	2     0-1
tr	14	3     14-15-16
main	19	6     19-20-21-22-23-24
list	25	4     25-26-27-28
f	6	2     6-7
temp	17	4     temp	File cannot be stored

1. Contiguous Allocation 
2. Indexed Allocation 
3. Linked Allocation 
4. Exit 
2
Input memory size 32
Input no. of files 6
Input file name, index block address, and blocks required for 1th file -- count 0 2
Input file name, index block address, and blocks required for 2th file -- tr 14 3
Input file name, index block address, and blocks required for 3th file -- main 19 6
Input file name, index block address, and blocks required for 4th file -- list 25 4
Input file name, index block address, and blocks required for 5th file -- f 6 2
Input file name, index block address, and blocks required for 6th file -- temp 17 4
name indexBlockAddress blockCount contentOfIndexBlock
count	0	2     7-9
tr	14	3     31-10-12
main	19	6     13-26-11-18-27-3
list	25	4     28-2-20-24
f	6	2     8-22
temp	17	4     23-5-29-30

1. Contiguous Allocation 
2. Indexed Allocation 
3. Linked Allocation 
4. Exit 
3
Input memory size 32
Input no. of files 6
Input file name and blocks required for 1th file -- count 2
Input file name and blocks required for 2th file -- tr 3
Input file name and blocks required for 3th file -- main 6
Input file name and blocks required for 4th file -- list 4
Input file name and blocks required for 5th file -- f 2
Input file name and blocks required for 6th file -- temp 4
name blockCount addressList
count	2     20-11
tr	3     18-13-6
main	6     27-17-14-2-1-29
list	4     28-7-16-9
f	2     30-23
temp	4     10-22-15-24

1. Contiguous Allocation 
2. Indexed Allocation 
3. Linked Allocation 
4. Exit 
4
Exiting on user input
*/
