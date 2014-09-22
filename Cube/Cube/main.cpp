#include <iostream>

#include "blocArray3d.h"
#include "blocinfo.h"
#include "chunk.h"
#include <iostream>
#include <string>
#include "define.h"



int main()
{
	//Test classe BlocInfo
	BlocInfo test(BlocType::BTYPE_GRASS, "Bloc de test");
	test.Afficher();

	//Test classe BlocArray3d
	BlocArray3d test2(200, 30, 40);
	test2.Reset(BlocType::BTYPE_GRASS);
	std::cout << test2.Get(2, 2, 2) << std::endl;

	//Test de la classe chunk
	Chunk test3;
	test3.SetBloc(5, 6, 7, BlocType::BTYPE_GRASS);
	std::cout << test3.GetBloc(5, 6, 7) << std::endl;


	
	system("pause");
	return 0;
}