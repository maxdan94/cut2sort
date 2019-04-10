#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <strings.h>//to use "bzero"
#include <time.h>//to estimate the runing time

#include "bisection.h"
#include "struct.h"

#define NLINKS 100000000 //maximum number of edges for memory allocation, will increase if needed

//compute the maximum of three unsigned long
inline unsigned long max3(unsigned long a,unsigned long b,unsigned long c){
	a=(a>b) ? a : b;
	return (a>c) ? a : c;
}

//reading the edgelist from file
adjlist* readedgelist(char* input){
	unsigned long e1=NLINKS;
	adjlist *g=malloc(sizeof(adjlist));
	FILE *file;

	g->n=0;
	g->e=0;
	file=fopen(input,"r");
	g->edges=malloc(e1*sizeof(edge));
	while (fscanf(file,"%lu %lu", &(g->edges[g->e].s), &(g->edges[g->e].t))==2) {
		g->n=max3(g->n,g->edges[g->e].s,g->edges[g->e].t);
		if (g->e++==e1) {
			e1+=NLINKS;
			g->edges=realloc(g->edges,e1*sizeof(edge));
		}
	}
	fclose(file);
	g->n++;

	g->edges=realloc(g->edges,g->e*sizeof(edge));

	return g;
}

//building the adjacency matrix
void mkadjlist(adjlist* g){
	unsigned long i,u,v;
	g->map=malloc(g->n*sizeof(unsigned long));
	unsigned long *new=malloc(g->n*sizeof(unsigned long)),*d;
	bool *b=calloc(g->n,sizeof(bool));

	g->n=0;///////////////
	for (i=0;i<g->e;i++) {
		if (b[g->edges[i].s]==0){
			b[g->edges[i].s]=1;
			g->map[g->n]=g->edges[i].s;
			new[g->edges[i].s]=g->n++;
		}
		if (b[g->edges[i].t]==0){
			b[g->edges[i].t]=1;
			g->map[g->n]=g->edges[i].t;
			new[g->edges[i].t]=g->n++;
		}
		g->edges[i].s=new[g->edges[i].s];
		g->edges[i].t=new[g->edges[i].t];
	}
	g->map=realloc(g->map,g->n*sizeof(unsigned long));

	bzero(new,g->n*sizeof(unsigned long));
	d=new;

	for (i=0;i<g->e;i++) {
		d[g->edges[i].s]++;
		d[g->edges[i].t]++;
	}

	g->cd=malloc((g->n+1)*sizeof(unsigned long));
	g->cd[0]=0;
	for (i=1;i<g->n+1;i++) {
		g->cd[i]=g->cd[i-1]+d[i-1];
		d[i-1]=0;
	}

	g->adj=malloc(2*g->e*sizeof(unsigned long));

	for (i=0;i<g->e;i++) {
		u=g->edges[i].s;
		v=g->edges[i].t;
		g->adj[ g->cd[u] + d[u]++ ]=v;
		g->adj[ g->cd[v] + d[v]++ ]=u;
	}

	free(new);
	free(b);
}


//building the adjacency matrix
void mkadjlist2(adjlist* g,unsigned long* new){
	unsigned long i,u,v;
	unsigned long *d=calloc(g->n,sizeof(unsigned long));

	for (i=0;i<g->e;i++) {
		g->edges[i].s=new[g->edges[i].s];
		g->edges[i].t=new[g->edges[i].t];
	}

	for (i=0;i<g->e;i++) {
		d[g->edges[i].s]++;
		d[g->edges[i].t]++;
	}

	g->cd=malloc((g->n+1)*sizeof(unsigned long));
	g->cd[0]=0;
	for (i=1;i<g->n+1;i++) {
		g->cd[i]=g->cd[i-1]+d[i-1];
		d[i-1]=0;
	}

	g->adj=malloc(2*g->e*sizeof(unsigned long));

	for (i=0;i<g->e;i++) {
		u=g->edges[i].s;
		v=g->edges[i].t;
		g->adj[ g->cd[u] + d[u]++ ]=v;
		g->adj[ g->cd[v] + d[v]++ ]=u;
	}

	free(d);
}



//freeing memory
void free_adjlist(adjlist *g){
	free(g->edges);
	free(g->cd);
	free(g->adj);
	free(g->map);
	free(g);
}


