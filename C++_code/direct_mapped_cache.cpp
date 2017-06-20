#include <iostream>
#include <stdio.h>
#include <math.h>
#include <iomanip>
#define FILENAME "DCACHE.txt"

using namespace std;

struct cache_content{
	bool v;
	unsigned int  tag;
//	unsigned int	data[16];
};

const int K=1024;

int lg( double n )
{
    // log(n)/log(2) is log2.
    return int(log( n ) / log(double(2))+0.5);
}


void simulate(int cache_size, int block_size)
{
	unsigned int tag, index, byte_address;

	int offset_bit = (int) lg(block_size);//bytes offset
	int index_bit = (int) lg(cache_size / block_size);
	int num_block = cache_size >> (offset_bit);//how many blocks

	cache_content *cache = new cache_content[num_block];		//blocks
	//cout<<"cache num_block:"<<num_block<<endl;

	for(int j=0;j<num_block;j++)//initialize every block
		cache[j].v=false;

    FILE * fp = fopen(FILENAME, "r");//read file

    unsigned int index_mask = num_block - 1;
	//index_mask is for giving consecutive 1s of length index

    unsigned int total = 0;
    unsigned int miss = 0;

	while(fscanf(fp,"%x",&byte_address)!=EOF)
	{
		++total;
		//cout << hex << x << " ";
		index = (byte_address >> offset_bit) & index_mask;//get index, getting right digits
		tag = byte_address >> (index_bit + offset_bit);//get tag
		if(cache[index].v && cache[index].tag == tag)
		{//if the block is valid and the tag is the same, hit
			cache[index].v = true;
		}
		else
		{//miss
			++miss;
			cache[index].v = true;
			cache[index].tag = tag;
		}
	}

	fclose(fp);
	cout << setw(6) << setprecision(3) << (100.0 * miss / total) << "%";

	delete [] cache;
}

int main(){
	//name of the input file
	cout << FILENAME << endl;
	cout << "Miss Rate" << endl;
	cout << "x-axis is Block size, y-axis is Cache size" << endl;
	cout << "   ";
	for(int i = 16; i <=256; i *= 2)
		cout << setw(7) << i;
	cout << endl;
	//cache size
	for(int i = 4; i <= 256; i *= 4)
	{
		cout << setw(3) << i << "K";//print cache size
		//block size
		for(int j = 16; j <= 256; j *= 2)
		{
			//run, cache size : i * K , block size : j
			simulate(i * K, j);
		}
		cout << endl;
	}
}
