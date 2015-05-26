#include<iostream>
#include<stdio.h>
#include<stdlib.h>
#include<pthread.h>
#include<stdint.h>
#include<map>
#include<vector>
#include<list>
#include"splay_tree.h"
using namespace std;

//=====Shared section=====
struct THREAD_INFO{
	int pid;
	char fname[32];
};
pthread_barrier_t *barr;
pthread_mutex_t m_WRITE_RD;
list<uint64_t> *L_INFINITE;//local infinity
map<uint64_t,uint64_t> G_ReuseDistance;//global
uint64_t G_INFINITE;
uint64_t G_MAX_DIST;
int NUM_THREAD;


void* compute_RD(void *input)
{
	//==========Inital section============
	struct THREAD_INFO *info = (THREAD_INFO*)input;
	int PID = info->pid;
	map<uint64_t,uint64_t> HASH_TABLE;//store last access time
	uint64_t now_t = 1;
	FILE *fp = fopen(info->fname,"r");
	if(!fp){
		printf("openfile fail!\n");
		exit(1);
	}
	//==========Variable section==========
	TREE_NODE *tree = NULL;
	map<uint64_t,uint64_t> L_ReuseDistance;//local
	uint64_t m_Address;
	uint64_t max_dist=0; 	
	char c;
	//==========Compute reuse distance==========
	//Phase 1, local reuse distance
	while(fscanf(fp,"%c%lx\n",&c,&m_Address) != EOF ){
		m_Address >>= 6;
		uint64_t dist=0;
		uint64_t last_t = HASH_TABLE[m_Address];
		//dist = infinite 
		if( last_t == 0 ){
			if(PID == 0)
				G_INFINITE++;
			else
				L_INFINITE[PID].push_back(m_Address);
			tree_insert(&tree,now_t,m_Address);
		}
		else{ 
			dist = distance_compute(&tree,last_t,true);
			tree_insert(&tree,now_t,m_Address);
			L_ReuseDistance[dist]++;
			if( dist > max_dist)
				max_dist = dist;
		}
		HASH_TABLE[m_Address] = now_t;
		now_t++;
	}
	fclose(fp);
	//write local RD to global RD
	pthread_mutex_lock(&m_WRITE_RD);
	for(uint64_t i= 0; i <= max_dist ; i++)
		G_ReuseDistance[i] += L_ReuseDistance[i];	
	if(max_dist > G_MAX_DIST)
		G_MAX_DIST = max_dist;
	pthread_mutex_unlock(&m_WRITE_RD);
	pthread_barrier_wait(&barr[NUM_THREAD]);
	//Phase2 process local infinity from right process
	for(uint64_t i=0; i<NUM_THREAD-PID-1 ;i++)
	{
		L_ReuseDistance.clear();
		//max_dist = 0;	
		uint64_t lsize = L_INFINITE[PID+1].size();;	
		pthread_barrier_wait(&barr[NUM_THREAD-i-1]);//each loop should start at the same time
		for(uint64_t j=0; j<lsize ;j++ )
		{
			m_Address = L_INFINITE[PID+1].front();//get infinity addr from right process
			L_INFINITE[PID+1].pop_front();
			uint64_t dist=0;
			uint64_t last_t = HASH_TABLE[m_Address];
			//dist = infinite 
			if( last_t == 0 ){
				if(PID == 0)
					G_INFINITE++;
				else 
					L_INFINITE[PID].push_back(m_Address);
			}
			else{ 
				dist = distance_compute(&tree,last_t,true) + j;
				L_ReuseDistance[dist]++;
				if( dist > max_dist)
					max_dist = dist;
			}
		}
		//write local RD to global RD
		pthread_mutex_lock(&m_WRITE_RD);
		if(max_dist > G_MAX_DIST)
			G_MAX_DIST = max_dist;
		for(uint64_t k= 0; k <= max_dist ; k++)
			G_ReuseDistance[k] += L_ReuseDistance[k];	
		pthread_mutex_unlock(&m_WRITE_RD);
		pthread_barrier_wait(&barr[NUM_THREAD-i-1]);//each loop should start at the same time
	}
	cout<<PID<<','<<"over!\n";
	//tree_free(&tree);
}

int main(int argc,char *argv[])
{
	NUM_THREAD = atoi(argv[1]);
	pthread_t *THREADS = new pthread_t[NUM_THREAD];	
	struct THREAD_INFO *P_INFO = new THREAD_INFO[NUM_THREAD];
	L_INFINITE = new list<uint64_t>[NUM_THREAD];
	barr = new pthread_barrier_t[NUM_THREAD+1];
	for(int i=0; i < NUM_THREAD+1;i++)
		pthread_barrier_init(&barr[i],NULL,i);
	pthread_mutex_init(&m_WRITE_RD,NULL);

	for(int i=0; i < NUM_THREAD; i++){
		P_INFO[i].pid = i;
		sprintf(P_INFO[i].fname,"%s%02d",argv[2],i);
		pthread_create(&THREADS[i],NULL,compute_RD,(void*)&P_INFO[i]);
	}
	for(int i=0; i < NUM_THREAD; i++)
		pthread_join(THREADS[i],NULL);
	
	//==========Output the result to file==========
	FILE *fp;
	char oname[32];
	if( argv[3][1] == '1')	//L1's trace file
		sprintf(oname,"reuse_dist_1");
	else 				  	//L2's trace file
		sprintf(oname,"reuse_dist_2_%s",argv[4]);
	fp = fopen(oname,"w");
	for(uint64_t i= 0; i <= G_MAX_DIST ; i++)
		fprintf(fp,"%-6lu %-6lu\n", i, G_ReuseDistance[i]);
	fprintf(fp,"-1 %-6lu\n", G_INFINITE);

		
	for(int i=0; i < NUM_THREAD+1;i++)
		pthread_barrier_destroy(&barr[i]);
	pthread_mutex_destroy(&m_WRITE_RD);
	delete THREADS;
	delete P_INFO;
}
