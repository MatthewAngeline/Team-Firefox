struct pcb {
	char name[50];
	char pcbClass[50];
	//higher number = higher priority
	int priority;
	char state[50];
	unsigned char* stack;
	//next is towards tail of queue, prev is towards head
	struct pcb* nextPCB;
	struct pcb* prevPCB;
}pcb;

struct stack{
	unsigned char* head;
	unsigned char* base;
	
}stack;
struct queue{
	int count;
	struct pcb* head;
	struct pcb* tail;
}queue;
void addToQueue(struct queue Q, struct pcb PCB);

