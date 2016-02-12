#include "inventory.h"
#include "parametre.h"

Inventory::Inventory()
{
	Init();
	for (size_t i = 0; i < INVENTORY_SIZE; i++)
		m_objets[i] = Item();
}


Inventory::~Inventory()
{

}

void Inventory::AddItemQ(BlockType type) {
	AddItemQ(type, 1);
}

void Inventory::AddItemQ(BlockType type, int number) {
	bool isAdded = false;

	for(int i = 0; i < INVENTORY_SIZE; i++) {
		if(m_objets[i].GetType() == type) {
			m_objets[i].Add(number);
			isAdded = true;
			break;
		}	
	}
	
	if(!isAdded) {
		for(int i = 0; i < INVENTORY_SIZE; i++) {
			if(m_objets[i].GetQuantity() == 0) {
				m_objets[i].AddNew(type,number);
				break;
			}	
		}
	}
}

bool Inventory::RemoveItemQ(BlockType type) {
	return RemoveItemQ(type, 1);
}

bool Inventory::RemoveItemQ(BlockType type, int number) {
	for(int i = 0; i < INVENTORY_SIZE; i++) {
		if(m_objets[i].GetType() == type && m_objets[i].GetQuantity() > 0) {
			m_objets[i].Remove(number);
			return true;
		}
	}

	return false;
}

bool Inventory::RemoveItemALL(BlockType type) {	
	int quantity;
	for(int i = 0; i < INVENTORY_SIZE; i++) {
		if(m_objets[i].GetType() == type && (quantity = m_objets[i].GetQuantity()) > 0) {
			m_objets[i].Remove(quantity);
			return true;
		}
	}
	return false;
}

void Inventory::SwitchItems(int index_item1, int index_item2) {
	if (!index_item1 != index_item2) {
		Item tempItem = m_objets[index_item1];
		m_objets[index_item1] = m_objets[index_item2];
		m_objets[index_item2] = tempItem;
	}
}

Item * Inventory::GetItems() {
	return m_objets;
}

void Inventory::OnKeyDown(unsigned char key) {
	
	int currentRow = m_currentIndex / 3;	
	

	if(key == sf::Keyboard::Up) {
		m_currentIndex += 3;
	} else if (key == sf::Keyboard::Down) {
		m_currentIndex -= 3;
	} else if (key == sf::Keyboard::Right) {
		m_currentIndex -= 1;
	} else if (key == sf::Keyboard::Left) {
		m_currentIndex += 1;
	}

	if((currentRow != m_currentIndex / 3 || m_currentIndex == -1) && (key == sf::Keyboard::Left || key == sf::Keyboard::Right)) {
		m_currentIndex += key == sf::Keyboard::Left ? -3 : +3;
	} else if((key == sf::Keyboard::Up || key == sf::Keyboard::Left) && m_currentIndex > INVENTORY_SIZE - 1) {
		m_currentIndex -= INVENTORY_SIZE;
	} else if ((key == sf::Keyboard::Down || key == sf::Keyboard::Right) && m_currentIndex < 0) {
		m_currentIndex += INVENTORY_SIZE;
	}

	if(key == Parametre::GetInstance().m_inventaire_enlever_element) {
		RemoveItemALL(m_objets[m_currentIndex].GetType());
	} else if (key == Parametre::GetInstance().m_inventaire_bouger_element) {
		if(m_currentMoveIndex == -2) {
			m_currentMoveIndex = m_currentIndex;
		} else {
			SwitchItems(m_currentMoveIndex, m_currentIndex);
			m_currentMoveIndex = -2;
		}
	} 

}

int Inventory::GetCurrentIndex() {
	return m_currentIndex;
}

void Inventory::SetCurrentIndex(int currentIndex) {
	m_currentIndex = currentIndex;
}

int Inventory::GetCurrentMoveIndex() {
	return m_currentMoveIndex;
}

void Inventory::Init() {
	m_currentIndex = (-1);
	m_currentMoveIndex = (-2);
}
