#include <Arduino.h>


class TinyTools 
{
	private:
		

	public:
		TinyTools();
		static int GetNumberDigits(long number);
};

TinyTools::TinyTools()
{
	
}

int TinyTools::GetNumberDigits(long number) 
{
	int num_digits=0;
	if (number < 0)
		number = -number;
 
	while(number > 0) 
	{
		num_digits++;
		number/=10;
	}

	return num_digits;
}