//Make the 2 subgraphs of graph g using the labels "lab"
adjlist** mkkids(adjlist* g, bool* lab){
	unsigned long i,e1=NLINKS,e2=NLINKS;

	adjlist** clust=malloc(2*sizeof(adjlist*));
	clust[0]=malloc(sizeof(adjlist));
	clust[1]=malloc(sizeof(adjlist));

	clust[0]->edges=malloc(e1*sizeof(edge));
	clust[1]->edges=malloc(e2*sizeof(edge));

	clust[0]->e=0;
	clust[1]->e=0;
	for (i=0;i<g->e;i++) {
		if (lab[g->edges[i].s]==lab[g->edges[i].t]){
			if (lab[g->edges[i].s]==0){
				clust[0]->edges[clust[0]->e]=g->edges[i];
				if (clust[0]->e++==e1) {
					e1+=NLINKS;
					clust[0]->edges=realloc(clust[0]->edges,e1*sizeof(edge));
				}
			}
			else{
				clust[1]->edges[clust[1]->e]=g->edges[i];
				if (clust[1]->e++==e2) {
					e2+=NLINKS;
					clust[1]->edges=realloc(clust[1]->edges,e2*sizeof(edge));
				}
			}
		}
	}

	clust[0]->edges=realloc(clust[0]->edges,clust[0]->e*sizeof(edge));
	clust[1]->edges=realloc(clust[1]->edges,clust[1]->e*sizeof(edge));

	clust[0]->n=0;
	clust[1]->n=0;
	for (i=0;i<g->n;i++){
		if (lab[i]==0){
			(clust[0]->n)++;
		}
		else{
			(clust[1]->n)++;
		}
	}
	//printf("%lu %lu\n",clust[0]->n,clust[1]->n);
	unsigned long *new=malloc(g->n*sizeof(unsigned long));
	unsigned long u=0,v=0;
	clust[0]->map=malloc(clust[0]->n*sizeof(unsigned long));
	clust[1]->map=malloc(clust[1]->n*sizeof(unsigned long));
	for (i=0;i<g->n;i++){
		if (lab[i]==0){
			clust[0]->map[u]=g->map[i];
			new[i]=u++;
		}
		else{
			clust[1]->map[v]=g->map[i];
			new[i]=v++;
		}
	}

	mkadjlist2(clust[0],new);
	mkadjlist2(clust[1],new);

	free(new);

	return clust;
}

//printing line (corresponding to one tree's node and associated set of nodes) in file
void printres(unsigned long n, unsigned long *map, char* output){
	unsigned long u;
	FILE* file=fopen(output,"w");
	for (u=0;u<n;u++){
		fprintf(file,"%lu %lu\n",u,map[u]);
	}
	fclose(file);
}

//recursive function
void recurs(bisection bisec, adjlist* g, unsigned long n,unsigned long* map){

	if (g->e==0){
//printf("whhhaattt??? %lu\n",g->n);
		unsigned long u;

		for (u=0;u<g->n;u++){
//printf("%lu %lu\n",g->map[u],u);
			map[n++]=g->map[u];
		}

		free_adjlist(g);
	}
	else{
		bool* lab=bisec(g);
		adjlist** clust=mkkids(g,lab);
		free_adjlist(g);
		free(lab);

		unsigned long n2=clust[0]->n;
		recurs(bisec, clust[0],n,map);
		n+=n2;

		recurs(bisec, clust[1],n,map);
	}
}

//main function
int main(int argc,char** argv){
	adjlist* g;
	bisection bisec;
	unsigned long n;
	unsigned long *map;

	time_t t1=time(NULL),t2;
	srand(time(NULL));

	if (argc==3)
		bisec=choosebisection("1");
	else if (argc==4)
		bisec=choosebisection(argv[3]);
	else{
		printf("Command line arguments are not valid.\n");
		exit(1);
	}

	printf("Reading edgelist from file %s\n",argv[1]);
	g=readedgelist(argv[1]);

	printf("Building adjacency array\n");
	map=malloc(g->n*sizeof(unsigned long));
	mkadjlist(g);
	n=g->n;

	printf("Number of nodes: %lu\n",g->n);
	printf("Number of edges: %lu\n",g->e);

	printf("Starting recursive bisections\n");
	recurs(bisec, g, 0, map);

	printf("Prints resulting order in file %s\n",argv[2]);
	printres(n,map,argv[2]);

	t2=time(NULL);

	printf("- Overall time = %ldh%ldm%lds\n",(t2-t1)/3600,((t2-t1)%3600)/60,((t2-t1)%60));

	return 0;
}

