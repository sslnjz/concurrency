#include "thread_sequence.h"

int main()
{
	concurrency::thread_sequence ts;
   ts.start(3, 13);

	return 0;
}