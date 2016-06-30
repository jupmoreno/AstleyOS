#ifndef _IPC_H_
#define _IPC_H_

#include <alloc.h>

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
mq_node get_mq(uint64_t receiver);
void addMessage(uint64_t receiver, msg_node message);
void print_messages(uint64_t receiver);
msg_node read_message(uint64_t receiver, uint64_t sender);
msg_node read_messages(uint64_t receiver, uint64_t sender);
void delete_mq(uint64_t receiver);
msg_node read_next_message(uint64_t receiver);

#endif