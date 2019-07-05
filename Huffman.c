#include <stdio.h>
#include <stdlib.h>

typedef struct HuffNode{
    unsigned freq; 
    char data;
    struct HuffNode *left, *right;
} HuffNode;

typedef struct HuffHeap {
    unsigned size;
    unsigned capacity;
    HuffNode **heap;
} HuffHeap;

HuffNode* newNode(char data, unsigned freq){
    HuffNode* temp = (HuffNode*)malloc(sizeof(HuffNode));
    temp->data = data;
    temp->freq = freq;
    temp->left = temp->right = NULL;
    return temp;
}

HuffHeap* createHeap(unsigned capacity){
    HuffHeap* temp = (HuffHeap*)malloc(sizeof(HuffHeap));
    temp->size = 0;
    temp->capacity = capacity;
    temp->heap = (HuffNode**)malloc(capacity * sizeof(HuffNode*));
    return temp;
}
//-----------------------------------------------------------------------//
void freeTree(HuffNode *node){
    if(node == NULL)
        return;
    
    freeTree(node->left);
    freeTree(node->right);

    free(node);
}
//-----------------------------------------------------------------------//
void heapify(HuffHeap *heap, int index){
    int l = index*2+1;
    int r = index*2+2;
    int minInd = index;
    if(l < heap->size && heap->heap[l]->freq < heap->heap[index]->freq){
        minInd = l;
    }
    if(r < heap->size && heap->heap[r]->freq < heap->heap[minInd]->freq){
        minInd = r;
    }
    HuffNode* temp;
    if(minInd != index){
        temp = heap->heap[index];
        heap->heap[index] = heap->heap[minInd];
        heap->heap[minInd] = temp;
        heapify(heap, minInd);
    }
}

HuffHeap* buildMinHeap(char data[], unsigned freq[], unsigned size){
    HuffHeap *heap = createHeap(size);
    
    int i;
    for(i=0; i<size;i++){
        heap->heap[i] = newNode(data[i], freq[i]);
    }

    heap->size = size;

    int n = heap->size-1;

    for(i=(n-1)/2; i>=0; i--){
        heapify(heap,i);
    }

    return heap;
}

HuffNode* extractMin(HuffHeap *heap){
    HuffNode* temp = heap->heap[0]; 
    
    heap->size--;

    heap->heap[0] = heap->heap[heap->size]; 
    
    heapify(heap, 0); 

    return temp; 
}

void insertHeap(HuffHeap *heap, HuffNode *node){ 
    int i = heap->size; 
    while (i && node->freq < heap->heap[(i - 1) / 2]->freq) { 
        heap->heap[i] = heap->heap[(i - 1) / 2]; 
        i = (i - 1) / 2; 
    } 
    heap->size++;
    heap->heap[i] = node; 
}

HuffNode* buildHuffTree(char data[], unsigned freq[], unsigned size){
    HuffNode *left, *right, *top;
    
    HuffHeap *heap = buildMinHeap(data, freq, size);

    while(heap->size != 1){
        left = extractMin(heap);
        right = extractMin(heap);
        
        top = newNode('$', left->freq + right->freq);
        
        top->left = left;
        top->right = right;
        
        insertHeap(heap, top);
    }

    HuffNode* root = extractMin(heap);
    free(heap->heap);
    free(heap);
    return root;
}

//-----------------------------------------------------------------------//

int getNumberChars(char *fname){

	FILE *file = fopen(fname, "r");
        
	if(file == NULL){
		fclose(file);
		printf("ERROR: Null file");
		return 0;
	}
	int count;

    while(fgetc(file) != EOF){
		count++;
	}	

	fclose(file);
}

int getString(char* string, char *fname){
    
	FILE *file = fopen(fname, "r");
	if(file == NULL) {
		fclose(file);
		printf("ERROR: Null file\n");
        return 0;
	}
	
	char c;
	int i, strsize = sizeof(string);
	for(i = 0; (c = fgetc(file)) != EOF; i++){	
		if(i > strsize){
			fclose(file);
			printf("ERROR: buffer overflow\n");
            return 0;
		}
		string[i] = c;
	}
    fclose(file);

    return 1;
}
//-----------------------------------------------------------------------//

int main(int argv, char **args){
	if(argv != 2){
		printf("ERROR: Wrong number of arguments: must pass in one file\n");
		return EXIT_FAILURE;
	}	

	char *string = (char*)malloc(getNumberChars(args[1]));
	
	if(!getString(string, args[1])){
        return EXIT_FAILURE;
    };
	
    // TEST

	printf("%s",string);

    free(string);
		
}
