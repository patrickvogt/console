long randseed()
/* When run at startup gets a random number based on the changing CTA */
{
	long
		random = 1;
	int
		rand,
		prevnum = 0,
		count = 1;

	while (count < 30000)	//repeat through many times to make more random and to allow the CTA value to change multiple times
	{
		rand = VIP_REGS[CTA];						//CTA = (*(BYTE*)(0x0005F830));
		if (random == 0) random = 1;				//prevent % by zero
			
		random += ((rand*count) + (count%random));	//just randomly doing stuff to the number

		if (rand == prevnum)						//if the CTA value doesnt change then count up
			count++;
		else
			count = 0;								//if the number does change then restart the counter
		prevnum = rand;								//keep track of the last number
	}
	return random;									//returns the random seed
}

int randnum(long seed, int randnums)
/* Returns a random number in the requested range from the random seed */
{
	return (seed%randnums);							//returns the random number
}
