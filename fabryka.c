#include <stdio.h>
#include <pthread.h>

#define LPROD 8
#define LKONS 8
#define RBUF 15
#define LIMIT 800

pthread_t prod[LPROD];
pthread_t kons[LKONS];
pthread_mutex_t blokada;
int bufor;
int produkcja;

void *producent (void *i)
{
	int prod=0;
	int nr=((int) i) + 1;
	while(1)
	{
		if (produkcja==LIMIT && bufor == 0)
		{
			printf("Producent %d wyprodukowal %d produktow.\n",nr,prod);
			return 0;
		}
		pthread_mutex_lock(&blokada);
		if (bufor<RBUF && produkcja<LIMIT)
		{
			bufor++;
			produkcja++;
			prod++;
			printf("Producent %d tworzy produkt nr %d.\n", nr, bufor);
		}
		pthread_mutex_unlock(&blokada);
	}
}

void *konsument (void *i)
{
	int kons=0;
	int nr=((int) i) + 1;
	while(1)
	{
		if (produkcja==LIMIT && bufor==0)
		{
			printf("Konsument %d skonsumowal %d produktow.\n",nr,kons);
			return 0;
		}
		pthread_mutex_lock(&blokada);
		if (bufor>0)
		{
			printf("Konument %d komnsumuje produkt nr %d.\n",nr,bufor);
			bufor--;
			kons++;
		}
		pthread_mutex_unlock(&blokada);
	}
}

int main()
{
	int i;
	bufor=0;
	produkcja=0;
	pthread_mutex_init(&blokada,NULL);
	for(i=0;i<LPROD;i++)
		pthread_create(&prod[i], NULL, producent, (void*) i);
	for(i=0;i<LKONS;i++)
		pthread_create(&kons[i], NULL, konsument, (void*) i);
	for(i=0;i<LPROD;i++)
		pthread_join(prod[i],NULL);
	for(i=0;i<LKONS;i++)
		pthread_join(kons[i],NULL);
}


