#include <stdio.h>
#include <stdlib.h>

//un mensaje con el contenido y el id de quien lo manda
typedef struct message_t{
	uint64_t send_id;
	void* msg;
	uint64_t size;
	struct message_t* next;
}message_t;

typedef message_t* msg_node; 

//la lista de mensajes de cada receiver
typedef struct mqNode{
	uint64_t id;
	msg_node messages;
	msg_node last_message;
	struct mqNode* next;
}mqNode; 

typedef mqNode* mq_node;

//la lista con todas las listas de mensajes
typedef struct msg_queue{
	mq_node head;
}msg_queue;

static msg_queue* mq;

void new_message(uint64_t sender, uint64_t receiver, uint64_t size, void* message);
mq_node getNode(uint64_t receiver);
void addMessage(uint64_t receiver, msg_node message);
void print_messages(uint64_t receiver);
msg_node read_message(uint64_t receiver, uint64_t sender);


//agrega un mensaje enviado por sender a la lista de mensajes de receiver
void new_message(uint64_t sender, uint64_t receiver, uint64_t size, void* message){
	
	msg_node newMsg = malloc(sizeof(struct message_t));
	newMsg->send_id = sender;
	newMsg->msg = message;
	newMsg->size = size;
	newMsg->next = NULL;
	
	addMessage(receiver, newMsg);
}

//devuelve el nodo con la lista de los mensajes de id receiver
mq_node getNode(uint64_t receiver){
	mq_node current = mq->head;
	
	if(current == NULL){
		mq_node first = malloc(sizeof(mqNode));
		first->id = receiver;
		first->messages = NULL;
		first->last_message=NULL;
		first->next = NULL;
		mq->head = first;
		return first;
	}
	
	
	while(current->next != NULL ){
		if(current->id == receiver){
			return current;
		}
		current = current->next;
	}
	
	if(current->id == receiver){
		return current;
	}
	
	mq_node newNode = malloc(sizeof(mqNode));
	newNode ->id = receiver;
	newNode-> messages = NULL;
	newNode-> next = NULL;
	current->next = newNode;
	return newNode;
	
}

//agrega un mensaje
void addMessage(uint64_t receiver, msg_node message){
	mq_node node = getNode(receiver);
	
	if(node->messages == NULL){
		node->messages = message;
		node->last_message = message;
		return;
	}
	
	node->last_message->next = message;
	node->last_message = message;

}

//imprime todos los mensajes de un id dado
void print_messages(uint64_t receiver){
	mq_node node = getNode(receiver);
	msg_node msg = node->messages;
	if(msg == NULL){
		printf("ES NULL\n");
	}
	printf("receiver %llu\n", receiver);
	while(msg != NULL){
		printf("sender: %llu\n", msg->send_id);
		msg = msg->next;
	}
}

//devuelve el primer mensaje de la cola de mensajes de receiver enviado por sender
msg_node read_message(uint64_t receiver, uint64_t sender){
	mq_node node = getNode(receiver);
	msg_node msgs= node ->messages;
	while(msgs != NULL){
		if(msgs->send_id == sender){
			return msgs;
		}
		msgs = msgs->next;
	}
	return NULL;
	
}





