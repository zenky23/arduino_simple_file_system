/*
   Copyright 2013 Michael Crosson
 
 Licensed under the Apache License, Version 2.0 (the "License");
 you may not use this file except in compliance with the License.
 You may obtain a copy of the License at
 
 http://www.apache.org/licenses/LICENSE-2.0
 
 Unless required by applicable law or agreed to in writing, software
 distributed under the License is distributed on an "AS IS" BASIS,
 WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 See the License for the specific language governing permissions and
 limitations under the License.
 */

#include "SimpleTools.h"

int SimpleTools::GetNumberDigits(long number)  {
	int num_digits = 0;
	if (number < 0)
		number = -number;
 
	while (number > 0)  {
		num_digits++;
		number /= 10;
	}

	return num_digits;
}