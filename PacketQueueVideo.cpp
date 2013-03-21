#include "PacketQueueVideo.h"

PacketQueueVideo::PacketQueueVideo(Utility *manager)
{

	PacketQueueVideo::manager = manager;
	_mutex = new QMutex();
	_cond = new QWaitCondition();
}

PacketQueueVideo::PacketQueueVideo(void)
{
	manager = nullptr;
	_mutex = new QMutex();
	_cond = new QWaitCondition();
}


PacketQueueVideo::~PacketQueueVideo(void)
{
}

int PacketQueueVideo::Put(AVPacket *pkt){

	_mutex->lock();

	queue.push_back(*pkt);
														
	_cond->wakeOne();									//Invio una signal in modo da dire che c'� qualcosa prelevare al processo get
								
	_mutex->unlock();									//Esco dalla sezione critica

	return 0;
}

int PacketQueueVideo::Get(AVPacket *pkt, int block){

	AVPacket prelevato;
																				
	_mutex->lock();														//Entro nella sezione critica per accedere in modo esclusivo alla lista

	if(!queue.empty()) {
		prelevato = queue.front();										//ottengo il primo elemento
		queue.pop_front();												//elimino dalla lista elemento preso
		*pkt = prelevato;												//ottengo un puntatore all'oggetto prelevato
	}
	else if (!block) {													//Questo � un modo per evitare la wait, se nella chiamata di funzione si mette 1 nel parametro block nel caso non trovi 
		
	}
	else{																//caso lista vuota, mi metto in attesa
		_cond->wait(_mutex);
	}
	
	_mutex->unlock();

	return 0;
}

/**
ritorna la dimensione della lista
*/
int PacketQueueVideo::GetSize(){

	return queue.size();

}

QMutex* PacketQueueVideo::getMutex(){
	return _mutex;	
};
	
QWaitCondition* PacketQueueVideo::GetCond(){
	return _cond;	
};