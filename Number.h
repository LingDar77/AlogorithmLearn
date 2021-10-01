#pragma once
#include <string>

#ifndef Number_h__
#define Number_h__

namespace Struct
{
	using namespace std;
	//a universal type that contains numbers and support unlimited calculation,
	//but now it costs too much, and lacks of efficiency.
	class Number
	{
	private:
		
		string* digits;

	public:

		explicit Number() noexcept
			:digits(new string("")) { }

		~Number()
		{
			delete digits;
		}

		
		//to solve the accuracy problem, I delete the float and double constructor,
		//instead, read a string as a decimal number to construct a Number,
		//however, this constructor need to support e and other formats.
		Number(string&& data)
			:Number()
		{
			//here I decided to implement with a state machine
			/** State transportation table 
			 * State		+/-			Integer			e/E			.				End
			 * Start(0)		Symbol(1)   Integer(2)		Index(4)	Decimal(3)		End(5)
			 * Symbol(1)	End(5)		Integer(2)		Index(4)	Decimal(3)		End(5)
			 * Integer(2)   End(5)		Integer(2)		Index(4)	Decimal(3)		End(5)
			 * Decimal(3)   End(5)		Decimal(3)		End(5)		End(5)			End(5)
			 * Index(4)		End(5)		Index(4)		End(5)		End(5)			End(5)
			 * End(5)		End(5)		End(5)			End(5)		End(5)			End(5)
			 */

			vector<vector<int>> stateTable = { { 1, 2, 4, 3, 5 }, //Start
											   { 5, 2, 4, 3, 5 }, //Symbol
											   { 5, 2, 4, 3, 5 }, //Integer
											   { 5, 3, 5, 5, 5 }, //Decimal
											   { 5, 4, 5, 5, 5 }, //Index
											   { 5, 5, 5, 5, 5 }  //End
											 };
			
			int currState = 0;
			string pow = "";
			for (int i = 0; i != data.size(); ++i)
			{
				char c = data[i];
				if (c == '+' || c == '-')
				{//Symbol
					currState = stateTable[currState][0];
				}
				else if (c <= '9' && c >= '0')
				{//Integer
					currState = stateTable[currState][1];
				}
				else if (c == 'e' || c == 'E')
				{//Index
					currState = stateTable[currState][2];
					
				}
				else if (c == '.')
				{//Decimal
					currState = stateTable[currState][3];
				}
				else
					currState = 5;

				switch (currState)
				{
				case 1:

					digits->push_back(c == '-' ? '-' : '+');
					break;

				case 2:
					if(digits->size() == 0 && c != '0')
						digits->push_back(c);
					else if((*(digits->end() - 1) == '-' || *(digits->end() - 1) == '+') && c == '0')
						break;
					else if(*(digits->end() - 1) != '0')
						digits->push_back(c);
					break;

				case 3:

					digits->push_back(c);
					break;

				case 4:
					
					
					break;
				default:
					break;
				}
				
				if ((i != data.size() - 1 && currState == 5) || (i == data.size() - 1 && *(digits->end() - 1) == '.'))
					assert(0);
				else if(currState == 5)
					break;
			}
											 

		}

		Number(double&&) = delete;
		Number(float&&) = delete;

		//for integers
		template<class Type>
		Number(Type&& data)
			:Number()
		{
			if (data > 0)
			{
				digits->push_back('+');
			}
			else if (data < 0)
			{
				digits->push_back('-');
				data = -1 * data;
			}
			else
			{
				digits->push_back('0');
				return;
			}

			while (data)
			{
				Type digit = data % 10;
				data /= 10;
				digits->insert(digits->begin() + 1, '0' + digit);
			}
		}

	};


	
}
#endif // Number_h__