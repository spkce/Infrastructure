#include "stdio.h"
#include "link.h"

void link_test(void)
{
	const int number[10] = {0,1,2,3,4,5,6,7,8,9};
	Infra::CLink link;

	{
		for (int i = 0; i < 10; i++)
		{
			link.rise((void*)(number+i));
		}
		printf("link.linkSize() : %d \n", link.linkSize());
	}
	{
		printf("link.get() :");
		for (int i = 0; i < link.linkSize(); i++)
		{
			void* p = link.get(i);
			printf("%d ", *(int*)p);
		}
		printf("\n");
	}
	{
		void* p;
		link.reduce(&p);
		printf("link.reduce(%p)=%d : ", p, *(int*)p);

		for (int i = 0; i < link.linkSize(); i++)
		{
			void* p = link.get(i);
			printf("%d ", *(int*)p);
		}
		printf("\n");
	}
	{
		void* p = (void*)&(number[9]);
		link.insert(p, 1);
		link.insert(p, 0);
		link.insert(p, link.linkSize() - 1);
		for (int i = 0; i < link.linkSize(); i++)
		{
			void* p = link.get(i);
			printf("%d ", *(int*)p);
		}
		printf("\n");
		printf("remove 0:");
		link.remove(&p, 0);
		for (int i = 0; i < link.linkSize(); i++)
		{
			void* p = link.get(i);
			printf(" %d ", *(int*)p);
		}
		printf("\n");
		printf("remove 1:");
		link.remove(&p, 1);
		for (int i = 0; i < link.linkSize(); i++)
		{
			void* p = link.get(i);
			printf(" %d ", *(int*)p);
		}
		printf("\n");
		printf("remove n-1:");
		link.remove(&p, link.linkSize() - 1);
		for (int i = 0; i < link.linkSize(); i++)
		{
			void* p = link.get(i);
			printf(" %d ", *(int*)p);
		}
		printf("\n");
	}
	{
		while(link.linkSize() > 0)
		{
			void* p;
			link.reduce(&p);
			
		}
		printf("%d \n", link.linkSize());
	}

	{
		for (int i = 0; i < 10; i++)
		{
			link.rise((void*)(number+i));
		}
		printf("link.linkSize() : %d \n", link.linkSize());
		link.releas();
		printf("link.releas() : %d \n", link.linkSize());
	}

}