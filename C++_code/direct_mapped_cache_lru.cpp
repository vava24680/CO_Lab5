#include <iostream>
#include <stdio.h>
#include <math.h>
#include <iomanip>
#include <vector>
#define FILENAME "RADIX.txt"

using namespace std;

struct cache_content
{
	cache_content()
	{
		tag = 0u;
		timestamp = 0u;
	}
	unsigned int  tag;
	unsigned int  timestamp;
};

const int K=1024;

int lg( double n )
{
    // log(n)/log(2) is log2.
    return int(log( n ) / log(double(2))+0.5);
}


void simulate(int cache_size, int block_size, int associativity)
{
	unsigned int tag, index, byte_address;

	int offset_bit = (int) lg(block_size);						//bytes offset
	int index_bit = (int) lg(cache_size / block_size / associativity);	//how many bits in index
	int num_set = (cache_size >> offset_bit) / associativity;//how many blocks per set

	unsigned int index_mask = num_set - 1;//index_mask is for giving consecutive 1s of length index

	vector<cache_content> one_num_set_block(associativity);//blocks in a set
	vector< vector<cache_content> > cache(num_set, one_num_set_block);//num_set num_sets one_num_set_block forms a cache
	//cout<<"cache num_set:"<<num_set<<endl;

    FILE * fp = fopen(FILENAME, "r");					//read file

    unsigned int total = 0;
    unsigned int miss = 0;

	while(fscanf(fp,"%x",&byte_address)!=EOF)
	{
		++total;
		index = (byte_address >> offset_bit) & index_mask;
		tag   = (byte_address >> offset_bit) >> index_bit;

		int lruID = 0;
		bool hit = false;
		for(int i = 0; i < associativity; ++i)
		{
			if(cache[index][i].tag == tag)
			{	//hit, lruID = hit set
				cache[index][i].timestamp = total;
				hit = 1;
				break;
			}//get the least recent used set
			else if(cache[index][i].timestamp < cache[index][lruID].timestamp)
				lruID = i;
		}
		if(hit == 0)
		{
			++miss;		//miss
			cache[index][lruID].timestamp = total;			//give a new timestamp
			cache[index][lruID].tag = tag;
		}

	}
	fclose(fp);
	cout << setw(7) << setprecision(3) << (100.0 * miss / total) << "%";
}

int main(){
	cout << FILENAME << endl;
	cout << "Miss Rate" << endl;
	cout << "x-axis is Associativity, y-axis is Cache size" << endl;
	cout << "   ";
	for(int i = 1; i <= 8; i *= 2)cout << setw(4) << i << "-way";
	cout << endl;
	//cache size
	for(int i = 1; i <= 32; i *= 2)
	{
		cout << setw(2) << i << "K";
		//Associativity
		for(int j = 1; j <= 8; j *= 2)
		{
			//run with cache size : i * K, block size : 64 bytes, j-way cache
			simulate(i * K, 64, j);
		}
		cout << endl;
	}
}
