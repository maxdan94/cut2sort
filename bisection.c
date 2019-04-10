#include "bisection.h"


//generating n random labels (boolean values)
bool *init(adjlist *g){
	unsigned long i,n=g->n,s=0;
	bool *lab=malloc(n*sizeof(bool));

	//random side for each node:
	for (i=0;i<n;i++){
		lab[i]=rand() > (RAND_MAX / 2);
		if (lab[i]==0)
			s++;
	}

	//avoids all nodes on one side:
	if (s==n)
		lab[0]=1;
	else if (s==0)
		lab[0]=0;

	return lab;
}

//A greedy heuristic for sparsest cut
bool *greedySparsestcut(adjlist *g) {
	unsigned long n=g->n,u,v,i,s=0;
	double cut=0;
	unsigned iter=0;
	bool *lab=init(g);
	bool b;
	int *d0=calloc(g->n,sizeof(int)),*d1=calloc(g->n,sizeof(int));

	for (u=0;u<n;u++){
		if (lab[u]==0){
			s++;
		}
	}

	for (i=0;i<g->e;i++){
		u=g->edges[i].s;
		v=g->edges[i].t;
		if (lab[u]!=lab[v]){
			cut++;
		}
		if (lab[v]==0){
			d0[u]++;
		}
		else{
			d1[u]++;
		}
		if (lab[u]==0){
			d0[v]++;
		}
		else{
			d1[v]++;
		}
	}

	do {//greedy heuristics
		iter++;
		b=0;
		for (u=0;u<n;u++) {
			if (lab[u]==0 && s>1 && cut/(s*(n-s)) > (cut+d0[u]-d1[u])/((s-1)*(n-s+1))){
				b=1;
				s--;
				lab[u]=1;
				cut+=d0[u]-d1[u];
				for (i=g->cd[u];i<g->cd[u+1];i++){
					v=g->adj[i];
					d0[v]--;
					d1[v]++;
				}
			}
			else if (lab[u]==1 && n-s>1 && cut/(s*(n-s)) > (cut+d1[u]-d0[u])/((s+1)*(n-s-1))){
				b=1;
				s++;
				lab[u]=0;
				cut+=d1[u]-d0[u];
				for (i=g->cd[u];i<g->cd[u+1];i++){
					v=g->adj[i];
					d0[v]++;
					d1[v]--;
				}
			}
		}
	} while (b);

	//printf("Number of iterations = %u\n",iter);

	return lab;
}

//A greedy heuristic for densest cut
bool *greedyDensestcut(adjlist *g) {
	unsigned long n=g->n,u,v,i,s=0;
	double cut=0;
	unsigned iter=0;
	bool *lab=init(g);
	bool b;
	int *d0=calloc(g->n,sizeof(int)),*d1=calloc(g->n,sizeof(int));

	for (u=0;u<n;u++){
		if (lab[u]==0){
			s++;
		}
	}

	for (i=0;i<g->e;i++){
		u=g->edges[i].s;
		v=g->edges[i].t;
		if (lab[u]!=lab[v]){
			cut++;
		}
		if (lab[v]==0){
			d0[u]++;
		}
		else{
			d1[u]++;
		}
		if (lab[u]==0){
			d0[v]++;
		}
		else{
			d1[v]++;
		}
	}

	do {//greedy heuristics
		iter++;
		b=0;
		for (u=0;u<n;u++) {
			if (lab[u]==0 && s>1 && cut/(s*(n-s)) < (cut+d0[u]-d1[u])/((s-1)*(n-s+1))){
				b=1;
				s--;
				lab[u]=1;
				cut+=d0[u]-d1[u];
				for (i=g->cd[u];i<g->cd[u+1];i++){
					v=g->adj[i];
					d0[v]--;
					d1[v]++;
				}
			}
			else if (lab[u]==1 && n-s>1 && cut/(s*(n-s)) < (cut+d1[u]-d0[u])/((s+1)*(n-s-1))){
				b=1;
				s++;
				lab[u]=0;
				cut+=d1[u]-d0[u];
				for (i=g->cd[u];i<g->cd[u+1];i++){
					v=g->adj[i];
					d0[v]++;
					d1[v]--;
				}
			}
		}
	} while (b);

	//printf("Number of iterations = %u\n",iter);

	return lab;
}



//A greedy heuristic for maxcut
bool *greedyMaxcut(adjlist *g) {
	unsigned long n=g->n,u,v,i;
	double cut=0;
	unsigned iter=0;
	bool *lab=init(g);
	bool b;
	int *d0=calloc(g->n,sizeof(int)),*d1=calloc(g->n,sizeof(int));

	for (i=0;i<g->e;i++){
		u=g->edges[i].s;
		v=g->edges[i].t;
		if (lab[u]!=lab[v]){
			cut++;
		}
		if (lab[v]==0){
			d0[u]++;
		}
		else{
			d1[u]++;
		}
		if (lab[u]==0){
			d0[v]++;
		}
		else{
			d1[v]++;
		}
	}

	do {//greedy heuristics
		iter++;
		b=0;
		for (u=0;u<n;u++) {
			if (lab[u]==0 && d1[u] < d0[u] ){
				b=1;
				lab[u]=1;
				cut+=d0[u]-d1[u];
				for (i=g->cd[u];i<g->cd[u+1];i++){
					v=g->adj[i];
					d0[v]--;
					d1[v]++;
				}
			}
			else if (lab[u]==1 && d0[u] < d1[u] ){
				b=1;
				lab[u]=0;
				cut+=d1[u]-d0[u];
				for (i=g->cd[u];i<g->cd[u+1];i++){
					v=g->adj[i];
					d0[v]++;
					d1[v]--;
				}
			}
		}
	} while (b);

	//printf("Number of iterations = %u\n",iter);

	return lab;
}

bisection choosebisection(char *c){
	printf("Chosen bisection algorithm: ");
	if (strcmp(c,"0")==0){
		printf("Random Bisection\n");
		return init;
	}
	if (strcmp(c,"1")==0){
		printf("Greedy Sparsest Cut\n");
		return greedySparsestcut;
	}
	if (strcmp(c,"2")==0){
		printf("Greedy Densest Cut\n");
		return greedyDensestcut;
	}
	if (strcmp(c,"3")==0){
		printf("Greedy Maxcut\n");
		return greedyMaxcut;
	}
	printf("unknown\n");
	exit(1);
}


