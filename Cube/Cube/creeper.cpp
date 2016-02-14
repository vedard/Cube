#include "creeper.h"

Creeper::Creeper() :Monster(M_Creeper)
{
	m_health = 130;
}
Creeper::~Creeper()
{

}

//std::vector<Vector3<int>> Creeper::Explosion()
//{
//	std::vector<Vector3<int>> blocsDestroyed;
//
//	for (int x = 0; x < explosionRadius; x++)
//	{
//		for (int y = -explosionRadius; y < explosionRadius; y++)
//		{
//			for (int z = 0; z < explosionRadius; z++)
//			{
//				if ((x + z) <= (explosionRadius - abs(y)))
//				{
//					// Pour les 4 quartiles
//					blocsDestroyed.push_back(Vector3<int>((int)GetPosition().x + x, (int)GetPosition().y + y, (int)GetPosition().z + z));
//					blocsDestroyed.push_back(Vector3<int>((int)GetPosition().x + x, (int)GetPosition().y + y, (int)GetPosition().z - z));
//					blocsDestroyed.push_back(Vector3<int>((int)GetPosition().x - x, (int)GetPosition().y + y, (int)GetPosition().z - z));
//					blocsDestroyed.push_back(Vector3<int>((int)GetPosition().x - x, (int)GetPosition().y + y, (int)GetPosition().z + z));
//				}
//			}
//		}
//	}
//
//	return blocsDestroyed;
//}