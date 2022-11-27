// Experiment No. 2
// Paging Techniques

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

struct logicalAddress {
	int pageNumber;
	int offset;
};

struct pageTable {
	int pageNumber;
	int frameNumber;
};

int main() {
	int pageTableSize;
	printf("Input page table size -> "); scanf("%d", &pageTableSize);
	struct pageTable table[pageTableSize];
	printf("Input %d sets of page no. and frame no. into table\n", pageTableSize);
	for (int i=0; i<pageTableSize; i++) {
		scanf("%d", &table[i].pageNumber); scanf("%d", &table[i].frameNumber);
	}

	struct logicalAddress logAd;
	printf("Input pageno. and offset values of logical address - ");
	scanf("%d", &logAd.pageNumber); scanf("%d", &logAd.offset);

	int pageSize;
	printf("Input page size (page size = frame size) - ");
	scanf("%d", &pageSize);

	bool found = false;
	int i;
	for (i=0; i<pageTableSize; i++) {
		if (table[i].pageNumber == logAd.pageNumber) { found = true; break; }
	}
	if (found == false) {
		printf("Match not found in page table\n\n");
		return 0;
	}
	int physicalAddress = table[i].frameNumber * pageSize + logAd.offset;
	printf("Physical address = %d\n\n", physicalAddress);
	
	return 0;
}