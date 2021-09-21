typedef struct pcb {
	char name[50];
	char pcbClass[50];
	//higher number = higher priority
	int priority;
	char state[50];
	unsigned char stack[1024];
	unsigned char* stackBase;
	unsigned char* stackHead;
	//next is towards tail of queue, prev is towards head
	struct pcb* nextPCB;
	struct pcb* prevPCB;
}pcb;


typedef struct queue{
	int count;
	struct pcb* head;
	struct pcb* tail;
}queue;

queue* getReadyQueue();
int allocatePCB();

int freePCB(pcb* PCB);

struct pcb* setupPCB(char name[], char pcbClass[], int priority);

struct pcb* findPCB(char name[]);

void addToReadyQueue(pcb* PCB);

void addToBlockedQueue(pcb* PCB);

int removeFromQueue(pcb* PCB);


