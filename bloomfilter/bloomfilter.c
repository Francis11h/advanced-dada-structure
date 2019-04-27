#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>

typedef struct Bloomfilter
{
	char bits_array[8][250000];
	unsigned int random[8][2];
}bf_t;

unsigned int seed[16] = {31, 131, 1319, 1523, 7717, 7583, 10177, 10663};
unsigned int P = 2000003;

unsigned int BKDRHash(char *str, unsigned int seed)
{
    unsigned int hash = 0;
 
    while (*str)
    {
        hash = hash * seed + (*str++);
    }
 
    return (hash & 0x7FFFFFFF);
}


unsigned int Hash(char *str, unsigned int a, unsigned int b, unsigned int seed)
{
	return ((a * BKDRHash(str, seed) + b) % P) % 2000000; 
}

/* creates a Bloom filter with the above specification */
bf_t * create_bf()
{
	bf_t* bf = NULL;
	int i = 0;

	bf = (bf_t*)malloc(sizeof(bf_t));
	memset(bf->bits_array, 0, sizeof(char) * 8 * 250000);
	srand((unsigned int)time(NULL));
	for(i = 0; i < 8; i = i + 1)
	{
		bf->random[i][0] = rand() % P;
		bf->random[i][1] = rand() % P;
	}

	return bf;
}

void bit_set(char* array, int index)
{
	int array_index = index / 8;
	int bit_index = index % 8;
	char set[8] = {0x01, 0x02, 0x04, 0x08, 0x10, 0x20, 0x40, 0x80};
	array[array_index] = array[array_index] | set[bit_index];
}

/* inserts the string *s into the Bloom filter *b */
void insert_bf(bf_t *b, char *s)
{
	int i = 0;
	int index = 0;

	for(i = 0; i < 8; i = i + 1)
	{
		index = Hash(s, b->random[i][0], b->random[i][1], seed[i]);
		bit_set(b->bits_array[i], index);
	}
} 

char bit_get(char* array, int index)
{
    int array_index = index / 8;
    int bit_index = index % 8;
    char set[8] = {0x01, 0x02, 0x04, 0x08, 0x10, 0x20, 0x40, 0x80};
    return array[array_index] & set[bit_index];
}

/* returns 1 if the string *q is accepted by the Bloom filter, and 0 else */
int is_element(bf_t *b, char *q) {
    if (q == NULL)
        return 0;

    int i;
    int index = 0;

    for (i = 0; i < 8; i++) {
        index = Hash(q, b->random[i][0], b->random[i][1], seed[i]);
        if(bit_get(b->bits_array[i], index) == 0) {
            return 0;
        }
    }
	return 1;
}

void sample_string_A(char *s, long i)
{  s[0] = (char)(1 + (i%254));
   s[1] = (char)(1 + ((i/254)%254));
   s[2] = (char)(1 + (((i/254)/254)%254));
   s[3] = 'a';
   s[4] = 'b';
   s[5] = (char)(1 + ((i*(i-3)) %217));
   s[6] = (char)(1 + ((17*i+129)%233 ));
   s[7] = '\0';
}
void sample_string_B(char *s, long i)
{  s[0] = (char)(1 + (i%254));
   s[1] = (char)(1 + ((i/254)%254));
   s[2] = (char)(1 + (((i/254)/254)%254));
   s[3] = 'a';
   s[4] = (char)(1 + ((i*(i-3)) %217));
   s[5] = (char)(1 + ((17*i+129)%233 ));
   s[6] = '\0';
}
void sample_string_C(char *s, long i)
{  s[0] = (char)(1 + (i%254));
   s[1] = (char)(1 + ((i/254)%254));
   s[2] = 'a';
   s[3] = (char)(1 + ((i*(i-3)) %217));
   s[4] = (char)(1 + ((17*i+129)%233 ));
   s[5] = '\0';
}
void sample_string_D(char *s, long i)
{  s[0] = (char)(1 + (i%254));
   s[1] = (char)(1 + ((i/254)%254));
   s[2] = (char)(1 + (((i/254)/254)%254));
   s[3] = 'b';
   s[4] = 'b';
   s[5] = (char)(1 + ((i*(i-3)) %217));
   s[6] = (char)(1 + ((17*i+129)%233 ));
   s[7] = '\0';
}
void sample_string_E(char *s, long i)
{  s[0] = (char)(1 + (i%254));
   s[1] = (char)(1 + ((i/254)%254));
   s[2] = (char)(1 + (((i/254)/254)%254));
   s[3] = 'a';
   s[4] = (char)(2 + ((i*(i-3)) %217));
   s[5] = (char)(1 + ((17*i+129)%233 ));
   s[6] = '\0';
}



int main()
{  long i,j; 
   bf_t * bloom;
   bloom = create_bf();
   printf("Created Filter\n");
   for( i= 0; i< 1450000; i++ )
   {  char s[8];
      sample_string_A(s,i);
      insert_bf( bloom, s );
   }
   for( i= 0; i< 500000; i++ )
   {  char s[7];
      sample_string_B(s,i);
      insert_bf( bloom, s );
   }
   for( i= 0; i< 50000; i++ )
   {  char s[6];
      sample_string_C(s,i);
      insert_bf( bloom, s );
   }
   printf("inserted 2,000,000 strings of length 8,7,6.\n");
   
   for( i= 0; i< 1450000; i++ )
   {  char s[8];
      sample_string_A(s,i);
      if( is_element( bloom, s ) != 1 )
	{  printf("found negative error (1)\n"); exit(0); }
   }
   for( i= 0; i< 500000; i++ )
   {  char s[7];
      sample_string_B(s,i);
      if( is_element( bloom, s ) != 1 )
	{  printf("found negative error (2)\n"); exit(0); }
   }
   for( i= 0; i< 50000; i++ )
   {  char s[6];
      sample_string_C(s,i);
      if( is_element( bloom, s ) != 1 )
	{  printf("found negative error (3)\n"); exit(0); }
   }
   j = 0;
   for( i= 0; i< 500000; i++ )
   {  char s[8];
      sample_string_D(s,i);
      if( is_element( bloom, s ) != 0 )
	j+=1;
   }
   for( i= 0; i< 500000; i++ )
   {  char s[7];
      sample_string_E(s,i);
      if( is_element( bloom, s ) != 0 )
	j+=1;
   }
   printf("Found %d positive errors out of 1,000,000 tests.\n",j);
   printf("Positive error rate %f\%.\n", (float)j/10000.0);

} 
