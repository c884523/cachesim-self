#include<stdio.h>
#include<stdlib.h>
#include<stdint.h>
#include<iostream>
#include<map>
#include<queue>
#include<list>
#include<vector>
using namespace std;

int main(int argc,char*argv[])
{
	FILE *fp = fopen(argv[1],"r");
	if(!fp){
		printf("開檔失敗\n");
		exit(1);
	}

	list<uint64_t> Stack;
	map<uint32_t,uint32_t> ReuseDistance;
	uint64_t m_Address;
	int infinite_dis=0,max_dist=0; 	

	int counts=0;
	char c;
	while(fscanf(fp,"%c%lx\n",&c,&m_Address) != EOF ){
		counts++;
		printf("%c %lx\n",c,m_Address);
		uint32_t dist=0;
		bool has_find=false;
		if(!Stack.empty()){
			for(list<uint64_t>::iterator lit = Stack.begin() ; lit != Stack.end(); lit++)
			{	
				if(*lit == m_Address){
					Stack.push_front(m_Address);
					Stack.erase(lit);
					has_find=true;
					break;
				}
				dist++;
			}	
		}
		
		//dist = infinite
		if( !has_find ){
			Stack.push_front(m_Address);
			infinite_dis++;
		}
		else{ 
			ReuseDistance[dist]++;
			if( dist > max_dist)
				max_dist = dist;
		}
	}
	fclose(fp);
	printf("%d\n",counts);
	char oname[30];
	if( argv[2][1] == '1')	//L1's trace
		sprintf(oname,"reuse_dist_1");
	else 				  	//L2's trace
		sprintf(oname,"reuse_dist_2_%s",argv[3]);
	fp = fopen(oname,"w");
	for(int i= 0; i <= max_dist ; i++)
		fprintf(fp,"%-6d %-6d\n", i, ReuseDistance[i]);
	fprintf(fp,"-1 %-6d\n", infinite_dis);
	/*//Catch interesting data address into the MAP(Stack Distance)
	  map<uint64_t,map<uint32_t,uint32_t> > ReuseDistance;
	  map<uint64_t,map<uint32_t,uint32_t> >::iterator mm_it;
	  map<uint32_t,uint32_t>::iterator m_it;
	  list<uint64_t> Stack;
	  uint64_t m_Address;
	  while(!feof(fp)){
	  fscanf(fp,"%ld",&m_Address);	
	  fscanf(fp,"%lx",&m_Address);
	  ReuseDistance[m_Address][0xffffffff] = 0;
	  }
	  fclose(fp);
	//Start to compute ReuseDistance
	fp = fopen("All_trace","r");
	while(!feof(fp)){
	fscanf(fp,"%lx",&m_Address);
	//printf("%lx\n",m_Address);
	list<uint64_t>::iterator lit = Stack.begin();
	uint32_t dist=0;
	while(lit != Stack.end()){
	if(*lit == m_Address){
	Stack.push_front(m_Address);
	Stack.erase(lit);
	break;
	}
	lit++;
	dist++;
	}
	//dist = infinite
	if(lit == Stack.end()){
	Stack.push_front(m_Address);
	dist = 0xffffffff;
	}
	//m_Address is interesting
	mm_it = ReuseDistance.find(m_Address); 
	if(mm_it != ReuseDistance.end()){
	m_it = mm_it->second.find(dist);
	if(m_it == mm_it->second.end())
	mm_it->second.insert(pair<uint32_t,uint32_t>(dist,1));
	else 
	m_it->second++;
	}
	}
	fclose(fp);
	//Output the result
	char out_name[32];
	for(mm_it = ReuseDistance.begin(); mm_it != ReuseDistance.end() ;mm_it++){
	sprintf(out_name,"./reuse_distance_log/%lx",mm_it->first);
	fp = fopen(out_name,"w");
	for(m_it = mm_it->second.begin(); m_it != mm_it->second.end() ;m_it++)
	fprintf(fp,"%u %u\n",m_it->first,m_it->second);
	fclose(fp);
	}*/
}
