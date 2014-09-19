#include <iostream>

#include "blocinfo.h"
#include <iostream>
#include <string>
#include "define.h"

int main()
{
	
	BlocInfo test(BlocType::BTYPE_GRASS ,"Bloc de test");
	test.Afficher();

	system("pause");
	return 0;
}