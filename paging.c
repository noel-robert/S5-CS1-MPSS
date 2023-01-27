#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

struct pageTable {
	int pageNumber;
	int frameNumber;
};

struct logicalAddress {
	int pageNumber;
	int offset;
};

int main() {
	// user inputs logical address consisting of pageno and offset
	struct logicalAddress logAddr;
	printf("Input page number and offset for logical address - ");
	scanf("%d", &logAddr.pageNumber);
	scanf("%d", &logAddr.offset);
	
	// user inputs page/frame size
	int pageSize;
	printf("Input page size - ");
	scanf("%d", &pageSize);
	
	// user inputs size of page table
	int pageTableSize;
	printf("Input no. of pages - ");
	scanf("%d", &pageTableSize);
	struct pageTable table[pageTableSize];
	
	// input page no. and frame no.
	printf("Input page number and frame number for %d pages\n", pageTableSize);
	for(int i=0; i<pageTableSize; i++) {
		scanf("%d", &table[i].pageNumber);
		scanf("%d", &table[i].frameNumber);
	}
	
	
	/*DEBUG START
	printf("\nverify");
	for(int i=0; i<pageTableSize; i++) {
		printf("%d %d \n", table[i].pageNumber, table[i].frameNumber);
	}
	printf("%d %d", logAddr.pageNumber, logAddr.offset);
	DEBUG END*/
	
	if (logAddr.offset >= pageSize) {
		printf("Invalid offset (offset > pagesize)\n"); 
		return 0;
	}

	// compare logical address page no. with page table page no.
	int pos;
	bool found = false;
	for(pos=0; pos<pageTableSize; pos++) {
		if(table[pos].pageNumber == logAddr.pageNumber) { found = true; break; }
	}
	if(found != true) {
		printf("Specified page number not found in page table\n");
		return 0;
	}
	
	
	// physical address = frame number * pagesize + offset
	int physicalAddress = table[pos].frameNumber * pageSize + logAddr.offset;
	printf("Physical Address = %d\n", physicalAddress);
	
	return 0;
}