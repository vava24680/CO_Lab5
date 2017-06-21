#include <iostream>
#include <cmath>
using namespace std;
#define block_size 64
#define byte_offset 6
int main(void)
{
	int cache_size;
	int associativity;
	while(1)
	{
		cout << "Input cache size(KB) :";
		cin >> cache_size;
		if(cache_size==0)
			break;
		cout << "Input associativity :";
		cin >> associativity;
		int index_offset = (int)log2(cache_size*1024/block_size/associativity);
		int tag_bit = 32-byte_offset-index_offset;
		int total_bits_per_block_set = (tag_bit + block_size*8 + 1) * associativity;
		//valid bit is variable since there are multiple blocks in a set
		cout << "Total bits in a " << associativity << "-way cache : " << total_bits_per_block_set*cache_size*1024/block_size/associativity << endl;
	}
	return 0;
}